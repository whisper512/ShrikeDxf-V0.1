#include "Spline.h"

QRectF EntitySpline::boundingBox(double padding ) const
{
    std::vector<QPointF> pts;
    for (const auto& pt : controlPoints) pts.emplace_back(pt.x(), pt.y());
    for (const auto& pt : fitPoints) pts.emplace_back(pt.x(), pt.y());
    return EntUtil::boundingBoxFromPoints(pts, padding);
}

double EntitySpline::distanceTo(double px, double py) const
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

void EntitySpline::translate(double dx, double dy) 
{
    for (auto& p : controlPoints) {
        p.setX(p.x() + dx);
        p.setY(p.y() + dy);
    }
    for (auto& p : fitPoints) {
        p.setX(p.x() + dx);
        p.setY(p.y() + dy);
    }
}