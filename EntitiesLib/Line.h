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
    double distanceTo(double px, double py) const;
    QRectF boundingBox(double padding) const;
};

