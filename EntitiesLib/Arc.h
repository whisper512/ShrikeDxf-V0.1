#pragma once
#include "EntityBase.h"

// ─── Arc ────────────────────────────────────────────────────
struct EntityArc
{
    EntityProp prop;
    Vertex3D   center;
    double     radius = 0.0;
    double     startAngle = 0.0;  // 弧度
    double     endAngle = 0.0;    // 弧度
    bool       isCCW = true;

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const
    {
        double r = radius + padding;
        std::vector<QPointF> pts = {
            QPointF(center.x() - r, center.y() - r),
            QPointF(center.x() + r, center.y() + r)
        };
        return EntUtil::boundingBoxFromPoints(pts, 0.0);
    }

    double distanceTo(double px, double py) const
    {
        double dx = px - center.x(), dy = py - center.y();
        double d = std::sqrt(dx * dx + dy * dy);
        return std::abs(d - radius);
    }

};


// 三点计算圆心,半径,起止角度
static bool ThreePointsToArc(QPointF p1, QPointF p2, QPointF p3,
    QPointF& center, qreal& radius,
    qreal& startAngle, qreal& endAngle)
{
    qreal A = p1.x() - p2.x();
    qreal B = p1.y() - p2.y();
    qreal C = p2.x() - p3.x();
    qreal D = p2.y() - p3.y();
    qreal E = (p1.x() * p1.x() - p2.x() * p2.x() + p1.y() * p1.y() - p2.y() * p2.y()) / 2.0;
    qreal F = (p2.x() * p2.x() - p3.x() * p3.x() + p2.y() * p2.y() - p3.y() * p3.y()) / 2.0;
    qreal denominator = A * D - B * C;
    if (qAbs(denominator) < 1e-10)
        return false;   // 三点共线
    center.setX((E * D - B * F) / denominator);
    center.setY((A * F - E * C) / denominator);
    radius = QLineF(center, p1).length();
    startAngle = atan2(p1.y() - center.y(), p1.x() - center.x());
    endAngle = atan2(p3.y() - center.y(), p3.x() - center.x());
    return true;
}


// 判断弧是否逆时针(即中间点是否在起点→终点的逆时针路径上)
static bool IsArcCCW(QPointF p1, QPointF p2, QPointF p3, QPointF center)
{
    qreal a1 = atan2(p1.y() - center.y(), p1.x() - center.x());
    qreal a2 = atan2(p2.y() - center.y(), p2.x() - center.x());
    qreal a3 = atan2(p3.y() - center.y(), p3.x() - center.x());
    // 归一化到 [0, 2*PI)
    auto norm = [](qreal a) -> qreal {
        a = fmod(a, 2 * M_PI);
        if (a < 0) a += 2 * M_PI;
        return a;
        };
    a1 = norm(a1);
    a2 = norm(a2);
    a3 = norm(a3);
    if (a1 < a3)
    {
        return (a2 > a1 && a2 < a3);
    }
    else
    {
        return (a2 > a1 || a2 < a3);
    }
}