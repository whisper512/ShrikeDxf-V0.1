#include<QGraphicsItem>
#include<QGraphicsEllipseItem>
#include<QGraphicsLineItem>
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
    m_PreviewItems.clear();
}


void CDxfGraphicsScene::ClearPreviewItems()
{
    if (!this) 
    {
        return;
    }

    if (m_PreviewItems.isEmpty()) 
    {
        return;
    }

    // 创建临时列表，避免在迭代过程中修改原列表
    QList<QGraphicsItem*> itemsToRemove;

    
    for (int i = 0; i < m_PreviewItems.size(); ++i) {
        QGraphicsItem* item = m_PreviewItems.at(i);
        if (item) {
            itemsToRemove.append(item);
        }
    }
    m_PreviewItems.clear();

    for (QGraphicsItem* item : itemsToRemove) {
        if (item) {
            try {
                // 检查项是否有效且还在场景中
                bool isValid = true;
                try {
                    // 尝试访问项的场景，可能会抛出异常
                    QGraphicsScene* itemScene = item->scene();
                    if (itemScene != this) {
                        isValid = false;
                    }
                }
                catch (...) {
                    // 如果访问场景时抛出异常，认为项无效
                    isValid = false;
                }

                if (isValid) {
                    try {
                        removeItem(item);
                    }
                    catch (...) {
                        // 移除项时可能抛出异常，忽略
                    }
                }
                try {
                    delete item;
                }
                catch (...) {
                    // 删除项时可能抛出异常，忽略
                }
            }
            catch (...) {
                // 捕获所有可能的异常，防止程序崩溃
                continue;
            }
        }
    }
}

void CDxfGraphicsScene::DrawPreviewPoint(const Point& point)
{
    // 清除之前的预览图形
    ClearPreviewItems();

    // 获取场景尺寸
    QRectF sceneRect = this->sceneRect();
    qreal sceneSize = qMin(sceneRect.width(), sceneRect.height());

    qreal size = sceneSize * 0.005;
    size = qMax(size, 1.0);

    QGraphicsEllipseItem* centerPoint = new QGraphicsEllipseItem(
        point.x - size / 2, point.y - size / 2,
        size, size);
    centerPoint->setPen(QPen(Qt::red, CalculateDynamicPenWidth()));
    centerPoint->setBrush(QBrush(Qt::red));
    centerPoint->setZValue(1000);
    addItem(centerPoint);
    m_PreviewItems.append(centerPoint);

    // 根据场景大小动态调整十字标记大小
    qreal crossSize = sceneSize * 0.02;
    crossSize = qMax(crossSize, size * 2);

    QGraphicsLineItem* hLine = new QGraphicsLineItem(
        point.x - crossSize / 2, point.y,
        point.x + crossSize / 2, point.y
    );
    hLine->setPen(QPen(Qt::red, CalculateDynamicPenWidth()));
    hLine->setZValue(1000);
    addItem(hLine);
    m_PreviewItems.append(hLine);
    QGraphicsLineItem* vLine = new QGraphicsLineItem(
        point.x, point.y - crossSize / 2,
        point.x, point.y + crossSize / 2
    );
    vLine->setPen(QPen(Qt::red, CalculateDynamicPenWidth()));
    vLine->setZValue(1000);
    addItem(vLine);
    m_PreviewItems.append(vLine);
}

void CDxfGraphicsScene::DrawPreviewLine(const Line& line)
{
    // 清除之前的预览图形
    ClearPreviewItems();

    QGraphicsLineItem* previewLine = new QGraphicsLineItem(
        line.StartX(), line.StartY(),
        line.EndX(), line.EndY()
    );

    // 设置预览直线的样式
    previewLine->setPen(QPen(Qt::red, CalculateDynamicPenWidth()));
    previewLine->setZValue(1000);

    addItem(previewLine);
    m_PreviewItems.append(previewLine);
}

void CDxfGraphicsScene::DrawPoint(const Point& point, const QColor& color)
{
    qreal size = 1;
    addEllipse(point.x - size / 2, point.y - size / 2, size, size,QPen(color, CalculateDynamicPenWidth()), QBrush(color));

    // 获取场景尺寸
    QRectF sceneRect = this->sceneRect();
    qreal sceneSize = qMin(sceneRect.width(), sceneRect.height());

    // 根据场景大小动态调整十字标记大小
    qreal crossSize = sceneSize * 0.02; // 使用场景尺寸的2%作为十字标记的大小

    // 确保十字标记有最小尺寸
    crossSize = qMax(crossSize, size * 2);

    addLine(point.x - crossSize / 2, point.y, point.x + crossSize / 2, point.y, QPen(color, CalculateDynamicPenWidth()));
    addLine(point.x, point.y - crossSize / 2, point.x, point.y + crossSize / 2, QPen(color, CalculateDynamicPenWidth()));
}

void CDxfGraphicsScene::DrawLine(const Line& line, const QColor& color)
{
    addLine(line.StartX(), line.StartY(), line.EndX(), line.EndY(), QPen(color, CalculateDynamicPenWidth()));
}

void CDxfGraphicsScene::DrawCircle(const Circle& circle, const QColor& color)
{
    addEllipse(circle.pointCenter.x - circle.radius, circle.pointCenter.y - circle.radius, circle.radius * 2, circle.radius * 2, QPen(color, CalculateDynamicPenWidth()), Qt::NoBrush);
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
    addPath(path, QPen(color, CalculateDynamicPenWidth()), Qt::NoBrush);
}

void CDxfGraphicsScene::DrawPolyline(const Polyline& polyline, const QColor& color)
{
    if (polyline.vecVertices.size() < 2) 
    {
        return; // 如果点数少于2个，无法绘制多段线
    }

    QPen pen(color,CalculateDynamicPenWidth());
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



qreal CDxfGraphicsScene::CalculateDynamicPenWidth()
{
    QRectF sceneRect = this->sceneRect();
    qreal sceneSize = qMin(sceneRect.width(), sceneRect.height());
    return sceneSize * 0.001;
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
            totalBounds = totalBounds.united(QRectF(QPointF(line.StartX(), line.StartY()),
                QPointF(line.EndX(), line.EndY())));
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
