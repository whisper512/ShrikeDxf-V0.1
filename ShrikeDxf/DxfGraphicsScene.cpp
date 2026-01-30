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
    if (bounds.isNull() || bounds.width() == 0 || bounds.height() == 0)
    {
        bounds = QRectF(0, 0, 500, 500);
    }

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


void CDxfGraphicsScene::MouseMove(QPointF pos,QColor color)
{
    switch (m_PreviewEntity.type)
    {
    case enumPreviewEntity_None:
    {
       ClearPreviewItems();
        break;
    }
    case enumPreviewEntity_Point:
    {
        // 绘制预览点
        DrawPreviewPoint(Point(pos.x(), pos.y()), color);
        break;
    }
    case enumPreviewEntity_Line:
    {
        ProcessPreviewLineWhenMouseMove(pos, color);
        break;
    }
    case enumPreviewEntity_Center_Radius_Circle:
    {
        ProcessPreviewCircleCenterRadiusWhenMouseMove(pos, color);
        break;
    }
    case enumPreviewEntity_Diameter_Circle:
    {
        ProcessPreviewCircleDiameterWhenMouseMove(pos, color);
        break;
    }
    case enumPreviewEntity_Center_Endpoint_Arc:
    {
        ProcessPreviewArcCenterEndpointWhenMouseMove(pos, color);
        break;
    }
    case enumPreviewEntity_ThreePoints_Arc:
    {
        ProcessPreviewArcThreePointsWhenMouseMove(pos, color);
        break;
    }
    case enumPreviewEntity_Polyline:
    {
        ProcessPreviewPolylineWhenMouseMove(pos, color);
        break;
    }
    default:
        break;
    }
}

void CDxfGraphicsScene::MouseLeftClick(QPointF pos, QColor color)
{
    switch (m_PreviewEntity.type)
    {
    case enumPreviewEntity_Point:
    {
        Point newPoint(pos.x(), pos.y());
        break;
    }
    case enumPreviewEntity_Line:
    {
        ProcessPreviewLineWhenMouseLeftClick(pos, color);
        break;
    }
    case enumPreviewEntity_Center_Radius_Circle:
    {
        ProcessPreviewCircleCenterRadiusWhenMouseLeftClick(pos, color);
        break;
    }
    case enumPreviewEntity_Diameter_Circle:
    {
        ProcessPreviewCircleDiameterWhenMouseLeftClick(pos, color);
        break;
    }
    case enumPreviewEntity_Center_Endpoint_Arc:
    {
        ProcessPreviewArcCenterEndpointWhenMouseLeftClick(pos, color);
        break;
    }
    case enumPreviewEntity_ThreePoints_Arc:
    {
        ProcessPreviewArcThreePointsWhenMouseLeftClick(pos, color);
        break;
    }
    case enumPreviewEntity_Polyline:
    {
        ProcessPreviewPolylineWhenMouseLeftClick(pos, color);
        break;
    }
    default:
        break;
    }
}

void CDxfGraphicsScene::MouseRightClick(QPointF pos)
{
}

void CDxfGraphicsScene::ChangePreviewEntityByMouseState()
{
    switch (m_MouseState)
    {
    case enumMouseStateInView_None:
        m_PreviewEntity.type = enumPreviewEntity_None;
        break;
    case enumMouseStateInView_PreviewPoint:
        m_PreviewEntity.type = enumPreviewEntity_Point;
        break;
    case enumMouseStateInView_PreviewLine:
        m_PreviewEntity.type = enumPreviewEntity_Line;
        break;
    case enumMouseStateInView_PreviewCircleCenterRadius:
        m_PreviewEntity.type = enumPreviewEntity_Center_Radius_Circle;
        break;
    case enumMouseStateInView_PreviewCircleDiameter:
        m_PreviewEntity.type = enumPreviewEntity_Diameter_Circle;
        break;
    case enumMouseStateInView_PreviewArcCenterEndpoint:
        m_PreviewEntity.type = enumPreviewEntity_Center_Endpoint_Arc;
        break;
    case enumMouseStateInView_PreviewArcThreePoints:
        m_PreviewEntity.type = enumPreviewEntity_ThreePoints_Arc;
        break;
    case enumMouseStateInView_PreviewPolyline:
        m_PreviewEntity.type = enumPreviewEntity_Polyline;
        break;
    default:
        break;
    }
}

