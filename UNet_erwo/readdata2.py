import sys
import math
import torch
import torch.nn as nn
from torch.utils.data import Dataset
import numpy as np
import os
import SimpleITK as sitk
import cv2 as cv
import matplotlib.pyplot as plt
from nilearn.image import new_img_like, resample_to_img
import nibabel as nib
import visdom
import random
import pydicom
import h5py as h5


def copimage(volume, label, shape, train=True):
    x1, y1 = volume.shape
    x2, y2 = shape
    if train:
        x = np.random.randint(x1 - x2)
        y = np.random.randint(y1 - y2)

    else:
        x = x1 - x2
        y = y1 - y2
        x = int(x / 2)
        y = int(y / 2)
    volume = volume[x:x2 + x, y:y2 + y]
    if label is not None:

        label = label[x:x2 + x, y:y2 + y]
    return volume, label


def norm_vol(data):
    data = data.astype(np.float)

    for i in range(4):
        mean = np.mean(data[i])
        std = np.std(data[i])
        data[i] = (data[i] - mean) / std

    return data



def extract_patches(volume, patch_shape, extraction_step, datype='float64'):
    patch_h, patch_w = patch_shape[0], patch_shape[1]
    stride_h, stride_w = extraction_step[0], extraction_step[1]
    img_h, img_w = volume.shape[0], volume.shape[1]
    N_patches_h = (img_h - patch_h) // stride_h + 1
    N_patches_w = (img_w - patch_w) // stride_w + 1
    N_patches_img = N_patches_h * N_patches_w
    raw_patch_martrix = np.zeros((N_patches_img, patch_h, patch_w), dtype=datype)
    k = 0

    # iterator over all the patches
    for h in range((img_h - patch_h) // stride_h + 1):
        for w in range((img_w - patch_w) // stride_w + 1):

            raw_patch_martrix[k] = volume[h * stride_h:(h * stride_h) + patch_h, \
                                   w * stride_w:(w * stride_w) + patch_w]
            k += 1
    assert (k == N_patches_img)

    return raw_patch_martrix

"""
To recompose an array of 3D images from patches
"""
def recompose3D_overlap(preds, img_h, img_w, stride_h, stride_w):
  patch_h = preds.shape[1]
  patch_w = preds.shape[2]
  N_patches_h = (img_h-patch_h)//stride_h+1
  N_patches_w = (img_w-patch_w)//stride_w+1
  N_patches_img = N_patches_h * N_patches_w
  # print("N_patches_h: " ,N_patches_h)
  # print("N_patches_w: " ,N_patches_w)
  # print("N_patches_img: ",N_patches_img)
  assert(preds.shape[0]%N_patches_img==0)
  N_full_imgs = preds.shape[0]//N_patches_img
  # print("According to the dimension inserted, there are " \
  #         +str(N_full_imgs) +" full images (of " +str(img_h)+"x" +" each)")
  # itialize to zero mega array with sum of Probabilities
  raw_pred_martrix = torch.zeros((N_full_imgs,img_h,img_w)).cuda()
  raw_sum = torch.zeros((N_full_imgs,img_h,img_w)).cuda()

  k = 0
  # iterator over all the patches
  for i in range(N_full_imgs):
    for h in range((img_h-patch_h)//stride_h+1):
      for w in range((img_w-patch_w)//stride_w+1):
          raw_pred_martrix[i,h*stride_h:(h*stride_h)+patch_h,\
                                w*stride_w:(w*stride_w)+patch_w]+=preds[k]
          raw_sum[i,h*stride_h:(h*stride_h)+patch_h,\
                          w*stride_w:(w*stride_w)+patch_w]+=1.0
          k+=1
  assert(k==preds.shape[0])
  #To check for non zero sum matrix
  assert(torch.min(raw_sum).item()>=1.0)
  # final_matrix = np.around(raw_pred_martrix/raw_sum)
  final_matrix =raw_pred_martrix/raw_sum
  return final_matrix


class dataset(Dataset):
    def __init__(self, data, label, config, train=True):

        # self.data = h5.File('/home/lab426/pointcloud/erwo/data/data2.h5')['data'][()]
        # self.label = h5.File('/home/lab426/pointcloud/erwo/data/data2.h5')['label'][()]
        self.data = data
        self.label = label
        # self.length1 = len(self.data)
        # self.length2 = le
        self.train = train
        self.config = config

    def __getitem__(self, index):

        image, label = self.data[index], self.label[index]  # 获取单个数据和标签，包括文件名
        mean = np.mean(image)
        std = np.std(image)
        image = (image - mean) / std
        size = self.config['input_shape']
        step = self.config['stride']

        if self.train is True:
            # rotate
            (h, w) = image.shape[:2]  # 10
            center = (w // 2, h // 2)  # 11
            angle = np.random.randint(-10, 10)
            M = cv.getRotationMatrix2D(center, angle, 1.0)  # 12
            image = cv.warpAffine(image, M, (w, h))
            label = cv.warpAffine(label, M, (w, h))
            image, label = copimage(image, label, size)



        else:
            image = extract_patches(image, size, step)

        image = torch.from_numpy(image).float()  # Float Tensor 4, 240, 240
        label = torch.from_numpy(label).float() # Float Tensor 240, 240
        if self.train:
            image = torch.unsqueeze(image, dim=0)
            label = torch.unsqueeze(label, dim=0)
        if self.config['VAE_enable'] and self.train:
            label = torch.cat((label, image), dim=0)
        return image, label

    def __len__(self):
        return len(self.data)



if __name__ == "__main__":

    data_dir = '/home/lab426/pointcloud/erwo/data/train_dicom'  # '../data_sample/'
    data = dataset(data_dir=data_dir, train=True)
    train_loader = torch.utils.data.DataLoader(dataset=data,
                                               batch_size=1,
                                               shuffle=True,
                                               )

    for i, data, label in enumerate(train_loader):
        print(data.shape)
    print('finish!')
