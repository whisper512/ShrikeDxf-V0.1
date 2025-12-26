#include<QPainterPath>

#include "DxfGraphicsScene.h"



CDxfGraphicsScene::CDxfGraphicsScene(QObject* parent):
    QGraphicsScene(parent),
    m_scale(1.0)
{
}

CDxfGraphicsScene::~CDxfGraphicsScene()
{

}

void CDxfGraphicsScene::DxfDraw(const map<string,stuLayer>& mapdxf)
{
    ClearScene();

    for (const auto& pairLayer : mapdxf)
    {
        const stuLayer layer = pairLayer.second;
       

        for (auto itPoint = layer.vecPoints.begin(); itPoint != layer.vecPoints.end(); ++itPoint)
        {
            //DrawPoint(*itPoint, layer.color);
        }
        for (auto itLine = layer.vecLines.begin(); itLine != layer.vecLines.end(); ++itLine)
        {
            //DrawLine(*itLine, layer.color);
        }
        for (auto itCircle = layer.vecCircles.begin(); itCircle != layer.vecCircles.end(); ++itCircle)
        {
            //DrawCircle(*itCircle, layer.color);
        }
        for (auto itArc = layer.vecArcs.begin(); itArc != layer.vecArcs.end(); ++itArc)
        {
            DrawArc(*itArc, Qt::red);
        }
        for (auto itPolyline = layer.vecPolylines.begin(); itPolyline != layer.vecPolylines.end(); ++itPolyline)
        {
            //DrawPolyline(*itPolyline, layer.color);
        }
        for (auto itText = layer.vecTexts.begin(); itText != layer.vecTexts.end(); ++itText)
        {
            //DrawText(*itText, layer.color);
        }   
    }
    setSceneRect(itemsBoundingRect());
    QRectF rect = sceneRect();
}

void CDxfGraphicsScene::ClearScene()
{
    clear();
}

void CDxfGraphicsScene::DrawPoint(const Point& point, const QColor& color)
{
    qreal size = 1;
    addEllipse(point.x - size / 2, point.y - size / 2, size, size,QPen(color, 2), QBrush(color));

    // 添加十字标记使点更加明显
    qreal crossSize = size + size * 10;
    addLine(point.x - crossSize / 2, point.y,point.x + crossSize / 2, point.y, QPen(color, 2));
    addLine(point.x, point.y - crossSize / 2,point.x, point.y + crossSize / 2, QPen(color, 2));
}

void CDxfGraphicsScene::DrawLine(const Line& line, const QColor& color)
{
    addLine(line.pointStart.x, line.pointStart.y, line.pointEnd.x, line.pointEnd.y, QPen(color));
}

void CDxfGraphicsScene::DrawCircle(const Circle& circle, const QColor& color)
{
    addEllipse(circle.pointCenter.x, circle.pointCenter.y, circle.radius, circle.radius, QPen(color), Qt::NoBrush);
}

void CDxfGraphicsScene::DrawArc(const Arc& arc, const QColor& color)
{
    // 创建一个矩形作为圆弧的边界框

    //50,50为左下坐标位置

    QRectF rectAngle(50,50,100,100);
    QPainterPath path;
    //path.moveTo(50 + 100*cos(290), 50+100*sin(290));  // 矩形右边中点
    path.moveTo(100, 100);
    
    path.arcTo(rectAngle,290, 270);  // 起始角度0度，跨越90度

    // 将路径添加到场景中
    addPath(path, QPen(Qt::red, 2), Qt::NoBrush);
    


    // 将路径添加到场景中
    addPath(path, QPen(Qt::red, 2), Qt::NoBrush);
}

void CDxfGraphicsScene::DrawPolyline(const Polyline& polyline, const QColor& color)
{
}

void CDxfGraphicsScene::DrawText(const Text& text, const QColor& color)
{
}