void CDxfGraphicsScene::clearPreviewEntityData()
{
    m_pointLineStart = QPointF();
    m_pointCircleCenter = QPointF();
    m_pointDiameterStart = QPointF();
    m_pointArcCenter = QPointF();
    m_pointArcStart = QPointF();
    m_ArcFirstPoint = QPointF();
    m_ArcSecondPoint = QPointF();
    m_vecPolylinePoints.clear();
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
    
    for (int i = 0; i < m_PreviewItems.size(); ++i) 
    {
        QGraphicsItem* item = m_PreviewItems.at(i);
        if (item) 
        {
            itemsToRemove.append(item);
        }
    }
    m_PreviewItems.clear();

    for (QGraphicsItem* item : itemsToRemove) 
    {
        if (item) 
        {
            try 
            {
                // 检查项是否有效且还在场景中
                bool isValid = true;
                try 
                {
                    // 尝试访问项的场景，可能会抛出异常
                    QGraphicsScene* itemScene = item->scene();
                    if (itemScene != this) 
                    {
                        isValid = false;
                    }
                }
                catch (...) 
                {
                    // 如果访问场景时抛出异常，认为项无效
                    isValid = false;
                }

                if (isValid) 
                {
                    try 
                    {
                        removeItem(item);
                    }
                    catch (...) 
                    {
                        // 移除项时可能抛出异常，忽略
                    }
                }
                try 
                {
                    delete item;
                }
                catch (...) 
                {
                    // 删除项时可能抛出异常，忽略
                }
            }
            catch (...) 
            {
                // 捕获所有可能的异常，防止程序崩溃
                continue;
            }
        }
    }
}

void CDxfGraphicsScene::DrawPreviewPoint(const Point& point,QColor color)
{
    // 清除之前的预览图形
    ClearPreviewItems();

    // 获取场景尺寸
    QRectF sceneRect = this->sceneRect();
    qreal sceneSize = qMin(sceneRect.width(), sceneRect.height());

    qreal size = sceneSize * 0.005;
    size = qMax(size, 1.0);

    QGraphicsEllipseItem* centerPoint = new QGraphicsEllipseItem(point.x() - size / 2, point.y() - size / 2,size, size);
    centerPoint->setPen(QPen(color, CalculateDynamicPenWidth()));
    centerPoint->setBrush(QBrush(color));
    centerPoint->setZValue(1000);
    addItem(centerPoint);
    m_PreviewItems.append(centerPoint);

    // 根据场景大小动态调整十字标记大小
    qreal crossSize = sceneSize * 0.02;
    crossSize = qMax(crossSize, size * 2);

    QGraphicsLineItem* hLine = new QGraphicsLineItem(point.x() - crossSize / 2, point.y(), point.x() + crossSize / 2, point.y());
    hLine->setPen(QPen(color, CalculateDynamicPenWidth()));
    hLine->setZValue(1000);
    addItem(hLine);
    m_PreviewItems.append(hLine);
    QGraphicsLineItem* vLine = new QGraphicsLineItem(point.x(), point.y() - crossSize / 2, point.x(), point.y() + crossSize / 2);
    vLine->setPen(QPen(color, CalculateDynamicPenWidth()));
    vLine->setZValue(1000);
    addItem(vLine);
    m_PreviewItems.append(vLine);
}

void CDxfGraphicsScene::DrawPreviewLine(const Line& line,QColor color)
{
    //清除之前的预览图形
    ClearPreviewItems();

    QGraphicsLineItem* previewLine = new QGraphicsLineItem(line.StartX(), line.StartY(),line.EndX(), line.EndY());
    previewLine->setPen(QPen(color, CalculateDynamicPenWidth(), Qt::DashLine));
    previewLine->setZValue(1000);

    addItem(previewLine);
    m_PreviewItems.append(previewLine);
}

