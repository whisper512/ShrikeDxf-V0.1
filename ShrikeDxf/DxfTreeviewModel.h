#pragma once

#include <QStandardItemModel>
#include <QString>
#include <map>
#include <vector>

#include "DxfStruct.h"

//#include "stuLayout.h"  // 假设stuLayout和相关几何类已定义

//用于显示dxf的treeview的model
class CDxfTreeviewModel : public QStandardItemModel
{
	Q_OBJECT

public:
	explicit CDxfTreeviewModel(QObject* parent = nullptr);
	~CDxfTreeviewModel();

	// 更新model
	void UpdateLayoutItem(const std::map<std::string, stuLayer>& layer);
	void ClearModel();

private:
	std::map<std::string, stuLayer> m_layer;



};
