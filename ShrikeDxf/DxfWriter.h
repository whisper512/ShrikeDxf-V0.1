#pragma once

#include <QString>
#include <string>
#include <map>
#include <vector>

#include "DxfData.h"
#include "../EntitiesLib/Entity.h"
#include "drw_interface.h"
#include "libdxfrw.h"

/**
 * @brief DXF 文件写入类（使用 libdxfrw）
 *
 * 继承 DRW_Interface，与 CDxfReader 对称。
 * 内部持有 dxfRW 指针，利用其 write*() 方法输出各图元。
 */
class CDxfWriter : public DRW_Interface
{
public:
    explicit CDxfWriter(CDxfData* pData);
    ~CDxfWriter() override = default;

    bool SaveFile(const QString& strPath, DRW::Version ver = DRW::AC1021, bool bin = false);

private:
    // ======== DRW_Interface 回调 ========

    // 读取回调
    void addHeader(const DRW_Header* data) override {}
    void addLType(const DRW_LType& data) override {}
    void addLayer(const DRW_Layer& data) override {}
    void addDimStyle(const DRW_Dimstyle& data) override {}
    void addVport(const DRW_Vport& data) override {}
    void addTextStyle(const DRW_Textstyle& data) override {}
    void addAppId(const DRW_AppId& data) override {}
    void addBlock(const DRW_Block& data) override {}
    void setBlock(const int handle) override {}
    void endBlock() override {}
    void addPoint(const DRW_Point& data) override {}
    void addLine(const DRW_Line& data) override {}
    void addRay(const DRW_Ray& data) override {}
    void addXline(const DRW_Xline& data) override {}
    void addCircle(const DRW_Circle& data) override {}
    void addArc(const DRW_Arc& data) override {}
    void addEllipse(const DRW_Ellipse& data) override {}
    void addLWPolyline(const DRW_LWPolyline& data) override {}
    void addPolyline(const DRW_Polyline& data) override {}
    void addSpline(const DRW_Spline* data) override {}
    void addKnot(const DRW_Entity& data) override {}
    void addInsert(const DRW_Insert& data) override {}
    void addTrace(const DRW_Trace& data) override {}
    void add3dFace(const DRW_3Dface& data) override {}
    void addSolid(const DRW_Solid& data) override {}
    void addMText(const DRW_MText& data) override {}
    void addText(const DRW_Text& data) override {}
    void addDimAlign(const DRW_DimAligned* data) override {}
    void addDimLinear(const DRW_DimLinear* data) override {}
    void addDimRadial(const DRW_DimRadial* data) override {}
    void addDimDiametric(const DRW_DimDiametric* data) override {}
    void addDimAngular(const DRW_DimAngular* data) override {}
    void addDimAngular3P(const DRW_DimAngular3p* data) override {}
    void addDimOrdinate(const DRW_DimOrdinate* data) override {}
    void addLeader(const DRW_Leader* data) override {}
    void addHatch(const DRW_Hatch* data) override {}
    void addViewport(const DRW_Viewport& data) override {}
    void addImage(const DRW_Image* data) override {}
    void linkImage(const DRW_ImageDef* data) override {}
    void addComment(const char* comment) override {}
    void addPlotSettings(const DRW_PlotSettings* data) override {}

    // ======== 写入回调 ========
    void writeHeader(DRW_Header& data) override;
    void writeBlocks() override;
    void writeBlockRecords() override;
    void writeEntities() override;
    void writeLTypes() override;
    void writeLayers() override;
    void writeTextstyles() override;
    void writeVports() override;
    void writeDimstyles() override;
    void writeObjects() override;
    void writeAppId() override;

private:
    // ---- 图元转写辅助 ----
    void WritePoint(const EntityPoint& pt);
    void WriteLine(const EntityLine& line);
    void WriteCircle(const EntityCircle& circle);
    void WriteArc(const EntityArc& arc);
    void WriteEllipse(const EntityEllipse& ellipse);
    void WriteLWPolyline(const EntityLWPolyline& polyline);
    void WritePolyline(const EntityPolyline& polyline);
    void WriteSpline(const EntitySpline& spline);
    void WriteText(const EntityText& text);
    void WriteMText(const EntityMText& mtext);
    void WriteInsert(const EntityInsert& insert);
    void WriteSolid(const EntitySolid& solid);
    void WriteHatch(const EntityHatch& hatch);

    CDxfData* m_pData = nullptr;
    dxfRW* m_pDxfRW = nullptr;   // 写入时由 SaveFile 传入
};
