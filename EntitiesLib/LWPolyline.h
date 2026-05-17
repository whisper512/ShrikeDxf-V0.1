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

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const
    {
        std::vector<QPointF> pts;
        pts.reserve(vecVertices.size());
        for (const auto& v : vecVertices)
            pts.emplace_back(v.point.x(), v.point.y());
        return EntUtil::boundingBoxFromPoints(pts, padding);
    }

    double distanceTo(double px, double py) const {
        if (vecVertices.empty()) return -1;
        double minDist = 1e100;
        int n = static_cast<int>(vecVertices.size());
        for (int i = 0; i < n; ++i) {
            int next = (i + 1) % n;
            if (!isClosed() && next == 0) break;
            double d = EntUtil::pointToSegmentDist(px, py,
                vecVertices[i].point.x(), vecVertices[i].point.y(),
                vecVertices[next].point.x(), vecVertices[next].point.y());
            if (d < minDist) minDist = d;
        }
        return minDist;
    }
};