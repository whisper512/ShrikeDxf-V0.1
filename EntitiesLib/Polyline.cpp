#include "Polyline.h"

QRectF EntityPolyline::boundingBox(double padding) const
{
    std::vector<QPointF> pts;
    pts.reserve(vecVertices.size());
    for (const auto& v : vecVertices)
        pts.emplace_back(v.point.x(), v.point.y());
    return EntUtil::boundingBoxFromPoints(pts, padding);
}

double EntityPolyline::distanceTo(double px, double py) const {
    if (vecVertices.empty()) return -1;
    double minDist = 1e100;
    int n = static_cast<int>(vecVertices.size());
    for (int i = 0; i < n; ++i) 
    {
        int next = (i + 1) % n;
        if (!isClosed() && next == 0) break;
        double d = EntUtil::pointToSegmentDist(px, py,
            vecVertices[i].point.x(), vecVertices[i].point.y(),
            vecVertices[next].point.x(), vecVertices[next].point.y());
        if (d < minDist) minDist = d;
    }
    return minDist;
}

void  EntityPolyline::translate(double dx, double dy) 
{
    for (auto& v : vecVertices) {
        v.point.setX(v.point.x() + dx);
        v.point.setY(v.point.y() + dy);
    }
}