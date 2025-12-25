#pragma once

#include <QStandardItemModel>
#include <QString>
#include <map>
#include <vector>

#include "DxfStruct.h"


//用于treeview的model
class CDxfTreeviewModel : public QStandardItemModel
{
	Q_OBJECT

public:
	explicit CDxfTreeviewModel(QObject* parent = nullptr);
	~CDxfTreeviewModel();

	// 更新model
	void UpdateLayoutItemModel(const std::map<std::string, stuLayer>& mapDxf);
	void ClearModel();

private:
	std::map<std::string, stuLayer> m_mapDxf;



};
