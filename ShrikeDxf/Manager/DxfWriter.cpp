#include "DxfWriter.h"

CDxfWriter::CDxfWriter(CDxfData* pData)
    : m_pData(pData)
{
}

bool CDxfWriter::SaveFile(const QString& strPath, DRW::Version ver, bool bin)
{
    if (!m_pData) return false;

    dxfRW dxf(strPath.toStdString().c_str());
    m_pDxfRW = &dxf;
    bool ok = dxf.write(this, ver, bin);
    m_pDxfRW = nullptr;
    return ok;
}


void CDxfWriter::writeHeader(DRW_Header& data)
{
    // 设置版本
    const QString& ver = m_pData->GetVersion();
    DRW::Version dv = DRW::AC1021;
    if (ver == "AC1009") dv = DRW::AC1009;
    else if (ver == "AC1012") dv = DRW::AC1012;
    else if (ver == "AC1014") dv = DRW::AC1014;
    else if (ver == "AC1015") dv = DRW::AC1015;
    else if (ver == "AC1018") dv = DRW::AC1018;
    else if (ver == "AC1021") dv = DRW::AC1021;
    else if (ver == "AC1024") dv = DRW::AC1024;
    else if (ver == "AC1027") dv = DRW::AC1027;
    else if (ver == "AC1032") dv = DRW::AC1032;

    data.addStr("$ACADVER", ver.toStdString(), 1);
    data.addInt("$INSUNITS", static_cast<int>(m_pData->GetInsUnits()), 70);

    Vertex3D extMin = m_pData->GetExtMin();
    data.addCoord("$EXTMIN", DRW_Coord(extMin.x(), extMin.y(), extMin.z()), 10);

    Vertex3D extMax = m_pData->GetExtMax();
    data.addCoord("$EXTMAX", DRW_Coord(extMax.x(), extMax.y(), extMax.z()), 10);

    data.addDouble("$LTSCALE", m_pData->GetLtScale(), 40);
}


void CDxfWriter::writeLTypes()
{
    
}

void CDxfWriter::writeLayers()
{
    for (const auto& [name, layer] : m_pData->GetLayers())
    {
        DRW_Layer drwLayer;
        drwLayer.name = name;
        drwLayer.color = DxfColorMap::getColorIndex(layer.color);
        drwLayer.lineType = layer.lineType.toStdString();
        drwLayer.flags = layer.isLocked ? 4 : 0;
        drwLayer.lWeight = DRW_LW_Conv::dwgInt2lineWidth(layer.lineWeight);

        if (!layer.isVisible)
            drwLayer.color = -drwLayer.color;   // 负数为关闭

        m_pDxfRW->writeLayer(&drwLayer);
    }
}

void CDxfWriter::writeTextstyles()
{
    
}

void CDxfWriter::writeVports()
{
    
}

void CDxfWriter::writeDimstyles()
{
    
}

void CDxfWriter::writeAppId()
{
    
}


void CDxfWriter::writeObjects()
{
    
}


void CDxfWriter::writeBlockRecords()
{
    // 注册块记录
    for (const auto& [name, block] : m_pData->GetDocument().blocks)
    {
        m_pDxfRW->writeBlockRecord(name);
    }
}

void CDxfWriter::writeBlocks()
{
    for (const auto& [name, block] : m_pData->GetDocument().blocks)
    {
        DRW_Block drwBlock;
        drwBlock.name = name;
        drwBlock.basePoint = DRW_Coord(block.basePoint.x(),
            block.basePoint.y(),
            block.basePoint.z());
        drwBlock.flags = block.flags;
        m_pDxfRW->writeBlock(&drwBlock);

        // 写入块内的图元
        for (const auto& entity : block.entities)
        {
            switch (GetEntityType(entity))
            {
            case EntityType::Point:
                WritePoint(std::get<EntityPoint>(entity));
                break;
            case EntityType::Line:
                WriteLine(std::get<EntityLine>(entity));
                break;
                
            default:
                break;
            }
        }
    }
}


