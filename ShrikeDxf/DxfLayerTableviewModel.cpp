#include "DxfLayerTableviewModel.h"


CDxfLayerTableviewModel::CDxfLayerTableviewModel()
{
}

CDxfLayerTableviewModel::~CDxfLayerTableviewModel()
{
}

void CDxfLayerTableviewModel::UpdateLayerTableViewModel(const std::map<std::string, stuLayer>& mapDxf)
{
	this->setHorizontalHeaderLabels({ "NO", "NAME", "COLOR" });

	QList<QStandardItem*> listItems;
	int Index = 0;
	
	for (auto it = mapDxf.begin(); it != mapDxf.end(); ++it)
	{
		QStandardItem* pItemNo = new QStandardItem(QString::number(Index + 1));
		listItems.append(pItemNo);

        QStandardItem* pItemLayerName = new QStandardItem(QString::fromStdString(it->first));
		listItems.append(pItemLayerName);

		QStandardItem* pItemColor = new QStandardItem();
		pItemColor->setBackground(QBrush(it->second.color));
		QString strColor = it->second.color.name();
		pItemColor->setText(strColor);
		listItems.append(pItemColor);

		appendRow(listItems);
		Index++;
	}
}

void CDxfLayerTableviewModel::ClearModel()
{
	clear();
}



