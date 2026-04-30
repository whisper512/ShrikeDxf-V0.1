#include <QtMath>
#include <QFileInfo>
#include "DxfReader.h"

CDxfReader::CDxfReader(CDxfData* pData)
    : m_pData(pData)
{
}

void CDxfReader::SetDataTarget(CDxfData* pData)
{
    m_pData = pData;
}

bool CDxfReader::ReadFile(const QString& filePath)
{
    if (!m_pData) {
        std::cerr << "[CDxfReader] No data target set!" << std::endl;
        return false;
    }
    // libdxfrw 使用 dxfRW 进行解析
    dxfRW dxf(filePath.toStdString().c_str());
    if (!dxf.read(this, false)) { 
        std::cerr << "[CDxfReader] Failed to open/parse: "
            << filePath.toStdString() << std::endl;
        return false;
    }
    return true;
}


void CDxfReader::FillEntityProp(const DRW_Entity& src, EntityProp& dst)
{
    //dst.layer = src.layer;
    //dst.lineType = src.lineType;
    //dst.color = src.color;        // ACI 索引
    //dst.color24 = src.color24;
    //dst.lineWeight = src.lineWeight;
    //dst.ltypeScale = src.ltypeScale;
    //dst.visible = src.visible;
    //dst.thickness = src.thickness;

    //// 挤出方向
    //if (src.extrusion.x != 0.0 || src.extrusion.y != 0.0 ||
    //    std::abs(src.extrusion.z - 1.0) > 1e-9) {
    //    dst.haveExtrusion = true;
    //    dst.extrusionDir = Vertex3D(src.extrusion.x, src.extrusion.y, src.extrusion.z);
    //}

    // 透明度（libdxfrw 中 transparency 在 DRW_Entity 中）
    // 注：如果你的 libdxfrw 版本没有 transparency 字段，注释掉即可
    // dst.transparency = src.transparency;
}

void CDxfReader::StoreEntity(const variantDxfEntity& entity, const std::string& layer)
{
    if (!m_pData) return;

    if (m_currentBlock.empty()) {
        // 存到图层
        m_pData->EnsureLayer(layer);
        m_pData->AddEntity(layer, entity);
    }
    else {
        // 存入块定义
        m_pData->AddEntityToBlock(m_currentBlock, entity);
    }
}


void CDxfReader::addHeader(const DRW_Header* data)
{
    //if (!m_pData || !data) return;

    //// 版本
    //if (!data->getVersion().empty())
    //    m_pData->SetVersion(QString::fromStdString(data->getVersion()));

    //// 单位
    //m_pData->SetInsUnits(data->getInsUnits());

    //// 范围
    //Vertex3D extMin(data->getExtMin().x, data->getExtMin().y, data->getExtMin().z);
    //Vertex3D extMax(data->getExtMax().x, data->getExtMax().y, data->getExtMax().z);
    //m_pData->SetExtents(extMin, extMax);

    //// 线型比例
    //m_pData->SetLtScale(data->getLtScale());
}

void CDxfReader::addLType(const DRW_LType& data)
{
}

void CDxfReader::addLayer(const DRW_Layer& data)
{
    //if (!m_pData) return;

    //// 确保图层存在，设置颜色、线型、可见性等
    //stuLayer& layer = m_pData->EnsureLayer(data.name);
    //layer.color = QColor::fromRgb(
    //    (data.color24 >> 16) & 0xFF,
    //    (data.color24 >> 8) & 0xFF,
    //    (data.color24) & 0xFF);
    //if (data.color > 0 && data.color < 256) {
    //    
    //}
    //layer.lineType = QString::fromStdString(data.lineType);
    ////layer.lineWeight = data.lineWeight;
    ////layer.isVisible = data.visible;
    //layer.isLocked = (data.flags & 0x04) != 0;  // DXF 图层标志
}

void CDxfReader::addDimStyle(const DRW_Dimstyle& /*data*/) {}
void CDxfReader::addVport(const DRW_Vport& /*data*/) {}
void CDxfReader::addTextStyle(const DRW_Textstyle& /*data*/) {}
void CDxfReader::addAppId(const DRW_AppId& /*data*/) {}


