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

    QRectF bounds = CalculateSceneBounds(mapdxf);
    setSceneRect(bounds);

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
    addEllipse(point.x - size / 2, point.y - size / 2, size, size,QPen(color, CalculateDynamicPenWidth(1)), QBrush(color));

    // 获取场景尺寸
    QRectF sceneRect = this->sceneRect();
    qreal sceneSize = qMin(sceneRect.width(), sceneRect.height());

    // 根据场景大小动态调整十字标记大小
    qreal crossSize = sceneSize * 0.02; // 使用场景尺寸的2%作为十字标记的大小

    // 确保十字标记有最小尺寸
    crossSize = qMax(crossSize, size * 2);

    addLine(point.x - crossSize / 2, point.y, point.x + crossSize / 2, point.y, QPen(color, CalculateDynamicPenWidth(1)));
    addLine(point.x, point.y - crossSize / 2, point.x, point.y + crossSize / 2, QPen(color, CalculateDynamicPenWidth(1)));
}

void CDxfGraphicsScene::DrawLine(const Line& line, const QColor& color)
{
    addLine(line.pointStart.x, line.pointStart.y, line.pointEnd.x, line.pointEnd.y, QPen(color,CalculateDynamicPenWidth(1)));
}

void CDxfGraphicsScene::DrawCircle(const Circle& circle, const QColor& color)
{
    addEllipse(circle.pointCenter.x, circle.pointCenter.y, circle.radius, circle.radius, QPen(color,CalculateDynamicPenWidth(1)), Qt::NoBrush);
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
    addPath(path, QPen(color, CalculateDynamicPenWidth(1)), Qt::NoBrush);
}

void CDxfGraphicsScene::DrawPolyline(const Polyline& polyline, const QColor& color)
{
    if (polyline.vecVertices.size() < 2) 
    {
        return; // 如果点数少于2个，无法绘制多段线
    }

    QPen pen(color,CalculateDynamicPenWidth(1));
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



qreal CDxfGraphicsScene::CalculateDynamicPenWidth(qreal basewidth)
{
    QRectF sceneRect = this->sceneRect();
    qreal sceneSize = qMin(sceneRect.width(), sceneRect.height());
    return qMin(basewidth, sceneSize * 0.001);
}


QRectF CDxfGraphicsScene::CalculateSceneBounds(const map<string, stuLayer>& mapdxf)
{
    QRectF totalBounds;
    for (const auto& pairLayer : mapdxf)
    {
        const stuLayer layer = pairLayer.second;
        for (const auto& point : layer.vecPoints)
        {
            totalBounds = totalBounds.united(QRectF(point.x, point.y, 0, 0));
        }
        for (const auto& line : layer.vecLines)
        {
            totalBounds = totalBounds.united(QRectF(QPointF(line.pointStart.x, line.pointStart.y),
                QPointF(line.pointEnd.x, line.pointEnd.y)));
        }
        for (const auto& circle : layer.vecCircles)
        {
            totalBounds = totalBounds.united(QRectF(circle.pointCenter.x - circle.radius,
                circle.pointCenter.y - circle.radius,
                circle.radius * 2, circle.radius * 2));
        }
        for (const auto& arc : layer.vecArcs)
        {
            totalBounds = totalBounds.united(QRectF(arc.pointCenter.x - arc.radius,
                arc.pointCenter.y - arc.radius,
                arc.radius * 2, arc.radius * 2));
        }
        for (const auto& polyline : layer.vecPolylines)
        {
            for (const auto& vertex : polyline.vecVertices)
            {
                totalBounds = totalBounds.united(QRectF(vertex.x, vertex.y, 0, 0));
            }
        }    
    }
    return totalBounds;
}
