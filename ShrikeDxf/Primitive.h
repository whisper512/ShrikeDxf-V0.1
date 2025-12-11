#pragma once
#ifndef PRIMITIVE_H
#define PRIMITIVE_H

//Dxf映射类中的图元

struct Point
{
	double x;
	double y;
	double z;
	Point() :x(0), y(0), z(0) {}
	Point(double x, double y, double z) :x(x), y(y), z(z) {};
};


#endif // PRIMITIVE_H