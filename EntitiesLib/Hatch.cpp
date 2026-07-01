#include "Hatch.h"

QPointF EntityHatch::centerPoint() const
{
    if (loops.empty())
        return QPointF(0, 0);

    double minX = 0, minY = 0, maxX = 0, maxY = 0;
    bool first = true;

    // 辅助：更新包围盒
    auto updateBB = [&](double x, double y) {
        if (first) {
            minX = maxX = x;
            minY = maxY = y;
            first = false;
        }
        else {
            if (x < minX) minX = x;
            if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
        }
        };

    for (const auto& loop : loops)
    {
        if (loop.isPolyline)
        {
            for (const auto& p : loop.polylinePath)
                updateBB(p.x(), p.y());
        }
        else
        {
            for (const auto& edge : loop.edges)
            {
                std::visit([&](const auto& e) {
                    using T = std::decay_t<decltype(e)>;

                    if constexpr (std::is_same_v<T, HatchEdgeLine>)
                    {
                        updateBB(e.start.x(), e.start.y());
                        updateBB(e.end.x(), e.end.y());
                    }
                    else if constexpr (std::is_same_v<T, HatchEdgeArc>)
                    {
                        // 用圆心 ± 半径近似包围盒
                        updateBB(e.center.x() - e.radius, e.center.y() - e.radius);
                        updateBB(e.center.x() + e.radius, e.center.y() + e.radius);
                    }
                    else if constexpr (std::is_same_v<T, HatchEdgeEllipse>)
                    {
                        updateBB(e.center.x(), e.center.y());
                        updateBB(e.majorAxisEndpoint.x(), e.majorAxisEndpoint.y());
                    }
                    else if constexpr (std::is_same_v<T, HatchEdgeSpline>)
                    {
                        for (const auto& p : e.controlPoints)
                            updateBB(p.x(), p.y());
                        for (const auto& p : e.fitPoints)
                            updateBB(p.x(), p.y());
                    }
                    }, edge);
            }
        }
    }

    if (!first)
        return QPointF((minX + maxX) / 2.0, (minY + maxY) / 2.0);

    return QPointF(0, 0);
}


QRectF EntityHatch::boundingBox(double padding) const
{
    // 块引用没有固定大小,返回一个点
    return QRectF(0, 0, 0, 0);
}

double EntityHatch::distanceTo(double px, double py) const
{
    return 0.0;
}


void EntityHatch::translateEdge(HatchEdgeLine& e, double dx, double dy) {
    e.start.setX(e.start.x() + dx); e.start.setY(e.start.y() + dy);
    e.end.setX(e.end.x() + dx);     e.end.setY(e.end.y() + dy);
}

void EntityHatch::translateEdge(HatchEdgeArc& e, double dx, double dy) {
    e.center.setX(e.center.x() + dx);
    e.center.setY(e.center.y() + dy);
}

void EntityHatch::translateEdge(HatchEdgeEllipse& e, double dx, double dy) {
    e.center.setX(e.center.x() + dx);
    e.center.setY(e.center.y() + dy);
    e.majorAxisEndpoint.setX(e.majorAxisEndpoint.x() + dx);
    e.majorAxisEndpoint.setY(e.majorAxisEndpoint.y() + dy);
}

void EntityHatch::translateEdge(HatchEdgeSpline& e, double dx, double dy) {
    for (auto& p : e.controlPoints) { p.setX(p.x() + dx); p.setY(p.y() + dy); }
    for (auto& p : e.fitPoints) { p.setX(p.x() + dx); p.setY(p.y() + dy); }
}

void EntityHatch::translate(double dx, double dy) {
    for (auto& loop : loops) {
        if (loop.isPolyline) {
            for (auto& p : loop.polylinePath) {
                p.setX(p.x() + dx);
                p.setY(p.y() + dy);
            }
        }
        else {
            for (auto& edge : loop.edges) {
                std::visit([=](auto& e) { translateEdge(e, dx, dy); }, edge);
            }
        }
    }
}



