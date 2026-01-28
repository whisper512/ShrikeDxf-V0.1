#pragma once
#include <QString>
#include <map>
#include <vector>
#include < QGraphicsScene>

#include "DxfStruct.h"

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
	QList<QGraphicsItem*> m_PreviewItems;
	void ClearPreviewItems();
	void DrawPreviewPoint(const Point& point,QColor color);
	void DrawPreviewLine(const Line& line,QColor color);
    void DrawPreviewCircleWithCenterAndRadius(const Circle& circle, const QPointF& MousePos, QColor color);
	void DrawPreviewCircleWithDiameter(const Line& diameter, QColor color);
	void DrawPreviewArc(const Arc& arc, const QColor& color);
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
