#pragma once
#ifndef DXF_MAPPING_H
#define DXF_MAPPING_H
#include <iostream>
#include <QString>
#include <vector>

#include "dl_dxf.h"
#include "dl_creationadapter.h"
#include "Primitive.h"
#include "DxfStruct.h"
using namespace std;

//dxf文件的映射类,继承DL_CreationAdapter来读取dxf文件

class CDxfMapping : public DL_CreationAdapter
{
public:
	CDxfMapping();
	~CDxfMapping();

	
private:
	CDxfStruct m_DxfStruct;
	DL_Attributes currentAttrib;
	Polyline currentPolyline;
	bool inPolyline = false;


public:

    //virtual void setAttributes(const DL_Attributes& attrib) override 
    //{
    //    currentAttrib = attrib;
    //}

    // 处理点
    virtual void addPoint(const DL_PointData& data) override;

    // 处理直线
    virtual void addLine(const DL_LineData& data) override;

    // 处理圆
    virtual void addCircle(const DL_CircleData& data) override;

    // 处理圆弧
    virtual void addArc(const DL_ArcData& data) override;

    // 处理多段线
    virtual void addPolyline(const DL_PolylineData& data) override;

    // 处理多段线顶点
    virtual void addVertex(const DL_VertexData& data) override;

    virtual void endEntity() override;

    // 处理文本
    virtual void addText(const DL_TextData & data) override;

	
private:

	
};

#endif // DXF_MAPPING_H
