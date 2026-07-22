#include <QtMath>
#include <QFileInfo>
#include "DxfReader.h"

DxfReader::DxfReader(DxfData* pData)
    : m_data(pData)
{
}

void DxfReader::setDataTarget(DxfData* pData)
{
    m_data = pData;
}

bool DxfReader::readFile(const QString& filePath)
{
    if (!m_data) {
        std::cerr << "[DxfReader] No data target set!" << std::endl;
        return false;
    }
    // 使用 dxfRW 进行解析
    dxfRW dxf(filePath.toStdString().c_str());
    if (!dxf.read(this, false)) { 
        std::cerr << "[DxfReader] Failed to open/parse: "
            << filePath.toStdString() << std::endl;
        return false;
    }

    auto it = m_data;

    return true;
}

void DxfReader::fillEntityProp(const DRW_Entity& src, EntityProp& dst)
{
    dst.layer = src.layer;
    dst.lineType = src.lineType;
    dst.color = src.color;        // ACI 索引
    dst.color24 = src.color24;
    dst.lineWeight = DRW_LW_Conv::lineWidth2dxfInt(src.lWeight);  // ← 修正：lWeight
    dst.ltypeScale = src.ltypeScale;
    dst.visible = src.visible;
    // 透明度
    dst.transparency = src.transparency;
    // 挤出方向 — extPoint 在 DRW_Point 级别
    const DRW_Point* asPoint = dynamic_cast<const DRW_Point*>(&src);
    if (asPoint) {
        dst.thickness = asPoint->thickness;
        // 判断是否有挤出方向
        if (asPoint->extPoint.x != 0.0 || asPoint->extPoint.y != 0.0 ||
            std::abs(asPoint->extPoint.z - 1.0) > 1e-9) {
            dst.haveExtrusion = true;
            dst.extrusionDir = Vertex3D(
                asPoint->extPoint.x, asPoint->extPoint.y, asPoint->extPoint.z);
        }
    }
}

void DxfReader::storeEntity(const variantDxfEntity& entity, const std::string& layer)
{
    if (!m_data) return;

    if (m_currentBlock.empty()) {
        // 存到图层
        m_data->ensureLayer(layer);
        m_data->addEntity(layer, entity);
    }
    else {
        // 存入块定义
        m_data->addEntityToBlock(m_currentBlock, entity);
    }
}

void DxfReader::storeLayer(const Layer& layer)
{
    if (!m_data) return;
    m_data->addLayer(layer);
}


void DxfReader::addHeader(const DRW_Header* data)
{
    if (!m_data || !data) return;
    // $ACADVER
    auto itVer = data->vars.find("$ACADVER");
    if (itVer != data->vars.end() && itVer->second->type() == DRW_Variant::STRING) {
        m_data->setVersion(QString::fromStdString(*itVer->second->content.s));
    }
    // $INSUNITS
    auto itUnits = data->vars.find("$INSUNITS");
    if (itUnits != data->vars.end() && itUnits->second->type() == DRW_Variant::INTEGER) {
        m_data->setInsUnits(static_cast<double>(itUnits->second->content.i));
    }
    // $EXTMIN / $EXTMAX — 需要取两个值后才调用 SetExtents
    Vertex3D extMin, extMax;
    bool hasExtMin = false, hasExtMax = false;
    auto itExtMin = data->vars.find("$EXTMIN");
    if (itExtMin != data->vars.end() && itExtMin->second->type() == DRW_Variant::COORD) {
        DRW_Coord* c = itExtMin->second->content.v;
        if (c) { extMin = Vertex3D(c->x, c->y, c->z); hasExtMin = true; }
    }
    auto itExtMax = data->vars.find("$EXTMAX");
    if (itExtMax != data->vars.end() && itExtMax->second->type() == DRW_Variant::COORD) {
        DRW_Coord* c = itExtMax->second->content.v;
        if (c) { extMax = Vertex3D(c->x, c->y, c->z); hasExtMax = true; }
    }
    if (hasExtMin && hasExtMax) {
        m_data->setExtents(extMin, extMax);
    }
    // $LTSCALE
    auto itLtScale = data->vars.find("$LTSCALE");
    if (itLtScale != data->vars.end() && itLtScale->second->type() == DRW_Variant::DOUBLE) {
        m_data->setLtScale(itLtScale->second->content.d);
    }
}

