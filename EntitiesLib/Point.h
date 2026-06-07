#pragma once
#include "EntityBase.h"

// ─── Point ──────────────────────────────────────────────────
struct EntityPoint
{
    // 图元属性
    EntityProp prop;
    Vertex3D   point;
    double     angle = 0.0;

    QRectF boundingBox(double padding = 0.0) const;
    double distanceTo(double px, double py) const;
    void translate(double dx, double dy);
};

