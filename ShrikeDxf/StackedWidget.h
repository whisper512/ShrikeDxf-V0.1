#pragma once
#include <QObject>
#include <QStackedWidget>
#include <map>

#include "PointAttributeClass.h"

class CStackedWidgetManger : public QObject
{
public:
	CStackedWidgetManger(QWidget* pMainwnd);
	~CStackedWidgetManger();

	void CreateStackedWidget();

private:
	void AddPages();

private:
	QWidget* m_pMainwnd;
	QStackedWidget* m_pStackedWidget;
	PointAttributeClass* m_pPointAttributeClass;
	
	std::map<int, QString> m_mapPages;
};
