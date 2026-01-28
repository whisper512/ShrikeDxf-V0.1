#pragma once
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

//图元字符
#define STR_POINT "POINT"
#define STR_LINE "LINE"
#define STR_CIRCLE "CIRCLE"
#define STR_ARC "ARC"
#define STR_POLYLINE "POLYLINE"
#define STR_TEXT "TEXT"

#define STR_POINT_LOWERCASE "point"
#define STR_LINE_LOWERCASE "line"
#define STR_CIRCLE_LOWERCASE "circle"
#define STR_ARC_LOWERCASE "arc"
#define STR_POLYLINE_LOWERCASE "polyline"
#define STR_TEXT_LOWERCASE "text"

enum enumEntity
{
    enumEntity_None = -1,
    enumEntity_Point = 0,
    enumEntity_Line = 1,
    enumEntity_Circle = 2,
    enumEntity_Arc = 3,
    enumEntity_Polyline = 4,
    enumEntity_Text = 5
};

enum enumPreviewEntity
{
    enumPreviewEntity_None = -1,
    enumPreviewEntity_Point = 0,
    enumPreviewEntity_Line = 1,
    enumPreviewEntity_Center_Radius_Circle = 2,
    enumPreviewEntity_Diameter_Circle = 3,
    enumPreviewEntity_Center_Endpoint_Arc = 4,
    enumPreviewEntity_ThreePoints_Arc = 5,
    enumPreviewEntity_Polyline = 6,
};

//Dxf图元类
struct Point
{
	double x;
	double y;
	double z;
    std::string type = "POINT";

	Point() :x(0.0), y(0.0), z(0.0) {}
    Point(double x, double y) :x(x), y(y), z(0.0) {};
	Point(double x, double y, double z) :x(x), y(y), z(z) {};
    Point& operator=(const Point& other)
    {
        if (this != &other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            type = other.type;  // 添加这行
        }
        return *this;
    }
};

struct Line 
{
private:
    Point pointStart, pointEnd;
    std::string type = "LINE";

public:
    Line() : pointStart(0.0, 0.0, 0.0), pointEnd(0.0, 0.0, 0.0) {};
    Line(Point pStart, Point pEnd) :pointStart(pStart), pointEnd(pEnd) {};
    double StartX() const { return pointStart.x; }
    double StartY() const { return pointStart.y; }
    double StartZ() const { return pointStart.z; }
    double EndX() const { return pointEnd.x; }
    double EndY() const { return pointEnd.y; }
    double EndZ() const { return pointEnd.z; }
    //弧度
    double Angle() const {return atan2(pointEnd.y - pointStart.y, pointEnd.x - pointStart.x);}
    void setStartPoint(const Point& p) {pointStart = p;}
    void setEndPoint(const Point& p) {pointEnd = p;}
    void setStartX(double x) {pointStart.x = x;}
    void setStartY(double y) {pointStart.y = y;}
    void setEndX(double x) {pointEnd.x = x;}
    void setEndY(double y) {pointEnd.y = y;}
    void setPoints(const Point& pStart, const Point& pEnd) {pointStart = pStart;pointEnd = pEnd;}

    double Length() const {
        double dx = pointEnd.x - pointStart.x;
        double dy = pointEnd.y - pointStart.y;
        return sqrt(dx * dx + dy * dy);
    }

    Line& operator=(const Line& other) {
        if (this != &other) {
            pointStart = other.pointStart;
            pointEnd = other.pointEnd;
            type = other.type;
        }
        return *this;
    }
};

//圆
struct Circle 
{
    Point pointCenter;
    double radius;
    std::string type = "CIRCLE";

    Circle() :pointCenter(0.0,0.0,0.0), radius(0.0) {}
    Circle(Point pCenter, double r) :pointCenter(pCenter), radius(r) {};
    Circle& operator=(const Circle& other) 
    {
        if (this != &other) {
            pointCenter = other.pointCenter;
            radius = other.radius;
            type = other.type;
        }
        return *this;
    }
};

struct Arc 
{
    Point pointCenter;
    double radius;
    double startAngle, endAngle;//单位为度
    std::string type = "ARC";

    Arc() :pointCenter(0.0,0.0,0.0), radius(0.0), startAngle(0.0), endAngle(0.0) {};
    Arc(Point pCenter, double r, double startAngle, double endAngle) :pointCenter(pCenter), radius(r), startAngle(startAngle), endAngle(endAngle) {};
    Arc& operator=(const Arc& other) 
    {
        if (this != &other) {
            pointCenter = other.pointCenter;
            radius = other.radius;
            startAngle = other.startAngle;
            endAngle = other.endAngle;
            type = other.type;
        }
        return *this;
    }
};

struct Polyline 
{
    //顶点数
    int numVertices;
    //M方向顶点数
    int numVertices_M;
    //N方向顶点数
    int numVertices_N;
    //多段线标志位
    int flag;
    //凸值
    double dBulge;
    std::vector<Point> vecVertices;
    std::string type = "POLYLINE";
    
    Polyline() :numVertices(0), numVertices_M(0), numVertices_N(0), flag(0), dBulge(0.0) {};
    Polyline& operator=(const Polyline& other)
    {
        if (this != &other) {
            numVertices = other.numVertices;
            numVertices_M = other.numVertices_M;
            numVertices_N = other.numVertices_N;
            flag = other.flag;
            dBulge = other.dBulge;
            vecVertices = other.vecVertices;
            type = other.type;
        }
        return *this;
    }
};

struct Text 
{
    Point pointCenter;
    std::string content;
    double height;
    std::string type = "TEXT";
    Text() :height(0.0) {};
    Text(Point pCenter, std::string content, double height) :pointCenter(pCenter), content(content), height(height) {};
};

//dxf的viewpoint
struct ViewPort
{
    double left, right, bottom, top;
    ViewPort() :left(0.0), right(0.0), bottom(0.0), top(0.0) {};
    ViewPort(double left, double right, double button, double top) :left(left), right(right), bottom(button), top(top) {};
};



#endif // PRIMITIVE_H