#pragma once
#include "DxfStruct.h"

/**
 * @brief DXF 数据存储类
 */

class DxfData
{
public:
    DxfData();
    ~DxfData();

    // 文档级访问
    DxfDocument& getDocument();
    const DxfDocument& getDocument() const;

    void setVersion(const QString& version);
    void setInsUnits(double units);
    void setExtents(const Vertex3D& extMin, const Vertex3D& extMax);
    void setLtScale(double ltscale);

    const QString& getVersion() const;
    double         getInsUnits() const;
    double         getLtScale() const;
    Vertex3D       getExtMin() const;
    Vertex3D       getExtMax() const;

    // Layer 图层管理
    void addLayer(const Layer& layer);
    bool hasLayer(const std::string& name) const;
    const  std::map<std::string, Layer> getLayers() const;
    std::map<std::string, Layer>& getLayers();
    Layer* getLayer(const std::string& name);
    const Layer* getLayer(const std::string& name) const;
    std::vector<std::string> getAllLayerNames() const;
    size_t getLayerCount() const;
    QString getFirstLayerName() const;
    // 确保图层存在
    Layer& ensureLayer(const std::string& name);

    
    // 指定图层添加任意类型图元
    void addEntity(const std::string& layerName, const variantDxfEntity& entity);
    void addPoint(const std::string& layerName, const EntityPoint& ent);
    void addLine(const std::string& layerName, const EntityLine& ent);
    void addCircle(const std::string& layerName, const EntityCircle& ent);
    void addArc(const std::string& layerName, const EntityArc& ent);
    void addLWPolyline(const std::string& layerName, const EntityLWPolyline& ent);
    void addPolyline(const std::string& layerName, const EntityPolyline& ent);
    void addEllipse(const std::string& layerName, const EntityEllipse& ent);
    void addSpline(const std::string& layerName, const EntitySpline& ent);
    void addText(const std::string& layerName, const EntityText& ent);
    void addMText(const std::string& layerName, const EntityMText& ent);
    void addInsert(const std::string& layerName, const EntityInsert& ent);
    void addSolid(const std::string& layerName, const EntitySolid& ent);
    void addHatch(const std::string& layerName, const EntityHatch& ent);

    // Block 块定义管理
    Block& addBlock(const std::string& name, const Vertex3D& basePoint = Vertex3D());
    bool hasBlock(const std::string& name) const;
    Block* getBlock(const std::string& name);
    const Block* getBlock(const std::string& name) const;
    std::vector<std::string> getAllBlockNames() const;
    size_t getBlockCount() const;
    // 向块内添加图元
    void addEntityToBlock(const std::string& blockName, const variantDxfEntity& entity);


    // 查询/遍历
    int getTotalEntityCount() const;
    // 按类型统计
    int getEntityCount(EntityType type) const;
    // 按图层统计
    int getEntityCount(const std::string& layerName, EntityType type) const;
    // 按类型过滤
    std::vector<variantDxfEntity> getEntitiesByType(EntityType type) const;
    // 按图层过滤
    std::vector<variantDxfEntity> getEntitiesByLayer(const std::string& layerName) const;
    // 按图层+类型过滤
    std::vector<variantDxfEntity> getEntities(const std::string& layerName, EntityType type) const;

    PreviewEntity& getPreviewEntity();
    const PreviewEntity& getPreviewEntity() const;

    void clear();

private:
    DxfDocument m_document;
};
