'''
@Author: Zhou Kai
@GitHub: https://github.com/athon2
@Date: 2018-11-30 09:53:44
'''
import torch
from torch.autograd import Variable
from tqdm import tqdm
import cv2

from utils import AverageMeter, calculate_accuracy

def adjust_learning_rate(optimizer, cur_iter, max_iters, lr):
    scale_running_lr = ((1. - float(cur_iter) / max_iters) ** 0.9)
    running_lr = lr * scale_running_lr

    for param_group in optimizer.param_groups:
        param_group['lr'] = running_lr

def train_epoch(epoch, data_set, model, criterion, optimizer, opt):
    print('train at epoch {}'.format(epoch))

    model.train()

    losses = AverageMeter()
    dice = AverageMeter()
    l2= AverageMeter()
    kl = AverageMeter()
    accuracies = AverageMeter()
    nums = AverageMeter()
    train_loader = torch.utils.data.DataLoader(dataset=data_set,
                                               batch_size=opt["batch_size"],
                                               shuffle=True,
                                               )
    training_process = tqdm(train_loader)
    for i, (inputs, targets) in enumerate(training_process):
        if i > 0:
            training_process.set_description("Loss: %.4f, Acc: %.4f, lr: %.8f, nums: %.1f" % (losses.avg.item(), accuracies.avg.item(), optimizer.param_groups[0]['lr'], nums.avg.item()))

        if torch.cuda.is_available():
            inputs = inputs.type(torch.FloatTensor)
            inputs = inputs.cuda()
            targets = targets.type(torch.FloatTensor)
            targets = targets.cuda()

        if opt['VAE_enable']:
            outputs, distr = model(inputs)
            loss, dice_loss, l2_loss, kl_div = criterion(outputs, targets, distr)
            outputs = outputs[:, 0:1, :, :]
            targets = targets[:, 0:1, :, :]
            dice.update(dice_loss, inputs.size(0))
            l2.update(l2_loss, inputs.size(0))
            kl.update(kl_div, inputs.size(0))

        else:
            outputs = model(inputs)
            loss = criterion(outputs, targets)


        acc = calculate_accuracy(outputs.cpu(), targets.cpu())
        losses.update(loss.cpu(), inputs.size(0))
        accuracies.update(acc, inputs.size(0))
        nums.update((targets==1).sum().cpu())

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        cur_itr = i + (epoch % 150 - 1) * opt['train_length']
        adjust_learning_rate(optimizer, cur_itr, opt['max_iter'], opt['initial_learning_rate'])

    # logger.log(phase="train", values={
    #     'epoch': epoch,
    #     'loss': format(losses.avg.item(), '.4f'),
    #     'acc': format(accuracies.avg.item(), '.4f'),
    #     'lr': optimizer.param_groups[0]['lr']
    # })
    return losses.avg, accuracies.avg, dice.avg, l2.avg, kl.avg

    # if opt['VAE_enable']:
    #     return losses.avg, accuracies.avg, dice.avg, l2.avg, kl.avg
    # else:
    #     return losses.avg, accuracies.avg
