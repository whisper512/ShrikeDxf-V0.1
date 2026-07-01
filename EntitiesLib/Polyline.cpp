#include "Polyline.h"

QPointF EntityPolyline::centerPoint() const
{
    if (vecVertices.empty())
        return QPointF(0, 0);

    double minX = vecVertices[0].point.x();
    double minY = vecVertices[0].point.y();
    double maxX = minX, maxY = minY;

    for (const auto& v : vecVertices)
    {
        double x = v.point.x();
        double y = v.point.y();
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }

    return QPointF((minX + maxX) / 2.0, (minY + maxY) / 2.0);
}

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


void EntityPolyline::mirrorX()
{
    for (auto& v : vecVertices) {
        v.point.setY(-v.point.y());
        v.bulge = -v.bulge;        // 弧方向反转
        v.tangentDir = -v.tangentDir;   // 切线角取反
    }
}

void EntityPolyline::mirrorY()
{
    for (auto& v : vecVertices) {
        v.point.setX(-v.point.x());
        v.bulge = -v.bulge;        // 弧方向反转
        v.tangentDir = M_PI - v.tangentDir;  // 关于Y轴镜像
    }
}

void EntityPolyline::rotate(double angle, const QPointF& center)
{
    const double cosA = std::cos(angle);
    const double sinA = std::sin(angle);
    const double cx = center.x();
    const double cy = center.y();

    for (auto& v : vecVertices) {
        double dx = v.point.x() - cx;
        double dy = v.point.y() - cy;
        v.point.setX(cx + dx * cosA - dy * sinA);
        v.point.setY(cy + dx * sinA + dy * cosA);
        // bulge 不变（纯旋转不改变弧方向）
        v.tangentDir += angle;          // 切线方向随旋转累加
    }
}
