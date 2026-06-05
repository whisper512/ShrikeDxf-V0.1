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


	QIcon m_iconPoint;
	QIcon m_iconLine;
	QIcon m_iconCircle;
    QIcon m_iconArc;
	QIcon m_iconPloyline;
	QIcon m_iconLayer;
	QIcon m_iconEllipse;
    QIcon m_iconSolid;
	QIcon m_iconSpline;
    QIcon m_iconText;
    QIcon m_iconHatch;

};
