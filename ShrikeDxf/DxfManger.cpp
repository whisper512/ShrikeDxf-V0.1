#include "DxfManger.h"
#include "CommonDataManger.h"
#include <QMessageBox>



CDxfManger::CDxfManger(QWidget* pMainWnd) :
    m_pMainWnd(pMainWnd),
    m_strCurrentLayer("")
{
}

CDxfManger::~CDxfManger()
{
    delete m_pMainWnd;
}

bool CDxfManger::LoadDxfFile(const QString& strPath)
{
    DL_Dxf dxf;
    ClearDxfMappingData();
    if (!dxf.in(CCommonDataManager::QStringToStdString(strPath), &m_DxfMapping)) 
    {
        //打开文件失败
        QMessageBox::warning(nullptr, "Load File", "Open Dxf file failed");
        return false;
    }
    else
    {
        //更新model,通知treeview
        m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfMapping.m_mapDxfEntities);
        emit signalRefreshTreeview(&m_DxfTreeviewModel);
        emit signalRefreshTreeviewAfterRead();

        //更新label
        emit signalShowFileName(strPath);

        //更新通知tableview

        m_DxfLayerTableviewModel.UpdateLayerTableViewModel(m_DxfMapping.m_mapDxfEntities);
        emit signalRefreshLayerTableview(&m_DxfLayerTableviewModel);

        //绘制图形
        m_DxfGraphicsScene.DxfDraw(m_DxfMapping.m_mapDxfEntities);
        emit signalRefreshGraphicsview(&m_DxfGraphicsScene,true);

        //初始化步长
        emit signalSetStepLengthAndAngle(m_DxfMapping.m_dMoveStep, m_DxfMapping.m_dRotateStepRAD);

        return true;
    }

}

bool CDxfManger::SaveDxfFile(const QString& strPath)
{
    return false;
}

void CDxfManger::ClearDxfMappingData()
{
    m_DxfMapping.m_mapDxfEntities.clear();
}


QColor CDxfManger::GetCurrentLayerColor()
{
    if (m_DxfMapping.m_mapDxfEntities.find(m_strCurrentLayer.toStdString()) != m_DxfMapping.m_mapDxfEntities.end())
    {
        return m_DxfMapping.m_mapDxfEntities[m_strCurrentLayer.toStdString()].color;
    }
    return QColor();
}

void CDxfManger::RefreshTreeModelAndGraphicsview()
{
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfMapping.m_mapDxfEntities);
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
    m_DxfGraphicsScene.DxfDraw(m_DxfMapping.m_mapDxfEntities);
    emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
    //刷新stacked的属性
}


QString CDxfManger::handleSaveSelectedEntity(const QString& strLayer, const QString& strEntity)
{
    QString strEntityData;

    QRegularExpression re("(\\w+)(\\d+)");
    QRegularExpressionMatch match = re.match(strEntity);
    if (match.hasMatch())
    {
        //保存选择的图元信息
        m_DxfMapping.SaveSelectedEntity(strLayer, match.captured(1), match.captured(2));
        //通知stack刷新
        emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
        //通知MoveWidget
        emit signalSelectedEntityType(static_cast<int>(m_DxfMapping.m_SelectedEntity.type));
    }
    return strEntityData;
}

int CDxfManger::handleDeleteEntity(const QString& strLayer, const QString& strEntity)
{
    QRegularExpression re("(\\w+)(\\d+)");
    QRegularExpressionMatch match = re.match(strEntity);
    if (match.hasMatch())
    {
        m_DxfMapping.DeleteEntity(strLayer, match.captured(1), match.captured(2));
        RefreshTreeModelAndGraphicsview();
        return 1;
    }
    return -1;
}

int CDxfManger::handleCopyEntity()
{
    //通知graphics正在复制图元
    emit signalCopyintEntity();
    return 1;
}

void CDxfManger::handlePaste(QPointF pos)
{
    m_DxfMapping.PasteEntity(pos);
    RefreshTreeModelAndGraphicsview();
}

void CDxfManger::handleLayerModelChanged()
{
    m_DxfLayerTableviewModel.UpdateLayerData(m_DxfMapping.m_mapDxfEntities);
    RefreshTreeModelAndGraphicsview();
}

void CDxfManger::handleChangeCurrentLayer(QString strLayer)
{
    m_strCurrentLayer = strLayer;
    emit signalCurrentLayer(strLayer);
}

