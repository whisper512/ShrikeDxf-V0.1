#pragma once
#ifndef DXF_MAPPING_H
#define DXF_MAPPING_H
#include <iostream>
#include <QString>
#include <vector>

#include "dl_dxf.h"
#include "dl_creationadapter.h"
#include "Primitive.h"

using namespace std;

//dxf文件的映射类

class CDxfMapping : public DL_CreationAdapter
{
public:
	CDxfMapping();
	~CDxfMapping();

	
private:
	vector<Point> m_vecPoints;
public:
	//重写DL_CreationAdapter中的中的函数
	//每次解析到point会执行addPoint函数
	int GetPointCount();
	virtual void addPoint(const DL_PointData& data) override;
	
private:

	
};




#endif // DXF_MAPPING_H
