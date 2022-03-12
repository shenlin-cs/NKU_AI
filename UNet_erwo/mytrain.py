'''
@Author: Zhou Kai
@GitHub: https://github.com/athon2
@Date: 2018-11-30 09:53:44
'''
import os

os.environ["CUDA_VISIBLE_DEVICES"] = "0"
os.environ['KMP_DUPLICATE_LIB_OK'] = 'True'
import torch
import h5py as h5
from torch import optim
import model
from train import train_epoch


from validation2 import val_epoch
from readdata2 import dataset

from metrics import CombinedLoss, SoftDiceLoss
import visdom

config = dict()
config["cuda_devices"] = True
config["labels"] = (1,)  # change label to train
config["model_file"] = 'model'
config["initial_learning_rate"] = 1e-3
config["batch_size"] = 10
config["validation_batch_size"] = 1
config["image_shape"] = (800, 800)
config["input_shape"] = (384, 384)
config["stride"] = (104, 104)

config["n_labels"] = len(config["labels"])
config["all_modalities"] = ["t1", "t1ce", "flair", "t2"]
config["training_modalities"] = config["all_modalities"]  # change this if you want to only use some of the modalities
config["nb_channels"] = len(config["training_modalities"])
config["loss_k1_weight"] = 0.1
config["loss_k2_weight"] = 0.1
config["random_offset"] = True  # Boolean. Augments the data by randomly move an axis during generating a data
config["random_flip"] = True  # Boolean. Augments the data by randomly flipping an axis during generating a data
# config["permute"] = True  # data shape must be a cube. Augments the data by permuting in various directions
config["result_path"] = "./checkpoint_models/"
config["saved_model_file"] = None
config["overwrite"] = False  # If True, will create new files. If False, will use previously written files.
config["L2_norm"] = 1e-5
config["patience"] = 2
config["lr_decay"] = 0.8
config["epochs"] = 300
config["checkpoint"] = True  # Boolean. If True, will save the best model as checkpoint.
config["label_containing"] = True  # Boolean. If True, will generate label with overlapping.
config["VAE_enable"] = False  # Boolean. If True, will enable the VAE module.


def main():
    # init or load model
    print("init model with input shape", config["input_shape"])
    # net = torch.load('/home/lab426/pointcloud/erwo/checkpoint_models/model/model_r2channel2.pth')
    net = model.UNet(num_classes=1)
    # net = model.Unet(1, 1)

    vgg_model = model.VGGNet(requires_grad=True, pretrained=False)
    # net = model.FCN8s(pretrained_net=vgg_model, n_class=1)
    parameters = net.parameters()
    optimizer = optim.Adam(parameters,
                           lr=config["initial_learning_rate"],
                           weight_decay=config["L2_norm"])
    start_epoch = 1

    if config["VAE_enable"]:
        loss_function = CombinedLoss(k1=config["loss_k1_weight"], k2=config["loss_k2_weight"])
    else:
        loss_function = SoftDiceLoss()
    # data_generator
    print("data generating")

    # from PIL import Image
    # import numpy as np

    X_train = h5.File('best_train.h5')['data'][()]
    # print(X_train.shape)
    # print(X_train[0])
    # image = Image.open('./image/train/data/0.png')  # 用PIL中的Image.open打开图像
    # image_arr = np.array(image)  # 转化成numpy数组
    # print(image_arr)

    y_train = h5.File('best_train.h5')['label'][()]
    X_test = h5.File('best_test.h5')['data'][()]
    y_test = h5.File('best_test.h5')['label'][()]

    # import h5py
    # import matplotlib.pyplot as plt
    #
    # h2 = h5py.File('best_test.h5', 'r')
    # for i in range (len(h2['label'])):
    #     train_set_data = h2['label'][i][:]
    #     plt.imsave('./image/test/label/%s.png'%(str(i)), train_set_data)
    #     plt.imshow(train_set_data)
    #     plt.show()
    # h2.close()

    training_data = dataset(X_train, y_train, config, train=True)
    valildation_data = dataset(X_test, y_test, config, train=False)
    config['train_length'] = X_train.shape[0]
    config['max_iter'] = config['train_length'] * 150


    if torch.cuda.is_available():
        net = net.cuda()
        loss_function = loss_function.cuda()


    print("training on label:{}".format(config["labels"]))
    max_val_acc = 0.
    vis = visdom.Visdom()
    for i in range(start_epoch, config["epochs"]):

        loss, acc, dice_loss, l2_loss, kl_loss = train_epoch(epoch=i,
                                data_set=training_data,
                                model=net,
                                criterion=loss_function,
                                optimizer=optimizer,
                                opt=config,
                                )
        # vis.line(X=torch.Tensor([i]).detach().numpy(), Y=torch.Tensor([loss]).detach().numpy(), win='polynomial', update='append' if i > 1 else None)
        # vis.line(X=[torch.Tensor([i]).detach().numpy()], Y=[torch.Tensor([loss]).detach().numpy(),torch.Tensor([acc]).detach().numpy()], win='train', update='append',opts=dict(title='loss', legend=['loss', 'acc'], xlabel='epoch', ylabel='loss') if i > 1 else None)
        vis.line(torch.Tensor([loss]), torch.Tensor([i]), win='trainloss', opts=dict(title='train_loss', xlabel='epoch', ylabel='train_loss'), update='append' if i > 1 else None)
        vis.line(torch.Tensor([acc]), torch.Tensor([i]), win='trainacc', opts=dict(title='train_accuracy', xlabel='epoch', ylabel='train_accuracy'), update='append' if i > 1 else None)
        vis.line(torch.Tensor([dice_loss]), torch.Tensor([i]), win='dice_loss', opts=dict(title='dice_loss', xlabel='epoch', ylabel='dice_loss'), update='append' if i > 1 else None)
        vis.line(torch.Tensor([l2_loss]), torch.Tensor([i]), win='l2_loss', opts=dict(title='l2_loss', xlabel='epoch', ylabel='l2_loss'), update='append' if i > 1 else None)
        vis.line(torch.Tensor([kl_loss]), torch.Tensor([i]), win='kl_loss', opts=dict(title='kl_loss', xlabel='epoch', ylabel='kl_loss'), update='append' if i > 1 else None)
        val_loss, val_acc = val_epoch(epoch=i,
                                      data_set=valildation_data,
                                      model=net,
                                      criterion=SoftDiceLoss(),
                                      opt=config,
                                      optimizer=optimizer,
                                      )
        # # scheduler.step(val_loss)
        #
        vis.line(torch.Tensor([val_loss]), torch.Tensor([i]), win='testloss', opts=dict(title='test_loss', xlabel='epoch', ylabel='test_loss'), update='append' if i > 1 else None)
        vis.line(torch.Tensor([val_acc]), torch.Tensor([i]), win='testacc', opts=dict(title='test_accuracy', xlabel='epoch', ylabel='test_accuracy'), update='append' if i > 1 else None)
        if config["checkpoint"] and val_acc > max_val_acc:
            max_val_acc = val_acc
            save_dir = os.path.join(config["result_path"], config["model_file"])
            if not os.path.exists(save_dir):
                os.makedirs(save_dir)
            save_model_path = os.path.join(save_dir, "mymodel.pth")
            if os.path.exists(save_model_path):
                os.system("rm " + save_model_path)
            torch.save(net, save_model_path)

        torch.save(net, './checkpoint_models/model/final.pth')


if __name__ == '__main__':
    main()

