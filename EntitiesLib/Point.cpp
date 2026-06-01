#include "Point.h"

QRectF EntityPoint::boundingBox(double padding ) const
{
    return QRectF(point.x() - padding, point.y() - padding, padding * 2, padding * 2);
}

double EntityPoint::distanceTo(double px, double py) const
{
    double dx = px - point.x(), dy = py - point.y();
    return std::sqrt(dx * dx + dy * dy);
}