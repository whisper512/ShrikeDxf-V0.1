#pragma once
#include "EntityBase.h"
#include <QFontMetricsF>
#include <QGraphicsTextItem>

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

    // 计算边界(场景坐标Y-down)
    QRectF boundingBox(double padding = 0.0) const;
    double distanceTo(double px, double py) const;
    void translate(double dx, double dy) {
        insertPoint.setX(insertPoint.x() + dx);
        insertPoint.setY(insertPoint.y() + dy);
        if (!alignPoint.isNull()) {   // 如果定义了对齐点
            alignPoint.setX(alignPoint.x() + dx);
            alignPoint.setY(alignPoint.y() + dy);
        }
    }

};