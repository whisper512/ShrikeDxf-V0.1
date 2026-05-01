#pragma once
#include <QObject>
#include <QStackedWidget>
#include <map>

#include "PointAttribute.h"
#include "LineAttribute.h"
#include "CircleAttributeWidget.h"
#include "ArcAttritubeWidget.h"
#include "PolylineAttributeWidget.h"

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
	void ChangeWidgets();

public:
	QWidget* m_pMainwnd;
	QGridLayout* m_pLayout;
	QStackedWidget* m_pStackedWidget;
	CPointAttributeWidget* m_pPointAttributeWidget;
	CLineAttributeWidget* m_pLineAttributeWidget;
	CCircleAttributeWidget* m_pCircleAttributeWidget;
	CArcAttritubeWidget* m_pArcAttributeWidget;
	CPolylineAttributeWidget* m_pPolylineAttributeWidget;



	EntityType m_entityType = EntityType::None;


	std::map<int, QString> m_mapPages;


signals:
	// 统治对应图元界面修改属性
	void signalPointAttribute(EntityPoint point);
	void signalLineAttribute(EntityLine line);
	void signalCircleAttribute(EntityCircle circle);
	void signalArcAttribute(EntityArc arc);
	void signalPolylineAttribute(EntityPolyline polyline);
    void signalTextAttribute(EntityText text);
public slots:

	void handleRefreshStackedWidget(const stuSelectedEntity& entity);


};
