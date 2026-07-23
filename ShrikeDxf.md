# dxf 读取和写入
- [x] 替换 dxflab 为 libdxfrw
- [ ] libdxf 支持版本是够的，但是无法解析新的如阵列等图元

# 图元库
- [x] 拆分
- [x] 计算边界
- [x] 计算点到直线距离
- [x] 考虑每个图元新建 cpp 文件用来完成函数的实现
- [ ] 曲线的包围盒不准确,通过控制点画出的线会把控制点的位置也加入包围盒

## 图元 boundingBox 问题清单
- [ ] Text — 居中/右对齐时包围盒偏移：忽略 `alignH`/`alignV`，需根据对齐值计算文本盒相对 `insertPoint` 的偏移
- [ ] Text — 旋转文本包围盒偏小：忽略 `rotation`，需用 `QTransform::rotateRadians` 包裹文本盒
- [ ] Text — 字宽影响未计入：忽略 `widthFactor`，文本宽度需 × widthFactor
- [ ] MText — 同上 rotation 问题
- [ ] LWPolyline — 圆弧段(bulge)超出包围盒：只取顶点未采样弧上点，需对 bulge≠0 的段沿弧采样 4~8 个中间点

## 图元 stretch 问题清单
- [ ] Spline — 方向向量异常缩放：`normalVec`/`tgStart`/`tgEnd` 被当作位置点乘以 `s`，应缩放后归一化保持方向
- [ ] LWPolyline — 圆弧段变形：`bulge` 未重算，缩放顶点后需重新计算 bulge
- [ ] LWPolyline — 线宽不变：`constantWidth`/`startWidth`/`endWidth` 未缩放，需 × `s`
- [ ] 全部 — 四边中点夹点无单轴拉伸：只实现了四角均匀缩放，需分 X/Y 轴分别计算比例因子

## Clipper2 技术分工
- [ ] boundingBox 计算 → 不用 Clipper2：纯几何/字体计算，无多边形处理
- [ ] 8 点拉伸 → 不用 Clipper2：仿射变换，非多边形布尔运算
- [ ] HitTest 命中检测 → 用 Clipper2：`PointInPolygon` 替代手写距离
- [ ] Hatch 填充 → 用 Clipper2：`Triangulate` + `BooleanOp`
- [ ] 图元裁剪/修剪 → 用 Clipper2：`BooleanOp` 多段线布尔

# 图元属性修复
- [x] 旧多段线，曲线，填充
- [x] 属性编辑修改实际数据
- [x] 属性页设置无法修改实际数据

# 图层
- [x] 图层属性显示
- [x] 图层颜色修改
- [ ] 图层颜色修改后表格颜色同步

# 树状图
- [x] 基本恢复

# 绘制
- [x] 恢复
- [x] scene 绘制重新计算大小
- [x] 点击选择图元，和外接框
- [x] 按钮点击绘制图元
- [x] 按钮绘制图元需要添加 椭圆+曲线+矩形+文本+多行文本
- [x] treeview 和 scene 同步选中图元
- [x] 新建一个类处理图元的编辑，把 hittest 移植到这个新类
- [x] 右键取消绘制

# 操作
- [x] scene 中的删除，剪切，复制，粘贴
- [x] 整理代码 包括图元库等
- [x] x 镜像，y 镜像，移动，拉伸，返回中心
- [ ] 实际数据操作在 editor ，先完成图元的函数
- [ ] 完成对图元的拉伸，但是没有实时渲染和高亮，以及精度有问题

# 管理类


# 文件操作
- [x] 保存，另存为
- [ ] 独立一个类

# 设置
- [ ] 中英文切换

# view
- [ ] 样式表主题设置

# help

# 其他
- [x] 修改 lib 为相对位置
- [x] 整理文件结构，都子啊一个目录下
- [x] 拆分总管理类
- [ ] 修改命名，舍弃匈牙利命名,修改完成除了widget的其他管理类




# 技术备忘录

```
双管线架构
一套参数数据 → 两条处理管线：
参数数据（knots / bulge / radius / center / controlPoints）
      │
      ├── 显示管线（编辑期实时）
      │     BSplineBasis 离散化（~100 采样点）
      │       ↓
      │     Clipper2（PointInPolygon / GetBounds / 布尔）
      │       ↓
      │     QPainterPath 渲染
      │     精度：1 像素，关注帧率 + 交互响应
      │
      └── 轨迹管线（输出期离线）
            参数原生值
              ↓
            轨迹生成器
              ├─ Arc/Circle → G02/G03 圆弧插补
              ├─ Line → G01 直线走刀
              ├─ Spline → 按精度自适应 G01 多段短线段
              └─ 精度：0.01mm


**核心原则：** 参数永远不从折线反推。编辑 stretch 直接改参数值，重新离散化显示。输出端始终读取原始参数，不做二次拟合。

## 离散化策略

| 图元类型 | 显示采样 | 用途 |
|----------|---------|------|
| Line | 2 点 | — |
| Circle/Arc | 32~64 点 | bulge 弧段同理 |
| Ellipse | 64 点 | 参数弧也采样 |
| LWPolyline (bulge=0) | 顶点 | — |
| LWPolyline (bulge≠0) | 每弧段 8 点 | 补包围盒缺口 |
| Spline | 100~200 点 | BSplineBasis 等距采样 |
| Text/MText | Qt 字体测量 | 补 align + rotation |

## Clipper2 职责

``
BSplineBasis (已有) → 离散折线 → Clipper2 (新增)
                                    ├─ PointInPolygon → HitTest
                                    ├─ GetBounds → boundingBox
                                    ├─ InflatePaths → Offset
                                    ├─ BooleanOp → 裁剪/修剪
                                    └─ Triangulate → Hatch 填充
``

**不用 Clipper2 的场景：** boundingBox（纯几何计算）、stretch（仿射变换改参数）