void CDxfManger::handlePointAttributeChanged(Point point)
{
    if (m_DxfMapping.m_SelectedEntity.type == enumEntity_Point)
    {
        m_DxfMapping.ChangePointProperty(point);
        RefreshTreeModelAndGraphicsview();
    }
}

void CDxfManger::handleLineAttributeChanged(Line line)
{
    if (m_DxfMapping.m_SelectedEntity.type == enumEntity_Line)
    {
        m_DxfMapping.ChangeLineProperty(line);
        RefreshTreeModelAndGraphicsview();
    }
}

void CDxfManger::handleCircleAttributeChanged(Circle circle)
{
    if (m_DxfMapping.m_SelectedEntity.type == enumEntity_Circle)
    {
        m_DxfMapping.ChangeCircleProperty(circle);
        RefreshTreeModelAndGraphicsview();
    }
}

void CDxfManger::handleArcAttributeChanged(Arc arc)
{
    if (m_DxfMapping.m_SelectedEntity.type == enumEntity_Arc)
    {
        m_DxfMapping.ChangeArcProperty(arc);
        RefreshTreeModelAndGraphicsview();
    }
}

void CDxfManger::handlePolylineAttributeChanged(Polyline polyline)
{
    if (m_DxfMapping.m_SelectedEntity.type == enumEntity_Polyline)
    {
        m_DxfMapping.ChangePolylineProperty(polyline);
        RefreshTreeModelAndGraphicsview();
    }
}

void CDxfManger::handleOnBtnUpClicked()
{
    m_DxfMapping.MoveUpSelectedEntity();
    RefreshTreeModelAndGraphicsview();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);

}

void CDxfManger::handleOnBtnDownClicked()
{
    m_DxfMapping.MoveDownSelectedEntity();
    RefreshTreeModelAndGraphicsview();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
}

void CDxfManger::handleOnBtnLeftClicked()
{
    m_DxfMapping.MoveLeftSelectedEntity();
    RefreshTreeModelAndGraphicsview();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
}

void CDxfManger::handleOnBtnRightClicked()
{
    m_DxfMapping.MoveRightSelectedEntity();
    RefreshTreeModelAndGraphicsview();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
}

void CDxfManger::handleOnBtnCWClicked()
{
    m_DxfMapping.RotateCWSelectedEntity();
    RefreshTreeModelAndGraphicsview();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
}

void CDxfManger::handleOnBtnCCWClicked()
{
    m_DxfMapping.RotateCCWSelectedEntity();
    RefreshTreeModelAndGraphicsview();
    emit signalRefreshStackedWidget(m_DxfMapping.m_SelectedEntity.entity);
}

void CDxfManger::handleOnStepLengthOrAngleChanged(double dStepLength, double dRotationAngle)
{
    m_DxfMapping.m_dMoveStep = dStepLength;
    //角度转弧度
    m_DxfMapping.m_dRotateStepRAD = dRotationAngle * (M_PI / 180);
}

void CDxfManger::handleMouseStatus(int iIndex)
{
    m_DxfMapping.iGraphicsMouseState = iIndex;

    if (iIndex != 1) 
    {  
        m_pointLineStart = QPointF();
    }
    if (iIndex != 2) 
    {
        m_pointCircleCenter = QPointF();
    }
    if (iIndex != 3)
    {
        m_pointDiameterStart = QPointF();
    }
    if (iIndex != 4)
    {
        m_pointArcCenter = QPointF();
        m_pointArcStart = QPointF();
    }
    if (iIndex != 5)
    {
        m_ArcFirstPoint = QPointF();
        m_ArcSecondPoint = QPointF();
    }

    switch (m_DxfMapping.iGraphicsMouseState)
    {
    case -1:
        m_DxfMapping.m_PreviewEntity.type = enumPreviewEntity_None;
        break;
    case 0:
        m_DxfMapping.m_PreviewEntity.type = enumPreviewEntity_Point;
        break;
    case 1:
        m_DxfMapping.m_PreviewEntity.type = enumPreviewEntity_Line;
        break;
    case 2:
        m_DxfMapping.m_PreviewEntity.type = enumPreviewEntity_Center_Radius_Circle;
        break;
    case 3:
        m_DxfMapping.m_PreviewEntity.type = enumPreviewEntity_Diameter_Circle;
        break;
    case 4:
        m_DxfMapping.m_PreviewEntity.type = enumPreviewEntity_Center_Endpoint_Arc;
        break;
    case 5:
        m_DxfMapping.m_PreviewEntity.type = enumPreviewEntity_ThreePoints_Arc;
        break;
    case 6:
        m_DxfMapping.m_PreviewEntity.type = enumPreviewEntity_Polyline;
        break;
    default:
        break;
    }
}