void DxfReader::addLType(const DRW_LType& data)
{
}

void DxfReader::addLayer(const DRW_Layer& data)
{
    if (!m_data) return;
    Layer layer;
    // ACI 颜色转 QColor
    if (data.color24 >= 0) {
        // code 420: 24-bit RGB，格式 0x00RRGGBB
        layer.color = QColor::fromRgb(
            (data.color24 >> 16) & 0xFF,
            (data.color24 >> 8) & 0xFF,
            (data.color24) & 0xFF);
    }
    else if (data.color > 0 && data.color < 256) {
        const auto& colorMap = DxfColorMap::getColorMap();
        auto it = colorMap.find(data.color);
        if (it != colorMap.end())
            layer.color = it->second;
    }
    layer.name = QString::fromStdString(data.name);
    layer.lineType = QString::fromStdString(data.lineType);
    layer.lineWeight = DRW_LW_Conv::lineWidth2dxfInt(data.lWeight);
    layer.isVisible = (data.color >= 0);    // code 62 负值 = 图层关闭
    layer.isLocked = (data.flags & 0x04) != 0;
    storeLayer(layer);
}

void DxfReader::addDimStyle(const DRW_Dimstyle& data)
{
    // 标注风格
}
void DxfReader::addVport(const DRW_Vport& data) 
{
    // 视口配置
}
void DxfReader::addTextStyle(const DRW_Textstyle& data) 
{
    // 文字样式
}
void DxfReader::addAppId(const DRW_AppId& data) 
{
    // 应用程序ID
}


void DxfReader::addBlock(const DRW_Block& data)
{
    if (!m_data) return;
    // 注册块定义
    m_data->addBlock(data.name, Vertex3D(
        data.basePoint.x, data.basePoint.y, data.basePoint.z));
    // 记录当前块名，后续图元会进入该块
    m_currentBlock = data.name;
}

void DxfReader::setBlock(const int handle)
{
    //// 在 DWG 中可能使用 handle 切换当前块
    //auto it = m_blockHandles.find(handle);
    //if (it != m_blockHandles.end()) {
    //    m_currentBlock = it->second;
    //}
}

void DxfReader::endBlock()
{
    // 离开当前块
    m_currentBlock.clear();
}


void DxfReader::addPoint(const DRW_Point& data)
{
    if (!m_data) return;
    EntityPoint ent;
    fillEntityProp(data, ent.prop);
    ent.point = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    storeEntity(ent, data.layer);
}

void DxfReader::addLine(const DRW_Line& data)
{
    if (!m_data) return;
    EntityLine ent;
    fillEntityProp(data, ent.prop);
    ent.startPoint = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.endPoint = Vertex3D(data.secPoint.x, data.secPoint.y, data.secPoint.z);
    storeEntity(ent, data.layer);
}

void DxfReader::addRay(const DRW_Ray& /*data*/)
{
    // 射线
}

void DxfReader::addXline(const DRW_Xline& /*data*/)
{
    // 构造线
}

void DxfReader::addCircle(const DRW_Circle& data)
{
    if (!m_data) return;
    EntityCircle ent;
    fillEntityProp(data, ent.prop);
    ent.center = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.radius = data.radious;
    storeEntity(ent, data.layer);
}

void DxfReader::addEllipse(const DRW_Ellipse& data)
{
    if (!m_data) return;
    EntityEllipse ent;
    fillEntityProp(data, ent.prop);
    ent.center = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    // secPoint 是长轴端点(相对于中心的向量或绝对坐标)
    ent.majorAxisEndpoint = Vertex3D(data.secPoint.x, data.secPoint.y, data.secPoint.z);
    ent.ratio = data.ratio;
    ent.startParam = data.staparam;
    ent.endParam = data.endparam;
    storeEntity(ent, data.layer);
}