void CDxfGraphicsScene::DrawPreviewCircleWithCenterAndRadius(const Circle& circle,const QPointF& MousePos, QColor color)
{
    ClearPreviewItems();

    //创建圆的图形项
    QGraphicsEllipseItem* pCircleItem = new QGraphicsEllipseItem(circle.pointCenter.x() - circle.radius,circle.pointCenter.y() - circle.radius,circle.radius * 2,circle.radius * 2);

    //设置圆的样式
    QPen pen(color);
    pen.setWidth(CalculateDynamicPenWidth());
    pen.setStyle(Qt::DashLine);
    pCircleItem->setPen(pen);
    pCircleItem->setBrush(Qt::NoBrush);

    addItem(pCircleItem);
    m_PreviewItems.append(pCircleItem);

    //半径线
    QGraphicsLineItem* pRadiusLine = new QGraphicsLineItem(circle.pointCenter.x(), circle.pointCenter.y(), MousePos.x(), MousePos.y());
    pRadiusLine->setPen(pen);
    pRadiusLine->setZValue(1000);
    addItem(pRadiusLine);
    m_PreviewItems.append(pRadiusLine);
}

void CDxfGraphicsScene::DrawPreviewCircleWithDiameter(const Line& diameter, QColor color)
{
    ClearPreviewItems();

    //计算圆心
    double centerX = (diameter.StartX() + diameter.EndX()) / 2;
    double centerY = (diameter.StartY() + diameter.EndY()) / 2;
    double radius = sqrt(pow(diameter.EndX() - diameter.StartX(), 2) + pow(diameter.EndY() - diameter.StartY(), 2)) / 2;

    QGraphicsEllipseItem* pCircleItem = new QGraphicsEllipseItem(centerX - radius,centerY - radius,radius * 2,radius * 2);

    QPen pen(color);
    pen.setWidth(CalculateDynamicPenWidth());
    pen.setStyle(Qt::DashLine);
    pCircleItem->setPen(pen);
    pCircleItem->setBrush(Qt::NoBrush);

    addItem(pCircleItem);
    m_PreviewItems.append(pCircleItem);

    QGraphicsLineItem* pDiameterLine = new QGraphicsLineItem(diameter.StartX(), diameter.StartY(),diameter.EndX(), diameter.EndY());
    pDiameterLine->setPen(pen);
    pDiameterLine->setZValue(1000); 
    addItem(pDiameterLine);
    m_PreviewItems.append(pDiameterLine);
}

