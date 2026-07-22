#pragma once
#include <QPointF>
#include <QRectF>
#include <QLineF>
#include <QtMath>
#include <algorithm>

#include "../EntitiesLib/EntityBase.h"

enum class MouseStateInView
{
    None = -1,
    // ----- 绘制状态 -----
    Point,
    Line,
    CircleCenterRadius,
    CircleDiameter,
    ArcCenterEndpoint,
    ArcThreePoints,
    Polyline,
    EllipseCenterRadius,
    Rectangle,
    SplineFitPoint,
    SplineControlPoint,
    Text,
    MText,
    // ----- 编辑状态 -----
    Move,      // 移动选中图元
    Stretch,   // 拉伸
    Rotate,    // 旋转
    Scale,     // 缩放
};

// 夹点枚举(boundingBox四角)
enum class StretchGripInView
{
    None = -1,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
};

static_assert(static_cast<int>(StretchGripInView::None) == static_cast<int>(StretchGrip::None));
static_assert(static_cast<int>(StretchGripInView::TopLeft) == static_cast<int>(StretchGrip::TopLeft));
static_assert(static_cast<int>(StretchGripInView::TopRight) == static_cast<int>(StretchGrip::TopRight));
static_assert(static_cast<int>(StretchGripInView::BottomLeft) == static_cast<int>(StretchGrip::BottomLeft));
static_assert(static_cast<int>(StretchGripInView::BottomRight) == static_cast<int>(StretchGrip::BottomRight));