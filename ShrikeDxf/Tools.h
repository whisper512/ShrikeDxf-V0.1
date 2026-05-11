#pragma once
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
};