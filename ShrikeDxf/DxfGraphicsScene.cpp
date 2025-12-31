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

    QPainterPath path;
    QPointF center(arc.pointCenter.x, arc.pointCenter.y);
    //圆弧外接矩形
    QRectF rect(center.x() - arc.radius,center.y() - arc.radius,arc.radius * 2, arc.radius * 2);

    //计算起点,起始角度是针对圆形中心的角度
    double startAngleRad = qDegreesToRadians(arc.startAngle);
    QPointF startPoint(center.x() + arc.radius * cos(startAngleRad),center.y() + arc.radius * sin(startAngleRad));
    path.moveTo(startPoint);
    //取整
    double startAngle = fmod(arc.startAngle, 360.0);
    double endAngle = fmod(arc.endAngle, 360.0);

    //计算跨度-保持DXF的逆时针方向画弧
    double spanLength = endAngle - startAngle;
    if (spanLength < 0) 
    {
        spanLength += 360.0;
    }

    //由于View做了Y轴镜像，角度需要转换
    // Qt起始角 = (360° - DXF起始角) % 360°
    double qtStartAngle = fmod(360.0 - startAngle, 360.0);
    //转为顺时针绘制圆弧
    path.arcTo(rect, qtStartAngle, -spanLength);
    addPath(path, QPen(color, 1), Qt::NoBrush);
}

void CDxfGraphicsScene::DrawPolyline(const Polyline& polyline, const QColor& color)
{
    if (polyline.vecVertices.size() < 2) 
    {
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
