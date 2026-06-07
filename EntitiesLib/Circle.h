#pragma once
#include "EntityBase.h"

// ─── Circle ─────────────────────────────────────────────────
struct EntityCircle
{
    EntityProp prop;
    Vertex3D   center;
    double     radius = 0.0;

    
    QRectF boundingBox(double padding = 0.0) const;
    double distanceTo(double px, double py) const;
    void translate(double dx, double dy) {
        center.setX(center.x() + dx);
        center.setY(center.y() + dy);
    }

};