// ─── X 镜像（关于 X 轴翻转）────────────────────────────────
void EntityHatch::mirrorX()
{
    for (auto& loop : loops) {
        if (loop.isPolyline) {
            for (auto& p : loop.polylinePath)
                p.setY(-p.y());
        }
        else {
            for (auto& edge : loop.edges) {
                std::visit([](auto& e) {
                    using T = std::decay_t<decltype(e)>;
                    // ── Line ──
                    if constexpr (std::is_same_v<T, HatchEdgeLine>) {
                        e.start.setY(-e.start.y());
                        e.end.setY(-e.end.y());
                    }
                    // ── Arc ──
                    else if constexpr (std::is_same_v<T, HatchEdgeArc>) {
                        e.center.setY(-e.center.y());
                        e.startAngle = -e.startAngle;
                        e.endAngle = -e.endAngle;
                        e.isCCW = !e.isCCW;
                    }
                    // ── Ellipse ──
                    else if constexpr (std::is_same_v<T, HatchEdgeEllipse>) {
                        e.center.setY(-e.center.y());
                        e.majorAxisEndpoint.setY(-e.majorAxisEndpoint.y());
                        e.startParam = -e.startParam;
                        e.endParam = -e.endParam;
                    }
                    // ── Spline ──
                    else if constexpr (std::is_same_v<T, HatchEdgeSpline>) {
                        for (auto& p : e.controlPoints) p.setY(-p.y());
                        for (auto& p : e.fitPoints)     p.setY(-p.y());
                    }
                    }, edge);
            }
        }
    }
    // 图案填充角度
    angle = -angle;
}

void EntityHatch::mirrorY()
{
    for (auto& loop : loops) {
        if (loop.isPolyline) {
            for (auto& p : loop.polylinePath)
                p.setX(-p.x());
        }
        else {
            for (auto& edge : loop.edges) {
                std::visit([](auto& e) {
                    using T = std::decay_t<decltype(e)>;
                    // ── Line ──
                    if constexpr (std::is_same_v<T, HatchEdgeLine>) {
                        e.start.setX(-e.start.x());
                        e.end.setX(-e.end.x());
                    }
                    // ── Arc ──
                    else if constexpr (std::is_same_v<T, HatchEdgeArc>) {
                        e.center.setX(-e.center.x());
                        e.startAngle = M_PI - e.startAngle;
                        e.endAngle = M_PI - e.endAngle;
                        e.isCCW = !e.isCCW;
                    }
                    // ── Ellipse ──
                    else if constexpr (std::is_same_v<T, HatchEdgeEllipse>) {
                        e.center.setX(-e.center.x());
                        e.majorAxisEndpoint.setX(-e.majorAxisEndpoint.x());
                        e.startParam = M_PI - e.startParam;
                        e.endParam = M_PI - e.endParam;
                    }
                    // ── Spline ──
                    else if constexpr (std::is_same_v<T, HatchEdgeSpline>) {
                        for (auto& p : e.controlPoints) p.setX(-p.x());
                        for (auto& p : e.fitPoints)     p.setX(-p.x());
                    }
                    }, edge);
            }
        }
    }
    angle = M_PI - angle;
}

void EntityHatch::rotate(double ang, const QPointF& center)
{
    const double cosA = std::cos(ang);
    const double sinA = std::sin(ang);
    const double cx = center.x();
    const double cy = center.y();

    auto rotPt = [&](Vertex3D& v) {
        double dx = v.x() - cx;
        double dy = v.y() - cy;
        v.setX(cx + dx * cosA - dy * sinA);
        v.setY(cy + dx * sinA + dy * cosA);
        };
    auto rotVec = [&](Vertex3D& v) {
        double nx = v.x() * cosA - v.y() * sinA;
        double ny = v.x() * sinA + v.y() * cosA;
        v.setX(nx);
        v.setY(ny);
        };

    for (auto& loop : loops) {
        if (loop.isPolyline) {
            for (auto& p : loop.polylinePath)
                rotPt(p);
        }
        else {
            for (auto& edge : loop.edges) {
                std::visit([&](auto& e) {
                    using T = std::decay_t<decltype(e)>;
                    if constexpr (std::is_same_v<T, HatchEdgeLine>) {
                        rotPt(e.start);
                        rotPt(e.end);
                    }
                    else if constexpr (std::is_same_v<T, HatchEdgeArc>) {
                        rotPt(e.center);
                        e.startAngle += ang;
                        e.endAngle += ang;
                    }
                    else if constexpr (std::is_same_v<T, HatchEdgeEllipse>) {
                        rotPt(e.center);
                        rotVec(e.majorAxisEndpoint);
                        e.startParam += ang;
                        e.endParam += ang;
                    }
                    else if constexpr (std::is_same_v<T, HatchEdgeSpline>) {
                        for (auto& p : e.controlPoints) rotPt(p);
                        for (auto& p : e.fitPoints)     rotPt(p);
                    }
                    }, edge);
            }
        }
    }
    // 图案填充角度
    angle += ang;
}
