#pragma once
#include "EntityBase.h"
#include "Point.h"
#include "Line.h"
#include "Circle.h"
#include "Arc.h"
#include "Ellipse.h"
#include "LWPolyline.h"
#include "Polyline.h"
#include "Spline.h"

// ─── Hatch 边界环中的图元（用 variant 存储） ──────────────
struct EntityPoint;
struct EntityLine;
struct EntityCircle;
struct EntityArc;
struct EntityEllipse;
struct EntityLWPolyline;
struct EntityPolyline;
struct EntitySpline;

// 边界环中的边:可以是直线、弧、椭圆弧、样条
struct HatchEdgeLine { Vertex3D start, end; };
struct HatchEdgeArc { Vertex3D center; double radius, startAngle, endAngle; bool isCCW; };
struct HatchEdgeEllipse { Vertex3D center; Vertex3D majorAxisEndpoint; double ratio; double startParam, endParam; };
struct HatchEdgeSpline { int degree; std::vector<Vertex3D> controlPoints; std::vector<Vertex3D> fitPoints; std::vector<double> knots; };

using HatchEdge = std::variant<
    HatchEdgeLine,
    HatchEdgeArc,
    HatchEdgeEllipse,
    HatchEdgeSpline
>;

// Hatch 边界环
struct HatchLoop
{
    int type = 0;                       // 0=默认, 1=外部, 2=内部
    std::vector<HatchEdge> edges;       // 边列表（当边界由多条边构成时）
    std::vector<Vertex3D> polylinePath; // 多段线路径顶点（当 type=2 即 polyline 边界时）
    bool isPolyline = false;
};

// ─── Hatch（填充） ─────────────────────────────────────────
struct EntityHatch
{
    EntityProp prop;

    // 基本信息
    std::string patternName = "SOLID";  // 图案名称 (code 2)
    bool        solidFill = true;       // 是否实心填充 (code 70: 1=实心, 0=图案)
    int         associative = 0;        // (code 71)
    int         style = 0;              // (code 75): 0=普通, 1=外层, 2=忽略
    int         patternType = 1;        // (code 76)
    int         doubleFlag = 0;         // (code 77)
    double      angle = 0.0;            // 图案旋转角（弧度, code 52）
    double      scale = 1.0;            // 图案缩放 (code 41)

    // 边界路径
    int                  loopCount = 0;  // (code 91)
    std::vector<HatchLoop> loops;        // 所有边界环

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const
    {
        // 块引用没有固定大小,返回一个点
        return QRectF(0, 0, 0, 0);
    }

    // 计算到指定点距离
    double distanceTo(double px, double py) const 
    {   
        return 0.0;
    }

};