#pragma once
#include "DxfStruct.h"

/**
 * @brief DXF 数据存储类
 */
class CDxfData
{
public:
    CDxfData();
    ~CDxfData();

    // 文档级访问
    DxfDocument& GetDocument();
    const DxfDocument& GetDocument() const;


    // Header 头部变量
    void SetVersion(const QString& version);
    void SetInsUnits(double units);
    void SetExtents(const Vertex3D& extMin, const Vertex3D& extMax);
    void SetLtScale(double ltscale);

    const QString& GetVersion() const;
    double         GetInsUnits() const;
    Vertex3D       GetExtMin() const;
    Vertex3D       GetExtMax() const;
    double         GetLtScale() const;

    // Layer 图层管理
    void AddLayer(const stuLayer& layer);
    bool HasLayer(const std::string& name) const;
    const  std::map<std::string, stuLayer> GetLayers() const;
    stuLayer* GetLayer(const std::string& name);
    const stuLayer* GetLayer(const std::string& name) const;
    std::vector<std::string> GetAllLayerNames() const;
    size_t GetLayerCount() const;
    QString GetFirstLayerName() const;

    // 确保图层存在
    stuLayer& EnsureLayer(const std::string& name);

    
    // 指定图层添加任意类型图元
    void AddEntity(const std::string& layerName, const variantDxfEntity& entity);
    // 各类型便捷接口
    void AddPoint(const std::string& layerName, const EntityPoint& ent);
    void AddLine(const std::string& layerName, const EntityLine& ent);
    void AddCircle(const std::string& layerName, const EntityCircle& ent);
    void AddArc(const std::string& layerName, const EntityArc& ent);
    void AddLWPolyline(const std::string& layerName, const EntityLWPolyline& ent);
    void AddPolyline(const std::string& layerName, const EntityPolyline& ent);
    void AddEllipse(const std::string& layerName, const EntityEllipse& ent);
    void AddSpline(const std::string& layerName, const EntitySpline& ent);
    void AddText(const std::string& layerName, const EntityText& ent);
    void AddMText(const std::string& layerName, const EntityMText& ent);
    void AddInsert(const std::string& layerName, const EntityInsert& ent);
    void AddSolid(const std::string& layerName, const EntitySolid& ent);
    void AddHatch(const std::string& layerName, const EntityHatch& ent);

    // Block 块定义管理
    stuBlock& AddBlock(const std::string& name, const Vertex3D& basePoint = Vertex3D());
    bool HasBlock(const std::string& name) const;
    stuBlock* GetBlock(const std::string& name);
    const stuBlock* GetBlock(const std::string& name) const;
    std::vector<std::string> GetAllBlockNames() const;
    size_t GetBlockCount() const;

    // 向块内添加图元
    void AddEntityToBlock(const std::string& blockName, const variantDxfEntity& entity);


    // 查询/遍历
    int GetTotalEntityCount() const;
    int GetEntityCount(EntityType type) const;
    int GetEntityCount(const std::string& layerName, EntityType type) const;
    // 按类型过滤
    std::vector<variantDxfEntity> GetEntitiesByType(EntityType type) const;
    // 按图层过滤
    std::vector<variantDxfEntity> GetEntitiesByLayer(const std::string& layerName) const;
    // 按图层+类型过滤
    std::vector<variantDxfEntity> GetEntities(const std::string& layerName, EntityType type) const;

    void SetSelectedEntity(const stuSelectedEntity& sel);
    void ClearSelection();
    stuSelectedEntity& GetSelectedEntity();
    const stuSelectedEntity& GetSelectedEntity() const;

    void SetPreviewEntity(const stuPreviewEntity& prev);
    void ClearPreview();
    stuPreviewEntity& GetPreviewEntity();
    const stuPreviewEntity& GetPreviewEntity() const;

    void   SetMoveStep(double step);
    double GetMoveStep() const;
    void   SetRotateStepRad(double rad);
    double GetRotateStepRad() const;

    void Clear();

private:
    DxfDocument m_document;
};
