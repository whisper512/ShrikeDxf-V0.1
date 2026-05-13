#pragma once
#include <QString>
#include <QList>
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
	void DxfDraw(const std::map<std::string, stuLayer>& mapDxf);
	
	double GetScale() { return m_scale; }
	void ClearScene();

	void ClearPreview();
	void AddPreviewPoint(QPointF pos);
	void AddPreviewLine(QPointF p1, QPointF p2);
	void AddPreviewCircle(QPointF center, qreal radius);
	void AddPreviewArc(QPointF center, qreal radius, qreal startAngle, qreal endAngle);
	void AddPreviewPolyline(const QVector<QPointF>& points, const QPointF& mousePos);
	void AddPreviewRectangle(QPointF p1, QPointF p2);
	void AddPreviewEllipse(QPointF center, QPointF majorEnd, double ratio);
	void AddPreviewSplineFit(const QVector<QPointF>& fitPoints, const QPointF& mousePos);
	void AddPreviewSplineControl(const QVector<QPointF>& ctrlPoints, const QPointF& mousePos);
	void AddPreviewTextRect(QPointF p1, QPointF p2);


public:
	//缩放比例
	double m_scale;
	//graphics鼠标当前状态
	


private:
	const std::map<std::string, stuLayer>* m_pCurrentLayersEntitiesData = nullptr;

	// 预览图元存储列表
	QList<QGraphicsItem*> m_previewItems;

private:
	// 绘制图元
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
	void DrawSceneBackground(QRectF& rect);


private:
	// 计算场景边界
	QRectF CalculateSceneBounds(const std::map<std::string, stuLayer>& mapDxf);
	// 获取entity颜色
	QColor GetEntityColor(const EntityProp& prop) const;
	 // B样条基函数
	double BSplineBasis(int i, int k, double u, const std::vector<double>& knots);
	

};
