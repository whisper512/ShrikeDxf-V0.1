#pragma once
#ifndef DxfStruct_h
#define DxfStruct_h


#include <iostream>
#include <QString>
#include <vector>
#include <map>

#include "Primitive.h"

//存储dxf文件数据结构

using namespace std;

//层图实体数据
using DxfEntity = std::variant<Point, Line, Circle, Arc, Polyline, Text>;

//图层类
struct stuLayer
{
    //QColor color;

	vector<Point> vecPoints;
    vector<Line> vecLines;
    vector<Circle> vecCircles;
    vector<Arc> vecArcs;
    vector<Polyline> vecPolylines;
    vector<Text> vecTexts;

};

//图层类,图元不做区分
struct stuLayoutMix
{
	vector<DxfEntity> vecEntities;
};

class CDxfStruct
{
public:
	CDxfStruct();
	~CDxfStruct();

private:




public:

};



#endif // DxfStruct_h