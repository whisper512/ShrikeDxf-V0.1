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
    std::vector<PolylineVertex2D> vecVertices;
    int     flags = 0;
    double  constantWidth = 0.0;
    double  elevation = 0.0;

    // 是否闭合
    bool isClosed() const { return (flags & 1) != 0; }
    // 顶点数量
    int  numVertices() const { return static_cast<int>(vecVertices.size()); }
    // 包围盒
    QRectF boundingBox(double padding = 0.0) const;
    // 计算点到多段线的距离
    double distanceTo(double px, double py) const;
    // 平移
    void translate(double dx, double dy);

};