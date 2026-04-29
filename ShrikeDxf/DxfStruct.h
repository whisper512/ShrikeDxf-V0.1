#pragma once
#ifndef DXFSTRUCT_H
#define DXFSTRUCT_H

#include <iostream>
#include <QString>
#include <QColor>
#include <vector>
#include <map>
#include <variant>

#include "Entity.h"

// ═══════════════════════════════════════════════════════════════
// DXF 颜色索引映射（保留原有逻辑）
// ═══════════════════════════════════════════════════════════════
class DxfColorMap
{
public:
    static const std::map<int, QColor>& getColorMap()
    {
        //static std::map<int, QColor> colorMap = []()
        //    {
        //        std::map<int, QColor> map;

        //        // 标准颜色 (1-7)
        //        map[1] = Qt::red;
        //        map[2] = Qt::yellow;
        //        map[3] = Qt::green;
        //        map[4] = Qt::cyan;
        //        map[5] = Qt::blue;
        //        map[6] = Qt::magenta;
        //        map[7] = Qt::white;

        //        // 8-9 灰度
        //        for (int i = 8; i <= 9; ++i) {
        //            int gray = 255 - (i - 8) * 50;
        //            map[i] = QColor(gray, gray, gray);
        //        }

        //        // 10-249 彩色
        //        for (int i = 10; i <= 249; ++i) {
        //            int colorIndex = i - 10;
        //            int colorGroup = colorIndex / 16;
        //            int indexInGroup = colorIndex % 16;

        //            int r = 0, g = 0, b = 0;
        //            switch (colorGroup) {
        //            case 0:  r = 255;           g = indexInGroup * 17;   b = 0;              break;
        //            case 1:  r = 255 - indexInGroup * 17;  g = 255;    b = 0;              break;
        //            case 2:  r = 0;             g = 255;                b = indexInGroup * 17; break;
        //            case 3:  r = 0;             g = 255 - indexInGroup * 17; b = 255;       break;
        //            case 4:  r = indexInGroup * 17;  g = 0;            b = 255;            break;
        //            case 5:  r = 255;           g = 0;                  b = 255 - indexInGroup * 17; break;
        //            case 6:  r = 255 - indexInGroup * 17;  g = 0;      b = 0;              break;
        //            case 7:  r = 255;           g = indexInGroup * 17; b = indexInGroup * 17; break;
        //            case 8:  r = 255 - indexInGroup * 17;  g = 255;    b = indexInGroup * 17; break;
        //            case 9:  r = 255 - indexInGroup * 17;  g = 255 - indexInGroup * 17; b = 255; break;
        //            default: r = g = b = 128; break;
        //            }
        //            map[i] = QColor(r, g, b);
        //        }

        //        // 250-255 灰度
        //        for (int i = 250; i <= 255; ++i) {
        //            int gray = 255 - (i - 250) * 40;
        //            map[i] = QColor(gray, gray, gray);
        //        }

        //        // 0号颜色（随块）
        //        map[0] = Qt::black;

        //        return map;
        //    }();
        //return colorMap;
    }

    static int getColorIndex(const QColor& color)
    {
        const auto& colorMap = getColorMap();
        for (const auto& pair : colorMap) {
            if (pair.second == color)
                return pair.first;
        }

        int minDistance = INT_MAX;
        int closestIndex = 7;
        for (const auto& pair : colorMap) {
            int rDiff = abs(pair.second.red() - color.red());
            int gDiff = abs(pair.second.green() - color.green());
            int bDiff = abs(pair.second.blue() - color.blue());
            int distance = rDiff + gDiff + bDiff;
            if (distance < minDistance) {
                minDistance = distance;
                closestIndex = pair.first;
            }
        }
        return closestIndex;
    }
};

// ═══════════════════════════════════════════════════════════════
// 图层信息
// ═══════════════════════════════════════════════════════════════
struct stuLayer
{
    QColor  color = Qt::white;
    QString lineType = "BYLAYER";
    int     lineWeight = -1;
    bool    isVisible = true;
    bool    isLocked = false;

