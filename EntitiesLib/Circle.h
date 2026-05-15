#pragma once
#include "EntityBase.h"

// ─── Circle ─────────────────────────────────────────────────
struct EntityCircle
{
    EntityProp prop;
    Vertex3D   center;
    double     radius = 0.0;

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const 
    {
        double r = radius + padding;
        return QRectF(center.x() - r, center.y() - r, r * 2, r * 2);
    }

    // 计算到指定点距离
    double distanceTo(double px, double py) const 
    {
        double dx = px - center.x(), dy = py - center.y();
        double d = std::sqrt(dx * dx + dy * dy);
        return std::abs(d - radius);
    }
};