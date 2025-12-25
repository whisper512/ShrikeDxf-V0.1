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
            //DrawPoint(*itPoint, Qt::red);
            Point testPoint1 = { 0, 0 ,0};
            Point testPoint2 = { 100, 100,0 };
            Point testPoint3 = { -100, -100,0 };
            DrawPoint(testPoint1, Qt::red);
            DrawPoint(testPoint2, Qt::blue);
            DrawPoint(testPoint3, Qt::green);
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
            //DrawArc(*itArc, layer.color);
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
    //addEllipse(point.x-1, (-point.y)-1, 50, 50, QPen(color), QBrush(color));
     // 使用更大的尺寸和更明显的样式
    qreal size = 20;  // 增大点的尺寸
    addEllipse(point.x - size / 2, point.y - size / 2, size, size,
        QPen(color, 2), QBrush(color));

    // 添加十字标记使点更加明显
    qreal crossSize = size;
    addLine(point.x - crossSize / 2, point.y,
        point.x + crossSize / 2, point.y, QPen(color, 2));
    addLine(point.x, point.y - crossSize / 2,
        point.x, point.y + crossSize / 2, QPen(color, 2));
}

void CDxfGraphicsScene::DrawLine(const Line& line, const QColor& color)
{
    //addLine(line.pointStart.x, line.pointStart.y, line.pointEnd.x, line.pointEnd.y, QPen(color));
}

void CDxfGraphicsScene::DrawCircle(const Circle& circle, const QColor& color)
{
    //addEllipse(circle.pointCenter.x, circle.pointCenter.y, circle.radius, circle.radius, QPen(color), QBrush(color));
}

void CDxfGraphicsScene::DrawArc(const Arc& arc, const QColor& color)
{
}

void CDxfGraphicsScene::DrawPolyline(const Polyline& polyline, const QColor& color)
{
}

void CDxfGraphicsScene::DrawText(const Text& text, const QColor& color)
{
}
