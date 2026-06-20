#pragma once
#include "EntityBase.h"

// ─── Circle ─────────────────────────────────────────────────
struct EntityCircle
{
    EntityProp prop;
    Vertex3D   center;
    double     radius = 0.0;

    // 包围盒
    QRectF boundingBox(double padding = 0.0) const;
    // 点到圆的距离
    double distanceTo(double px, double py) const;
    // 平移
    void translate(double dx, double dy);

};