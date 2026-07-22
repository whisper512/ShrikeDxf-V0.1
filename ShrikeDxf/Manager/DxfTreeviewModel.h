#pragma once

#include <QStandardItemModel>
#include <QString>
#include <map>
#include <vector>

#include "DxfStruct.h"


//用于treeview的model
class DxfTreeviewModel : public QStandardItemModel
{
	Q_OBJECT

public:
	explicit DxfTreeviewModel(QObject* parent = nullptr);
	~DxfTreeviewModel();

	// 更新model
	void updateLayoutItemModel(const std::map<std::string, Layer>& mapDxf);
	void clearModel();

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