void DxfReader::addArc(const DRW_Arc& data)
{
    if (!m_data) return;
    EntityArc ent;
    fillEntityProp(data, ent.prop);
    ent.center = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.radius = data.radious;
    ent.startAngle = data.staangle;   // libdxfrw 弧度
    ent.endAngle = data.endangle;     // libdxfrw 弧度
    ent.isCCW = data.isccw == 1 ? true : false;
    storeEntity(ent, data.layer);
}

void DxfReader::addMText(const DRW_MText& data)
{
    if (!m_data) return;
    EntityMText ent;
    fillEntityProp(data, ent.prop);
    ent.insertPoint = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.xAxisDir = Vertex3D(data.secPoint.x, data.secPoint.y, data.secPoint.z);
    ent.text = data.text;
    ent.height = data.height;
    ent.rotation = data.angle * M_PI / 180.0;
    ent.widthFactor = data.widthscale;
    ent.style = data.style;
    ent.lineSpacing = data.interlin;
    ent.attachPoint = static_cast<int>(data.alignV); // Attach 枚举 1-9
    // textDir / lineSpaceStyle — libdxfrw 没有暴露保持默认值
    storeEntity(ent, data.layer);
}

void DxfReader::addText(const DRW_Text& data)
{
    if (!m_data) return;
    EntityText ent;
    fillEntityProp(data, ent.prop);
    ent.insertPoint = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.alignPoint = Vertex3D(data.secPoint.x, data.secPoint.y, data.secPoint.z);
    ent.text = data.text;
    ent.height = data.height;
    ent.rotation = data.angle;
    ent.widthFactor = data.widthscale;
    ent.obliqueAngle = data.oblique;
    ent.style = data.style;
    ent.textGen = data.textgen;
    ent.alignH = data.alignH;
    ent.alignV = data.alignV;
    storeEntity(ent, data.layer);
}

void DxfReader::addDimAlign(const DRW_DimAligned* /*data*/) 
{
    // 对齐标注
}
void DxfReader::addDimLinear(const DRW_DimLinear* /*data*/) 
{
    // 线性标注
}
void DxfReader::addDimRadial(const DRW_DimRadial* /*data*/) 
{
    // 半径标注
}
void DxfReader::addDimDiametric(const DRW_DimDiametric* /*data*/) 
{
    // 直径标注
}
void DxfReader::addDimAngular(const DRW_DimAngular* /*data*/) 
{
    // 角度标注
}
void DxfReader::addDimAngular3P(const DRW_DimAngular3p* /*data*/) 
{
    // 三点角度标注
}
void DxfReader::addDimOrdinate(const DRW_DimOrdinate* /*data*/) 
{
    // 坐标标注
}
void DxfReader::addLeader(const DRW_Leader* /*data*/) 
{
    // 指引线
}

