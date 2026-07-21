#include "DxfData.h"
#include "DxfGraphicsScene.h"

DxfData::DxfData()
{
}

DxfData::~DxfData()
{
}

DxfDocument& DxfData::getDocument()
{
    return m_document;
}

const DxfDocument& DxfData::getDocument() const
{
    return m_document;
}


void DxfData::setVersion(const QString& version)
{
}

void DxfData::setInsUnits(double units)
{
}

void DxfData::setExtents(const Vertex3D& extMin, const Vertex3D& extMax)
{
}

void DxfData::setLtScale(double ltscale)
{
}

const QString& DxfData::getVersion() const
{
    return m_document.version;
}

double DxfData::getInsUnits() const
{
    return m_document.insUnits;
}

Vertex3D DxfData::getExtMin() const
{
    return m_document.extMin;
}

Vertex3D DxfData::getExtMax() const
{
    return m_document.extMax;
}

double DxfData::getLtScale() const
{
    return m_document.ltscale;
}

// Layer 图层管理
void  DxfData::addLayer(const stuLayer& layer)
{
    m_document.layers[layer.name.toStdString()] = layer;
}

bool DxfData::hasLayer(const std::string& name) const
{
    return m_document.layers.find(name) != m_document.layers.end();
}

const std::map<std::string, stuLayer> DxfData::getLayers() const
{
    return m_document.layers;
}

std::map<std::string, stuLayer>& DxfData::getLayers()
{
    return m_document.layers;
}

stuLayer* DxfData::getLayer(const std::string& name)
{
    auto it = m_document.layers.find(name);
    if (it != m_document.layers.end())
        return &it->second;
    return nullptr;
}

const stuLayer* DxfData::getLayer(const std::string& name) const
{
    auto it = m_document.layers.find(name);
    if (it != m_document.layers.end())
        return &it->second;
    return nullptr;
}

std::vector<std::string> DxfData::getAllLayerNames() const
{
    std::vector<std::string> names;
    for (const auto& [name, _] : m_document.layers)
        names.push_back(name);
    return names;
}

size_t DxfData::getLayerCount() const
{
    return m_document.layers.size();
}

QString DxfData::getFirstLayerName() const
{
    if (m_document.layers.empty())
        return QString();
    return QString::fromStdString(m_document.layers.begin()->first);
}

stuLayer& DxfData::ensureLayer(const std::string& name)
{
    return m_document.layers[name];
}

// 图元添加
void DxfData::addEntity(const std::string& layerName, const variantDxfEntity& entity)
{
    auto& layer = ensureLayer(layerName);
    layer.entities.push_back(entity);
}

void DxfData::addPoint(const std::string& layerName, const EntityPoint& ent)
{
}

void DxfData::addLine(const std::string& layerName, const EntityLine& ent)
{
}

void DxfData::addCircle(const std::string& layerName, const EntityCircle& ent)
{
}

void DxfData::addArc(const std::string& layerName, const EntityArc& ent)
{
}

void DxfData::addLWPolyline(const std::string& layerName, const EntityLWPolyline& ent)
{
}

void DxfData::addPolyline(const std::string& layerName, const EntityPolyline& ent)
{
}

void DxfData::addEllipse(const std::string& layerName, const EntityEllipse& ent)
{
}

void DxfData::addSpline(const std::string& layerName, const EntitySpline& ent)
{
}

void DxfData::addText(const std::string& layerName, const EntityText& ent)
{
}

void DxfData::addMText(const std::string& layerName, const EntityMText& ent)
{
}

void DxfData::addInsert(const std::string& layerName, const EntityInsert& ent)
{
}

void DxfData::addSolid(const std::string& layerName, const EntitySolid& ent)
{
}

void DxfData::addHatch(const std::string& layerName, const EntityHatch& ent)
{
}


// Block 块定义管理
stuBlock& DxfData::addBlock(const std::string& name, const Vertex3D& basePoint)
{
    return m_document.blocks[name];
}

bool DxfData::hasBlock(const std::string& name) const
{
    return m_document.blocks.find(name) != m_document.blocks.end();
}

stuBlock* DxfData::getBlock(const std::string& name)
{
    auto it = m_document.blocks.find(name);
    if (it != m_document.blocks.end())
        return &it->second;
    return nullptr;
}

const stuBlock* DxfData::getBlock(const std::string& name) const
{
    auto it = m_document.blocks.find(name);
    if (it != m_document.blocks.end())
        return &it->second;
    return nullptr;
}

std::vector<std::string> DxfData::getAllBlockNames() const
{
    std::vector<std::string> names;
    for (const auto& [name, _] : m_document.blocks)
        names.push_back(name);
    return names;
}

size_t DxfData::getBlockCount() const
{
    return m_document.blocks.size();
}

void DxfData::addEntityToBlock(const std::string& blockName, const variantDxfEntity& entity)
{
}

// 查询/遍历
int DxfData::getTotalEntityCount() const
{
    return m_document.GetTotalEntityCount();
}

int DxfData::getEntityCount(EntityType type) const
{
    return 0;
}

int DxfData::getEntityCount(const std::string& layerName, EntityType type) const
{
    return 0;
}

std::vector<variantDxfEntity> DxfData::getEntitiesByType(EntityType type) const
{
    return {};
}

std::vector<variantDxfEntity> DxfData::getEntitiesByLayer(const std::string& layerName) const
{
    auto it = m_document.layers.find(layerName);
    if (it != m_document.layers.end())
        return it->second.entities;
    return {};
}

std::vector<variantDxfEntity> DxfData::getEntities(const std::string& layerName, EntityType type) const
{
    return {};
}

stuPreviewEntity& DxfData::getPreviewEntity()
{
    return m_document.previewEntity;
}

const stuPreviewEntity& DxfData::getPreviewEntity() const
{
    return m_document.previewEntity;
}
