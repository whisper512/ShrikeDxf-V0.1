#include "DxfWriter.h"

DxfWriter::DxfWriter(DxfData* data)
    : m_data(data)
{
}

bool DxfWriter::saveFile(const QString& path, DRW::Version ver, bool bin)
{
    if (!m_data) return false;

    dxfRW dxf(path.toStdString().c_str());
    m_dxfRW = &dxf;
    bool ok = dxf.write(this, ver, bin);
    m_dxfRW = nullptr;
    return ok;
}


void DxfWriter::writeHeader(DRW_Header& data)
{
    // 设置版本
    const QString& ver = m_data->getVersion();
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
    data.addInt("$INSUNITS", static_cast<int>(m_data->getInsUnits()), 70);

    Vertex3D extMin = m_data->getExtMin();
    data.addCoord("$EXTMIN", DRW_Coord(extMin.x(), extMin.y(), extMin.z()), 10);

    Vertex3D extMax = m_data->getExtMax();
    data.addCoord("$EXTMAX", DRW_Coord(extMax.x(), extMax.y(), extMax.z()), 10);

    data.addDouble("$LTSCALE", m_data->getLtScale(), 40);
}


void DxfWriter::writeLTypes()
{
    
}

void DxfWriter::writeLayers()
{
    for (const auto& [name, layer] : m_data->getLayers())
    {
        DRW_Layer drwLayer;
        drwLayer.name = name;
        drwLayer.color = DxfColorMap::getColorIndex(layer.color);
        drwLayer.lineType = layer.lineType.toStdString();
        drwLayer.flags = layer.isLocked ? 4 : 0;
        drwLayer.lWeight = DRW_LW_Conv::dwgInt2lineWidth(layer.lineWeight);

        if (!layer.isVisible)
            drwLayer.color = -drwLayer.color;   // 负数为关闭

        m_dxfRW->writeLayer(&drwLayer);
    }
}

void DxfWriter::writeTextstyles()
{
    
}

void DxfWriter::writeVports()
{
    
}

void DxfWriter::writeDimstyles()
{
    
}

void DxfWriter::writeAppId()
{
    
}


void DxfWriter::writeObjects()
{
    
}


void DxfWriter::writeBlockRecords()
{
    // 注册块记录
    for (const auto& [name, block] : m_data->getDocument().blocks)
    {
        m_dxfRW->writeBlockRecord(name);
    }
}

void DxfWriter::writeBlocks()
{
    for (const auto& [name, block] : m_data->getDocument().blocks)
    {
        DRW_Block drwBlock;
        drwBlock.name = name;
        drwBlock.basePoint = DRW_Coord(block.basePoint.x(),
            block.basePoint.y(),
            block.basePoint.z());
        drwBlock.flags = block.flags;
        m_dxfRW->writeBlock(&drwBlock);

        // 写入块内的图元
        for (const auto& entity : block.entities)
        {
            switch (GetEntityType(entity))
            {
            case EntityType::Point:
                writePoint(std::get<EntityPoint>(entity));
                break;
            case EntityType::Line:
                writeLine(std::get<EntityLine>(entity));
                break;
                
            default:
                break;
            }
        }
    }
}


void DxfWriter::writeEntities()
{
    for (const auto& [layerName, layer] : m_data->getLayers())
    {
        if (!layer.isVisible) continue;

        for (const auto& entity : layer.entities)
        {
            switch (GetEntityType(entity))
            {
            case EntityType::Point:
                writePoint(std::get<EntityPoint>(entity));
                break;
            case EntityType::Line:
                writeLine(std::get<EntityLine>(entity));
                break;
            case EntityType::Circle:
                writeCircle(std::get<EntityCircle>(entity));
                break;
            case EntityType::Arc:
                writeArc(std::get<EntityArc>(entity));
                break;
            case EntityType::Ellipse:
                writeEllipse(std::get<EntityEllipse>(entity));
                break;
            case EntityType::LWPolyline:
                writeLWPolyline(std::get<EntityLWPolyline>(entity));
                break;
            case EntityType::Polyline:
                writePolyline(std::get<EntityPolyline>(entity));
                break;
            case EntityType::Spline:
                writeSpline(std::get<EntitySpline>(entity));
                break;
            case EntityType::Text:
                writeText(std::get<EntityText>(entity));
                break;
            case EntityType::MText:
                writeMText(std::get<EntityMText>(entity));
                break;
            case EntityType::Insert:
                writeInsert(std::get<EntityInsert>(entity));
                break;
            case EntityType::Solid:
                writeSolid(std::get<EntitySolid>(entity));
                break;
            case EntityType::Hatch:
                writeHatch(std::get<EntityHatch>(entity));
                break;
            default:
                break;
            }
        }
    }
}