void CDxfWriter::writeEntities()
{
    for (const auto& [layerName, layer] : m_pData->GetLayers())
    {
        if (!layer.isVisible) continue;

        for (const auto& entity : layer.entities)
        {
            switch (GetEntityType(entity))
            {
            case EntityType::Point:
                WritePoint(std::get<EntityPoint>(entity));
                break;
            case EntityType::Line:
                WriteLine(std::get<EntityLine>(entity));
                break;
            case EntityType::Circle:
                WriteCircle(std::get<EntityCircle>(entity));
                break;
            case EntityType::Arc:
                WriteArc(std::get<EntityArc>(entity));
                break;
            case EntityType::Ellipse:
                WriteEllipse(std::get<EntityEllipse>(entity));
                break;
            case EntityType::LWPolyline:
                WriteLWPolyline(std::get<EntityLWPolyline>(entity));
                break;
            case EntityType::Polyline:
                WritePolyline(std::get<EntityPolyline>(entity));
                break;
            case EntityType::Spline:
                WriteSpline(std::get<EntitySpline>(entity));
                break;
            case EntityType::Text:
                WriteText(std::get<EntityText>(entity));
                break;
            case EntityType::MText:
                WriteMText(std::get<EntityMText>(entity));
                break;
            case EntityType::Insert:
                WriteInsert(std::get<EntityInsert>(entity));
                break;
            case EntityType::Solid:
                WriteSolid(std::get<EntitySolid>(entity));
                break;
            case EntityType::Hatch:
                WriteHatch(std::get<EntityHatch>(entity));
                break;
            default:
                break;
            }
        }
    }
}



static void SetEntityCommon(DRW_Entity& drw, const EntityProp& prop)
{
    drw.layer = prop.layer;
    drw.lineType = prop.lineType;
    drw.color = prop.color;
    drw.ltypeScale = prop.ltypeScale;
    drw.visible = prop.visible;
    if (prop.color24 >= 0)
        drw.color24 = prop.color24;
    drw.lWeight = DRW_LW_Conv::dwgInt2lineWidth(prop.lineWeight);
}


void CDxfWriter::WritePoint(const EntityPoint& pt)
{
    DRW_Point drw;
    SetEntityCommon(drw, pt.prop);
    drw.basePoint = DRW_Coord(pt.point.x(), pt.point.y(), pt.point.z());
    m_pDxfRW->writePoint(&drw);
}


void CDxfWriter::WriteLine(const EntityLine& line)
{
    DRW_Line drw;
    SetEntityCommon(drw, line.prop);
    drw.basePoint = DRW_Coord(line.startPoint.x(), line.startPoint.y(), line.startPoint.z());
    drw.secPoint = DRW_Coord(line.endPoint.x(), line.endPoint.y(), line.endPoint.z());
    m_pDxfRW->writeLine(&drw);
}


void CDxfWriter::WriteCircle(const EntityCircle& circle)
{
    DRW_Circle drw;
    SetEntityCommon(drw, circle.prop);
    drw.basePoint = DRW_Coord(circle.center.x(), circle.center.y(), circle.center.z());
    drw.radious = circle.radius;
    m_pDxfRW->writeCircle(&drw);
}


void CDxfWriter::WriteArc(const EntityArc& arc)
{
    DRW_Arc drw;
    SetEntityCommon(drw, arc.prop);
    drw.basePoint = DRW_Coord(arc.center.x(), arc.center.y(), arc.center.z());
    drw.radious = arc.radius;
    drw.staangle = arc.startAngle;
    drw.endangle = arc.endAngle;
    drw.isccw = arc.isCCW ? 1 : 0;
    m_pDxfRW->writeArc(&drw);
}

void CDxfWriter::WriteEllipse(const EntityEllipse& ellipse)
{
    DRW_Ellipse drw;
    SetEntityCommon(drw, ellipse.prop);
    drw.basePoint = DRW_Coord(ellipse.center.x(), ellipse.center.y(), ellipse.center.z());
    drw.secPoint = DRW_Coord(ellipse.majorAxisEndpoint.x(),
        ellipse.majorAxisEndpoint.y(),
        ellipse.majorAxisEndpoint.z());
    drw.ratio = ellipse.ratio;
    drw.staparam = ellipse.startParam;
    drw.endparam = ellipse.endParam;
    m_pDxfRW->writeEllipse(&drw);
}

void CDxfWriter::WriteLWPolyline(const EntityLWPolyline& polyline)
{
    DRW_LWPolyline drw;
    SetEntityCommon(drw, polyline.prop);
    drw.flags = polyline.flags;
    drw.width = polyline.constantWidth;
    drw.elevation = polyline.elevation;

    for (const auto& v : polyline.vecVertices)
    {
        DRW_Vertex2D vert;
        vert.x = v.point.x();
        vert.y = v.point.y();
        vert.bulge = v.bulge;
        vert.stawidth = v.startWidth;
        vert.endwidth = v.endWidth;
        drw.vertlist.push_back(std::make_shared<DRW_Vertex2D>(vert));
    }

    m_pDxfRW->writeLWPolyline(&drw);
}


