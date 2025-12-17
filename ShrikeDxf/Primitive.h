#pragma once
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

//Dxf图元类

struct Point
{
	double x;
	double y;
	double z;
    std::string type = "POINT";
	Point() :x(0.0), y(0.0), z(0.0) {}
	Point(double x, double y, double z) :x(x), y(y), z(z) {};
};

struct Line 
{
    Point pointStart, pointEnd;
    std::string type = "LINE";
    Line(Point pStart,Point pEnd) :pointStart(pStart), pointEnd(pEnd) {};
};

struct Circle 
{
    Point pointCenter;
    double radius;
    std::string type = "CIRCLE";
    Circle() :radius(0.0) {}
    Circle(Point pCenter, double r) :pointCenter(pCenter), radius(r) {};
};

struct Arc 
{
    Point pointCenter;
    double radius;
    double startAngle, endAngle;
    std::string type = "ARC";
   // Arc() :radius(0.0), strrtAngle(0.0), endAngle2(0.0) {}
   Arc(Point pCenter, double r, double startAngle, double endAngle) 
       :pointCenter(pCenter), radius(r), startAngle(startAngle), endAngle(endAngle) {};
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