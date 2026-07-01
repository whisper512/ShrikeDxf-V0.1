#pragma once
#include "EntityBase.h"

// ─── Circle ─────────────────────────────────────────────────
struct EntityCircle
{
    EntityProp prop;
    Vertex3D   center;
    double     radius = 0.0;

    // 中心
    QPointF centerPoint() { return QPointF(center.x(), center.y()); }
    // 包围盒
    QRectF boundingBox(double padding = 0.0) const;
    // 点到圆的距离
    double distanceTo(double px, double py) const;
    // 平移
    void translate(double dx, double dy);
    // X镜像
    void mirrorX();
    // Y镜像
    void mirrorY();
    // 旋转
    void rotate(double angle, const QPointF& center) {};
    // 拉伸 — 四角均匀缩放
    void stretch(StretchGrip grip, const QPointF& newPos);
    
};