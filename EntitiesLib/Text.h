#pragma once
#include "EntityBase.h"

// ─── Text ───────────────────────────────────────────────────
struct EntityText
{
    EntityProp prop;
    Vertex3D   insertPoint;
    Vertex3D   alignPoint;
    std::string text;
    double     height = 0.0;
    double     rotation = 0.0;   // 弧度
    double     widthFactor = 1.0;
    double     obliqueAngle = 0.0;
    std::string style = "STANDARD";
    int     textGen = 0;
    int     alignH = 0;
    int     alignV = 0;

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const 
    {
        double w = height * 2.0;
        double h = height;
        return QRectF(insertPoint.x() - w * 0.1 - padding, insertPoint.y() - h * 0.5 - padding, w + padding * 2, h + padding * 2);
    }

    // 计算到指定点距离
    double distanceTo(double px, double py) const {
        double w = height * 2.0;
        double h = height;
        double left = insertPoint.x() - w * 0.1;
        double top = insertPoint.y() - h * 0.5;
        if (px >= left && px <= left + w && py >= top && py <= top + h)
            return 0.0;
        double dLeft = std::abs(px - left);
        double dRight = std::abs(px - (left + w));
        double dTop = std::abs(py - top);
        double dBottom = std::abs(py - (top + h));
        if (px >= left && px <= left + w) return std::min(dTop, dBottom);
        if (py >= top && py <= top + h) return std::min(dLeft, dRight);
        return std::sqrt(std::min({ dLeft * dLeft + dTop * dTop,
                                   dLeft * dLeft + dBottom * dBottom,
                                   dRight * dRight + dTop * dTop,
                                   dRight * dRight + dBottom * dBottom }));
    }
};