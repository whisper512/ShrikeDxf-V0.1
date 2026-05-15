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

    // 订单数量
    int  numVertices() const { return static_cast<int>(vecVertices.size()); }

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const 
    {
        if (vecVertices.empty()) return QRectF();
        double minX = 1e100, minY = 1e100, maxX = -1e100, maxY = -1e100;
        for (const auto& v : vecVertices) {
            minX = std::min(minX, v.point.x());
            minY = std::min(minY, v.point.y());
            maxX = std::max(maxX, v.point.x());
            maxY = std::max(maxY, v.point.y());
        }
        return QRectF(minX - padding, minY - padding, maxX - minX + padding * 2, maxY - minY + padding * 2);
    }

    // 计算到指定点距离
    double distanceTo(double px, double py) const {
        if (vecVertices.empty()) return -1;
        double minDist = 1e100;
        int n = static_cast<int>(vecVertices.size());
        for (int i = 0; i < n; ++i) {
            int next = (i + 1) % n;
            if (!isClosed() && next == 0) break;
            double ax = vecVertices[i].point.x(), ay = vecVertices[i].point.y();
            double bx = vecVertices[next].point.x(), by = vecVertices[next].point.y();
            double abx = bx - ax, aby = by - ay;
            double apx = px - ax, apy = py - ay;
            double ab2 = abx * abx + aby * aby;
            if (ab2 < 1e-12) {
                minDist = std::min(minDist, std::sqrt(apx * apx + apy * apy));
                continue;
            }
            double t = (apx * abx + apy * aby) / ab2;
            t = std::max(0.0, std::min(1.0, t));
            double cx = ax + abx * t, cy = ay + aby * t;
            double dx = px - cx, dy = py - cy;
            minDist = std::min(minDist, std::sqrt(dx * dx + dy * dy));
        }
        return minDist;
    }
};