void CDxfWriter::WritePolyline(const EntityPolyline& polyline)
{
    DRW_Polyline drw;
    SetEntityCommon(drw, polyline.prop);
    drw.flags = polyline.flags;
    drw.defstawidth = polyline.defStartWidth;
    drw.defendwidth = polyline.defEndWidth;
    drw.vertexcount = polyline.vertexCount;

    for (const auto& v : polyline.vecVertices)
    {
        DRW_Vertex vert;
        vert.basePoint = DRW_Coord(v.point.x(), v.point.y(), v.point.z());
        vert.bulge = v.bulge;
        vert.stawidth = v.startWidth;
        vert.endwidth = v.endWidth;
        vert.flags = v.vertexFlags;
        drw.vertlist.push_back(std::make_shared<DRW_Vertex>(vert));
    }

    m_pDxfRW->writePolyline(&drw);
}

void CDxfWriter::WriteSpline(const EntitySpline& spline)
{
    DRW_Spline drw;
    SetEntityCommon(drw, spline.prop);
    drw.flags = spline.flags;
    drw.degree = spline.degree;
    drw.nknots = static_cast<dint32>(spline.knots.size());
    drw.ncontrol = static_cast<dint32>(spline.controlPoints.size());
    drw.nfit = static_cast<dint32>(spline.fitPoints.size());
    drw.tolknot = spline.knotTolerance;
    drw.tolcontrol = spline.controlTolerance;
    drw.tolfit = spline.fitTolerance;
    // 法向量
    if (spline.normalVec != Vertex3D())
        drw.normalVec = DRW_Coord(spline.normalVec.x(),
            spline.normalVec.y(),
            spline.normalVec.z());
    // 节点矢量
    for (double k : spline.knots)
        drw.knotslist.push_back(k);
    // 权重
    for (double w : spline.weights)
        drw.weightlist.push_back(w);
    // 控制点
    for (const auto& cp : spline.controlPoints)
        drw.controllist.push_back(std::make_shared<DRW_Coord>(cp.x(), cp.y(), cp.z()));
    // 拟合点
    for (const auto& fp : spline.fitPoints)
        drw.fitlist.push_back(std::make_shared<DRW_Coord>(fp.x(), fp.y(), fp.z()));
    // 切向量
    if (spline.tgStart != Vertex3D())
        drw.tgStart = DRW_Coord(spline.tgStart.x(), spline.tgStart.y(), spline.tgStart.z());
    if (spline.tgEnd != Vertex3D())
        drw.tgEnd = DRW_Coord(spline.tgEnd.x(), spline.tgEnd.y(), spline.tgEnd.z());
    m_pDxfRW->writeSpline(&drw);
}


void CDxfWriter::WriteText(const EntityText& text)
{
    DRW_Text drw;
    SetEntityCommon(drw, text.prop);
    drw.basePoint = DRW_Coord(text.insertPoint.x(), text.insertPoint.y(), text.insertPoint.z());
    drw.secPoint = DRW_Coord(text.alignPoint.x(), text.alignPoint.y(), text.alignPoint.z());
    drw.text = text.text;
    drw.height = text.height;
    drw.angle = text.rotation;
    drw.widthscale = text.widthFactor;
    drw.oblique = text.obliqueAngle;
    drw.style = text.style;
    drw.textgen = text.textGen;
    drw.alignH = static_cast<DRW_Text::HAlign>(text.alignH);
    drw.alignV = static_cast<DRW_Text::VAlign>(text.alignV);
    m_pDxfRW->writeText(&drw);
}


void CDxfWriter::WriteMText(const EntityMText& mtext)
{
    DRW_MText drw;
    SetEntityCommon(drw, mtext.prop);
    drw.basePoint = DRW_Coord(mtext.insertPoint.x(), mtext.insertPoint.y(), mtext.insertPoint.z());
    drw.secPoint = DRW_Coord(mtext.xAxisDir.x(), mtext.xAxisDir.y(), mtext.xAxisDir.z());
    drw.text = mtext.text;
    drw.height = mtext.height;
    drw.angle = mtext.rotation * 180.0 / M_PI;
    drw.widthscale = mtext.widthFactor;
    drw.interlin = mtext.lineSpacing;
    drw.style = mtext.style;
    drw.alignV = static_cast<DRW_Text::VAlign>(mtext.attachPoint);
    drw.textgen = 1;
    m_pDxfRW->writeMText(&drw);
}


void CDxfWriter::WriteInsert(const EntityInsert& insert)
{
    DRW_Insert drw;
    SetEntityCommon(drw, insert.prop);
    drw.basePoint = DRW_Coord(insert.insertPoint.x(), insert.insertPoint.y(), insert.insertPoint.z());
    drw.name = insert.blockName;
    drw.xscale = insert.xScale;
    drw.yscale = insert.yScale;
    drw.zscale = insert.zScale;
    drw.angle = insert.rotation;    
    drw.colcount = insert.colCount;     
    drw.rowcount = insert.rowCount; 
    drw.colspace = insert.colSpacing;
    drw.rowspace = insert.rowSpacing;
    m_pDxfRW->writeInsert(&drw);
}


