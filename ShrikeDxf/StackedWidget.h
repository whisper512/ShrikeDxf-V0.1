#pragma once
#include <QObject>
#include <QStackedWidget>
#include <map>

#include "PointAttribute.h"
#include "LineAttribute.h"


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
	void AdjustWidget();
	void ChangeWidgets();
	
private:
	QWidget* m_pMainwnd;
	QStackedWidget* m_pStackedWidget;
	CPointAttributeWidget* m_pPointAttributeWidget;
	CLineAttributeWidget* m_pLineAttributeWidget;

	enumEntity m_entityType;


	std::map<int, QString> m_mapPages;


signals:
	void NoticePointAttribute(Point point);
	void NoticeLineAttribute(Line line);
	void NoticeCircleAttribute(Circle circle);
	void NoticeArcAttribute(Arc arc);
	void NoticePolylineAttribute(Polyline polyline);
    void NoticeTextAttribute(Text text);

public slots:
	void handleRefreshStackedWidget(DxfEntity dxfentity);


};