static void setEntityCommon(DRW_Entity& drw, const EntityProp& prop)
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


void DxfWriter::writePoint(const EntityPoint& pt)
{
    DRW_Point drw;
    setEntityCommon(drw, pt.prop);
    drw.basePoint = DRW_Coord(pt.point.x(), pt.point.y(), pt.point.z());
    m_dxfRW->writePoint(&drw);
}


void DxfWriter::writeLine(const EntityLine& line)
{
    DRW_Line drw;
    setEntityCommon(drw, line.prop);
    drw.basePoint = DRW_Coord(line.startPoint.x(), line.startPoint.y(), line.startPoint.z());
    drw.secPoint = DRW_Coord(line.endPoint.x(), line.endPoint.y(), line.endPoint.z());
    m_dxfRW->writeLine(&drw);
}


void DxfWriter::writeCircle(const EntityCircle& circle)
{
    DRW_Circle drw;
    setEntityCommon(drw, circle.prop);
    drw.basePoint = DRW_Coord(circle.center.x(), circle.center.y(), circle.center.z());
    drw.radious = circle.radius;
    m_dxfRW->writeCircle(&drw);
}


void DxfWriter::writeArc(const EntityArc& arc)
{
    DRW_Arc drw;
    setEntityCommon(drw, arc.prop);
    drw.basePoint = DRW_Coord(arc.center.x(), arc.center.y(), arc.center.z());
    drw.radious = arc.radius;
    drw.staangle = arc.startAngle;
    drw.endangle = arc.endAngle;
    drw.isccw = arc.isCCW ? 1 : 0;
    m_dxfRW->writeArc(&drw);
}

void DxfWriter::writeEllipse(const EntityEllipse& ellipse)
{
    DRW_Ellipse drw;
    setEntityCommon(drw, ellipse.prop);
    drw.basePoint = DRW_Coord(ellipse.center.x(), ellipse.center.y(), ellipse.center.z());
    drw.secPoint = DRW_Coord(ellipse.majorAxisEndpoint.x(),
        ellipse.majorAxisEndpoint.y(),
        ellipse.majorAxisEndpoint.z());
    drw.ratio = ellipse.ratio;
    drw.staparam = ellipse.startParam;
    drw.endparam = ellipse.endParam;
    m_dxfRW->writeEllipse(&drw);
}

void DxfWriter::writeLWPolyline(const EntityLWPolyline& polyline)
{
    DRW_LWPolyline drw;
    setEntityCommon(drw, polyline.prop);
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

    m_dxfRW->writeLWPolyline(&drw);
}


void DxfWriter::writePolyline(const EntityPolyline& polyline)
{
    DRW_Polyline drw;
    setEntityCommon(drw, polyline.prop);
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

    m_dxfRW->writePolyline(&drw);
}

void DxfWriter::writeSpline(const EntitySpline& spline)
{
    DRW_Spline drw;
    setEntityCommon(drw, spline.prop);
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
    m_dxfRW->writeSpline(&drw);
}


void DxfWriter::writeText(const EntityText& text)
{
    DRW_Text drw;
    setEntityCommon(drw, text.prop);
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
    m_dxfRW->writeText(&drw);
}


void DxfWriter::writeMText(const EntityMText& mtext)
{
    DRW_MText drw;
    setEntityCommon(drw, mtext.prop);
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
    m_dxfRW->writeMText(&drw);
}


void DxfWriter::writeInsert(const EntityInsert& insert)
{
    DRW_Insert drw;
    setEntityCommon(drw, insert.prop);
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
    m_dxfRW->writeInsert(&drw);
}


void DxfWriter::writeSolid(const EntitySolid& solid)
{
    DRW_Solid drw;
    setEntityCommon(drw, solid.prop);
    drw.basePoint = DRW_Coord(solid.corner[0].x(), solid.corner[0].y(), solid.corner[0].z());
    drw.secPoint = DRW_Coord(solid.corner[1].x(), solid.corner[1].y(), solid.corner[1].z());
    drw.thirdPoint = DRW_Coord(solid.corner[2].x(), solid.corner[2].y(), solid.corner[2].z());
    drw.fourPoint = DRW_Coord(solid.corner[3].x(), solid.corner[3].y(), solid.corner[3].z());
    m_dxfRW->writeSolid(&drw);
}


void DxfWriter::writeHatch(const EntityHatch& hatch)
{
    DRW_Hatch drw;
    setEntityCommon(drw, hatch.prop);
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
    m_dxfRW->writeHatch(&drw);
}
