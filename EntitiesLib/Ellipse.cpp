#include "Ellipse.h"

QRectF EntityEllipse::boundingBox(double padding) const
{
    double majorLen = std::sqrt(majorAxisEndpoint.x() * majorAxisEndpoint.x() + majorAxisEndpoint.y() * majorAxisEndpoint.y());
    if (majorLen < 1e-10) return QRectF();
    double minorLen = majorLen * ratio;
    double angleRad = std::atan2(majorAxisEndpoint.y(), majorAxisEndpoint.x());
    double cosA = std::cos(angleRad), sinA = std::sin(angleRad);
    double cx = center.x(), cy = center.y();
    std::vector<QPointF> pts;
    for (int i = 0; i <= 64; ++i)
    {
        double theta = startParam + (endParam - startParam) * i / 64.0;
        double lx = majorLen * std::cos(theta);
        double ly = minorLen * std::sin(theta);
        pts.emplace_back(cx + lx * cosA - ly * sinA,
            cy + lx * sinA + ly * cosA);
    }
    return EntUtil::boundingBoxFromPoints(pts, padding);
}

double EntityEllipse::distanceTo(double px, double py) const 
{

    double majorLen = std::sqrt(
        majorAxisEndpoint.x() * majorAxisEndpoint.x() +
        majorAxisEndpoint.y() * majorAxisEndpoint.y());
    if (majorLen < 1e-10) return 1e10;
    double minorLen = majorLen * ratio;
    double angleRad = std::atan2(majorAxisEndpoint.y(), majorAxisEndpoint.x());
    double cosA = std::cos(-angleRad), sinA = std::sin(-angleRad);
    double lx = (px - center.x()) * cosA - (py - center.y()) * sinA;
    double ly = (px - center.x()) * sinA + (py - center.y()) * cosA;
    double val = (lx * lx) / (majorLen * majorLen) + (ly * ly) / (minorLen * minorLen);
    if (val < 1e-6) return 0.0;
    return std::abs(std::sqrt(val) - 1.0) * std::min(majorLen, minorLen);
}

void EntityEllipse::translate(double dx, double dy) 
{
    center.setX(center.x() + dx);
    center.setY(center.y() + dy);
}