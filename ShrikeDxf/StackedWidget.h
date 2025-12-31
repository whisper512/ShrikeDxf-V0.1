#pragma once
#include <QObject>
#include <QStackedWidget>

class CStackedWidgetManger : public QObject
{
public:
	CStackedWidgetManger(QWidget* pMainwnd);
	~CStackedWidgetManger();

	void CreateStackedWidget();
private:
	QWidget* m_pMainwnd;
	QStackedWidget* m_pStackedWidget;
};
