'''
@Author: Zhou Kai
@GitHub: https://github.com/athon2
@Date: 2018-11-30 09:53:44
'''
import torch
from torch.autograd import Variable
import time
import matplotlib.pyplot as plt
from tqdm import tqdm
from utils import AverageMeter, calculate_accuracy
from readdata2 import recompose3D_overlap
import numpy as np
import cv2


def val_epoch(epoch, data_set, model, criterion, optimizer, opt):
    print('validation at epoch {}'.format(epoch))

    model.eval()

    losses = AverageMeter()
    accwhole = AverageMeter()

    valildation_loader = torch.utils.data.DataLoader(dataset=data_set,
                                     batch_size=opt["validation_batch_size"],
                                     shuffle=False,
                                     pin_memory=True)
    val_process = tqdm(valildation_loader)

    for i, (inputs, targets) in enumerate(val_process):
        if i > 0:
            val_process.set_description("Loss: %.4f, Acc: %.4f" % (losses.avg, accwhole.avg))
        patchnum = inputs.shape[1]

        size = opt['image_shape']
        step = opt['stride']
        input_shape = opt['input_shape']
        outputs = torch.zeros(patchnum, 1, input_shape[0], input_shape[1]).cuda()
        for j in range(patchnum):
            input = inputs[:, j:j + 1, :, :].type(torch.FloatTensor)

            if torch.cuda.is_available():
                input = input.cuda()
                targets = targets.cuda()
            with torch.no_grad():
                if opt['VAE_enable']:
                    output, _ = model(input)
                    output = output[:, 0:1, :, :]
                else:
                    output = model(input)

                outputs[j] = output

        # size1 = 800
        # size2 = inputs.shape[2]
        # w = (2 * size2 - size1)
        # outputs2[:, :size2, :size2] = outputs[0]
        # outputs2[:, :size2, size2-w:] += outputs[1]
        # outputs2[:, size2-w:, :size2] += outputs[2]
        # outputs2[:, size2-w:, size2-w:] += outputs[3]
        # outputs2[:, size2-w:size2, :] /= 2
        # outputs2[:, :, size2-w:size2] /= 2

        outputs = torch.squeeze(outputs)
        outputs2 = recompose3D_overlap(outputs, size[0], size[1], step[0], step[1])

        loss = criterion(outputs2, targets)

        whole = calculate_accuracy(outputs2.cpu(), targets.cpu())
        # y_pred = torch.squeeze(outputs2).cpu().numpy()
        # num_component, component = cv2.connectedComponents(y_pred.astype(np.uint8))  # 获得联通成分
        # predictions = np.zeros(y_pred.shape)
        # for c in range(1, num_component):
        #     p = component == c
        #     if p.sum() > 100:
        #         predictions[p] = 1
        # y_pred = torch.from_numpy(predictions)
        # y_pred = torch.unsqueeze(y_pred, dim=0)
        # whole = calculate_accuracy(y_pred, targets.cpu())
        losses.update(loss.cpu(), inputs.size(0))
        accwhole.update(whole, opt['validation_batch_size'])
    print("validation: epoch:{0}\t whole_acc:{1:.4f}".format(epoch, accwhole.avg))

    return losses.avg, accwhole.avg