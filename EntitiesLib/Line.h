#pragma once
#include "EntityBase.h"

// ─── Line ───────────────────────────────────────────────────
struct EntityLine
{
    EntityProp prop;
    Vertex3D   startPoint, endPoint;

    // 中心
    QPointF center() { return QPointF((startPoint.x() + endPoint.x()) / 2.0, (startPoint.y() + endPoint.y()) / 2.0); }
    // 计算长度
    double length() const;
    // 计算角度
    double angle() const;
    // 包围盒
    QRectF boundingBox(double padding = 0.0) const;
    // 计算点到线段的距离
    double distanceTo(double px, double py) const;
    // 平移
    void translate(double dx, double dy);
    // X镜像
    void mirrorX();
    // Y镜像
    void mirrorY();
    // 旋转(弧度) 
    void rotate(double angle, const QPointF& center);
    // 拉伸 — 四角均匀缩放
    void stretch(StretchGrip grip, const QPointF& newPos);


};

