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
        std::vector<QPointF> pts = {
            QPointF(center.x() - radius, center.y() - radius),
            QPointF(center.x() + radius, center.y() + radius)
        };
        return EntUtil::boundingBoxFromPoints(pts, padding);
    }

    double distanceTo(double px, double py) const
    {
        double dx = px - center.x(), dy = py - center.y();
        double d = std::sqrt(dx * dx + dy * dy);
        return std::abs(d - radius);
    }
};