#pragma once
#include "EntityBase.h"

// ─── Line ───────────────────────────────────────────────────
struct EntityLine
{
    EntityProp prop;
    Vertex3D   startPoint, endPoint;

    // 计算长度
    double length() const 
    {
        double dx = endPoint.x() - startPoint.x();
        double dy = endPoint.y() - startPoint.y();
        return std::sqrt(dx * dx + dy * dy);
    }

    // 计算角度
    double angle() const 
    {
        return std::atan2(endPoint.y() - startPoint.y(),
            endPoint.x() - startPoint.x());
    }

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const 
    {
        qreal x1 = startPoint.x(), y1 = startPoint.y();
        qreal x2 = endPoint.x(), y2 = endPoint.y();
        return QRectF(std::min(x1, x2) - padding, std::min(y1, y2) - padding,
            std::abs(x2 - x1) + padding * 2, std::abs(y2 - y1) + padding * 2);
    }

    // 计算到指定点距离
    double distanceTo(double px, double py) const 
    {
        double ax = startPoint.x(), ay = startPoint.y();
        double bx = endPoint.x(), by = endPoint.y();
        double abx = bx - ax, aby = by - ay;
        double apx = px - ax, apy = py - ay;
        double ab2 = abx * abx + aby * aby;
        if (ab2 < 1e-12)
            return std::sqrt(apx * apx + apy * apy);
        double t = (apx * abx + apy * aby) / ab2;
        t = std::max(0.0, std::min(1.0, t));
        double cx = ax + abx * t, cy = ay + aby * t;
        double dx = px - cx, dy = py - cy;
        return std::sqrt(dx * dx + dy * dy);
    }
};

