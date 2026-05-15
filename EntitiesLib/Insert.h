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
        // 块引用没有固定大小,返回一个点
        return QRectF(insertPoint.x() - padding, insertPoint.y() - padding, padding * 2, padding * 2);
    }

    // 计算到指定点距离
    double distanceTo(double px, double py) const {
        double dx = px - insertPoint.x(), dy = py - insertPoint.y();
        return std::sqrt(dx * dx + dy * dy);
    }
};