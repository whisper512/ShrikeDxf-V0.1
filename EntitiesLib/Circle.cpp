#include "Circle.h"

QRectF EntityCircle::boundingBox(double padding) const
{
    std::vector<QPointF> pts = {
        QPointF(center.x() - radius, center.y() - radius),
        QPointF(center.x() + radius, center.y() + radius)
    };
    return EntUtil::boundingBoxFromPoints(pts, padding);
}

double EntityCircle::distanceTo(double px, double py) const
{
    double dx = px - center.x(), dy = py - center.y();
    double d = std::sqrt(dx * dx + dy * dy);
    return std::abs(d - radius);
}

void EntityCircle::translate(double dx, double dy) 
{
    center.setX(center.x() + dx);
    center.setY(center.y() + dy);
}