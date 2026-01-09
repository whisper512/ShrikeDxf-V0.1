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

	
public:

    // 存放dxf文件中所有的数据
    // 图层名 图层数据
    map<string, stuLayer> m_mapDxfEntities;



public:
    //dxflib中的回调函数

    // 处理图层
    virtual void addLayer(const DL_LayerData& data) override;

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

    // 处理文本
    virtual void addText(const DL_TextData & data) override;

    //清空数据
    void ClearDxfData();
   
    
public:
    //获取entity数据
    QString GetEntityInfo(QString strLayer, QString strType,QString strNum);
    //获取特定的entity
    DxfEntity GetEntity(QString strLayer,QString strType,QString strNum);
    //删除entity
    int DeleteEntity(QString strLayer, QString strType, QString strNum);
	
private:
    //当前多段线信息
    Polyline* m_pCurPolyline;
};

#endif // DXF_MAPPING_H
