#pragma once
#include "EntityBase.h"

// ─── Ellipse ────────────────────────────────────────────────
struct EntityEllipse
{
    EntityProp prop;
    Vertex3D   center;
    Vertex3D   majorAxisEndpoint;
    double     ratio = 1.0;
    double     startParam = 0.0;  // 弧度
    double     endParam = 0.0;    // 弧度

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const
    {
        double majorLen = std::sqrt(majorAxisEndpoint.x() * majorAxisEndpoint.x() +
            majorAxisEndpoint.y() * majorAxisEndpoint.y());
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

    double distanceTo(double px, double py) const {
        // 椭圆距离保持原有近似算法，不修改
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
};