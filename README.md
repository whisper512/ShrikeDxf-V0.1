Shrike-Dxf v0.1

在**ShrikeDxf.vcxproj**中修改`<QtInstall>6.9.3_msvc2022_64</QtInstall>`适配qt版本,注意区分debug和release

(https://www.qcad.org/en/dxflib-downloads)


1.treeview::
添加图元的图标，
禁用编辑仅查看，

2.graphicView::

添加标尺

点的绘制可以根据view的大小来调整后划十字

text绘制暂时不考虑

3.右侧图层属性设置：

修改text内数据，修改数据后更新

文本居中

4.右侧图元信息显示：

添加各种图元的的widget

添加点位的widget，考虑怎么通知数据到stackedwidget

5.顶部菜单栏：文件
打开完成
保存
关闭

6.顶部菜单栏：view

设置graphics的幅面

7.其他

表头颜色设置和列比例的设置

打开后在graphics中圆的大小不太对


dxf数据总类，包含映射类，操作类(未实现)，绘图类(未实现)




