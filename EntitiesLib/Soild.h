#pragma once
#include "EntityBase.h"

// ─── Solid（3/4 顶点填充面） ────────────────────────────────
struct EntitySolid
{
    EntityProp prop;
    Vertex3D   corner[4];
    // 中心点
    QPointF centerPoint() const;
    // 包围盒
    QRectF boundingBox(double padding = 0.0) const;
    // 点到hatch的距离
    double distanceTo(double px, double py) const;
    // 平移
    void translate(double dx, double dy);
    // X镜像
    void mirrorX();
    // Y镜像
    void mirrorY();
    // 旋转
    void rotate(double angle, const QPointF& center);
    // 拉伸 — 四角均匀缩放
    void stretch(StretchGrip grip, const QPointF& newPos);

};