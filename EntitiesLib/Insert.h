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
};