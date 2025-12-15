#pragma once
#ifndef DxfStruct_h
#define DxfStruct_h


#include <iostream>
#include <QString>
#include <vector>
#include <map>

#include "Primitive.h"

//存储dxf文件数据结构

using namespace std;

//层图实体数据
using DxfEntity = std::variant<Point, Line, Circle, Arc, Polyline, Text>;

class CDxfStruct
{
public:
	CDxfStruct();
	~CDxfStruct();

private:
	//所有图层数据
	map<string, vector<DxfEntity>> m_mapAllLayerEntities;
	// 辅助数据结构：快速统计
	map<string, int> m_mapAllLayerEntityCount;
	//viewport数据
    vector<ViewPort> m_vecViewPorts;

public:
	//添加图层数据
	void addEntity(const string& layer, const DxfEntity& entity);

	// 获取所有图层名称
	vector<string> getLayerNames() const;

	// 获取指定图层的所有实体
	const vector<DxfEntity>& getEntities(const string& layer) const;

	// 获取整个数据结构
	const std::map<std::string, std::vector<DxfEntity>>& getAllEntities() const;

	// 检查图层是否存在
	bool hasLayer(const std::string& layer) const;

	// 获取图层数量
	size_t getLayerCount() const;

	// 获取实体总数
	size_t getTotalEntityCount() const;

	// 获取指定图层的实体数量
	size_t getEntityCount(const std::string& layer) const;

	// 清空所有数据
	void clearAll();

	// 删除指定图层
	bool removeLayer(const std::string& layer);

	// 合并图层
	void mergeLayers(const std::string& targetLayer, const std::string& sourceLayer);

	// 统计信息
	void printStatistics() const;
};



#endif // DxfStruct_h