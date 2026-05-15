#pragma once
#include "EntityBase.h"

// ─── Spline ─────────────────────────────────────────────────
struct EntitySpline
{
    EntityProp prop;
    Vertex3D   normalVec;                          //法向量
    Vertex3D   tgStart, tgEnd;                     //起点切向向量,终点切向向量
    int     flags = 0;
    int     degree = 3;                            //样条阶数
    double  knotTolerance = 0.0000001;             //节点容差
    double  controlTolerance = 0.0000001;          //控制点容差
    double  fitTolerance = 0.0000001;              //拟合点容差

    std::vector<double>      knots;                //节点矢量
    std::vector<double>      weights;              //权重值
    std::vector<Vertex3D>    controlPoints;        //控制点
    std::vector<Vertex3D>    fitPoints;            //拟合点

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const 
    {
        double minX = 1e100, minY = 1e100, maxX = -1e100, maxY = -1e100;
        auto update = [&](double x, double y) 
            {
            minX = std::min(minX, x); minY = std::min(minY, y);
            maxX = std::max(maxX, x); maxY = std::max(maxY, y);
            };
        for (const auto& pt : controlPoints) update(pt.x(), pt.y());
        for (const auto& pt : fitPoints) update(pt.x(), pt.y());
        if (minX > maxX) return QRectF();
        return QRectF(minX - padding, minY - padding, maxX - minX + padding * 2, maxY - minY + padding * 2);
    }

    // 计算到指定点距离
    double distanceTo(double px, double py) const 
    {
        double minDist = 1e100;
        for (const auto& cp : controlPoints) {
            double dx = px - cp.x(), dy = py - cp.y();
            minDist = std::min(minDist, std::sqrt(dx * dx + dy * dy));
        }
        for (const auto& fp : fitPoints) {
            double dx = px - fp.x(), dy = py - fp.y();
            minDist = std::min(minDist, std::sqrt(dx * dx + dy * dy));
        }
        return minDist;
    }
};
