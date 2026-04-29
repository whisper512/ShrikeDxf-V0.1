#pragma once
#ifndef DXF_MAPPING_H
#define DXF_MAPPING_H
#include <iostream>
#include <QString>
#include <vector>
#include <QPointF>
//dxflib库
#include "dl_dxf.h"
#include "dl_creationadapter.h"

#include "Primitive.h"
#include "DxfStruct.h"
//libdxfrw库
#include "libdxfrw.h"

using namespace std;

//dxf文件的映射类,继承DRW_Interface来读取dxf文件

class CDxfMapping : public DRW_Interface
{
public:
	CDxfMapping();
	~CDxfMapping();

	
public:

    // 存放dxf文件中所有的数据
    // 图层名 图层数据
    map<string, stuLayer> m_mapDxfEntities;
    //tree当前选择的entity
    stuSelectedEntity m_SelectedEntity;

    //移动的步长
    double m_dMoveStep = 1.0;
    double m_dRotateStepRAD = qDegreesToRadians(1.0);



public:
    /** Called when header is parsed.  */
    virtual void addHeader(const DRW_Header* data) override;

    /** Called for every line Type.  */
    virtual void addLType(const DRW_LType& data) override;
    /** Called for every layer. */
    virtual void addLayer(const DRW_Layer& data) override;
    /** Called for every dim style. */
    virtual void addDimStyle(const DRW_Dimstyle& data) override;
    /** Called for every VPORT table. */
    virtual void addVport(const DRW_Vport& data) override;
    /** Called for every text style. */
    virtual void addTextStyle(const DRW_Textstyle& data) override;
    /** Called for every AppId entry. */
    virtual void addAppId(const DRW_AppId& data) override;

    /**
     * Called for every block. Note: all entities added after this
     * command go into this block until endBlock() is called.
     *
     * @see endBlock()
     */
    virtual void addBlock(const DRW_Block& data) override;

    /**
     * In DWG called when the following entities corresponding to a
     * block different from the current. Note: all entities added after this
     * command go into this block until setBlock() is called already.
     *
     * int handle are the value of DRW_Block::handleBlock added with addBlock()
     */
    virtual void setBlock(const int handle) override;

    /** Called to end the current block */
    virtual void endBlock() override;

    /** Called for every point */
    virtual void addPoint(const DRW_Point& data) override;

    /** Called for every line */
    virtual void addLine(const DRW_Line& data) override;

    /** Called for every ray */
    virtual void addRay(const DRW_Ray& data) override;

    /** Called for every xline */
    virtual void addXline(const DRW_Xline& data) override;

    /** Called for every arc */
    virtual void addArc(const DRW_Arc& data) override;

    /** Called for every circle */
    virtual void addCircle(const DRW_Circle& data) override;

    /** Called for every ellipse */
    virtual void addEllipse(const DRW_Ellipse& data) override;

    /** Called for every lwpolyline */
    virtual void addLWPolyline(const DRW_LWPolyline& data) override;

    /** Called for every polyline start */
    virtual void addPolyline(const DRW_Polyline& data) override;

    /** Called for every spline */
    virtual void addSpline(const DRW_Spline* data) override;

    /** Called for every spline knot value */
    virtual void addKnot(const DRW_Entity& data) override;

    /** Called for every insert. */
    virtual void addInsert(const DRW_Insert& data) override;

    /** Called for every trace start */
    virtual void addTrace(const DRW_Trace& data) override;

    /** Called for every 3dface start */
    virtual void add3dFace(const DRW_3Dface& data) override;

    /** Called for every solid start */
    virtual void addSolid(const DRW_Solid& data) override;


    /** Called for every Multi Text entity. */
    virtual void addMText(const DRW_MText& data) override;

    /** Called for every Text entity. */
    virtual void addText(const DRW_Text& data) override;

    /**
     * Called for every aligned dimension entity.
     */
    virtual void addDimAlign(const DRW_DimAligned* data) override;
    /**
     * Called for every linear or rotated dimension entity.
     */
    virtual void addDimLinear(const DRW_DimLinear* data) override;

    /**
     * Called for every radial dimension entity.
     */
    virtual void addDimRadial(const DRW_DimRadial* data) override;

    /**
     * Called for every diametric dimension entity.
     */
    virtual void addDimDiametric(const DRW_DimDiametric* data) override;

    /**
     * Called for every angular dimension (2 lines version) entity.
     */
    virtual void addDimAngular(const DRW_DimAngular* data) override;

    /**
     * Called for every angular dimension (3 points version) entity.
     */
    virtual void addDimAngular3P(const DRW_DimAngular3p* data) override;

    /**
     * Called for every ordinate dimension entity.
     */
    virtual void addDimOrdinate(const DRW_DimOrdinate* data) override;

    /**
     * Called for every leader start.
     */
    virtual void addLeader(const DRW_Leader* data) override;

    /**
     * Called for every hatch entity.
     */
    virtual void addHatch(const DRW_Hatch* data) override;

    /**
     * Called for every viewport entity.
     */
    virtual void addViewport(const DRW_Viewport& data) override;

    /**
     * Called for every image entity.
     */
    virtual void addImage(const DRW_Image* data) override;

    /**
     * Called for every image definition.
     */
    virtual void linkImage(const DRW_ImageDef* data) override;

    /**
     * Called for every comment in the DXF file (code 999).
     */
    virtual void addComment(const char* comment) override;

    /**
     * Called for PLOTSETTINGS object definition.
     */
    virtual void addPlotSettings(const DRW_PlotSettings* data) override;

    virtual void writeHeader(DRW_Header& data) override;
    virtual void writeBlocks() override;
    virtual void writeBlockRecords() override;
    virtual void writeEntities() override;
    virtual void writeLTypes() override;
    virtual void writeLayers() override;
    virtual void writeTextstyles() override;
    virtual void writeVports() override;
    virtual void writeDimstyles() override;
    virtual void writeObjects() override;
    virtual void writeAppId() override;



    //清空数据
    void ClearDxfData();
   
    
public:
    //获取variant中存储的数据和类型
    enumEntity GetVariantDxfEntity(variantDxfEntity dxfEntity, Point& point, Line& line, Circle& circle, Arc& arc, Polyline& polyline);
    //获取entity数据
    QString GetEntityInfo(QString strLayer, QString strType,QString strNum);
    //获取特定的entity
    variantDxfEntity GetEntity(QString strLayer,QString strType,QString strNum);
    //保存选择的entity
    void SaveSelectedEntity(QString strLayer, QString strType, QString strNum);
    //删除entity
    int DeleteEntity(QString strLayer, QString strType, QString strNum);
    //粘贴entity
    int PasteEntity(QPointF pos);

    //修改图层属性
    void ChangeLayerProperty();
    //移动图元
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

public:
    void addPointToLayer(Point point ,QString strLayer);
    void addLineToLayer(Line line, QString strLayer);
    void addCircleToLayer(Circle circle, QString strLayer);
    void addArcToLayer(Arc arc, QString strLayer);
    void addPolylineToLayer(QVector<QPointF>& m_vecPoints, QString strLayer);
private:
   
};

#endif // DXF_MAPPING_H
