#include "DxfStruct.h"

CDxfStruct::CDxfStruct()
{
}

CDxfStruct::~CDxfStruct()
{
}

void CDxfStruct::addEntity(const string& layer, const DxfEntity& entity)
{
	m_mapAllLayerEntities[layer].push_back(entity);
	m_mapAllLayerEntityCount[layer]++;
}

vector<string> CDxfStruct::getLayerNames() const
{
	std::vector<std::string> vecLayers;
    vecLayers.reserve(m_mapAllLayerEntities.size());
    //layer接受第一个元素,_占位忽略
    for (const auto& [layer, _] : m_mapAllLayerEntities)
    {
        vecLayers.push_back(layer);
    }
    return vecLayers;
}

const vector<DxfEntity>& CDxfStruct::getEntities(const string& layer) const
{
    static const std::vector<DxfEntity> vecEmpty;
    auto it = m_mapAllLayerEntities.find(layer);
    if (it != m_mapAllLayerEntities.end())
    {
        return it->second;
    }
    return vecEmpty;
}

const std::map<std::string, std::vector<DxfEntity>>& CDxfStruct::getAllEntities() const
{
    return m_mapAllLayerEntities;
}

bool CDxfStruct::hasLayer(const std::string& layer) const
{
    return m_mapAllLayerEntities.find(layer) != m_mapAllLayerEntities.end();
}

size_t CDxfStruct::getLayerCount() const
{
    return m_mapAllLayerEntities.size();
}

size_t CDxfStruct::getTotalEntityCount() const
{
    size_t TotalEntities = 0;
    for (const auto& [_, entities] : m_mapAllLayerEntities)
    {
        TotalEntities += entities.size();
    }
    return TotalEntities;
}

size_t CDxfStruct::getEntityCount(const std::string& layer) const
{
    auto it = m_mapAllLayerEntities.find(layer);
    if (it != m_mapAllLayerEntities.end())
    {
        return it->second.size();
    }
    return 0;
}

void CDxfStruct::clearAll()
{
    m_mapAllLayerEntities.clear();
    m_mapAllLayerEntityCount.clear();
}

bool CDxfStruct::removeLayer(const std::string& layer)
{
    auto it = m_mapAllLayerEntities.find(layer);
    if (it != m_mapAllLayerEntities.end())
    {
        m_mapAllLayerEntities.erase(it);
        m_mapAllLayerEntityCount.erase(layer);
        return true;
    }
    return false;
}

void CDxfStruct::mergeLayers(const std::string& targetLayer, const std::string& sourceLayer)
{
    auto sourceIt = m_mapAllLayerEntities.find(sourceLayer);
    //没有source图层,直接返回
    if (sourceIt == m_mapAllLayerEntities.end()) return;

    auto& targetVec = m_mapAllLayerEntities[targetLayer];
    targetVec.insert(targetVec.end(), sourceIt->second.begin(), sourceIt->second.end());

    m_mapAllLayerEntities.erase(sourceIt);
    m_mapAllLayerEntityCount[targetLayer] += m_mapAllLayerEntityCount[sourceLayer];
    m_mapAllLayerEntityCount.erase(sourceLayer);
}

void CDxfStruct::printStatistics() const
{
    //std::cout << "\n=== DXF Statistics ===\n";
    //std::cout << "Total layers: " << getLayerCount() << "\n";
    //std::cout << "Total entities: " << getTotalEntityCount() << "\n\n";

    //for (const auto& [layer, entities] : m_mapAllLayerEntities) {
    //    std::cout << "Layer: " << layer << "\n";
    //    std::cout << "  Entities: " << entities.size() << "\n";

    //    // 统计每种类型的数量
    //    std::map<std::string, int> typeCount;
    //    for (const auto& entity : entities)
    //    {
    //        std::visit([&typeCount](auto&& arg)
    //            {
    //                typeCount[arg.type]++;
    //            }, entity);
    //    }

    //    for (const auto& [type, count] : typeCount)
    //    {
    //        std::cout << "    " << type << ": " << count << "\n";
    //    }
    //}
}

