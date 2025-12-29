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
            DrawPoint(*itPoint, layer.color);
        }
        for (auto itLine = layer.vecLines.begin(); itLine != layer.vecLines.end(); ++itLine)
        {
            DrawLine(*itLine, layer.color);
        }
        for (auto itCircle = layer.vecCircles.begin(); itCircle != layer.vecCircles.end(); ++itCircle)
        {
            DrawCircle(*itCircle, layer.color);
        }
        for (auto itArc = layer.vecArcs.begin(); itArc != layer.vecArcs.end(); ++itArc)
        {
            DrawArc(*itArc, layer.color);
        }
        for (auto itPolyline = layer.vecPolylines.begin(); itPolyline != layer.vecPolylines.end(); ++itPolyline)
        {
            DrawPolyline(*itPolyline, layer.color);
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
    //daxflib中读取到的坐标y轴向上为正方向,需要翻转y坐标
    QPainterPath path;
    // 翻转y坐标
    QPointF flippedCenter(arc.pointCenter.x, arc.pointCenter.y);
    //弧形外界矩形
    QRectF flippedRect(flippedCenter.x() - arc.radius, flippedCenter.y() - arc.radius, arc.radius * 2, arc.radius * 2);
    
    double startAngleRad = qDegreesToRadians(-arc.startAngle);
    path.moveTo(flippedCenter + QPointF(arc.radius * cos(startAngleRad), -arc.radius * sin(startAngleRad)));

    double startAngle = fmod(arc.startAngle, 360);
    double endAngle = fmod(arc.endAngle, 360);

    //在dxflib和scene中，都以逆时针方向为角度正方向,0度在x轴正方向
    double spanLength = endAngle - startAngle;
    if (spanLength < 0)
    {
        //y轴镜像,起始角度取负
        //如果spanLength为负数，则表示弧线跨越了0度，需要将其调整为正数,顺时针画弧
        spanLength += 360;
        // 顺时针画过0度的弧线
        path.arcTo(flippedRect, -startAngle, -spanLength);
    }
    else
    {
        path.arcTo(flippedRect, -startAngle, -spanLength);
    }
    addPath(path, QPen(color, 1), Qt::NoBrush);
}

void CDxfGraphicsScene::DrawPolyline(const Polyline& polyline, const QColor& color)
{
    if (polyline.vecVertices.size() < 2) {
        return; // 如果点数少于2个，无法绘制多段线
    }

    QPen pen(color);
    QPainterPath path;

    // 移动到第一个点
    path.moveTo(polyline.vecVertices[0].x, polyline.vecVertices[0].y);

    // 连接后续所有点
    for (size_t i = 1; i < polyline.vecVertices.size(); ++i) 
    {
        path.lineTo(polyline.vecVertices[i].x, polyline.vecVertices[i].y);
    }

    // 如果多段线是闭合的，连接最后一个点和第一个点
    if (polyline.flag & 1) 
    {
        path.lineTo(polyline.vecVertices[0].x, polyline.vecVertices[0].y);
    }

    addPath(path, pen);
}

void CDxfGraphicsScene::DrawText(const Text& text, const QColor& color)
{
}
