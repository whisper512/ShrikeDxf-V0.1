#include "DxfData.h"

CDxfData::CDxfData()
{
}

CDxfData::~CDxfData()
{
}

DxfDocument& CDxfData::GetDocument()
{
    return m_document;
}

const DxfDocument& CDxfData::GetDocument() const
{
    return m_document;
}


void CDxfData::SetVersion(const QString& version)
{
}

void CDxfData::SetInsUnits(double units)
{
}

void CDxfData::SetExtents(const Vertex3D& extMin, const Vertex3D& extMax)
{
}

void CDxfData::SetLtScale(double ltscale)
{
}

const QString& CDxfData::GetVersion() const
{
    return m_document.version;
}

double CDxfData::GetInsUnits() const
{
    return m_document.insUnits;
}

Vertex3D CDxfData::GetExtMin() const
{
    return m_document.extMin;
}

Vertex3D CDxfData::GetExtMax() const
{
    return m_document.extMax;
}

double CDxfData::GetLtScale() const
{
    return m_document.ltscale;
}


// Layer 图层管理
void  CDxfData::AddLayer(const stuLayer& layer)
{
    m_document.layers[layer.name.toStdString()] = layer;
}

bool CDxfData::HasLayer(const std::string& name) const
{
    return m_document.layers.find(name) != m_document.layers.end();
}

stuLayer* CDxfData::GetLayer(const std::string& name)
{
    auto it = m_document.layers.find(name);
    if (it != m_document.layers.end())
        return &it->second;
    return nullptr;
}

const stuLayer* CDxfData::GetLayer(const std::string& name) const
{
    auto it = m_document.layers.find(name);
    if (it != m_document.layers.end())
        return &it->second;
    return nullptr;
}

std::vector<std::string> CDxfData::GetAllLayerNames() const
{
    std::vector<std::string> names;
    for (const auto& [name, _] : m_document.layers)
        names.push_back(name);
    return names;
}

size_t CDxfData::GetLayerCount() const
{
    return m_document.layers.size();
}

stuLayer& CDxfData::EnsureLayer(const std::string& name)
{
    return m_document.layers[name];
}

// 图元添加
void CDxfData::AddEntity(const std::string& layerName, const variantDxfEntity& entity)
{
    auto& layer = EnsureLayer(layerName);
    layer.entities.push_back(entity);
}

void CDxfData::AddPoint(const std::string& layerName, const EntityPoint& ent)
{
}

void CDxfData::AddLine(const std::string& layerName, const EntityLine& ent)
{
}

void CDxfData::AddCircle(const std::string& layerName, const EntityCircle& ent)
{
}

void CDxfData::AddArc(const std::string& layerName, const EntityArc& ent)
{
}

void CDxfData::AddLWPolyline(const std::string& layerName, const EntityLWPolyline& ent)
{
}

void CDxfData::AddPolyline(const std::string& layerName, const EntityPolyline& ent)
{
}

void CDxfData::AddEllipse(const std::string& layerName, const EntityEllipse& ent)
{
}

void CDxfData::AddSpline(const std::string& layerName, const EntitySpline& ent)
{
}

void CDxfData::AddText(const std::string& layerName, const EntityText& ent)
{
}

void CDxfData::AddMText(const std::string& layerName, const EntityMText& ent)
{
}

void CDxfData::AddInsert(const std::string& layerName, const EntityInsert& ent)
{
}

void CDxfData::AddSolid(const std::string& layerName, const EntitySolid& ent)
{
}

void CDxfData::AddHatch(const std::string& layerName, const EntityHatch& ent)
{
}


// Block 块定义管理
stuBlock& CDxfData::AddBlock(const std::string& name, const Vertex3D& basePoint)
{
    return m_document.blocks[name];
}

bool CDxfData::HasBlock(const std::string& name) const
{
    return m_document.blocks.find(name) != m_document.blocks.end();
}

stuBlock* CDxfData::GetBlock(const std::string& name)
{
    auto it = m_document.blocks.find(name);
    if (it != m_document.blocks.end())
        return &it->second;
    return nullptr;
}

const stuBlock* CDxfData::GetBlock(const std::string& name) const
{
    auto it = m_document.blocks.find(name);
    if (it != m_document.blocks.end())
        return &it->second;
    return nullptr;
}

std::vector<std::string> CDxfData::GetAllBlockNames() const
{
    std::vector<std::string> names;
    for (const auto& [name, _] : m_document.blocks)
        names.push_back(name);
    return names;
}

size_t CDxfData::GetBlockCount() const
{
    return m_document.blocks.size();
}

void CDxfData::AddEntityToBlock(const std::string& blockName, const variantDxfEntity& entity)
{
}

// 查询/遍历
int CDxfData::GetTotalEntityCount() const
{
    return m_document.GetTotalEntityCount();
}

int CDxfData::GetEntityCount(EntityType type) const
{
    return 0;
}

int CDxfData::GetEntityCount(const std::string& layerName, EntityType type) const
{
    return 0;
}

std::vector<variantDxfEntity> CDxfData::GetEntitiesByType(EntityType type) const
{
    return {};
}

std::vector<variantDxfEntity> CDxfData::GetEntitiesByLayer(const std::string& layerName) const
{
    auto it = m_document.layers.find(layerName);
    if (it != m_document.layers.end())
        return it->second.entities;
    return {};
}

std::vector<variantDxfEntity> CDxfData::GetEntities(const std::string& layerName, EntityType type) const
{
    return {};
}

// =========================================================================
// 选中
// =========================================================================

void CDxfData::SetSelectedEntity(const stuSelectedEntity& sel)
{
}

void CDxfData::ClearSelection()
{
}

stuSelectedEntity& CDxfData::GetSelectedEntity()
{
    return m_document.selectedEntity;
}

const stuSelectedEntity& CDxfData::GetSelectedEntity() const
{
    return m_document.selectedEntity;
}

// =========================================================================
// 预览
// =========================================================================

void CDxfData::SetPreviewEntity(const stuPreviewEntity& prev)
{
}

void CDxfData::ClearPreview()
{
}

stuPreviewEntity& CDxfData::GetPreviewEntity()
{
    return m_document.previewEntity;
}

const stuPreviewEntity& CDxfData::GetPreviewEntity() const
{
    return m_document.previewEntity;
}

// =========================================================================
// 编辑参数
// =========================================================================

void CDxfData::SetMoveStep(double step)
{
}

double CDxfData::GetMoveStep() const
{
    return m_document.moveStep;
}

void CDxfData::SetRotateStepRad(double rad)
{
}

double CDxfData::GetRotateStepRad() const
{
    return m_document.rotateStepRad;
}

// =========================================================================
// 清空
// =========================================================================

void CDxfData::Clear()
{
}