void CDxfReader::addBlock(const DRW_Block& data)
{
    //if (!m_pData) return;

    //// 注册块定义
    //m_pData->AddBlock(data.name, Vertex3D(
    //    data.basePoint.x, data.basePoint.y, data.basePoint.z));

    //// 记录 handle → 块名 映射（用于 setBlock）
    //m_blockHandles[data.handleBlock] = data.name;

    //// 设置当前块，后面 addLine / addCircle … 都会进入该块
    //m_currentBlock = data.name;
}

void CDxfReader::setBlock(const int handle)
{
    //// 在 DWG 中可能使用 handle 切换当前块
    //auto it = m_blockHandles.find(handle);
    //if (it != m_blockHandles.end()) {
    //    m_currentBlock = it->second;
    //}
}

void CDxfReader::endBlock()
{
    // 离开当前块
    m_currentBlock.clear();
}


void CDxfReader::addPoint(const DRW_Point& data)
{
    if (!m_pData) return;
    EntityPoint ent;
    FillEntityProp(data, ent.prop);
    ent.point = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    StoreEntity(ent, data.layer);
}

void CDxfReader::addLine(const DRW_Line& data)
{
    if (!m_pData) return;
    EntityLine ent;
    FillEntityProp(data, ent.prop);
    ent.startPoint = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.endPoint = Vertex3D(data.secPoint.x, data.secPoint.y, data.secPoint.z);
    StoreEntity(ent, data.layer);
}

void CDxfReader::addRay(const DRW_Ray& /*data*/)
{
    // 射线
}

void CDxfReader::addXline(const DRW_Xline& /*data*/)
{
    // 构造线
}

void CDxfReader::addCircle(const DRW_Circle& data)
{
    if (!m_pData) return;
    EntityCircle ent;
    FillEntityProp(data, ent.prop);
    ent.center = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.radius = data.radious;
    StoreEntity(ent, data.layer);
}

void CDxfReader::addEllipse(const DRW_Ellipse& data)
{
    if (!m_pData) return;
    EntityEllipse ent;
    FillEntityProp(data, ent.prop);
    ent.center = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    // secPoint 是长轴端点(相对于中心的向量或绝对坐标)
    ent.majorAxisEndpoint = Vertex3D(data.secPoint.x, data.secPoint.y, data.secPoint.z);
    ent.ratio = data.ratio;
    ent.startParam = data.staparam;
    ent.endParam = data.endparam;
    StoreEntity(ent, data.layer);
}

void CDxfReader::addArc(const DRW_Arc& data)
{
    if (!m_pData) return;
    EntityArc ent;
    FillEntityProp(data, ent.prop);
    ent.center = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.radius = data.radious;
    ent.startAngle = data.staangle;   // libdxfrw 弧度
    ent.endAngle = data.endangle;     // libdxfrw 弧度
    ent.isCCW = true;             // DXF 默认逆时针
    StoreEntity(ent, data.layer);
}

void CDxfReader::addMText(const DRW_MText& data)
{
    if (!m_pData) return;
    EntityMText ent;
    FillEntityProp(data, ent.prop);
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
    StoreEntity(ent, data.layer);
}

void CDxfReader::addText(const DRW_Text& data)
{
    if (!m_pData) return;
    EntityText ent;
    FillEntityProp(data, ent.prop);
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
    StoreEntity(ent, data.layer);
}

void CDxfReader::addDimAlign(const DRW_DimAligned* /*data*/) 
{
    // 对齐标注
}
void CDxfReader::addDimLinear(const DRW_DimLinear* /*data*/) 
{
    // 线性标注
}
void CDxfReader::addDimRadial(const DRW_DimRadial* /*data*/) 
{
    // 半径标注
}
void CDxfReader::addDimDiametric(const DRW_DimDiametric* /*data*/) 
{
    // 直径标注
}
void CDxfReader::addDimAngular(const DRW_DimAngular* /*data*/) 
{
    // 角度标注
}
void CDxfReader::addDimAngular3P(const DRW_DimAngular3p* /*data*/) 
{
    // 三点角度标注
}
void CDxfReader::addDimOrdinate(const DRW_DimOrdinate* /*data*/) 
{
    // 坐标标注
}
void CDxfReader::addLeader(const DRW_Leader* /*data*/) 
{
    // 指引线
}