void CDxfManger::handleGraphicsViewMouseMove(QPointF pos)
{
    switch (m_DxfMapping.m_PreviewEntity.type)
    {
    case enumPreviewEntity_None:
    {
        m_DxfGraphicsScene.ClearPreviewItems();
        emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
        break;
    }
    case enumPreviewEntity_Point:
    {

        // 绘制预览点
        m_DxfGraphicsScene.DrawPreviewPoint(Point(pos.x(), pos.y()), GetCurrentLayerColor());
        emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
        break;
    }
    case enumPreviewEntity_Line:
    {
        if (m_pointLineStart.isNull()) 
        {
            //绘制预览起始点
            Point previewPoint(pos.x(), pos.y());
            m_DxfGraphicsScene.DrawPreviewPoint(previewPoint,GetCurrentLayerColor());
        }
        else 
        {
            //绘制预览直线
            Line previewLine(Point(m_pointLineStart.x(), m_pointLineStart.y()), Point(pos.x(), pos.y()));
            m_DxfGraphicsScene.DrawPreviewLine(previewLine,GetCurrentLayerColor());
        }
        emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
        break;
    }
    case enumPreviewEntity_Center_Radius_Circle:
    {
        if (m_pointCircleCenter.isNull()) 
        {
            //绘制预览圆心
            Point previewPoint(pos.x(), pos.y());
            m_DxfGraphicsScene.DrawPreviewPoint(previewPoint, GetCurrentLayerColor());
        }
        else 
        {
            //绘制预览圆
            double radius = sqrt(pow(pos.x() - m_pointCircleCenter.x(), 2) +
                pow(pos.y() - m_pointCircleCenter.y(), 2));
            Circle previewCircle(Point(m_pointCircleCenter.x(), m_pointCircleCenter.y()), radius);
            m_DxfGraphicsScene.DrawPreviewCircleWithCenterAndRadius(previewCircle, pos, GetCurrentLayerColor());
        }
        emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
        break;
    }
    case enumPreviewEntity_Diameter_Circle:
    {
        if (m_pointDiameterStart.isNull())
        {
            //绘制预览直径起点
            Point previewPoint(pos.x(), pos.y());
            m_DxfGraphicsScene.DrawPreviewPoint(previewPoint, GetCurrentLayerColor());
        }
        else
        {
            //绘制预览圆
            Line diameter(Point(m_pointDiameterStart.x(), m_pointDiameterStart.y()),
                Point(pos.x(), pos.y()));
            m_DxfGraphicsScene.DrawPreviewCircleWithDiameter(diameter, GetCurrentLayerColor());
        }
        emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
        break;
    }
    case enumPreviewEntity_Center_Endpoint_Arc:
    {
        if (m_pointArcCenter.isNull() && m_pointArcStart.isNull())
        {
            // 绘制预览圆心
            Point previewPoint(pos.x(), pos.y());
            m_DxfGraphicsScene.DrawPreviewPoint(previewPoint, GetCurrentLayerColor());
        }
        else if (m_pointArcStart.isNull() && !m_pointArcCenter.isNull())
        {
            // 绘制预览圆心和起点
            Point centerPoint(m_pointArcCenter.x(), m_pointArcCenter.y());
            Point startPoint(pos.x(), pos.y());
            m_DxfGraphicsScene.DrawPreviewPoint(centerPoint, GetCurrentLayerColor());
            m_DxfGraphicsScene.DrawPreviewPoint(startPoint, GetCurrentLayerColor());

            // 绘制圆心到起点的连线
            Line radiusLine(centerPoint, startPoint);
            m_DxfGraphicsScene.DrawPreviewLine(radiusLine, GetCurrentLayerColor());
        }
        else if(!m_pointArcStart.isNull() && !m_pointArcCenter.isNull())
        {
            // 计算弧线参数
            Point centerPoint(m_pointArcCenter.x(), m_pointArcCenter.y());
            Point startPoint(m_pointArcStart.x(), m_pointArcStart.y());
            Point endPoint(pos.x(), pos.y());

            // 计算半径
            double radius = sqrt(pow(startPoint.x - centerPoint.x, 2) +
                pow(startPoint.y - centerPoint.y, 2));

            // 计算起点和终点的角度
            double startAngle = atan2(startPoint.y - centerPoint.y, startPoint.x - centerPoint.x) * 180 / M_PI;
            double endAngle = atan2(endPoint.y - centerPoint.y, endPoint.x - centerPoint.x) * 180 / M_PI;

            Line radiusLine(centerPoint, startPoint);
            m_DxfGraphicsScene.DrawPreviewLine(radiusLine, Qt::red);

            // 创建预览弧线
            Arc previewArc(centerPoint, radius, startAngle, endAngle);
            m_DxfGraphicsScene.DrawPreviewArc(previewArc, GetCurrentLayerColor());

        }
        emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
        break;
    }
    case enumPreviewEntity_ThreePoints_Arc:
    {
        //if (m_ArcFirstPoint.isNull() && m_ArcSecondPoint.isNull())
        //{
        //    // 绘制预览起点
        //    Point previewPoint(pos.x(), pos.y());
        //    m_DxfGraphicsScene.DrawPreviewPoint(previewPoint, GetCurrentLayerColor());
        //}
        //else if (m_ArcSecondPoint.isNull() && !m_ArcFirstPoint.isNull())
        //{
        //    // 绘制预览起点和中间点
        //    Point firstPoint(m_ArcFirstPoint.x(), m_ArcFirstPoint.y());
        //    Point secondPoint(pos.x(), pos.y());
        //    m_DxfGraphicsScene.DrawPreviewPoint(firstPoint, GetCurrentLayerColor());
        //    m_DxfGraphicsScene.DrawPreviewPoint(secondPoint, GetCurrentLayerColor());

        //    // 绘制起点到中间点的连线
        //    Line line(firstPoint, secondPoint);
        //    m_DxfGraphicsScene.DrawPreviewLine(line, GetCurrentLayerColor());
        //}
        //else if(!m_ArcSecondPoint.isNull() && !m_ArcFirstPoint.isNull())
        //{
        //    // 计算三点确定的圆弧
        //    Point firstPoint(m_ArcFirstPoint.x(), m_ArcFirstPoint.y());
        //    Point secondPoint(m_ArcSecondPoint.x(), m_ArcSecondPoint.y());
        //    Point thirdPoint(pos.x(), pos.y());

        //    // 计算圆心和半径
        //    Point centerPoint;
        //    double radius;
        //    CalculateCircleFromThreePoints(firstPoint, secondPoint, thirdPoint, centerPoint, radius);

        //    // 计算起点和终点的角度
        //    double startAngle = atan2(firstPoint.y - centerPoint.y, firstPoint.x - centerPoint.x) * 180 / M_PI;
        //    double endAngle = atan2(thirdPoint.y - centerPoint.y, thirdPoint.x - centerPoint.x) * 180 / M_PI;

        //    // 创建预览弧线
        //    Arc previewArc(centerPoint, radius, startAngle, endAngle);
        //    m_DxfGraphicsScene.DrawPreviewArc(previewArc, GetCurrentLayerColor());

        //}
        //emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
        break;
    }
    case enumPreviewEntity_Polyline:
    {
        
        break;
    }
    default:
        break;
    }
}

