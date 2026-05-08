#pragma once
#include "EntityBase.h"

// ─── LWPolyline 顶点 ──────────────────────────────────────
struct PolylineVertex2D
{
    Vertex3D point;
    double   bulge = 0.0;
    double   startWidth = 0.0;
    double   endWidth = 0.0;
};

// ─── LWPolyline ────────────────────────────────────────────
struct EntityLWPolyline
{
    EntityProp prop;
    std::vector<PolylineVertex2D> vertices;
    int     flags = 0;
    double  constantWidth = 0.0;
    double  elevation = 0.0;

    bool isClosed() const { return (flags & 1) != 0; }
    int  numVertices() const { return static_cast<int>(vertices.size()); }
};