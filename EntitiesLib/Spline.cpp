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


void EntitySpline::mirrorX()
{
    // 控制点与拟合点
    for (auto& p : controlPoints) p.setY(-p.y());
    for (auto& p : fitPoints)     p.setY(-p.y());

    // 方向向量 Y 分量取反
    normalVec.setY(-normalVec.y());
    tgStart.setY(-tgStart.y());
    tgEnd.setY(-tgEnd.y());
}

void EntitySpline::mirrorY()
{
    // 控制点与拟合点
    for (auto& p : controlPoints) p.setX(-p.x());
    for (auto& p : fitPoints)     p.setX(-p.x());

    // 方向向量 X 分量取反
    normalVec.setX(-normalVec.x());
    tgStart.setX(-tgStart.x());
    tgEnd.setX(-tgEnd.x());
}

void EntitySpline::rotate(double angle, const QPointF& center)
{
    const double cosA = std::cos(angle);
    const double sinA = std::sin(angle);
    const double cx = center.x();
    const double cy = center.y();

    // 旋转控制点
    for (auto& p : controlPoints) {
        double dx = p.x() - cx;
        double dy = p.y() - cy;
        p.setX(cx + dx * cosA - dy * sinA);
        p.setY(cy + dx * sinA + dy * cosA);
    }
    // 旋转拟合点
    for (auto& p : fitPoints) {
        double dx = p.x() - cx;
        double dy = p.y() - cy;
        p.setX(cx + dx * cosA - dy * sinA);
        p.setY(cy + dx * sinA + dy * cosA);
    }

    // 旋转方向向量（不平移，仅旋转）
    auto rotateVec = [cosA, sinA](Vertex3D& v) {
        double nx = v.x() * cosA - v.y() * sinA;
        double ny = v.x() * sinA + v.y() * cosA;
        v.setX(nx);
        v.setY(ny);
        };
    rotateVec(normalVec);
    rotateVec(tgStart);
    rotateVec(tgEnd);
}
