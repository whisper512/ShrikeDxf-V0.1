#include "DxfLayerTableviewModel.h"


CDxfLayerTableviewModel::CDxfLayerTableviewModel()
{
}

CDxfLayerTableviewModel::~CDxfLayerTableviewModel()
{
}

void CDxfLayerTableviewModel::UpdateLayerTableViewModel(const std::map<std::string, stuLayer>& mapDxf)
{
	this->clear();
	this->setHorizontalHeaderLabels({ "NO", "NAME", "COLOR" });

	QList<QStandardItem*> listItems;
	int Index = 0;
	
	for (auto it = mapDxf.begin(); it != mapDxf.end(); ++it)
	{
		QStandardItem* pItemNo = new QStandardItem(QString::number(Index + 1));
		pItemNo->setTextAlignment(Qt::AlignCenter);
		listItems.append(pItemNo);

        QStandardItem* pItemLayerName = new QStandardItem(QString::fromStdString(it->first));
		pItemLayerName->setTextAlignment(Qt::AlignCenter);
		listItems.append(pItemLayerName);

		QStandardItem* pItemColor = new QStandardItem();
		pItemColor->setBackground(QBrush(it->second.color));
		QString strColor = it->second.color.name();
		pItemColor->setText(strColor);
		pItemColor->setTextAlignment(Qt::AlignCenter);
		listItems.append(pItemColor);

		appendRow(listItems);
		Index++;
	}
}

void CDxfLayerTableviewModel::UpdateLayerChangedData(std::map<std::string, stuLayer>& mapDxf)
{
	int rowCount = this->rowCount();

	if (rowCount != static_cast<int>(mapDxf.size()))
	{
		return;
	}

	auto it = mapDxf.begin();
	for (int row = 0; row < rowCount && it != mapDxf.end(); ++row,++it)
	{
		QStandardItem* pItemNo = this->item(row, 0);
		
		if (pItemNo)
		{
			
		}
		QStandardItem* pItemName = this->item(row, 1);
		if (pItemName)
		{

		}
		QStandardItem* pItemColor = this->item(row, 2);
		if (pItemColor)
		{
			it->second.color = pItemColor->background().color();
		}
	}
}

void CDxfLayerTableviewModel::ClearModel()
{
	clear();
}



