# NKU_AI
## 虚拟手术规划导航软件系统
基于C++、Qt和VTK独立开发一套虚拟手术规划导航软件系统并进行部署，完成手术规划导航方法设计，进行钻制通道的自主规划，实现术前演练。
## U-Net
对19组CT序列的每张CT图像进行耳蜗和面神经部分的标注，每组CT图像的有效数据量，即同时出现耳蜗和面神经的CT数据为8张至12张不等，利用标注得到的数据集训练一个识别耳蜗和面神经部分的U-Net网络，根据训练好的网络进行耳蜗和面神经部分的自动识别以进行后续的规划实验。
![image](https://user-images.githubusercontent.com/61179305/158023403-dbd3f0d6-4378-4a94-8b12-ff39f32fe9eb.png)
![image](https://user-images.githubusercontent.com/61179305/158023459-a3067368-b74f-419b-84d7-c5fd6fa98d8c.png)

框架：Pytorch
训练集：152张CT图像
测试集：38张CT图像
![image](https://user-images.githubusercontent.com/61179305/158023481-9fceb816-0586-4772-ab8b-843c9a52295a.png)


