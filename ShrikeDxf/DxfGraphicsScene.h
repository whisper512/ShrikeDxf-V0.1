#pragma once
#include <QString>
#include <map>
#include <vector>
#include < QGraphicsScene>

#include "DxfStruct.h"

enum enumMouseStateInView
{
	enumMouseStateInView_None = -1,
	enumMouseStateInView_PreviewPoint,
	enumMouseStateInView_PreviewLine,
	enumMouseStateInView_PreviewCircleCenterRadius,
	enumMouseStateInView_PreviewCircleDiameter,
	enumMouseStateInView_PreviewArcCenterEndpoint,
	enumMouseStateInView_PreviewArcThreePoints,
	enumMouseStateInView_PreviewPolyline,
};

class CDxfGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	explicit CDxfGraphicsScene(QObject *parent = nullptr);
	~CDxfGraphicsScene();

	//获取数据并且绘制
	void DxfDraw(const map<string, stuLayer> &mapdxf);
	void ClearScene();
	double GetScale() { return m_scale; }



public:
	//缩放比例
	double m_scale;

private:

public:
    //graphics鼠标当前状态
	enumMouseStateInView m_MouseState = enumMouseStateInView_None;

	//预览图元
    stuPreviewEntity m_PreviewEntity;
	QList<QGraphicsItem*> m_PreviewItems;
	//预览线段起点
	QPointF m_pointLineStart;
	//存储圆心位置
	QPointF m_pointCircleCenter;
	//直径画圆存储直径起点位置
	QPointF m_pointDiameterStart;
	//圆心-起点-终点画弧
	QPointF m_pointArcCenter;  // 存储弧线圆心
	QPointF m_pointArcStart;   // 存储弧线起点
	//三点画弧存储弧线
	QPointF m_ArcFirstPoint;   // 存储弧线的起点
	QPointF m_ArcSecondPoint;  // 存储弧线的中间点
	//多段线
	QVector<QPointF> m_vecPolylinePoints;

	Line m_linePreview;
	Circle m_circlePreview;
	Arc m_arcPreview;

	void MouseMove(QPointF pos,QColor color);
	void MouseLeftClick(QPointF pos,QColor color);
	void MouseRightClick(QPointF pos);
	void ChangePreviewEntityByMouseState();

	void clearPreviewEntityData();
	void ClearPreviewItems();
	void DrawPreviewPoint(const Point& point,QColor color);
	void DrawPreviewLine(const Line& line,QColor color);
    void DrawPreviewCircleWithCenterAndRadius(const Circle& circle, const QPointF& MousePos, QColor color);
	void DrawPreviewCircleWithDiameter(const Line& diameter, QColor color);
	void DrawPreviewArc(const Arc& arc, const QColor& color,const Point& pointStart ,const Point& pointEnd);

	void ProcessPreviewLineWhenMouseMove(QPointF pos,QColor color);
	void ProcessPreviewLineWhenMouseLeftClick(QPointF pos,QColor color);

	void ProcessPreviewCircleCenterRadiusWhenMouseMove(QPointF pos,QColor color);
    void ProcessPreviewCircleCenterRadiusWhenMouseLeftClick(QPointF pos,QColor color);

	void ProcessPreviewCircleDiameterWhenMouseMove(QPointF pos,QColor color);
	void ProcessPreviewCircleDiameterWhenMouseLeftClick(QPointF pos,QColor color);

	void ProcessPreviewArcCenterEndpointWhenMouseMove(QPointF pos,QColor color);
    void ProcessPreviewArcCenterEndpointWhenMouseLeftClick(QPointF pos,QColor color);

	void ProcessPreviewArcThreePointsWhenMouseMove(QPointF pos,QColor color);
	void ProcessPreviewArcThreePointsWhenMouseLeftClick(QPointF pos,QColor color);
	void CalculateCircleFromThreePoints(const Point& p1, const Point& p2, const Point& p3, Point& center, double& radius);

	void ProcessPreviewPolylineWhenMouseMove(QPointF pos,QColor color);
    void ProcessPreviewPolylineWhenMouseLeftClick(QPointF pos,QColor color);
private:
	void DrawPoint(const Point& point,const QColor& color);
	void DrawLine(const Line& line, const QColor& color);
	void DrawCircle(const Circle& circle, const QColor& color);
	void DrawArc(const Arc& arc, const QColor& color);
	void DrawPolyline(const Polyline& polyline, const QColor& color);
	void DrawText(const Text& text, const QColor& color);

	//动态计算pen的宽度
	qreal CalculateDynamicPenWidth();
	//提前计算scene的的边界
	QRectF CalculateSceneBounds(const map<string, stuLayer>& mapdxf);

};
