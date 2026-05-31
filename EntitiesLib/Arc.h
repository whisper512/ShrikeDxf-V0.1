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
    bool       isCCW = true;      //注意再实体弧中iscccw恒定为1

private:
    // 将弧统一化为“逆时针从 s 到 e”的规范形式（s < e 且 sweep < 2π）
    struct CanonicalArc {
        double s;      // 规范化后的起始角
        double e;      // 规范化后的终止角
        double sweep;  // 扫掠角（0 ~ 2π）
    };

    CanonicalArc canonicalArc() const {
        CanonicalArc ca;
        if (endAngle > startAngle) {
            // 直接作为逆时针弧
            ca.s = startAngle;
            ca.e = endAngle;
            ca.sweep = endAngle - startAngle;
        }
        else if (startAngle > endAngle) {
            // 起点大于终点 → 取补弧（交换后逆时针）
            ca.s = endAngle;
            ca.e = startAngle;
            ca.sweep = startAngle - endAngle;
        }
        else {
            // 相等 → 退化为 0° 弧（但几何上可能是整圆，这里按 0 处理）
            ca.s = startAngle;
            ca.e = startAngle;
            ca.sweep = 0.0;
        }
        return ca;
    }

public:
    // 包围盒
    QRectF boundingBox(double padding = 0.0) const
    {
        const double r = radius;
        if (r <= 0.0)
            return QRectF(center.x(), center.y(), 0.0, 0.0);

        auto ca = canonicalArc();

        std::vector<QPointF> pts;
        pts.reserve(8);

        auto addPoint = [&](double ang) {
            pts.emplace_back(center.x() + r * std::cos(ang),
                center.y() + r * std::sin(ang));
            };

        // 端点
        addPoint(ca.s);
        addPoint(ca.e);

        // 四个象限极值点（0°, 90°, 180°, 270°）
        static const double quads[] = { 0.0, M_PI / 2.0, M_PI, 3.0 * M_PI / 2.0 };
        for (double q : quads) {
            // 计算 q 相对于 ca.s 的逆时针偏移
            double dist = std::fmod(q - ca.s + 2.0 * M_PI, 2.0 * M_PI);
            if (dist <= ca.sweep + 1e-9)
                addPoint(q);
        }

        QRectF box = EntUtil::boundingBoxFromPoints(pts, 0.0);
        return box.adjusted(-padding, -padding, padding, padding);
    }

    // 点到弧的最短距离
    double distanceTo(double px, double py) const
    {
        double dx = px - center.x();
        double dy = py - center.y();
        double d = std::sqrt(dx * dx + dy * dy);

        if (d < 1e-12)          // 重合于圆心
            return radius;

        double angle = std::atan2(dy, dx);
        auto ca = canonicalArc();

        // 计算 angle 相对于 ca.s 的逆时针偏移
        double dist = std::fmod(angle - ca.s + 2.0 * M_PI, 2.0 * M_PI);

        if (dist <= ca.sweep + 1e-9)
            return std::abs(d - radius);   // 在弧上

        // 否则退化为到最近端点的距离（注意端点仍使用原始 start/end）
        double sx = center.x() + radius * std::cos(startAngle);
        double sy = center.y() + radius * std::sin(startAngle);
        double ex = center.x() + radius * std::cos(endAngle);
        double ey = center.y() + radius * std::sin(endAngle);

        double d1 = std::sqrt((px - sx) * (px - sx) + (py - sy) * (py - sy));
        double d2 = std::sqrt((px - ex) * (px - ex) + (py - ey) * (py - ey));
        return std::min(d1, d2);
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
        return false;
    center.setX((E * D - B * F) / denominator);
    center.setY((A * F - E * C) / denominator);
    radius = QLineF(center, p1).length();
    startAngle = atan2(p1.y() - center.y(), p1.x() - center.x());
    endAngle = atan2(p3.y() - center.y(), p3.x() - center.x());
    return true;
}


// 判断弧是否逆时针
static bool IsArcCCW(QPointF p1, QPointF p2, QPointF p3, QPointF center)
{
    qreal a1 = atan2(p1.y() - center.y(), p1.x() - center.x());
    qreal a2 = atan2(p2.y() - center.y(), p2.x() - center.x());
    qreal a3 = atan2(p3.y() - center.y(), p3.x() - center.x());
    auto norm = [](qreal a) -> qreal {
        a = fmod(a, 2 * M_PI);
        if (a < 0) a += 2 * M_PI;
        return a;
        };
    a1 = norm(a1);
    a2 = norm(a2);
    a3 = norm(a3);
    if (a1 < a3)
        return (a2 > a1 && a2 < a3);
    else
        return (a2 > a1 || a2 < a3);
}
