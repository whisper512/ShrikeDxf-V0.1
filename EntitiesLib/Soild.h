#pragma once
#include "EntityBase.h"

// ─── Solid（3/4 顶点填充面） ────────────────────────────────
struct EntitySolid
{
    EntityProp prop;
    Vertex3D   corner[4];

    QRectF boundingBox(double padding = 0.0) const 
    {
        double minX = 1e100, minY = 1e100, maxX = -1e100, maxY = -1e100;
        for (int i = 0; i < 4; ++i) 
        {
            minX = std::min(minX, corner[i].x());
            minY = std::min(minY, corner[i].y());
            maxX = std::max(maxX, corner[i].x());
            maxY = std::max(maxY, corner[i].y());
        }
        return QRectF(minX - padding, minY - padding, maxX - minX + padding * 2, maxY - minY + padding * 2);
    }

    // 计算到指定点距离
    double distanceTo(double px, double py) const {
        // 用包围盒近似
        double minX = 1e100, minY = 1e100, maxX = -1e100, maxY = -1e100;
        for (int i = 0; i < 4; ++i) {
            minX = std::min(minX, corner[i].x());
            minY = std::min(minY, corner[i].y());
            maxX = std::max(maxX, corner[i].x());
            maxY = std::max(maxY, corner[i].y());
        }
        if (px >= minX && px <= maxX && py >= minY && py <= maxY) return 0.0;
        double dLeft = std::abs(px - minX), dRight = std::abs(px - maxX);
        double dTop = std::abs(py - minY), dBottom = std::abs(py - maxY);
        if (px >= minX && px <= maxX) return std::min(dTop, dBottom);
        if (py >= minY && py <= maxY) return std::min(dLeft, dRight);
        return std::sqrt(std::min({ dLeft * dLeft + dTop * dTop, dLeft * dLeft + dBottom * dBottom,
                                   dRight * dRight + dTop * dTop, dRight * dRight + dBottom * dBottom }));
    }
};