    // 该图层所有图元 — 统一用 EntityVariant 存储
    std::vector<variantDxfEntity> entities;

    // 辅助：获取该图层某种类型的图元数量
    int GetEntityCount(EntityType type) const {
        int count = 0;
        for (auto& ent : entities) {
            if (GetEntityType(ent) == type)
                count++;
        }
        return count;
    }
};

// ═══════════════════════════════════════════════════════════════
// 块定义
// ═══════════════════════════════════════════════════════════════
struct stuBlock
{
    QString     name;           // 块名
    Vertex3D    basePoint;      // 基点
    int         flags = 0;      // 块标志

    // 块内的所有图元
    std::vector<variantDxfEntity> entities;
};

// ═══════════════════════════════════════════════════════════════
// 选中的图元
// ═══════════════════════════════════════════════════════════════
struct stuSelectedEntity
{
    EntityType  type = EntityType::None;
    QString     strLayer;
    int         index = -1;     // 在 stuLayer::entities 中的索引
    variantDxfEntity entity;

    stuSelectedEntity() = default;
};

// ═══════════════════════════════════════════════════════════════
// 预览图元（用于绘制时的临时预览）
// ═══════════════════════════════════════════════════════════════
struct stuPreviewEntity
{
    enum enumType
    {
        None = -1,
        Point,
        Line,
        CenterRadiusCircle,
        DiameterCircle,
        CenterEndpointArc,
        ThreePointsArc,
        Polyline,
    };

    enumType type = None;
    QString  strLayer;
};

// ═══════════════════════════════════════════════════════════════
// 文档整体结构
// ═══════════════════════════════════════════════════════════════
struct DxfDocument
{
    // ─── 头部变量 ───
    QString     version;            // DXF 版本 (AC1009, AC1015...)
    double      insUnits = 1.0;     // 单位 (1=毫米)
    Vertex3D    extMin, extMax;     // 图形范围
    double      ltscale = 1.0;      // 线型比例

    // ─── 表 ───
    std::map<std::string, stuLayer> layers;     // 图层
    std::map<std::string, stuBlock> blocks;     // 块定义

    // ─── 选中/预览状态 ───
    stuSelectedEntity  selectedEntity;
    stuPreviewEntity   previewEntity;

    // ─── 编辑参数 ───
    double moveStep = 1.0;
    double rotateStepRad = 0.0174533;   // 1度

    // ─── 便捷查询 ───
    // 获取所有图元（不分图层，展开所有块？还是只取模型空间？取决于实现）
    int GetTotalEntityCount() const {
        int count = 0;
        for (auto& [name, layer] : layers) {
            count += layer.entities.size();
        }
        return count;
    }

    // 清空所有数据
    void Clear() {
        version.clear();
        insUnits = 1.0;
        extMin = Vertex3D();
        extMax = Vertex3D();
        ltscale = 1.0;
        layers.clear();
        blocks.clear();
        selectedEntity = stuSelectedEntity();
        previewEntity = stuPreviewEntity();
    }
};

// ═══════════════════════════════════════════════════════════════
// 便捷工具函数（替代原来的 GetVariantDxfEntity）
// ═══════════════════════════════════════════════════════════════

// 按类型过滤图元
inline std::vector<variantDxfEntity> FilterEntitiesByType(
    const std::vector<variantDxfEntity>& entities, EntityType type)
{
    std::vector<variantDxfEntity> result;
    for (auto& ent : entities) {
        if (GetEntityType(ent) == type)
            result.push_back(ent);
    }
    return result;
}

// 按图层过滤图元
inline std::vector<variantDxfEntity> FilterEntitiesByLayer(
    const std::map<std::string, stuLayer>& layers, const std::string& layerName)
{
    auto it = layers.find(layerName);
    if (it != layers.end())
        return it->second.entities;
    return {};
}

#endif // DXFSTRUCT_H