void CDxfWriter::WriteSolid(const EntitySolid& solid)
{
    DRW_Solid drw;
    SetEntityCommon(drw, solid.prop);
    drw.basePoint = DRW_Coord(solid.corner[0].x(), solid.corner[0].y(), solid.corner[0].z());
    drw.secPoint = DRW_Coord(solid.corner[1].x(), solid.corner[1].y(), solid.corner[1].z());
    drw.thirdPoint = DRW_Coord(solid.corner[2].x(), solid.corner[2].y(), solid.corner[2].z());
    drw.fourPoint = DRW_Coord(solid.corner[3].x(), solid.corner[3].y(), solid.corner[3].z());
    m_pDxfRW->writeSolid(&drw);
}


void CDxfWriter::WriteHatch(const EntityHatch& hatch)
{
    DRW_Hatch drw;
    SetEntityCommon(drw, hatch.prop);
    drw.name = hatch.patternName;
    drw.solid = hatch.solidFill ? 1 : 0;
    drw.associative = hatch.associative;
    drw.hstyle = hatch.style;    
    drw.hpattern = hatch.patternType;
    drw.doubleflag = hatch.doubleFlag;
    drw.angle = hatch.angle;
    drw.scale = hatch.scale;
    drw.loopsnum = hatch.loopCount;
    // 转换边界环
    for (const auto& loop : hatch.loops)
    {
        auto drwLoop = std::make_shared<DRW_HatchLoop>(loop.type);
        drwLoop->numedges = 0;
        if (loop.isPolyline)
        {
            // 多段线边界
            drwLoop->type |= 2;
            // 多段线顶点存成 LWPolyline
            auto pl = std::make_shared<DRW_LWPolyline>();
            for (const auto& pt : loop.polylinePath)
            {
                DRW_Vertex2D vert;
                vert.x = pt.x();
                vert.y = pt.y();
                pl->vertlist.push_back(std::make_shared<DRW_Vertex2D>(vert));
            }
            drwLoop->objlist.push_back(pl);
            drwLoop->numedges = (int)loop.polylinePath.size();
        }
        else
        {
            // 边边界
            for (const auto& edge : loop.edges)
            {
                if (std::holds_alternative<HatchEdgeLine>(edge))
                {
                    const auto& e = std::get<HatchEdgeLine>(edge);
                    auto ln = std::make_shared<DRW_Line>();
                    ln->basePoint = DRW_Coord(e.start.x(), e.start.y(), 0.0);
                    ln->secPoint = DRW_Coord(e.end.x(), e.end.y(), 0.0);
                    drwLoop->objlist.push_back(ln);
                    drwLoop->numedges++;
                }
                else if (std::holds_alternative<HatchEdgeArc>(edge))
                {
                    const auto& e = std::get<HatchEdgeArc>(edge);
                    auto arc = std::make_shared<DRW_Arc>();
                    arc->basePoint = DRW_Coord(e.center.x(), e.center.y(), 0.0);
                    arc->radious = e.radius;
                    arc->staangle = e.startAngle;
                    arc->endangle = e.endAngle;
                    arc->isccw = e.isCCW ? 1 : 0;
                    drwLoop->objlist.push_back(arc);
                    drwLoop->numedges++;
                }
                else if (std::holds_alternative<HatchEdgeEllipse>(edge))
                {
                    const auto& e = std::get<HatchEdgeEllipse>(edge);
                    auto el = std::make_shared<DRW_Ellipse>();
                    el->basePoint = DRW_Coord(e.center.x(), e.center.y(), 0.0);
                    el->secPoint = DRW_Coord(e.majorAxisEndpoint.x(), e.majorAxisEndpoint.y(), 0.0);
                    el->ratio = e.ratio;
                    el->staparam = e.startParam;
                    el->endparam = e.endParam;
                    drwLoop->objlist.push_back(el);
                    drwLoop->numedges++;
                }
                else if (std::holds_alternative<HatchEdgeSpline>(edge))
                {
                    const auto& e = std::get<HatchEdgeSpline>(edge);
                    auto sp = std::make_shared<DRW_Spline>();
                    sp->flags = 0;
                    sp->degree = e.degree;
                    sp->nknots = (int)e.knots.size();
                    sp->ncontrol = (int)e.controlPoints.size();
                    for (double k : e.knots)
                        sp->knotslist.push_back(k);
                    for (const auto& cp : e.controlPoints)
                        sp->controllist.push_back(std::make_shared<DRW_Coord>(cp.x(), cp.y(), cp.z()));
                    drwLoop->objlist.push_back(sp);
                    drwLoop->numedges++;
                }
            }
        }
        drw.looplist.push_back(drwLoop);
    }
    m_pDxfRW->writeHatch(&drw);
}
