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

public:

	

private:
	void DrawPoint(const Point& point,const QColor& color);
	void DrawLine(const Line& line, const QColor& color);
	void DrawCircle(const Circle& circle, const QColor& color);
	void DrawArc(const Arc& arc, const QColor& color);
	void DrawPolyline(const Polyline& polyline, const QColor& color);
	void DrawText(const Text& text, const QColor& color);

};
