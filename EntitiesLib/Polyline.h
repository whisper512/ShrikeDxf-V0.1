#pragma once
#include "EntityBase.h"

// ─── Polyline 顶点(旧式多段线) ───────────────────────────
struct PolylineVertex3D
{
    Vertex3D point;
    double   bulge = 0.0;
    double   startWidth = 0.0;
    double   endWidth = 0.0;
    double   tangentDir = 0.0;
    int      vIndex[4] = {};
    int      vertexFlags = 0;
};

// ─── Polyline(旧式多段线) ────────────────────────────────
struct EntityPolyline
{
    EntityProp prop;
    std::vector<PolylineVertex3D> vecVertices;
    int     flags = 0;
    double  defStartWidth = 0.0;
    double  defEndWidth = 0.0;
    int     vertexCount = 0;
    int     faceCount = 0;
    int     smoothM = 0, smoothN = 0;
    int     curveType = 0;

    // 是否闭合
    bool isClosed() const { return (flags & 1) != 0; }

    QRectF boundingBox(double padding = 0.0) const;
    double distanceTo(double px, double py) const;
    void translate(double dx, double dy) {
        for (auto& v : vecVertices) {
            v.point.setX(v.point.x() + dx);
            v.point.setY(v.point.y() + dy);
        }
    }

};