void CDxfGraphicsScene::DrawPreviewArc(const Arc& arc, const QColor& color, const Point& pointStart, const Point& pointEnd)
{
    ClearPreviewItems();

    QRectF currentSceneRect = this->sceneRect();
    QPainterPath path;
    QPointF center(arc.pointCenter.x(), arc.pointCenter.y());
    QRectF rect(center.x() - arc.radius, center.y() - arc.radius, arc.radius * 2, arc.radius * 2);

    //计算起点,起始角度是针对圆形中心的角度
    double startAngleRad = qDegreesToRadians(arc.startAngle);
    QPointF startPoint(center.x() + arc.radius * cos(startAngleRad), center.y() + arc.radius * sin(startAngleRad));
    path.moveTo(startPoint);

    //取整角度
    double startAngle = fmod(arc.startAngle, 360.0);
    double endAngle = fmod(arc.endAngle, 360.0);

    // 计算跨度-保持DXF的逆时针方向画弧
    double spanLength = endAngle - startAngle;
    if (spanLength < 0)
    {
        spanLength += 360.0;
    }

    // 由于View做了Y轴镜像，角度需要转换
    // Qt起始角 = (360° - DXF起始角) % 360°
    double qtStartAngle = fmod(360.0 - startAngle, 360.0);
    //转为顺时针绘制圆弧
    path.arcTo(rect, qtStartAngle, -spanLength);

    qreal sceneSize = qMin(currentSceneRect.width(), currentSceneRect.height());
    qreal penWidth = sceneSize * 0.001;
    penWidth = qMin(penWidth, 2.0);
    penWidth = qMax(penWidth, 0.5);

    QPen pen(color);
    pen.setWidth(penWidth);
    pen.setStyle(Qt::DashLine);

    QGraphicsPathItem* arcItem = addPath(path, pen, Qt::NoBrush);
    m_PreviewItems.append(arcItem);
    //计算终止在圆弧上的点
    double endAngleRad = qDegreesToRadians(arc.endAngle);
    Point pointEndinCircle;
    pointEndinCircle.setX(center.x() + arc.radius * cos(endAngleRad));
    pointEndinCircle.setY(center.y() + arc.radius * sin(endAngleRad));

    QGraphicsLineItem* lineCenterToStart = new QGraphicsLineItem(arc.pointCenter.x(), arc.pointCenter.y(), pointStart.x(), pointStart.y());
    QGraphicsLineItem* lineCenterToEnd = new QGraphicsLineItem(arc.pointCenter.x(), arc.pointCenter.y(), pointEndinCircle.x(), pointEndinCircle.y());
    // 设置预览直线的样式
    lineCenterToStart->setPen(QPen(color, CalculateDynamicPenWidth(), Qt::DashLine));
    lineCenterToEnd->setPen(QPen(color, CalculateDynamicPenWidth(), Qt::DashLine));
    addItem(lineCenterToStart);
    addItem(lineCenterToEnd);
    m_PreviewItems.append(lineCenterToStart);
    m_PreviewItems.append(lineCenterToEnd);
}

void CDxfGraphicsScene::ProcessPreviewLineWhenMouseMove(QPointF pos,QColor color)
{
    if (m_pointLineStart.isNull()) 
    {
        //绘制预览起始点
        Point previewPoint(pos.x(), pos.y());
        DrawPreviewPoint(previewPoint,color);
    }
    else 
    {
        //绘制预览直线
        Line previewLine(Point(m_pointLineStart.x(), m_pointLineStart.y()), Point(pos.x(), pos.y()));
        DrawPreviewLine(previewLine,color);
    }
}

void CDxfGraphicsScene::ProcessPreviewLineWhenMouseLeftClick(QPointF pos,QColor color)
{
    if (m_pointLineStart.isNull()) 
    {
           m_pointLineStart = pos;
    }
    else
    {
        Line newLine(Point(m_pointLineStart.x(), m_pointLineStart.y()), Point(pos.x(), pos.y()));
        m_linePreview = newLine;
        m_pointLineStart = QPointF();
    }

}

void CDxfGraphicsScene::ProcessPreviewCircleCenterRadiusWhenMouseMove(QPointF pos, QColor color)
{
    if (m_pointCircleCenter.isNull())
    {
        //绘制预览圆心
        Point previewPoint(pos.x(), pos.y());
        DrawPreviewPoint(previewPoint, color);
    }
    else
    {
        //绘制预览圆
        double radius = sqrt(pow(pos.x() - m_pointCircleCenter.x(), 2) +
            pow(pos.y() - m_pointCircleCenter.y(), 2));
        Circle previewCircle(Point(m_pointCircleCenter.x(), m_pointCircleCenter.y()), radius);
        DrawPreviewCircleWithCenterAndRadius(previewCircle, pos, color);
    }
}

void CDxfGraphicsScene::ProcessPreviewCircleCenterRadiusWhenMouseLeftClick(QPointF pos, QColor color)
{
    if (m_pointCircleCenter.isNull())
    {
        //确认圆心
        m_pointCircleCenter = pos;
    }
    else
    {
        double radius = sqrt(pow(pos.x() - m_pointCircleCenter.x(), 2) + pow(pos.y() - m_pointCircleCenter.y(), 2));
        Circle newCircle(Point(m_pointCircleCenter.x(), m_pointCircleCenter.y()), radius);
        m_circlePreview = newCircle;
        m_pointCircleCenter = QPointF();
    }
}

