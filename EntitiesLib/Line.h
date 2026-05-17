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

    double distanceTo(double px, double py) const {
        return EntUtil::pointToSegmentDist(px, py,
            startPoint.x(), startPoint.y(),
            endPoint.x(), endPoint.y());
    }

    QRectF boundingBox(double padding) const {
        std::vector<QPointF> pts = {
            QPointF(startPoint.x(), startPoint.y()),
            QPointF(endPoint.x(),   endPoint.y())
        };
        return EntUtil::boundingBoxFromPoints(pts, padding);
    }
};

