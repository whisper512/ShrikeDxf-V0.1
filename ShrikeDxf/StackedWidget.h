#pragma once
#include <QObject>
#include <QStackedWidget>
#include <map>

#include "PointAttributeClass.h"
#include "DxfStruct.h"

class CStackedWidgetManger : public QObject
{
	Q_OBJECT
public:
	CStackedWidgetManger(QWidget* pMainwnd);
	~CStackedWidgetManger();

	void CreateStackedWidget();

private:
	void AddPages();
	void ConnectSignalAndSlot();

private:
	QWidget* m_pMainwnd;
	QStackedWidget* m_pStackedWidget;
	PointAttributeClass* m_pPointAttributeClass;
	int m_indexEntity;

	std::map<int, QString> m_mapPages;


signals:
	void NoticePointAttribute(Point point);


public slots:
	void handleRefreshStackedWidget(DxfEntity dxfentity);


};