void DxfReader::addHatch(const DRW_Hatch* data)
{
    if (!m_data || !data) return;
    EntityHatch ent;
    fillEntityProp(*data, ent.prop);
    ent.patternName = data->name;
    ent.solidFill = (data->solid != 0);       // DRW_Hatch::solid (code 70)
    ent.associative = data->associative;      // code 71
    ent.style = data->hstyle;                 // code 75
    ent.patternType = data->hpattern;         // code 76 
    ent.doubleFlag = data->doubleflag;        // code 77
    ent.angle = data->angle;                  // code 52
    ent.scale = data->scale;                  // code 41
    // 转换边界环 — looplist元素是 shared_ptr
    for (const auto& loopPtr : data->looplist) {
        if (!loopPtr) continue;
        HatchLoop hl;
        hl.type = loopPtr->type;                  // code 92: polyline=2
        hl.isPolyline = (loopPtr->type & 2) != 0; // 从 type 位判断
        // 遍历 objlist 中的边界边(实体对象)
        for (const auto& obj : loopPtr->objlist) {
            if (!obj) continue;
            switch (obj->eType) {
            case DRW::LINE: {
                // 直线边
                auto* line = static_cast<const DRW_Line*>(obj.get());
                HatchEdgeLine hel;
                hel.start = Vertex3D(line->basePoint.x, line->basePoint.y, 0.0);
                hel.end = Vertex3D(line->secPoint.x, line->secPoint.y, 0.0);
                hl.edges.emplace_back(hel);
                break;
            }
            case DRW::ARC: {
                // 圆弧边
                auto* arc = static_cast<const DRW_Arc*>(obj.get());
                HatchEdgeArc hea;
                hea.center = Vertex3D(arc->basePoint.x, arc->basePoint.y, 0.0);
                hea.radius = arc->radious;
                hea.startAngle = arc->staangle;
                hea.endAngle = arc->endangle;
                hea.isCCW = (arc->isccw != 0);
                hl.edges.emplace_back(hea);
                break;
            }
            case DRW::ELLIPSE: {
                // 椭圆弧边
                auto* el = static_cast<const DRW_Ellipse*>(obj.get());
                HatchEdgeEllipse hee;
                hee.center = Vertex3D(el->basePoint.x, el->basePoint.y, 0.0);
                hee.majorAxisEndpoint = Vertex3D(el->secPoint.x, el->secPoint.y, 0.0);
                hee.ratio = el->ratio;
                hee.startParam = el->staparam;
                hee.endParam = el->endparam;
                hl.edges.emplace_back(hee);
                break;
            }
            case DRW::SPLINE: {
                // 样条边
                auto* sp = static_cast<const DRW_Spline*>(obj.get());
                HatchEdgeSpline hes;
                hes.degree = sp->degree;
                for (const auto& cp : sp->controllist)
                    hes.controlPoints.emplace_back(cp->x, cp->y, cp->z);
                for (const auto& fp : sp->fitlist)
                    hes.fitPoints.emplace_back(fp->x, fp->y, fp->z);
                hes.knots = sp->knotslist;
                hl.edges.emplace_back(hes);
                break;
            }
            case DRW::LWPOLYLINE: {
                // 多段线边界 — 提取顶点到 polylinePath
                auto* pl = static_cast<const DRW_LWPolyline*>(obj.get());
                hl.isPolyline = true;
                for (const auto& v : pl->vertlist) {
                    if (v)
                        hl.polylinePath.emplace_back(v->x, v->y, 0.0);
                }
                break;
            }
            default:
                break;
            }
        }
        ent.loops.push_back(hl);
    }
    ent.loopCount = static_cast<int>(ent.loops.size());
    storeEntity(ent, data->layer);
}

void DxfReader::addViewport(const DRW_Viewport& /*data*/) 
{
    // 视图窗口
}
void DxfReader::addImage(const DRW_Image* /*data*/) 
{
    // 图片
}
void DxfReader::linkImage(const DRW_ImageDef* /*data*/) 
{
    // 图片定义
}
void DxfReader::addComment(const char* /*comment*/) 
{
    // 注释
}
void DxfReader::addPlotSettings(const DRW_PlotSettings* /*data*/) 
{
    // 打印设置
}


void DxfReader::addLWPolyline(const DRW_LWPolyline& data)
{
    if (!m_data) return;
    EntityLWPolyline ent;
    fillEntityProp(data, ent.prop);
    ent.flags = data.flags;
    ent.constantWidth = data.width;     // DRW_LWPolyline::width (code 43)
    ent.elevation = data.elevation;     // code 38
    for (const auto& vptr : data.vertlist) {
        if (!vptr) continue;
        PolylineVertex2D pv;
        pv.point = Vertex3D(vptr->x, vptr->y, 0.0);
        pv.bulge = vptr->bulge;
        pv.startWidth = vptr->stawidth;   // stawidth
        pv.endWidth = vptr->endwidth;     // endwidth
        ent.vecVertices.push_back(pv);
    }
    storeEntity(ent, data.layer);
}

