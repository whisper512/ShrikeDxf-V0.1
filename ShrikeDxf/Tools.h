#pragma once
#include <QPointF>
#include <QRectF>
#include <QLineF>
#include <QtMath>
#include <algorithm>

enum class enumMouseStateInView
{
	enumMouseState_None = -1,
	enumMouseState_Point,
	enumMouseState_Line,
	enumMouseState_CircleCenterRadius,
	enumMouseState_CircleDiameter,
	enumMouseState_ArcCenterEndpoint,
	enumMouseState_ArcThreePoints,
	enumMouseState_Polyline,
	enumMouseState_EllipseCenterRadius,
	enumMouseState_Rectangle,
    enumMouseState_SplineFitPoint,
    enumMouseState_SplineControlPoint,
	enumMouseState_Text,
    enumMouseState_MText,
};

// 点到线段的最短距离
static double pointToSegmentDist(QPointF p, QPointF a, QPointF b)
{
    QPointF ab = b - a;
    QPointF ap = p - a;
    double ab2 = QPointF::dotProduct(ab, ab);
    if (ab2 < 1e-12) return QLineF(p, a).length();  // a 和 b 重合

    double t = QPointF::dotProduct(ap, ab) / ab2;
    t = qBound(0.0, t, 1.0);
    QPointF closest = a + ab * t;
    return QLineF(p, closest).length();
}

// 点到矩形的最短距离
static double pointToRectDist(QPointF p, QRectF r)
{
    // 如果点在矩形内部，距离为 0
    if (r.contains(p)) return 0.0;

    // 到四条边的最短距离
    double dLeft = std::abs(p.x() - r.left());
    double dRight = std::abs(p.x() - r.right());
    double dTop = std::abs(p.y() - r.top());
    double dBottom = std::abs(p.y() - r.bottom());

    // 点在上方或下方
    if (p.x() >= r.left() && p.x() <= r.right())
        return std::min(dTop, dBottom);
    // 点在左侧或右侧
    if (p.y() >= r.top() && p.y() <= r.bottom())
        return std::min(dLeft, dRight);
    // 点在角部区域
    return std::sqrt(std::min(dLeft * dLeft + dTop * dTop,
        std::min(dLeft * dLeft + dBottom * dBottom,
            std::min(dRight * dRight + dTop * dTop,
                dRight * dRight + dBottom * dBottom))));
}