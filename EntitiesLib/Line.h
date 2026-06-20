#pragma once
#include "EntityBase.h"

// ─── Line ───────────────────────────────────────────────────
struct EntityLine
{
    EntityProp prop;
    Vertex3D   startPoint, endPoint;

    // 计算长度
    double length() const;
    // 计算角度
    double angle() const;
    // 包围盒
    QRectF boundingBox(double padding) const;
    // 计算点到线段的距离
    double distanceTo(double px, double py) const;
    // 平移
    void translate(double dx, double dy);
};

