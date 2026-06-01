#pragma once
#include "EntityBase.h"

// ─── Solid（3/4 顶点填充面） ────────────────────────────────
struct EntitySolid
{
    EntityProp prop;
    Vertex3D   corner[4];

    QRectF boundingBox(double padding = 0.0) const;
    double distanceTo(double px, double py) const;
};