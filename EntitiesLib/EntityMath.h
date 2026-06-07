#pragma once
#include <cmath>
#include <algorithm>
#include <QRectF>

namespace EntUtil {

    /// 计算一点到线段的距离
    inline double pointToSegmentDistSq(double px, double py,
        double ax, double ay,
        double bx, double by) {
        double dx = bx - ax;
        double dy = by - ay;
        double lenSq = dx * dx + dy * dy;
        if (lenSq < 1e-20) {
            return (px - ax) * (px - ax) + (py - ay) * (py - ay);
        }
        double t = ((px - ax) * dx + (py - ay) * dy) / lenSq;
        t = std::max(0.0, std::min(1.0, t));
        double projX = ax + t * dx;
        double projY = ay + t * dy;
        return (px - projX) * (px - projX) + (py - projY) * (py - projY);
    }

    /// 计算一点到线段的距离
    inline double pointToSegmentDist(double px, double py,
        double ax, double ay,
        double bx, double by) {
        return std::sqrt(pointToSegmentDistSq(px, py, ax, ay, bx, by));
    }

    /// 计算一点到轴对齐矩形的距离
    inline double rectDistanceToPoint(double px, double py,
        double left, double top,
        double right, double bottom) {
        if (px < left) {
            if (py < top)        return std::hypot(px - left, py - top);
            if (py > bottom)     return std::hypot(px - left, py - bottom);
            return left - px;
        }
        if (px > right) {
            if (py < top)        return std::hypot(px - right, py - top);
            if (py > bottom)     return std::hypot(px - right, py - bottom);
            return px - right;
        }
        if (py < top)    return top - py;
        if (py > bottom) return py - bottom;
        return 0.0; // inside
    }

    /// 根据一组点计算带边距的包围盒
    inline QRectF boundingBoxFromPoints(const std::vector<QPointF>& pts, double padding) {
        if (pts.empty()) return QRectF();
        double xmin = pts[0].x(), xmax = xmin, ymin = pts[0].y(), ymax = ymin;
        for (const auto& p : pts) {
            if (p.x() < xmin) xmin = p.x();
            if (p.x() > xmax) xmax = p.x();
            if (p.y() < ymin) ymin = p.y();
            if (p.y() > ymax) ymax = p.y();
        }
        return QRectF(xmin - padding, ymin - padding,
            (xmax - xmin) + 2 * padding,
            (ymax - ymin) + 2 * padding);
    }

} // namespace EntUtil
