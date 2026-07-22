#pragma once
#include <QStandardItemModel>
#include <QString>
#include "DxfStruct.h"


class DxfLayerTableviewModel : public QStandardItemModel
{
public:
	DxfLayerTableviewModel();
	~DxfLayerTableviewModel();

	//更新model
	void updateLayerTableViewModel(const std::map<std::string, Layer>& mapDxf);
	//table更新后,更新model
	void updateLayerChangedData(std::map<std::string, Layer>& mapDxf);
	void clearModel();
private:
	

};
