#include "Arc.h"
#include <cmath>
#include <QtMath>

// 私有规范弧
CanonicalArc EntityArc::canonicalArc() const
{
    CanonicalArc ca;
    if (endAngle > startAngle) {
        ca.s = startAngle;
        ca.e = endAngle;
        ca.sweep = endAngle - startAngle;
    }
    else if (startAngle > endAngle) {
        ca.s = endAngle;
        ca.e = startAngle;
        ca.sweep = startAngle - endAngle;
    }
    else {
        ca.s = startAngle;
        ca.e = startAngle;
        ca.sweep = 0.0;
    }
    return ca;
}

// 包围盒
QRectF EntityArc::boundingBox(double padding) const
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

    addPoint(ca.s);
    addPoint(ca.e);

    static const double quads[] = { 0.0, M_PI / 2.0, M_PI, 3.0 * M_PI / 2.0 };
    for (double q : quads) {
        double dist = std::fmod(q - ca.s + 2.0 * M_PI, 2.0 * M_PI);
        if (dist <= ca.sweep + 1e-9)
            addPoint(q);
    }

    QRectF box = EntUtil::boundingBoxFromPoints(pts, 0.0);
    return box.adjusted(-padding, -padding, padding, padding);
}

// 点到弧距离
double EntityArc::distanceTo(double px, double py) const
{
    double dx = px - center.x();
    double dy = py - center.y();
    double d = std::sqrt(dx * dx + dy * dy);

    if (d < 1e-12)
        return radius;

    double angle = std::atan2(dy, dx);
    auto ca = canonicalArc();

    double dist = std::fmod(angle - ca.s + 2.0 * M_PI, 2.0 * M_PI);
    if (dist <= ca.sweep + 1e-9)
        return std::abs(d - radius);

    double sx = center.x() + radius * std::cos(startAngle);
    double sy = center.y() + radius * std::sin(startAngle);
    double ex = center.x() + radius * std::cos(endAngle);
    double ey = center.y() + radius * std::sin(endAngle);

    double d1 = std::sqrt((px - sx) * (px - sx) + (py - sy) * (py - sy));
    double d2 = std::sqrt((px - ex) * (px - ex) + (py - ey) * (py - ey));
    return std::min(d1, d2);
}

// 平移
void EntityArc::translate(double dx, double dy) 
{
    center.setX(center.x() + dx);
    center.setY(center.y() + dy);
}

// 三点定弧
bool ThreePointsToArc(QPointF p1, QPointF p2, QPointF p3,
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
