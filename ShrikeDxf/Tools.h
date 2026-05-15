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