void CDxfGraphicsScene::ProcessPreviewCircleDiameterWhenMouseMove(QPointF pos, QColor color)
{
    if (m_pointDiameterStart.isNull())
    {
        //绘制预览直径起点
        Point previewPoint(pos.x(), pos.y());
        DrawPreviewPoint(previewPoint, color);
    }
    else
    {
        //绘制预览圆
        Line diameter(Point(m_pointDiameterStart.x(), m_pointDiameterStart.y()), Point(pos.x(), pos.y()));
        DrawPreviewCircleWithDiameter(diameter, color);
    }
}

void CDxfGraphicsScene::ProcessPreviewCircleDiameterWhenMouseLeftClick(QPointF pos, QColor color)
{
    if (m_pointDiameterStart.isNull())
    {
        //确认直径起点
        m_pointDiameterStart = pos;
    }
    else
    {
        Line diameter(Point(m_pointDiameterStart.x(), m_pointDiameterStart.y()), Point(pos.x(), pos.y()));
        double centerX = (diameter.StartX() + diameter.EndX()) / 2;
        double centerY = (diameter.StartY() + diameter.EndY()) / 2;
        double radius = sqrt(pow(diameter.EndX() - diameter.StartX(), 2) + pow(diameter.EndY() - diameter.StartY(), 2)) / 2;
        Circle newCircle(Point(centerX, centerY), radius);
        m_circlePreview = newCircle;
        m_pointDiameterStart = QPointF();
    }
}

void CDxfGraphicsScene::ProcessPreviewArcCenterEndpointWhenMouseMove(QPointF pos, QColor color)
{
    if (m_pointArcCenter.isNull() && m_pointArcStart.isNull())
    {
        // 绘制预览圆心
        Point previewPoint(pos.x(), pos.y());
        DrawPreviewPoint(previewPoint, color);
    }
    else if (m_pointArcStart.isNull() && !m_pointArcCenter.isNull())
    {
        // 绘制预览圆心和起点
        Point centerPoint(m_pointArcCenter.x(), m_pointArcCenter.y());
        Point startPoint(pos.x(), pos.y());
        DrawPreviewPoint(centerPoint, color);
        DrawPreviewPoint(startPoint, color);

        // 绘制圆心到起点的连线
        Line radiusLine(centerPoint, startPoint);
        DrawPreviewLine(radiusLine, color);
    }
    else if (!m_pointArcStart.isNull() && !m_pointArcCenter.isNull())
    {
        Point centerPoint(m_pointArcCenter.x(), m_pointArcCenter.y());
        Point startPoint(m_pointArcStart.x(), m_pointArcStart.y());
        Point endPoint(pos.x(), pos.y());
        // 计算半径
        double radius = sqrt(pow(startPoint.x() - centerPoint.x(), 2) + pow(startPoint.y() - centerPoint.y(), 2));

        // 计算起点和终点的角度
        double startAngle = atan2(startPoint.y() - centerPoint.y(), startPoint.x() - centerPoint.x()) * 180 / M_PI;
        double endAngle = atan2(endPoint.y() - centerPoint.y(), endPoint.x() - centerPoint.x()) * 180 / M_PI;

        Arc previewArc(centerPoint, radius, startAngle, endAngle);
        DrawPreviewArc(previewArc, color, startPoint, endPoint);
    }
}