void DxfReader::addPolyline(const DRW_Polyline& data)
{
    if (!m_data) return;
    EntityPolyline ent;
    fillEntityProp(data, ent.prop);
    ent.flags = data.flags;
    ent.defStartWidth = data.defstawidth;    // defstawidth
    ent.defEndWidth = data.defendwidth;
    ent.smoothM = data.smoothM;
    ent.smoothN = data.smoothN;
    ent.curveType = data.curvetype;          // curvetype
    ent.vertexCount = data.vertexcount;
    ent.faceCount = data.facecount;
    // vertlist 里的每个元素是 shared_ptr<DRW_Vertex>
    for (const auto& vptr : data.vertlist) {
        if (!vptr) continue;
        PolylineVertex3D pv;
        pv.point = Vertex3D(vptr->basePoint.x,
            vptr->basePoint.y,
            vptr->basePoint.z);              // 坐标在 basePoint 中
        pv.bulge = vptr->bulge;
        pv.startWidth = vptr->stawidth;            
        pv.endWidth = vptr->endwidth;                 
        pv.tangentDir = vptr->tgdir;                   
        pv.vIndex[0] = vptr->vindex1;                  
        pv.vIndex[1] = vptr->vindex2;
        pv.vIndex[2] = vptr->vindex3;
        pv.vIndex[3] = vptr->vindex4;
        pv.vertexFlags = vptr->flags;            
        ent.vecVertices.push_back(pv);
    }
    storeEntity(ent, data.layer);
}

void DxfReader::addSpline(const DRW_Spline* data)
{
    if (!m_data || !data) return;
    EntitySpline ent;
    fillEntityProp(*data, ent.prop);
    ent.flags = data->flags;
    ent.degree = data->degree;
    ent.knotTolerance = data->tolknot;       
    ent.controlTolerance = data->tolcontrol;  
    ent.fitTolerance = data->tolfit;
    ent.normalVec = Vertex3D(
        data->normalVec.x, data->normalVec.y, data->normalVec.z);
    ent.tgStart = Vertex3D(
        data->tgStart.x, data->tgStart.y, data->tgStart.z);
    ent.tgEnd = Vertex3D(
        data->tgEnd.x, data->tgEnd.y, data->tgEnd.z);
    ent.knots = data->knotslist; 
    ent.weights = data->weightlist;
    for (const auto& cp : data->controllist) {
        if (cp)
            ent.controlPoints.emplace_back(cp->x, cp->y, cp->z);
    }
    for (const auto& fp : data->fitlist) {
        if (fp)
            ent.fitPoints.emplace_back(fp->x, fp->y, fp->z);
    }
    storeEntity(ent, data->layer);
}

void DxfReader::addKnot(const DRW_Entity& /*data*/)
{
    // 样条节点值已经在 addSpline 中通过 data->knots 获取
    // 这个回调用于逐节点添加，视 libdxfrw 版本而定
}

void DxfReader::addInsert(const DRW_Insert& data)
{
    if (!m_data) return;
    EntityInsert ent;
    fillEntityProp(data, ent.prop);
    ent.insertPoint = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.blockName = data.name;
    ent.xScale = data.xscale;
    ent.yScale = data.yscale;
    ent.zScale = data.zscale;
    ent.rotation = data.angle;          
    ent.colCount = data.colcount;
    ent.rowCount = data.rowcount;
    ent.colSpacing = data.colspace;
    ent.rowSpacing = data.rowspace;
    storeEntity(ent, data.layer);
}

void DxfReader::addTrace(const DRW_Trace& /*data*/)
{

}

void DxfReader::add3dFace(const DRW_3Dface& /*data*/)
{
    // 转换为 Solid 或自定义处理
}

void DxfReader::addSolid(const DRW_Solid& data)
{
    if (!m_data) return;
    EntitySolid ent;
    fillEntityProp(data, ent.prop);
    ent.corner[0] = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.corner[1] = Vertex3D(data.secPoint.x, data.secPoint.y, data.secPoint.z);
    ent.corner[2] = Vertex3D(data.thirdPoint.x, data.thirdPoint.y, data.thirdPoint.z);
    ent.corner[3] = Vertex3D(data.fourPoint.x, data.fourPoint.y, data.fourPoint.z);
    storeEntity(ent, data.layer);
}



void DxfReader::writeHeader(DRW_Header& /*data*/) {}
void DxfReader::writeBlocks() {}
void DxfReader::writeBlockRecords() {}
void DxfReader::writeEntities() {}
void DxfReader::writeLTypes() {}
void DxfReader::writeLayers() {}
void DxfReader::writeTextstyles() {}
void DxfReader::writeVports() {}
void DxfReader::writeDimstyles() {}
void DxfReader::writeObjects() {}
void DxfReader::writeAppId() {}
