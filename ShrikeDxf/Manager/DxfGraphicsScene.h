#pragma once
#include <QString>
#include <QList>
#include <map>
#include <vector>
#include <QGraphicsScene>
#include <QGraphicsRectItem> 

#include "DxfStruct.h"



class DxfGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	explicit DxfGraphicsScene(QObject *parent = nullptr);
	~DxfGraphicsScene();

	//获取数据并且绘制
	void dxfDraw(const std::map<std::string, Layer>& mapDxf);
	
	double getScale() { return m_scale; }
	void clearScene();

	void clearPreview();
	void addPreviewPoint(QPointF pos);
	void addPreviewLine(QPointF p1, QPointF p2);
	void addPreviewCircle(QPointF center, qreal radius);
	void addPreviewArc(QPointF center, qreal radius, qreal startAngle, qreal endAngle);
	void addPreviewPolyline(const QVector<QPointF>& points, const QPointF& mousePos);
	void addPreviewRectangle(QPointF p1, QPointF p2);
	void addPreviewEllipse(QPointF center, QPointF majorEnd, double ratio);
	void addPreviewSplineFit(const QVector<QPointF>& fitPoints, const QPointF& mousePos);
	void addPreviewSplineControl(const QVector<QPointF>& ctrlPoints, const QPointF& mousePos);
	void addPreviewTextRect(QPointF p1, QPointF p2);

	void showGrips(const QRectF& bounds);
	void removeGrips();
	bool hasGrips() const { return !m_gripItems.isEmpty(); }
	int  gripAtPos(QPointF scenePos) const;   // 返回 -1 表示没点到, 0~7 表示手柄索引
	void updateGripRect(const QRectF& bounds); // 拖拽中更新虚线框
	QRectF getGripBounds() const;

public:
	//缩放比例
	double m_scale;
	
	


private:
	const std::map<std::string, Layer>* m_currentLayersEntitiesData = nullptr;

	// 预览图元存储列表
	QList<QGraphicsItem*> m_previewItems;

	// 虚线框
	QGraphicsRectItem* m_boundingBox = nullptr;
	// 8个手柄方块
	QVector<QGraphicsRectItem*> m_gripItems;

private:
	// 绘制图元
	void drawPoint(const EntityPoint& point);
    void drawLine(const EntityLine& line);
    void drawCircle(const EntityCircle& circle);
    void drawArc(const EntityArc& arc);
    void drawPolyline(const EntityPolyline& polyline);
	void drawLWPolyline(const EntityLWPolyline& polyline);
    void drawSpline(const EntitySpline& spline);
    void drawText(const EntityText& text);
    void drawMText(const EntityMText& mtext);
    void drawEllipse(const EntityEllipse& ellipse);
    void drawSolid(const EntitySolid& solid);
    void drawHatch(const EntityHatch& hatch);
	void drawSceneBackground(QRectF& rect);


private:
	// 计算场景边界
	QRectF calculateSceneBounds(const std::map<std::string, Layer>& mapDxf);
	// 获取entity颜色
	QColor getEntityColor(const EntityProp& prop) const;
	 // B样条基函数
	double bSplineBasis(int i, int k, double u, const std::vector<double>& knots);
	

};
