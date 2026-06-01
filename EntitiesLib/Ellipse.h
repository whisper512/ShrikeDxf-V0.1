#pragma once
#include "EntityBase.h"

// ─── Ellipse ────────────────────────────────────────────────
struct EntityEllipse
{
    EntityProp prop;
    Vertex3D   center;
    Vertex3D   majorAxisEndpoint;
    double     ratio = 1.0;
    double     startParam = 0.0;  // 弧度
    double     endParam = 0.0;    // 弧度

    QRectF boundingBox(double padding = 0.0) const;
    double distanceTo(double px, double py) const;
};