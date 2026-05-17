#pragma once
#include "EntityBase.h"

// ─── Insert（块引用） ──────────────────────────────────────
struct EntityInsert
{
    EntityProp prop;
    Vertex3D   insertPoint;
    std::string blockName;
    double     xScale = 1.0, yScale = 1.0, zScale = 1.0;
    double     rotation = 0.0;
    int     colCount = 1, rowCount = 1;
    double  colSpacing = 0.0, rowSpacing = 0.0;

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const
    {
        std::vector<QPointF> pts = { QPointF(insertPoint.x(), insertPoint.y()) };
        return EntUtil::boundingBoxFromPoints(pts, padding);
    }

    double distanceTo(double px, double py) const {
        double dx = px - insertPoint.x(), dy = py - insertPoint.y();
        return std::sqrt(dx * dx + dy * dy);
    }
};