void CDxfGraphicsScene::ProcessPreviewArcCenterEndpointWhenMouseLeftClick(QPointF pos, QColor color)
{
    if (m_pointArcCenter.isNull() && m_pointDiameterStart.isNull())
    {
        // 确认圆心
        m_pointArcCenter = pos;
        m_arcPreview.pointCenter = Point(pos.x(), pos.y());
    }
    else if (m_pointArcStart.isNull() && !m_pointArcCenter.isNull())
    {
        // 确认起点
        m_pointArcStart = pos;
    }
    else if (!m_pointArcStart.isNull() && !m_pointArcCenter.isNull())
    {
        //计算弧线参数
        Point centerPoint(m_pointArcCenter.x(), m_pointArcCenter.y());
        Point startPoint(m_pointArcStart.x(), m_pointArcStart.y());
        Point endPoint(pos.x(), pos.y());

        //计算半径
        double radius = sqrt(pow(startPoint.x() - centerPoint.x(), 2) +
            pow(startPoint.y() - centerPoint.y(), 2));

        //计算起点和终点的角度
        double startAngle = atan2(startPoint.y() - centerPoint.y(), startPoint.x() - centerPoint.x()) * 180 / M_PI;
        double endAngle = atan2(endPoint.y() - centerPoint.y(), endPoint.x() - centerPoint.x()) * 180 / M_PI;
        Arc newArc(centerPoint, radius, startAngle, endAngle);
        m_arcPreview = newArc;
        //重置状态
        m_pointArcCenter = QPointF();
        m_pointArcStart = QPointF();
    }
}

void CDxfGraphicsScene::ProcessPreviewArcThreePointsWhenMouseMove(QPointF pos, QColor color)
{
    //if (m_ArcFirstPoint.isNull() && m_ArcSecondPoint.isNull())
    //{
    //    // 绘制预览起点
    //    Point previewPoint(pos.x()(), pos.y()()());
    //    DrawPreviewPoint(previewPoint,color);
    //}
    //else if (m_ArcSecondPoint.isNull() && !m_ArcFirstPoint.isNull())
    //{
    //    // 绘制预览起点和中间点
    //    Point firstPoint(m_ArcFirstPoint.x()(), m_ArcFirstPoint.y()()());
    //    Point secondPoint(pos.x()(), pos.y()()());
    //    DrawPreviewPoint(firstPoint, color);
    //    DrawPreviewPoint(secondPoint, color);

    //    // 绘制起点到中间点的连线
    //    Line line(firstPoint, secondPoint);
    //    DrawPreviewLine(line, color);
    //}
    //else if (!m_ArcSecondPoint.isNull() && !m_ArcFirstPoint.isNull())
    //{
    //    // 计算三点确定的圆弧
    //    Point firstPoint(m_ArcFirstPoint.x()(), m_ArcFirstPoint.y()()());
    //    Point secondPoint(m_ArcSecondPoint.x()(), m_ArcSecondPoint.y()()());
    //    Point thirdPoint(pos.x()(), pos.y()()());

    //    // 计算圆心和半径
    //    Point centerPoint;
    //    double radius;
    //    CalculateCircleFromThreePoints(firstPoint, secondPoint, thirdPoint, centerPoint, radius);

    //    // 计算起点和终点的角度
    //    double startAngle = atan2(firstPoint.y()() - centerPoint.y()(), firstPoint.x() - centerPoint.x()) * 180 / M_PI;
    //    double endAngle = atan2(thirdPoint.y()() - centerPoint.y()(), thirdPoint.x() - centerPoint.x()) * 180 / M_PI;

    //    // 创建预览弧线
    //    Arc previewArc(centerPoint, radius, startAngle, endAngle);
    //    DrawPreviewArc(previewArc, color);
    //}
}

