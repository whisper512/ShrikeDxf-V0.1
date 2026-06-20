#pragma once
#include "EntityBase.h"

// ─── Point ──────────────────────────────────────────────────
struct EntityPoint
{
    // 图元属性
    EntityProp prop;
    Vertex3D   point;
    double     angle = 0.0;

    // 包围盒
    QRectF boundingBox(double padding = 0.0) const;
    // 点到点距离
    double distanceTo(double px, double py) const;
    // 平移
    void translate(double dx, double dy);
};

