#pragma once
#include "EntityBase.h"

// ─── Point ──────────────────────────────────────────────────
struct EntityPoint
{
    EntityProp prop;
    Vertex3D   point;
    double     angle = 0.0;
};