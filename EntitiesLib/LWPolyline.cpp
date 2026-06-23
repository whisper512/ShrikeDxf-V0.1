#include "LWPolyline.h"

// 计算边界
QRectF EntityLWPolyline::boundingBox(double padding ) const
{
    std::vector<QPointF> pts;
    pts.reserve(vecVertices.size());
    for (const auto& v : vecVertices)
        pts.emplace_back(v.point.x(), v.point.y());
    return EntUtil::boundingBoxFromPoints(pts, padding);
}

double EntityLWPolyline::distanceTo(double px, double py) const {
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

void EntityLWPolyline::translate(double dx, double dy)
{
    for (auto& v : vecVertices) {
        v.point.setX(v.point.x() + dx);
        v.point.setY(v.point.y() + dy);
    }
}

void EntityLWPolyline::mirrorX()
{
    for (auto& v : vecVertices) {
        v.point.setY(-v.point.y());
        v.bulge = -v.bulge;          // 圆弧方向反转
    }
}

void EntityLWPolyline::mirrorY()
{
    for (auto& v : vecVertices) {
        v.point.setX(-v.point.x());
        v.bulge = -v.bulge;          // 圆弧方向反转
    }
}


void EntityLWPolyline::rotate(double angle, const QPointF& center)
{
    const double cosA = std::cos(angle);
    const double sinA = std::sin(angle);
    const double cx = center.x();
    const double cy = center.y();

    for (auto& v : vecVertices) {
        // 平移到原点
        double dx = v.point.x() - cx;
        double dy = v.point.y() - cy;
        // 旋转 + 平移回去
        v.point.setX(cx + dx * cosA - dy * sinA);
        v.point.setY(cy + dx * sinA + dy * cosA);
        // bulge 保持不变（纯旋转不改变弧的方向）
    }
}
