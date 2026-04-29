#pragma once
#ifndef ENTITIES_H
#define ENTITIES_H

#include <QPointF>
#include <string>
#include <vector>
#include <variant>
#include <cmath>
#include <memory>

struct Vertex3D
{
    Vertex3D() = default;
    Vertex3D(double x, double y) : m_x(x), m_y(y) {}
    Vertex3D(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}
    explicit Vertex3D(const QPointF& pt) : m_x(pt.x()), m_y(pt.y()) {}

    double x() const { return m_x; }
    double y() const { return m_y; }
    double z() const { return m_z; }
    void setX(double v) { m_x = v; }
    void setY(double v) { m_y = v; }
    void setZ(double v) { m_z = v; }

    QPointF toQPointF() const { return QPointF(m_x, m_y); }

    bool operator==(const Vertex3D& o) const {
        return m_x == o.m_x && m_y == o.m_y && m_z == o.m_z;
    }
    bool operator!=(const Vertex3D& o) const { return !(*this == o); }

private:
    double m_x = 0.0, m_y = 0.0, m_z = 0.0;
};

struct EntityProp
{
    std::string layer = "0";
    std::string lineType = "BYLAYER";
    int         color = 256;        // 256=BYLAYER
    int         color24 = -1;
    int         lineWeight = -1;
    double      ltypeScale = 1.0;
    bool        visible = true;
    int         transparency = 0;
    bool        haveExtrusion = false;
    Vertex3D    extrusionDir;
    double      thickness = 0.0;
};


enum class EntityType : int
{
    None = -1,
    Point = 0,
    Line = 1,
    Circle = 2,
    Arc = 3,
    LWPolyline = 4,
    Polyline = 5,
    Ellipse = 6,
    Spline = 7,
    Text = 8,
    MText = 9,
    Insert = 10,
    Solid = 11,
    Hatch = 12,
};



// ─── Point ──────────────────────────────────────────────────
struct EntityPoint
{
    EntityProp prop;
    Vertex3D   point;
    double     angle = 0.0;
};

// ─── Line ───────────────────────────────────────────────────
struct EntityLine
{
    EntityProp prop;
    Vertex3D   startPoint, endPoint;

    double length() const {
        double dx = endPoint.x() - startPoint.x();
        double dy = endPoint.y() - startPoint.y();
        return std::sqrt(dx * dx + dy * dy);
    }
    double angle() const {
        return std::atan2(endPoint.y() - startPoint.y(),
            endPoint.x() - startPoint.x());
    }
};

// ─── Circle ─────────────────────────────────────────────────
struct EntityCircle
{
    EntityProp prop;
    Vertex3D   center;
    double     radius = 0.0;
};

// ─── Arc ────────────────────────────────────────────────────
struct EntityArc
{
    EntityProp prop;
    Vertex3D   center;
    double     radius = 0.0;
    double     startAngle = 0.0;  // 弧度
    double     endAngle = 0.0;    // 弧度
    bool       isCCW = true;
};

// ─── Ellipse ────────────────────────────────────────────────
struct EntityEllipse
{
    EntityProp prop;
    Vertex3D   center;
    Vertex3D   majorAxisEndpoint;
    double     ratio = 1.0;
    double     startParam = 0.0;  // 弧度
    double     endParam = 0.0;    // 弧度
};

// ─── LWPolyline 顶点 ──────────────────────────────────────
struct PolylineVertex2D
{
    Vertex3D point;
    double   bulge = 0.0;
    double   startWidth = 0.0;
    double   endWidth = 0.0;
};

// ─── LWPolyline ────────────────────────────────────────────
struct EntityLWPolyline
{
    EntityProp prop;
    std::vector<PolylineVertex2D> vertices;
    int     flags = 0;
    double  constantWidth = 0.0;
    double  elevation = 0.0;

    bool isClosed() const { return (flags & 1) != 0; }
    int  numVertices() const { return static_cast<int>(vertices.size()); }
};

// ─── Polyline 顶点（旧式多段线） ───────────────────────────
struct PolylineVertex3D
{
    Vertex3D point;
    double   bulge = 0.0;
    double   startWidth = 0.0;
    double   endWidth = 0.0;
    double   tangentDir = 0.0;
    int      vIndex[4] = {};
    int      vertexFlags = 0;
};

// ─── Polyline（旧式多段线） ────────────────────────────────
struct EntityPolyline
{
    EntityProp prop;
    std::vector<PolylineVertex3D> vertices;
    int     flags = 0;
    double  defStartWidth = 0.0;
    double  defEndWidth = 0.0;
    int     vertexCount = 0;
    int     faceCount = 0;
    int     smoothM = 0, smoothN = 0;
    int     curveType = 0;

    bool isClosed() const { return (flags & 1) != 0; }
};

// ─── Spline ─────────────────────────────────────────────────
struct EntitySpline
{
    EntityProp prop;
    Vertex3D   normalVec;
    Vertex3D   tgStart, tgEnd;
    int     flags = 0;
    int     degree = 3;
    double  knotTolerance = 0.0000001;
    double  controlTolerance = 0.0000001;
    double  fitTolerance = 0.0000001;

    std::vector<double>      knots;
    std::vector<double>      weights;
    std::vector<Vertex3D>    controlPoints;
    std::vector<Vertex3D>    fitPoints;
};

// ─── Text ───────────────────────────────────────────────────
struct EntityText
{
    EntityProp prop;
    Vertex3D   insertPoint;
    Vertex3D   alignPoint;
    std::string text;
    double     height = 0.0;
    double     rotation = 0.0;   // 弧度
    double     widthFactor = 1.0;
    double     obliqueAngle = 0.0;
    std::string style = "STANDARD";
    int     textGen = 0;
    int     alignH = 0;
    int     alignV = 0;
};

// ─── MText ──────────────────────────────────────────────────
struct EntityMText
{
    EntityProp prop;
    Vertex3D   insertPoint;
    Vertex3D   xAxisDir;
    std::string text;
    double     height = 0.0;
    double     rotation = 0.0;
    double     widthFactor = 1.0;
    double     lineSpacing = 1.0;
    std::string style = "STANDARD";
    int     attachPoint = 1;
    int     textDir = 1;
    int     lineSpaceStyle = 1;
};

// ─── Insert（块引用） ──────────────────────────────────────
struct EntityInsert
{
    EntityProp prop;
    Vertex3D   insertPoint;
    std::string blockName;
    double     xScale = 1.0, yScale = 1.0, zScale = 1.0;
    double     rotation = 0.0;
    int     colCount = 1, rowCount = 1;
    double  colSpacing = 0.0, rowSpacing = 0.0;
};

// ─── Solid（3/4 顶点填充面） ────────────────────────────────
struct EntitySolid
{
    EntityProp prop;
    Vertex3D   corner[4];
};

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
};


using variantDxfEntity = std::variant<
    EntityPoint,
    EntityLine,
    EntityCircle,
    EntityArc,
    EntityEllipse,
    EntityLWPolyline,
    EntityPolyline,
    EntitySpline,
    EntityText,
    EntityMText,
    EntityInsert,
    EntitySolid,
    EntityHatch
>;

inline EntityType GetEntityType(const variantDxfEntity& ent)
{
    return static_cast<EntityType>(ent.index());
}

inline EntityProp& GetProp(variantDxfEntity& ent)
{
    return std::visit([](auto& e) -> EntityProp& { return e.prop; }, ent);
}
inline const EntityProp& GetProp(const variantDxfEntity& ent)
{
    return std::visit([](const auto& e) -> const EntityProp& { return e.prop; }, ent);
}

#endif
