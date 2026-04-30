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
	void DxfDraw(const std::map<std::string, stuLayer>& mapDxf);
	void ClearScene();
	double GetScale() { return m_scale; }



public:
	//缩放比例
	double m_scale;

public:
    //graphics鼠标当前状态
	enumMouseStateInView m_MouseState = enumMouseStateInView_None;

	void MouseMove(QPointF pos,QColor color);
	//void MouseLeftClick(QPointF pos,QColor color);
	//void MouseRightClick(QPointF pos);
	//void ChangePreviewEntityByMouseState();

	
	

private:
	void DrawPoint(const EntityPoint& point);
    void DrawLine(const EntityLine& line);
    void DrawCircle(const EntityCircle& circle);
    void DrawArc(const EntityArc& arc);
    void DrawPolyline(const EntityPolyline& polyline);
	void DrawLWPolyline(const EntityLWPolyline& polyline);
    void DrawSpline(const EntitySpline& spline);
    void DrawText(const EntityText& text);
    void DrawMText(const EntityMText& mtext);
    void DrawEllipse(const EntityEllipse& ellipse);
    void DrawSolid(const EntitySolid& solid);
    void DrawHatch(const EntityHatch& hatch);


private:
	QRectF CalculateSceneBounds(const std::map<std::string, stuLayer>& mapDxf);
	QColor GetEntityColor(const EntityProp& prop) const;
	double BSplineBasis(int i, int k, double u, const std::vector<double>& knots);
	

};