void CDxfGraphicsScene::ProcessPreviewArcThreePointsWhenMouseLeftClick(QPointF pos, QColor color)
{
    //if (m_ArcFirstPoint.isNull() && m_ArcSecondPoint.isNull())
    //{
    //    // 确认起点
    //    m_ArcFirstPoint = pos;
    //}
    //else if (m_ArcSecondPoint.isNull() && !m_ArcFirstPoint.isNull())
    //{
    //    // 确认中间点
    //    m_ArcSecondPoint = pos;
    //}
    //else if (!m_ArcSecondPoint.isNull() && !m_ArcFirstPoint.isNull())
    //{
    //    // 计算三点确定的圆弧
    //    Point firstPoint(m_ArcFirstPoint.x()(), m_ArcFirstPoint.y()()());
    //    Point secondPoint(m_ArcSecondPoint.x()(), m_ArcSecondPoint.y()()());
    //    Point thirdPoint(pos.x()(), pos.y()()());

    //    // 计算圆心和半径
    //    Point centerPoint;
    //    double radius;
    //    CalculateCircleFromThreePoints(firstPoint, secondPoint, thirdPoint, centerPoint, radius);

    //    // 计算起点和终点的角度
    //    double startAngle = atan2(firstPoint.y()() - centerPoint.y()(), firstPoint.x() - centerPoint.x()) * 180 / M_PI;
    //    double endAngle = atan2(thirdPoint.y()() - centerPoint.y()(), thirdPoint.x() - centerPoint.x()) * 180 / M_PI;

    //    // 创建新弧线
    //    Arc newArc(centerPoint, radius, startAngle, endAngle);
    //    // m_DxfMapping.AddArcEntity(newArc); // 如果需要添加到实体集合中，取消注释

    //    // 重置状态
    //    m_ArcFirstPoint = QPointF();
    //    m_ArcSecondPoint = QPointF();
    //}
}

void CDxfGraphicsScene::CalculateCircleFromThreePoints(const Point& p1, const Point& p2, const Point& p3, Point& center, double& radius)
{
    // 计算向量
    double ax = p1.x() - p2.x();
    double ay = p1.y() - p2.y();
    double bx = p2.x() - p3.x();
    double by = p2.y() - p3.y();

    // 计算叉积，用于判断三点是否共线
    double crossProduct = ax * by - ay * bx;

    // 如果叉积接近0，说明三点共线，无法确定圆
    if (fabs(crossProduct) < 1e-10)
    {
        // 设置无效的圆心和半径
        center.setX(0);
        center.setY(0);
        radius = -1;
        return;
    }

    // 计算圆心
    double d = 2 * crossProduct;
    double ux = ((p1.x() * p1.x() + p1.y() * p1.y()) * (p2.y() - p3.y()) +
        (p2.x() * p2.x() + p2.y() * p2.y()) * (p3.y() - p1.y()) +
        (p3.x() * p3.x() + p3.y() * p3.y()) * (p1.y() - p2.y())) / d;
    double uy = ((p1.x() * p1.x() + p1.y() * p1.y()) * (p3.x() - p2.x()) +
        (p2.x() * p2.x() + p2.y() * p2.y()) * (p1.x() - p3.x()) +
        (p3.x() * p3.x() + p3.y() * p3.y()) * (p2.x() - p1.x())) / d;

    center.setX(ux);
    center.setY(uy);

    // 计算半径
    radius = sqrt(pow(p1.x() - center.x(), 2) + pow(p1.y() - center.y(), 2));
}

void CDxfGraphicsScene::ProcessPreviewPolylineWhenMouseMove(QPointF pos, QColor color)
{
    // 如果没有顶点，显示预览点
    if (m_vecPolylinePoints.isEmpty())
    {
        Point previewPoint(pos.x(), pos.y());
        DrawPreviewPoint(previewPoint, color);
    }
    else
    {
        // 清除之前的预览
        ClearPreviewItems();

        // 绘制已确定的线段
        for (int i = 0; i < m_vecPolylinePoints.size() - 1; ++i)
        {
            Line line(Point(m_vecPolylinePoints[i].x(), m_vecPolylinePoints[i].y()),
                Point(m_vecPolylinePoints[i + 1].x(), m_vecPolylinePoints[i + 1].y()));
            DrawLine(line, color);
        }

        // 绘制从最后一个顶点到当前鼠标位置的预览线
        Line previewLine(Point(m_vecPolylinePoints.last().x(), m_vecPolylinePoints.last().y()),
            Point(pos.x(), pos.y()));
        DrawPreviewLine(previewLine, color);
    }
}

