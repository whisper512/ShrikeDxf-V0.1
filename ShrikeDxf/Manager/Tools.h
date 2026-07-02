#pragma once
#include <QPointF>
#include <QRectF>
#include <QLineF>
#include <QtMath>
#include <algorithm>

#include "../EntitiesLib/EntityBase.h"

enum class enumMouseStateInView
{
    enumMouseState_None = -1,
    // ----- 绘制状态 -----
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
    // ----- 编辑状态 -----
    enumMouseState_Move,      // 移动选中图元
    enumMouseState_Stretch,   // 拉伸
    enumMouseState_Rotate,    // 旋转
    enumMouseState_Scale,     // 缩放
};

// 夹点枚举（boundingBox四角）
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