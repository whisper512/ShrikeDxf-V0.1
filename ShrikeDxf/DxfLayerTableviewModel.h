#pragma once
#include <QStandardItemModel>
#include <QString>
#include "DxfStruct.h"


class CDxfLayerTableviewModel : public QStandardItemModel
{
public:
	CDxfLayerTableviewModel();
	~CDxfLayerTableviewModel();

	void UpdateLayerTableViewModel(const std::map<std::string, stuLayer>& mapDxf);
	void ClearModel();
private:
	

};