void CDxfGraphicsScene::ProcessPreviewPolylineWhenMouseLeftClick(QPointF pos, QColor color)
{
    // 添加当前点作为多段线的一个顶点
    m_vecPolylinePoints.append(pos);

    // 绘制已确定的线段
    if (m_vecPolylinePoints.size() >= 2)
    {
        int lastIndex = m_vecPolylinePoints.size() - 1;
        Line line(Point(m_vecPolylinePoints[lastIndex - 1].x(), m_vecPolylinePoints[lastIndex - 1].y()),
            Point(m_vecPolylinePoints[lastIndex].x(), m_vecPolylinePoints[lastIndex].y()));
        DrawLine(line, color);
    }
}



void CDxfGraphicsScene::DrawPoint(const Point& point, const QColor& color)
{
    qreal size = 1;
    addEllipse(point.x() - size / 2, point.y() - size / 2, size, size,QPen(color, CalculateDynamicPenWidth()), QBrush(color));

    // 获取场景尺寸
    QRectF sceneRect = this->sceneRect();
    qreal sceneSize = qMin(sceneRect.width(), sceneRect.height());

    // 根据场景大小动态调整十字标记大小
    qreal crossSize = sceneSize * 0.02; // 使用场景尺寸的2%作为十字标记的大小

    // 确保十字标记有最小尺寸
    crossSize = qMax(crossSize, size * 2);

    addLine(point.x() - crossSize / 2, point.y(), point.x() + crossSize / 2, point.y(), QPen(color, CalculateDynamicPenWidth()));
    addLine(point.x(), point.y() - crossSize / 2, point.x(), point.y() + crossSize / 2, QPen(color, CalculateDynamicPenWidth()));
}

void CDxfGraphicsScene::DrawLine(const Line& line, const QColor& color)
{
    addLine(line.StartX(), line.StartY(), line.EndX(), line.EndY(), QPen(color, CalculateDynamicPenWidth()));
}

void CDxfGraphicsScene::DrawCircle(const Circle& circle, const QColor& color)
{
    addEllipse(circle.pointCenter.x() - circle.radius, circle.pointCenter.y() - circle.radius, circle.radius * 2, circle.radius * 2, QPen(color, CalculateDynamicPenWidth()), Qt::NoBrush);
}


void CDxfGraphicsScene::DrawArc(const Arc& arc, const QColor& color)
{

    QPainterPath path;
    QPointF center(arc.pointCenter.x(), arc.pointCenter.y());
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
    path.moveTo(polyline.vecVertices[0].x(), polyline.vecVertices[0].y());

    // 连接后续所有点
    for (size_t i = 1; i < polyline.vecVertices.size(); ++i) 
    {
        path.lineTo(polyline.vecVertices[i].x(), polyline.vecVertices[i].y());
    }

    // 如果多段线是闭合的，连接最后一个点和第一个点
    if (polyline.flag & 1) 
    {
        path.lineTo(polyline.vecVertices[0].x(), polyline.vecVertices[0].y());
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
            totalBounds = totalBounds.united(QRectF(point.x(), point.y(), 0, 0));
        }
        for (const auto& line : layer.vecLines)
        {
            totalBounds = totalBounds.united(QRectF(QPointF(line.StartX(), line.StartY()),
                QPointF(line.EndX(), line.EndY())));
        }
        for (const auto& circle : layer.vecCircles)
        {
            totalBounds = totalBounds.united(QRectF(circle.pointCenter.x() - circle.radius,
                circle.pointCenter.y() - circle.radius,
                circle.radius * 2, circle.radius * 2));
        }
        for (const auto& arc : layer.vecArcs)
        {
            totalBounds = totalBounds.united(QRectF(arc.pointCenter.x() - arc.radius,
                arc.pointCenter.y() - arc.radius,
                arc.radius * 2, arc.radius * 2));
        }
        for (const auto& polyline : layer.vecPolylines)
        {
            for (const auto& vertex : polyline.vecVertices)
            {
                totalBounds = totalBounds.united(QRectF(vertex.x(), vertex.y(), 0, 0));
            }
        }    
    }
    return totalBounds;
}