void CDxfManger::handleGraphicsViewLeftClick(QPointF pos)
{
    switch (m_DxfMapping.m_PreviewEntity.type)
    {
    case enumPreviewEntity_Point:
    {
        Point newPoint(pos.x(), pos.y());
        break;
    }
    case enumPreviewEntity_Line:
    {
        if (m_pointLineStart.isNull()) 
        {
            m_pointLineStart = pos;
        }
        else 
        {
            Line newLine(Point(m_pointLineStart.x(), m_pointLineStart.y()), Point(pos.x(), pos.y()));
            m_pointLineStart = QPointF();
            RefreshTreeModelAndGraphicsview();
        }
        break;
    }
    case enumPreviewEntity_Center_Radius_Circle:
    {
        if (m_pointCircleCenter.isNull()) 
        {
            //确认圆心
            m_pointCircleCenter = pos;
        }
        else
        {
            double radius = sqrt(pow(pos.x() - m_pointCircleCenter.x(), 2) +
                pow(pos.y() - m_pointCircleCenter.y(), 2));
            Circle newCircle(Point(m_pointCircleCenter.x(), m_pointCircleCenter.y()), radius);

            m_pointCircleCenter = QPointF();
            RefreshTreeModelAndGraphicsview();
        }
        break;
    }
    case enumPreviewEntity_Diameter_Circle:
    {
        if (m_pointDiameterStart.isNull())
        {
            //确认直径起点
            m_pointDiameterStart = pos;
        }
        else
        {
            double radius = sqrt(pow(pos.x() - m_pointCircleCenter.x(), 2) +
                pow(pos.y() - m_pointCircleCenter.y(), 2));
            Circle newCircle(Point(m_pointCircleCenter.x(), m_pointCircleCenter.y()), radius);
            
            m_pointDiameterStart = QPointF();
            RefreshTreeModelAndGraphicsview();
        }
        break;
    }
    case enumPreviewEntity_Center_Endpoint_Arc:
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
        else if(!m_pointArcStart.isNull() && !m_pointArcCenter.isNull())
        {
            //计算弧线参数
            Point centerPoint(m_pointArcCenter.x(), m_pointArcCenter.y());
            Point startPoint(m_pointArcStart.x(), m_pointArcStart.y());
            Point endPoint(pos.x(), pos.y());

            //计算半径
            double radius = sqrt(pow(startPoint.x - centerPoint.x, 2) +
                pow(startPoint.y - centerPoint.y, 2));

            //计算起点和终点的角度
            double startAngle = atan2(startPoint.y - centerPoint.y, startPoint.x - centerPoint.x) * 180 / M_PI;
            double endAngle = atan2(endPoint.y - centerPoint.y, endPoint.x - centerPoint.x) * 180 / M_PI;

            //重置状态
            m_pointArcCenter = QPointF();
            m_pointArcStart = QPointF();
            RefreshTreeModelAndGraphicsview();
        }
        break;
    }
    case enumPreviewEntity_ThreePoints_Arc:
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
        //    Point firstPoint(m_ArcFirstPoint.x(), m_ArcFirstPoint.y());
        //    Point secondPoint(m_ArcSecondPoint.x(), m_ArcSecondPoint.y());
        //    Point thirdPoint(pos.x(), pos.y());

        //    // 计算圆心和半径
        //    Point centerPoint;
        //    double radius;
        //    CalculateCircleFromThreePoints(firstPoint, secondPoint, thirdPoint, centerPoint, radius);

        //    // 计算起点和终点的角度
        //    double startAngle = atan2(firstPoint.y - centerPoint.y, firstPoint.x - centerPoint.x) * 180 / M_PI;
        //    double endAngle = atan2(thirdPoint.y - centerPoint.y, thirdPoint.x - centerPoint.x) * 180 / M_PI;

        //    // 创建新弧线
        //    Arc newArc(centerPoint, radius, startAngle, endAngle);
        //    // m_DxfMapping.AddArcEntity(newArc); // 如果需要添加到实体集合中，取消注释

        //    // 重置状态
        //    m_ArcFirstPoint = QPointF();
        //    m_ArcSecondPoint = QPointF();
        //    RefreshTreeModelAndGraphicsview();
        //}
        break;
    }
    case enumPreviewEntity_Polyline:
    {
        break;
    }
    default:
    {
        break;
    }
    }
}

