Shrike-Dxf v0.1

在**ShrikeDxf.vcxproj**中修改`<QtInstall>6.9.3_msvc2022_64</QtInstall>`适配qt版本,注意区分debug和release

(https://www.qcad.org/en/dxflib-downloads)

1.在treeview下方添加新的窗口用来显示界面，列出详细的图元信息，添加图元的图标，treeview显示过于寡淡

2.text图元的解析存在问题，暂时不做处理

3.考虑绘图相关的事项

dxf数据总类，包含映射类，操作类(未实现)，绘图类(未实现)




