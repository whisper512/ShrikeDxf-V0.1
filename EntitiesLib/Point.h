#pragma once
#include "EntityBase.h"

// ─── Point ──────────────────────────────────────────────────
struct EntityPoint
{
    // 图元属性
    EntityProp prop;
    Vertex3D   point;
    double     angle = 0.0;

    //计算边界
    QRectF boundingBox(double padding = 0.0) const 
    {
        return QRectF(point.x() - padding, point.y() - padding, padding * 2, padding * 2);
    }

    // 计算到指定点距离
    double distanceTo(double px, double py) const 
    {
        double dx = px - point.x(), dy = py - point.y();
        return std::sqrt(dx * dx + dy * dy);
    }

};

