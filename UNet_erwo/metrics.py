'''
@Author: Zhou Kai
@GitHub: https://github.com/athon2
@Date: 2018-11-03 09:47:14
'''

import torch
from torch.nn.modules.loss import _Loss
import torch.nn as nn
import torch.nn.functional as F


class SoftDiceLoss(_Loss):
    '''
    Soft_Dice = 2*|dot(A, B)| / (|dot(A, A)| + |dot(B, B)| + eps)
    eps is a small constant to avoid zero division,
    '''

    def __init__(self, *args, **kwargs):
        super(SoftDiceLoss, self).__init__()

    def forward(self, y_pred, y_true, eps=1e-8):
        batch_size = y_pred.shape[0]

        # intersection = torch.sum(torch.mul(y_pred, y_true))
        # union = torch.sum(torch.mul(y_pred, y_pred)) + torch.sum(torch.mul(y_true, y_true)) + eps
        #
        # dice = 2 * intersection / union
        # dice_loss = 1 - dice/y_pred.shape[0]
        # print(intersection.item(), union.item(), dice_loss.item())

        intersection = torch.sum(torch.mul(y_pred, y_true).view(batch_size, -1), dim=1) + eps / 2
        union = torch.sum(y_pred.view(batch_size, -1) ** 2, dim=1) + torch.sum(y_true.view(batch_size, -1) ** 2, dim=1) + eps
        dice = 2 * intersection / union
        dice_loss = 1 - torch.sum(dice) / batch_size
        # print(intersection.item(), union.item(), dice_loss.item())
        return dice_loss


class CustomKLLoss(_Loss):
    '''
    KL_Loss = (|dot(mean , mean)| + |dot(std, std)| - |log(dot(std, std))| - 1) / N
    N is the total number of image voxels
    '''

    def __init__(self, *args, **kwargs):
        super(CustomKLLoss, self).__init__()

    def forward(self, mean, std):
        return torch.mean(torch.mul(mean, mean)) + torch.mean(torch.mul(std, std)) - torch.mean(
            torch.log(torch.mul(std, std))) - 1


class CombinedLoss(_Loss):
    '''
    Combined_loss = Dice_loss + k1 * L2_loss + k2 * KL_loss
    As default: k1=0.1, k2=0.1
    '''

    def __init__(self, k1=0.1, k2=0.1):
        super(CombinedLoss, self).__init__()
        self.k1 = k1
        self.k2 = k2
        self.dice_loss = SoftDiceLoss()
        self.cross_loss = torch.nn.CrossEntropyLoss()

        self.l2_loss = nn.MSELoss()
        self.kl_loss = CustomKLLoss()

    def forward(self, y_pred, y_true, y_mid):
        est_mean, est_std = (y_mid[:, :128], y_mid[:, 128:])
        seg_pred, seg_truth = (y_pred[:, :1, :, :], y_true[:, :1, :, :])
        vae_pred, vae_truth = (y_pred[:, 1:2, :, :], y_true[:, 1:2, :, :])
        dice_loss = self.dice_loss(seg_pred, seg_truth)

        seg_truth = seg_truth.long()
        seg_pred = torch.cat((1-seg_pred, seg_pred), dim=1)
        seg_truth = torch.squeeze(seg_truth, dim=1)

        l2_loss = self.l2_loss(vae_pred, vae_truth)
        kl_div = self.kl_loss(est_mean, est_std)
        cross_loss = self.cross_loss(seg_pred, seg_truth)
        combined_loss = dice_loss + cross_loss + self.k1 * l2_loss + self.k2 * kl_div
        # print("dice_loss:%.4f, L2_loss:%.4f, KL_div:%.4f, combined_loss:%.4f"%(dice_loss,l2_loss,kl_div,combined_loss))

        return combined_loss, dice_loss, l2_loss, kl_div

