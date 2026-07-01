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

    // 中心
    QPointF centerPoint() const;
    // 包围盒
    QRectF boundingBox(double padding = 0.0) const;
    // 点到填充区域的最小距离
    double distanceTo(double px, double py) const;
    // 辅助:单条边平移
    static void translateEdge(HatchEdgeLine& e, double dx, double dy);
    static void translateEdge(HatchEdgeArc& e, double dx, double dy);
    static void translateEdge(HatchEdgeEllipse& e, double dx, double dy);
    static void translateEdge(HatchEdgeSpline& e, double dx, double dy);
    // 平移
    void translate(double dx, double dy);
    // X镜像
    void mirrorX();
    // Y镜像
    void mirrorY();
    // 旋转
    void rotate(double angle, const QPointF& center);
    // 拉伸 — 四角均匀缩放
    void stretch(StretchGrip grip, const QPointF& newPos);
    // 辅助:单条边拉伸
    static void stretchEdge(HatchEdgeLine& e, const QPointF& anchor, double scale);
    static void stretchEdge(HatchEdgeArc& e, const QPointF& anchor, double scale);
    static void stretchEdge(HatchEdgeEllipse& e, const QPointF& anchor, double scale);
    static void stretchEdge(HatchEdgeSpline& e, const QPointF& anchor, double scale);

};