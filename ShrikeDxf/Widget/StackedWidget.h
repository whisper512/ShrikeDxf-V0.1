#pragma once
#include <QObject>
#include <QStackedWidget>
#include <map>

#include "PointAttribute.h"
#include "LineAttribute.h"
#include "CircleAttributeWidget.h"
#include "ArcAttritubeWidget.h"
#include "LWPolylineAttributeWidget.h"
#include "EllipseAttritube.h"
#include "TextAttritubeWidget.h"
#include "MTextAttritubeWidget.h"
#include "PolylineAttritubeWidget.h"
#include "SplineAttributeWidget.h"
#include "HatchAttributeWidget.h"

#include "Manager/DxfStruct.h"

class StackedWidgetManager : public QObject
{
	Q_OBJECT
public:
	StackedWidgetManager(QWidget* mainWnd);
	~StackedWidgetManager();

	void createStackedWidget();

private:
	void addPages();
	void connectSignalAndSlot();
	void changeWidgets();

public:
	QWidget* m_mainWnd;
	QGridLayout* m_layout;
	QStackedWidget* m_stackedWidget;
	PointAttributeWidget* m_pointAttributeWidget;
	LineAttributeWidget* m_lineAttributeWidget;
	CircleAttributeWidget* m_circleAttributeWidget;
	ArcAttritubeWidget* m_arcAttributeWidget;
	LWPolylineAttributeWidget* m_lwPolylineAttributeWidget;
	EllipseAttritubeWidget* m_ellipseAttributeWidget;
	CTextAttritubeWidget* m_textAttributeWidget;
	MTextAttritubeWidget* m_mTextAttributeWidget;
	PolylineAttributeWidget* m_polylineAttributeWidget;
	SplineAttributeWidget* m_splineAttributeWidget;
	HatchAttributeWidget* m_hatchAttributeWidget;

	EntityType m_entityType = EntityType::None;


	std::map<int, QString> m_mapPages;


signals:
	// 通知对应图元界面修改属性
	void signalPointAttribute(EntityPoint point);
	void signalLineAttribute(EntityLine line);
	void signalCircleAttribute(EntityCircle circle);
	void signalArcAttribute(EntityArc arc);
	void signalLWPolylineAttribute(EntityLWPolyline lwpolyline);
	void signalPolylineAttribute(EntityPolyline polyline);
    void signalTextAttribute(EntityText text);
	void signalMTextAttribute(EntityMText mtext);
	void signalEllipseAttribute(EntityEllipse ellipse);
	void signalSplineAttribute(EntitySpline spline);
	void signalHatchAttribute(EntityHatch hatch);

	// 图元数据发生变化
	void signalPointChanged(const EntityPoint& point);
	void signalLineChanged(const EntityLine& line);
	void signalCircleChanged(const EntityCircle& circle);
	void signalArcChanged(const EntityArc& arc);
    void signalLWPolylineChanged(const EntityLWPolyline& lwpolyline);
	void signalPolylineChanged(const EntityPolyline& polyline);
	void signalTextChanged(const EntityText& text);
    void signalMTextChanged(const EntityMText& mtext);
	void signalEllipseChanged(const EntityEllipse& ellipse);
    void signalSplineChanged(const EntitySpline& spline);
	void signalHatchChanged(const EntityHatch& hatch);

public slots:

	void handleRefreshStackedWidget(const SelectedEntity& entity);

};