void CDxfReader::addHatch(const DRW_Hatch* data)
{
    if (!m_pData || !data) return;
    EntityHatch ent;
    FillEntityProp(*data, ent.prop);
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
    StoreEntity(ent, data->layer);
}

void CDxfReader::addViewport(const DRW_Viewport& /*data*/) 
{
    // 视图窗口
}
void CDxfReader::addImage(const DRW_Image* /*data*/) 
{
    // 图片
}
void CDxfReader::linkImage(const DRW_ImageDef* /*data*/) 
{
    // 图片定义
}
void CDxfReader::addComment(const char* /*comment*/) 
{
    // 注释
}
void CDxfReader::addPlotSettings(const DRW_PlotSettings* /*data*/) 
{
    // 打印设置
}


void CDxfReader::addLWPolyline(const DRW_LWPolyline& data)
{
    if (!m_pData) return;
    EntityLWPolyline ent;
    FillEntityProp(data, ent.prop);
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
        ent.vertices.push_back(pv);
    }
    StoreEntity(ent, data.layer);
}

void CDxfReader::addPolyline(const DRW_Polyline& data)
{
    if (!m_pData) return;
    EntityPolyline ent;
    FillEntityProp(data, ent.prop);
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
        ent.vertices.push_back(pv);
    }
    StoreEntity(ent, data.layer);
}

void CDxfReader::addSpline(const DRW_Spline* data)
{
    if (!m_pData || !data) return;
    EntitySpline ent;
    FillEntityProp(*data, ent.prop);
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
    // controllist / fitlist 元素是 shared_ptr<DRW_Coord>
    for (const auto& cp : data->controllist) {
        if (cp)
            ent.controlPoints.emplace_back(cp->x, cp->y, cp->z);
    }
    for (const auto& fp : data->fitlist) {
        if (fp)
            ent.fitPoints.emplace_back(fp->x, fp->y, fp->z);
    }
    StoreEntity(ent, data->layer);
}

void CDxfReader::addKnot(const DRW_Entity& /*data*/)
{
    // 样条节点值已经在 addSpline 中通过 data->knots 获取
    // 这个回调用于逐节点添加，视 libdxfrw 版本而定
}

void CDxfReader::addInsert(const DRW_Insert& data)
{
    if (!m_pData) return;
    EntityInsert ent;
    FillEntityProp(data, ent.prop);
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
    StoreEntity(ent, data.layer);
}

void CDxfReader::addTrace(const DRW_Trace& /*data*/)
{

}

void CDxfReader::add3dFace(const DRW_3Dface& /*data*/)
{
    // 转换为 Solid 或自定义处理
}

void CDxfReader::addSolid(const DRW_Solid& data)
{
    if (!m_pData) return;
    EntitySolid ent;
    FillEntityProp(data, ent.prop);
    ent.corner[0] = Vertex3D(data.basePoint.x, data.basePoint.y, data.basePoint.z);
    ent.corner[1] = Vertex3D(data.secPoint.x, data.secPoint.y, data.secPoint.z);
    ent.corner[2] = Vertex3D(data.thirdPoint.x, data.thirdPoint.y, data.thirdPoint.z);
    ent.corner[3] = Vertex3D(data.fourPoint.x, data.fourPoint.y, data.fourPoint.z);
    StoreEntity(ent, data.layer);
}



void CDxfReader::writeHeader(DRW_Header& /*data*/) {}
void CDxfReader::writeBlocks() {}
void CDxfReader::writeBlockRecords() {}
void CDxfReader::writeEntities() {}
void CDxfReader::writeLTypes() {}
void CDxfReader::writeLayers() {}
void CDxfReader::writeTextstyles() {}
void CDxfReader::writeVports() {}
void CDxfReader::writeDimstyles() {}
void CDxfReader::writeObjects() {}
void CDxfReader::writeAppId() {}
