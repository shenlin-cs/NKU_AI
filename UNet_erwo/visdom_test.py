# # coding=utf-8
# import time
# from visdom import Visdom
# import requests
# import os
# import numpy as np
#
# viz = Visdom(server='http://127.0.0.1', port=8097)
# assert viz.check_connection()
#
# # 视频下载可能比较慢，耐心等几分中
# video_file = "demo.ogv"
# if not os.path.exists(video_file):
#     video_url = 'http://media.w3.org/2010/05/sintel/trailer.ogv'
#     res = requests.get(video_url)
#     with open(video_file, "wb") as f:
#         f.write(res.content)
#
# viz.video(videofile=video_file)
#
# # 图片
# # 单张图片
# viz.image(
#     np.random.rand(3, 512, 256),
#     opts={
#         'title': 'Random',
#         'showlegend': True
#     }
# )
# # 多张图片
# viz.images(
#     np.random.rand(20, 3, 64, 64),
#     opts={
#         'title': 'multi-images',
#     }
# )
#
# # 散点图
# Y = np.random.rand(100)
# Y = (Y[Y > 0] + 1.5).astype(int),  # 100个标签1和2
#
# old_scatter = viz.scatter(
#     X=np.random.rand(100, 2) * 100,
#     Y=Y,
#     opts={
#         'title': 'Scatter',
#         'legend': ['A', 'B'],
#         'xtickmin': 0,
#         'xtickmax': 100,
#         'xtickstep': 10,
#         'ytickmin': 0,
#         'ytickmax': 100,
#         'ytickstep': 10,
#         'markersymbol': 'cross-thin-open',
#         'width': 800,
#         'height': 600
#     },
# )
# # time.sleep(5)
# # 更新样式
# viz.update_window_opts(
#     win=old_scatter,
#     opts={
#         'title': 'New Scatter',
#         'legend': ['Apple', 'Banana'],
#         'markersymbol': 'dot'
#     }
# )
# # 3D散点图
# viz.scatter(
#     X=np.random.rand(100, 3),
#     Y=Y,
#     opts={
#         'title': '3D Scatter',
#         'legend': ['Men', 'Women'],
#         'markersize': 5
#     }
# )
#
# # 柱状图
# viz.bar(X=np.random.rand(20))
# viz.bar(
#     X=np.abs(np.random.rand(5, 3)),  # 5个列，每列有3部分组成
#     opts={
#         'stacked': True,
#         'legend': ['A', 'B', 'C'],
#         'rownames': ['2012', '2013', '2014', '2015', '2016']
#     }
# )
#
# viz.bar(
#     X=np.random.rand(20, 3),
#     opts={
#         'stacked': False,
#         'legend': ['America', 'Britsh', 'China']
#     }
# )
#
# # 热力图，地理图，表面图
# viz.heatmap(
#     X=np.outer(np.arange(1, 6), np.arange(1, 11)),
#     opts={
#         'columnnames': ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'],
#         'rownames': ['y1', 'y2', 'y3', 'y4', 'y5'],
#         'colormap': 'Electric'
#     }
# )
#
# # 地表图
# x = np.tile(np.arange(1, 101), (100, 1))
# y = x.transpose()
# X = np.exp((((x - 50) ** 2) + ((y - 50) ** 2)) / -(20.0 ** 2))
# viz.contour(X=X, opts=dict(colormap='Viridis'))
#
# # 表面图
# viz.surf(X=X, opts={'colormap': 'Hot'})

#测试2
# import math
# import numpy as np
# from visdom import Visdom
# import time
#
#
# env = Visdom()
# assert env.check_connection() #测试一下链接，链接错误的话会报错
# Y = np.linspace(0, 2 * math.pi, 70)
# X = np.column_stack((np.sin(Y), np.cos(Y)))
# env.stem(
#         X=X,
#         Y=Y,
#         opts=dict(legend=['Sine', 'Cosine'])
#     )
#
# # 通过env参数指定Environments，如果名称包含了下划线,那么visdom会跟根据下划线分割并自动分组
# envtest = Visdom(env='test_mesh')
# assert envtest.check_connection()
#
#
# x = [0, 0, 1, 1, 0, 0, 1, 1]
# y = [0, 1, 1, 0, 0, 1, 1, 0]
# z = [0, 0, 0, 0, 1, 1, 1, 1]
# X = np.c_[x, y, z]
# i = [7, 0, 0, 0, 4, 4, 6, 6, 4, 0, 3, 2]
# j = [3, 4, 1, 2, 5, 6, 5, 2, 0, 1, 6, 3]
# k = [0, 7, 2, 3, 6, 7, 1, 1, 5, 5, 7, 6]
# Y = np.c_[i, j, k]
# envtest.mesh(X=X, Y=Y, opts=dict(opacity=0.5))
#
# # 更新损失函数
# x,y=0,0
# env2 = Visdom()
# pane1= env2.line(
#     X=np.array([x]),
#     Y=np.array([y]),
#     opts=dict(title='dynamic data'))
#
# for i in range(10):
#     time.sleep(1)        # 每隔一秒钟打印一次数据
#     x+=i
#     y=(y+i)*1.5
#     print(x,y)
#     env2.line(
#         X=np.array([x]),
#         Y=np.array([y]),
#         win=pane1,       # win参数确认使用哪一个pane
#         update='append') # 我们做的动作是追加，除了追加意外还有其他方式

# visdom

import visdom
import torch as t

# 新建一个连接客户端
# 指定env=u'test1' 默认端口为8097 host为localhost
vis = visdom.Visdom()#env=u'test1'

# append 追加数据
# for ii in range(0, 10):
#     # y = x
#     x = t.Tensor([ii])
#     y = x
#     print(type(x),x)
#     vis.line(X=x, Y=y, win='polynomial', update='append' if ii > 0 else None)

for i in range(1, 10):

    print(type(t.Tensor([i])),t.Tensor([i]))

    x = t.Tensor([i])
    y = x
    print(x,y)
    vis.line(X=x, Y=y, win='poly', update='append' if i > 1 else None)

# # 新增一条线
# x = t.arange(0, 9, 0.1)
# y = (x ** 2) / 9
# vis.line(X=x, Y=y, win='polynomial', update='append', name='this is a new Trace')
#
# # 可视化一张随机的黑白图片 H * W
# vis.image(t.randn(64, 64).numpy())
#
# # 可视化一张随机的彩色图片 C * H * W
# vis.image(t.randn(3, 64, 64).numpy(), win='random2')
#
# # 可视化36张随机的彩色图片 每一行6张 N * C * H * W
# vis.images(t.randn(36, 3, 64, 64).numpy(), nrow=6, win='random3', opts={'title': 'random_images'})
#
# # vis.text 支持HTML标签
# vis.text(u'''<h1>Hello visdom</h1> <br>visdom是Facebook专门为<b>PyTorch</b>开发的一个可视化工具,
#     在内部使用了很久，于2017年3月开源''',
#          win='visdom',
#          opts={'title': u'visdom简介'}
#          )