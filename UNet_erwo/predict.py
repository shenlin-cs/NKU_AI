'''
@Author: Zhou Kai
@GitHub: https://github.com/athon2
@Date: 2018-11-30 09:53:44
'''
import os
import cv2 as cv
import h5py as h5

import numpy as np
import torch
from tqdm import tqdm
from readdata2 import extract_patches, recompose3D_overlap, dataset

import matplotlib.pyplot as plt
import cv2
from utils import AverageMeter, calculate_accuracy

def eval1(model, valildation_loader, config):
    val_process = tqdm(valildation_loader)
    losses = AverageMeter()
    accwhole = AverageMeter()
    for i, (inputs, targets) in enumerate(val_process):
        if i > 0:
            val_process.set_description("Loss: %.4f, Acc: %.4f" % (losses.avg, accwhole.avg))
        patchnum = inputs.shape[1]

        size = config['image_shape']
        step = config['stride']
        input_shape = config['input_shape']
        outputs = torch.zeros(patchnum, 1, input_shape[0], input_shape[1]).cuda()
        for j in range(patchnum):
            input = inputs[:, j:j + 1, :, :].type(torch.FloatTensor)

            if torch.cuda.is_available():
                input = input.cuda()
                targets = targets.cuda()
            with torch.no_grad():
                if config['VAE_enable']:
                    output, _ = model(input)
                    output = output[:, 0:1, :, :]
                else:
                    output = model(input)

                outputs[j] = output

        outputs = torch.squeeze(outputs)
        outputs2 = recompose3D_overlap(outputs, size[0], size[1], step[0], step[1])

        whole = calculate_accuracy(outputs2.cpu(), targets.cpu())
        accwhole.update(whole, config['validation_batch_size'])

        from torchvision import transforms

        toPIL = transforms.ToPILImage()  # 这个函数可以将张量转为PIL图片，由小数转为0-255之间的像素值
        pic = toPIL(outputs2.cpu())
        pic.save('./image/result/output/%s.png' % (str(i)))
        pic = toPIL(targets.cpu())
        pic.save('./image/result/target/%s.png' % (str(i)))

    print("validation: whole_acc:{0:.4f}".format(accwhole.avg))

    return accwhole.avg


def evaluation(model):
    config = dict()
    config["cuda_devices"] = True
    # config["labels"] = (1, 2, 4)
    config["labels"] = (1,)  # change label to train
    config["model_file"] = 'model'
    config["initial_learning_rate"] = 1e-4
    config["batch_size"] = 1
    config["validation_batch_size"] = 1
    config["image_shape"] = (800, 800)
    config["input_shape"] = (384, 384)
    config["stride"] = (104, 104)
    config['VAE_enable'] = False
    data = h5.File('best_test.h5')['data'][()]
    label = h5.File('best_test.h5')['label'][()]
    valildation_data = dataset(data, label, config, train=False)
    model.eval()

    valildation_loader = torch.utils.data.DataLoader(dataset=valildation_data,
                                                     batch_size=config["validation_batch_size"],
                                                     shuffle=False,
                                                     pin_memory=True)

    eval1(model, valildation_loader, config)




if __name__ == "__main__":

    # model = torch.load('checkpoint_models/model/mymodel.pth')
    model = torch.load('checkpoint_models/model/final.pth')

    # predict(model)
    evaluation(model)