void CDxfManger::CalculateCircleFromThreePoints(const Point& p1, const Point& p2, const Point& p3, Point& center, double& radius)
{
    // 计算向量
    double ax = p1.x - p2.x;
    double ay = p1.y - p2.y;
    double bx = p2.x - p3.x;
    double by = p2.y - p3.y;

    // 计算叉积，用于判断三点是否共线
    double crossProduct = ax * by - ay * bx;

    // 如果叉积接近0，说明三点共线，无法确定圆
    if (fabs(crossProduct) < 1e-10)
    {
        // 设置无效的圆心和半径
        center.x = 0;
        center.y = 0;
        radius = -1;
        return;
    }

    // 计算圆心
    double d = 2 * crossProduct;
    double ux = ((p1.x * p1.x + p1.y * p1.y) * (p2.y - p3.y) +
        (p2.x * p2.x + p2.y * p2.y) * (p3.y - p1.y) +
        (p3.x * p3.x + p3.y * p3.y) * (p1.y - p2.y)) / d;
    double uy = ((p1.x * p1.x + p1.y * p1.y) * (p3.x - p2.x) +
        (p2.x * p2.x + p2.y * p2.y) * (p1.x - p3.x) +
        (p3.x * p3.x + p3.y * p3.y) * (p2.x - p1.x)) / d;

    center.x = ux;
    center.y = uy;

    // 计算半径
    radius = sqrt(pow(p1.x - center.x, 2) + pow(p1.y - center.y, 2));
}
