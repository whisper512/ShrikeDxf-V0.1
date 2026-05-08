#pragma once
#ifndef ENTITIES_H
#define ENTITIES_H

#include <QPointF>
#include <string>
#include <vector>
#include <variant>
#include <cmath>
#include <memory>

//图元字符
#define STR_POINT "POINT"
#define STR_LINE "LINE"
#define STR_CIRCLE "CIRCLE"
#define STR_ARC "ARC"
#define STR_LWPOLYLINE "LWPOLYLINE"
#define STR_POLYLINE "POLYLINE"
#define STR_TEXT "TEXT"
#define STR_MTEXT "MTEXT"
#define STR_ELLIPSE "ELLIPSE"
#define STR_SPLINE "SPLINE"
#define STR_HATCH "HATCH"

#define STR_POINT_LOWERCASE "point"
#define STR_LINE_LOWERCASE "line"
#define STR_CIRCLE_LOWERCASE "circle"
#define STR_ARC_LOWERCASE "arc"
#define STR_LWPOLYLINE_LOWERCASE "lwpolyline"
#define STR_POLYLINE_LOWERCASE "polyline"
#define STR_TEXT_LOWERCASE "text"
#define STR_MTEXT_LOWERCASE "mtext"
#define STR_ELLIPSE_LOWERCASE "ellipse"
#define STR_SPLINE_LOWERCASE "spline"
#define STR_HATCH_LOWERCASE "hatch"


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
    Ellipse = 4,
    LWPolyline = 5,
    Polyline = 6,
    Spline = 7,
    Text = 8,
    MText = 9,
    Insert = 10,
    Solid = 11,
    Hatch = 12,
};


#endif
