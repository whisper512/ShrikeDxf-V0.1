#pragma once
#include <QStandardItemModel>
#include <QString>
#include "DxfStruct.h"


class CDxfLayerTableviewModel : public QStandardItemModel
{
public:
	CDxfLayerTableviewModel();
	~CDxfLayerTableviewModel();

	//更新model
	void UpdateLayerTableViewModel(const std::map<std::string, stuLayer>& mapDxf);
	//table更新后,更新model
	void UpdateLayerChangedData(std::map<std::string, stuLayer>& mapDxf);
	void ClearModel();
private:
	

};
