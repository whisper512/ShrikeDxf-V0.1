#pragma once
#ifndef DXF_MAPPING_H
#define DXF_MAPPING_H
#include <iostream>
#include <QString>
#include <vector>
#include <QPointF>

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
    //复制操作临时存储的图元
    variantDxfEntity m_CopyingEntity;
    //复制操作临时存储的图元所在图层
    QString m_strCopyingLayer;
    //tree当前选择的entity
    stuSelectedEntity m_SelectedEntity;
    //获取variant中存储的数据和类型
    enumEntity GetVariantDxfEntity(variantDxfEntity dxfEntity,Point& point, Line& line, Circle& circle, Arc& arc, Polyline& polyline);

    double m_dMoveStep = 1.0;
    double m_dRotateStepRAD = 1.0;


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
    variantDxfEntity GetEntity(QString strLayer,QString strType,QString strNum);
    //保存选择的entity
    void SaveSelectedEntity(QString strLayer, QString strType, QString strNum);
    //删除entity
    int DeleteEntity(QString strLayer, QString strType, QString strNum);
	//保存复制entity
    int SaveCopyingEntity(QString strLayer, QString strType, QString strNum);
    //粘贴entity
    int PasteEntity(QPointF pos);

    void MoveUpSelectedEntity();
    void MoveDownSelectedEntity();
    void MoveLeftSelectedEntity();
    void MoveRightSelectedEntity();
    void RotateCWSelectedEntity();
    void RotateCCWSelectedEntity();

    //修改选中point属性
    void ChangePointProperty(Point point);
    //修改选中line属性
    void ChangeLineProperty(Line line);
    //修改选中circle属性
    void ChangeCircleProperty(Circle circle);
    //修改选中arc属性
    void ChangeArcProperty(Arc arc);
    //修改选中polyline属性
    void ChangePolylineProperty(Polyline polyline);

private:
    //当前多段线,用来读取dxf文档中的多段线
    Polyline* m_pCurPolyline;
   
};

#endif // DXF_MAPPING_H
