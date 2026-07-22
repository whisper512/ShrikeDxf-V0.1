#include "dxfDrawController.h"
#include "DxfManager.h"


dxfDrawController::dxfDrawController(DxfData* data, DxfGraphicsScene* scene, QObject* parent)
    : QObject(parent)
    , m_data(data)
    , m_scene(scene)
{
}

dxfDrawController::~dxfDrawController()
{
}

const QString& dxfDrawController::getCurrentLayer() const
{
    auto* pMgr = qobject_cast<DxfManager*>(parent());
    if (pMgr)
        return pMgr->getCurrentLayer();

    // 保底值
    static const QString s_defaultLayer = QStringLiteral("0");
    return s_defaultLayer;
}

void dxfDrawController::setMouseStatus(MouseStateInView mouseState)
{
    m_polyPoints.clear();
    m_splinePoints.clear();
    m_currentState = mouseState;
}

void dxfDrawController::onMouseMove(QPointF scenePos)
{
    if (!m_scene) return;

    if (m_currentState == MouseStateInView::enumMouseState_Point)                    // 画点预览
    {
        m_scene->clearPreview();
        m_scene->addPreviewPoint(scenePos);
    }  
    else if (m_currentState == MouseStateInView::enumMouseState_Line)                // 画线预览
    {
        if (m_step == 1)
        {
            // 已选起点,画从起点到鼠标位置的预览线
            m_scene->clearPreview();
            m_scene->addPreviewLine(m_startPoint, scenePos);
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_CircleCenterRadius)  // 圆心-半径画圆预览
    {
        if (m_step == 1)
        {
            qreal radius = QLineF(m_startPoint, scenePos).length();
            m_scene->clearPreview();
            m_scene->addPreviewCircle(m_startPoint, radius);
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_CircleDiameter)      // 直径画圆预览
    {
        if (m_step == 1)
        {
            QPointF center((m_startPoint.x() + scenePos.x()) / 2.0,
                (m_startPoint.y() + scenePos.y()) / 2.0);
            qreal radius = QLineF(m_startPoint, scenePos).length() / 2.0;
            m_scene->clearPreview();
            m_scene->addPreviewCircle(center, radius);
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_ArcCenterEndpoint)    // 圆心-端点画弧预览
    {
        if (m_step == 1)
        {
            // 已选圆心,画从圆心到鼠标位置的半径线 + 预览圆
            m_scene->clearPreview();
            qreal radius = QLineF(m_startPoint, scenePos).length();
            m_scene->addPreviewLine(m_startPoint, scenePos);
            m_scene->addPreviewCircle(m_startPoint, radius);
        }
        else if (m_step == 2)
        {
            // 已选起点,画从起点到鼠标位置的弧预览
            m_scene->clearPreview();
            qreal radius = QLineF(m_startPoint, m_midPoint).length();
            qreal startAngle = atan2(m_midPoint.y() - m_startPoint.y(), m_midPoint.x() - m_startPoint.x());
            qreal endAngle = atan2(scenePos.y() - m_startPoint.y(), scenePos.x() - m_startPoint.x());
            m_scene->addPreviewArc(m_startPoint, radius, startAngle, endAngle);
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_ArcThreePoints)       // 三点画弧预览
    {
        if (m_step == 1)
        {
            // 已选第一点,画从第一点到鼠标位置的预览线
            m_scene->clearPreview();
            m_scene->addPreviewLine(m_startPoint, scenePos);
        }
        else if (m_step == 2)
        {
            // 已有两点,用三点计算弧并预览
            m_scene->clearPreview();

            QPointF center;
            qreal radius, startAngle, endAngle;
            if (ThreePointsToArc(m_startPoint, m_midPoint, scenePos, center, radius, startAngle, endAngle))
            {
                m_scene->addPreviewArc(center, radius, startAngle, endAngle);
            }
            else
            {
                // 三点共线,画一条预览线
                m_scene->addPreviewLine(m_startPoint, scenePos);
            }
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_Polyline)              // 多段线预览
    {
        if (m_step >= 1 && !m_polyPoints.isEmpty())
        {
            m_scene->clearPreview();
            m_scene->addPreviewPolyline(m_polyPoints, scenePos);
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_EllipseCenterRadius)   // 中心-半径画椭圆预览
    {
        if (m_step == 1)
        {
            // 已选中心,画中心到鼠标的预览线 + 预览圆(临时)
            m_scene->clearPreview();
            qreal radius = QLineF(m_startPoint, scenePos).length();
            m_scene->addPreviewLine(m_startPoint, scenePos);
            m_scene->addPreviewCircle(m_startPoint, radius);
        }
        else if (m_step == 2)
        {
            // 已选第一轴端点,画完整椭圆预览
            m_scene->clearPreview();
            double majorLen = QLineF(m_startPoint, m_midPoint).length();
            double mouseDist = QLineF(m_startPoint, scenePos).length();
            double ratio = (majorLen > 1e-10) ? (mouseDist / majorLen) : 0.1;
            if (ratio > 1.0) ratio = 1.0;   // 短轴不能超过长轴
            if (ratio < 0.01) ratio = 0.01;
            m_scene->addPreviewEllipse(m_startPoint, m_midPoint, ratio);
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_Rectangle)             // 矩形预览 
    {
        if (m_step == 1)
        {
            m_scene->clearPreview();
            m_scene->addPreviewRectangle(m_startPoint, scenePos);
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_SplineFitPoint)        // 拟合点样条预览
    {
        if (m_step >= 1 && !m_splinePoints.isEmpty())
        {
            m_scene->clearPreview();
            m_scene->addPreviewSplineFit(m_splinePoints, scenePos);
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_SplineControlPoint)    // 控制点样条预览
    {
        if (m_step >= 1 && !m_splinePoints.isEmpty())
        {
            m_scene->clearPreview();
            m_scene->addPreviewSplineControl(m_splinePoints, scenePos);
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_Text)                   // 文本预览
    {
        if (m_step == 1)
        {
            m_scene->clearPreview();
            m_scene->addPreviewTextRect(m_startPoint, scenePos);
        }
    }
    else if (m_currentState == MouseStateInView::enumMouseState_MText)                  // 多行文本预览
    {
        if (m_step == 1)
        {
            m_scene->clearPreview();
            m_scene->addPreviewTextRect(m_startPoint, scenePos);
        }
    }
}

void dxfDrawController::onGraphicsViewLeftClick(QPointF scenePos)
{
    if (!m_data || !m_scene) 
        return;

    switch (m_currentState)
    {
    case MouseStateInView::enumMouseState_None:
        return;
    case MouseStateInView::enumMouseState_Point:                    // 画点
    {
        std::string layerName = getCurrentLayer().toStdString();

        EntityPoint pt;
        pt.prop.layer = layerName;
        pt.prop.color = 256;
        pt.point = scenePos;
        m_data->addEntity(layerName, pt);
        m_scene->clearPreview();
        m_scene->dxfDraw(m_data->getLayers());
        break;
    }
    case MouseStateInView::enumMouseState_Line:                      // 画线
    {
        if (m_step == 0)                                                 // 第一步:记录起点
        {
            m_startPoint = scenePos;
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_startPoint);
        }
        else if (m_step == 1)                                            // 第二步:终点点击,创建实际线段
        {
            std::string layerName = getCurrentLayer().toStdString();
            EntityLine line;
            line.prop.layer = layerName;
            line.prop.color = 256;
            line.prop.visible = true;
            line.startPoint = m_startPoint;
            line.endPoint = scenePos;
            m_data->addEntity(layerName, line);
            // 清理预览,重绘场景
            m_scene->clearPreview();
            m_scene->dxfDraw(m_data->getLayers());
            m_step = 0;
        }
        break;
    }
    case MouseStateInView::enumMouseState_CircleCenterRadius:       // 圆心-半径画圆
    {
        if (m_step == 0)                                                // 第一步:记录圆心
        {
            m_startPoint = scenePos;
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_startPoint);
        }
        else if (m_step == 1)                                           // 第二步:点击确定半径,创建实际圆
        {
            std::string layerName = getCurrentLayer().toStdString();
            qreal radius = QLineF(m_startPoint, scenePos).length();
            EntityCircle circle;
            circle.prop.layer = layerName;
            circle.prop.color = 256;
            circle.prop.visible = true;
            circle.center = m_startPoint;
            circle.radius = radius;
            m_data->addEntity(layerName, circle);
            m_scene->clearPreview();
            m_scene->dxfDraw(m_data->getLayers());
            m_step = 0;
        }
        break;
    }
    case MouseStateInView::enumMouseState_CircleDiameter:            // 直径画圆 
    {
        if (m_step == 0)
        {
            // 第一步:记录直径起点
            m_startPoint = scenePos;
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_startPoint);
        }
        else if (m_step == 1)
        {
            // 第二步:点击确定直径终点
            std::string layerName = getCurrentLayer().toStdString();
            QPointF center((m_startPoint.x() + scenePos.x()) / 2.0,
                (m_startPoint.y() + scenePos.y()) / 2.0);
            qreal radius = QLineF(m_startPoint, scenePos).length() / 2.0;
            EntityCircle circle;
            circle.prop.layer = layerName;
            circle.prop.color = 256;
            circle.prop.visible = true;
            circle.center = center;
            circle.radius = radius;
            m_data->addEntity(layerName, circle);
            m_scene->clearPreview();
            m_scene->dxfDraw(m_data->getLayers());
            m_step = 0;
        }
        break;
    }
    case MouseStateInView::enumMouseState_ArcCenterEndpoint:        // 圆心+起点画弧
    {
        if (m_step == 0)
        {
            m_startPoint = scenePos;                                       // 第一步:点击圆心
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_startPoint);
        }
        else if (m_step == 1)
        {
            m_midPoint = scenePos;                                         // 第二步:点击起点,确定半径和起始角度
            m_step = 2;
            qreal radius = QLineF(m_startPoint, m_midPoint).length();
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_midPoint);
            m_scene->addPreviewCircle(m_startPoint, radius);
        }
        else if (m_step == 2)                                           // 第三步:点击终点,确定结束角度,创建弧
        {   
            std::string layerName = getCurrentLayer().toStdString();
            qreal radius = QLineF(m_startPoint, m_midPoint).length();
            qreal startAngle = atan2(m_midPoint.y() - m_startPoint.y(),
                m_midPoint.x() - m_startPoint.x());
            qreal endAngle = atan2(scenePos.y() - m_startPoint.y(),
                scenePos.x() - m_startPoint.x());
            EntityArc arc;
            arc.prop.layer = layerName;
            arc.prop.color = 256;
            arc.prop.visible = true;
            arc.center = m_startPoint;
            arc.radius = radius;
            arc.startAngle = startAngle;
            arc.endAngle = endAngle;
            arc.isCCW = true; //实体弧中恒定为逆时针
            m_data->addEntity(layerName, arc);
            m_scene->clearPreview();
            m_scene->dxfDraw(m_data->getLayers());
            m_step = 0;
        }
        break;
    }
    case MouseStateInView::enumMouseState_ArcThreePoints:           // 三点画弧
    {
        if (m_step == 0)
        {   
            m_startPoint = scenePos;                                       // 第一步:点击第一点(起点)
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_startPoint);
        }
        else if (m_step == 1)
        {
            m_midPoint = scenePos;                                         // 第二步:点击第二点(弧上一点)
            m_step = 2;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_midPoint);
        }
        else if (m_step == 2)
        {
            std::string layerName = getCurrentLayer().toStdString();    // 第三步:点击第三点终点,计算并创建弧
            QPointF center;
            qreal radius, startAngle, endAngle;
            if (ThreePointsToArc(m_startPoint, m_midPoint, scenePos, center, radius, startAngle, endAngle))
            {
                EntityArc arc;
                arc.prop.layer = layerName;
                arc.prop.color = 256;
                arc.prop.visible = true;
                arc.center = center;
                arc.radius = radius;
                arc.startAngle = startAngle;
                arc.endAngle = endAngle;
                arc.isCCW = 1;
                m_data->addEntity(layerName, arc);
            }
            m_scene->clearPreview();
            m_scene->dxfDraw(m_data->getLayers());
            m_step = 0;
        }
        break;
    }
    case MouseStateInView::enumMouseState_Polyline:                // 多段线
    {
        if (m_step == 0)
        {
            m_polyPoints.clear();                                   // 第一步:点击第一个点
            m_polyPoints.append(scenePos);
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(scenePos);
        }
        else                                                           // 后续步骤:添加新顶点   
        {
            m_polyPoints.append(scenePos);
            m_step++;
            // 更新预览(画所有已确定的线段 + 新顶点十字)
            m_scene->clearPreview();
            m_scene->addPreviewPolyline(m_polyPoints, scenePos);
        }
        break;
    }
    case MouseStateInView::enumMouseState_EllipseCenterRadius:      // 圆心+半径画椭圆
    {
        if (m_step == 0)                                                // 第一步:点击中心
        {
            m_startPoint = scenePos;
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_startPoint);
        }
        else if (m_step == 1)                                            // 第二步:点击长轴端点
        {
            m_midPoint = scenePos;
            m_step = 2;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_midPoint);
        }
        else if (m_step == 2)                                            // 第三步:点击确定短轴比例
        {
            finishEllipse(scenePos);
        }
        break;
    }
    case MouseStateInView::enumMouseState_Rectangle:                  // 矩形(两点对角)
    {
        if (m_step == 0)                                                  // 第一步：点击第一个角点
        {
            m_startPoint = scenePos;
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_startPoint);
        }
        else if (m_step == 1)                                             // 第二步:点击对角点，完成矩形
        {
            finishRectangle(scenePos);
        }
        break;
    }
    case MouseStateInView::enumMouseState_SplineFitPoint:             // 拟合点样条
    {
        if (m_step == 0)
        {
            m_splinePoints.clear();
            m_splinePoints.append(scenePos);
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(scenePos);
        }
        else
        {
            m_splinePoints.append(scenePos);
            m_step++;
            m_scene->clearPreview();
            m_scene->addPreviewSplineFit(m_splinePoints, scenePos);
        }
        break;
    }
    case MouseStateInView::enumMouseState_SplineControlPoint:           // 控制点样条
    {
        if (m_step == 0)
        {
            m_splinePoints.clear();
            m_splinePoints.append(scenePos);
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(scenePos);
        }
        else
        {
            m_splinePoints.append(scenePos);
            m_step++;
            m_scene->clearPreview();
            m_scene->addPreviewSplineControl(m_splinePoints, scenePos);
        }
        break;
    }
    case MouseStateInView::enumMouseState_Text:                         // 单行文本
    {
        if (m_step == 0)                                                    // 第一步:点击插入点
        {
            m_startPoint = scenePos;
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_startPoint);
        }
        else if (m_step == 1)
        {
            finishText(scenePos);                                           // 第二步:点击确定文本区域，创建文本
        }
        break;
    }
    case MouseStateInView::enumMouseState_MText:                        // 多行文本
    {
        if (m_step == 0)                                                    // 第一步:点击插入点
        {
            m_startPoint = scenePos;
            m_step = 1;
            m_scene->clearPreview();
            m_scene->addPreviewPoint(m_startPoint);
        }
        else if (m_step == 1)                                               // 第二步:点击确定文本宽度，创建多行文本
        {
            finishMText(scenePos);
        }
        break;
    }
    default:
        
        break;
    }
}


void dxfDrawController::onGraphicsViewRightClick(QPointF scenePos)
{
    if (!m_data || !m_scene) return;
    switch (m_currentState)
    {
    case MouseStateInView::enumMouseState_Polyline:                   // 右键:完成多段线
    {
        if (m_step >= 1 && m_polyPoints.size() >= 2)
        {
            finishPolyline();
        }
        else
        {
            // 点数不够，直接取消
            cancelPolyline();
        }
        break;
    }
    case MouseStateInView::enumMouseState_SplineFitPoint:
    {
        if (m_step >= 1 && m_splinePoints.size() >= 2)
            finishSplineFit();
        else
            cancelSpline();
        break;
    }
    case MouseStateInView::enumMouseState_SplineControlPoint:
    {
        if (m_step >= 1 && m_splinePoints.size() >= 2)
            finishSplineControl();
        else
            cancelSpline();
        break;
    }
    default:
        
        break;
    }
}

void dxfDrawController::finishPolyline()
{
    if (!m_data || !m_scene) return;
    if (m_currentState != MouseStateInView::enumMouseState_Polyline)
        return;
    if (m_polyPoints.size() < 2)
    {
        // 点数不够,取消
        cancelPolyline();
        return;
    }

    std::string layerName = getCurrentLayer().toStdString();

    // 创建 LWPolyline(轻量多段线)
    EntityLWPolyline poly;
    poly.prop.layer = layerName;
    poly.prop.color = 256;
    poly.prop.visible = true;
    poly.flags = 0;              // 不闭合

    for (const auto& pt : m_polyPoints)
    {
        PolylineVertex2D vertex;
        vertex.point = pt;
        vertex.bulge = 0.0;     // 直线段
        poly.vecVertices.push_back(vertex);
    }

    m_data->addEntity(layerName, poly);

    m_scene->clearPreview();
    m_scene->dxfDraw(m_data->getLayers());

    // 重置
    m_polyPoints.clear();
    m_step = 0;
}

void dxfDrawController::cancelPolyline()
{
    if (m_scene)
        m_scene->clearPreview();
    m_polyPoints.clear();
    m_step = 0;
}

void dxfDrawController::finishEllipse(QPointF scenePos)
{
    if (!m_data || !m_scene) return;
    if (m_currentState != MouseStateInView::enumMouseState_EllipseCenterRadius)
        return;

    std::string layerName = getCurrentLayer().toStdString();

    double majorLen = QLineF(m_startPoint, m_midPoint).length();
    double mouseDist = QLineF(m_startPoint, scenePos).length();
    double ratio = (majorLen > 1e-10) ? (mouseDist / majorLen) : 0.1;
    if (ratio > 1.0) ratio = 1.0;
    if (ratio < 0.01) ratio = 0.01;

    EntityEllipse ellipse;
    ellipse.prop.layer = layerName;
    ellipse.prop.color = 256;
    ellipse.prop.visible = true;
    ellipse.center = m_startPoint;
    ellipse.majorAxisEndpoint.setX(m_midPoint.x() - m_startPoint.x());
    ellipse.majorAxisEndpoint.setY(m_midPoint.y() - m_startPoint.y());
    ellipse.ratio = ratio;
    ellipse.startParam = 0.0;
    ellipse.endParam = 2.0 * M_PI;

    m_data->addEntity(layerName, ellipse);

    m_scene->clearPreview();
    m_scene->dxfDraw(m_data->getLayers());
    m_step = 0;
}

void dxfDrawController::finishRectangle(QPointF scenePos)
{
    if (!m_data || !m_scene) return;
    if (m_currentState != MouseStateInView::enumMouseState_Rectangle)
        return;

    std::string layerName = getCurrentLayer().toStdString();

    qreal x1 = m_startPoint.x(), y1 = m_startPoint.y();
    qreal x2 = scenePos.x(), y2 = scenePos.y();

    EntityLWPolyline poly;
    poly.prop.layer = layerName;
    poly.prop.color = 256;
    poly.prop.visible = true;
    poly.flags = 1;  // 闭合

    // 四个顶点：左下/右下/右上/左上(或任意顺时针/逆时针顺序)
    PolylineVertex2D v1, v2, v3, v4;
    v1.point.setX(x1);  v1.point.setY(y1);  v1.bulge = 0.0;
    v2.point.setX(x2);  v2.point.setY(y1);  v2.bulge = 0.0;
    v3.point.setX(x2);  v3.point.setY(y2);  v3.bulge = 0.0;
    v4.point.setX(x1);  v4.point.setY(y2);  v4.bulge = 0.0;

    poly.vecVertices.push_back(v1);
    poly.vecVertices.push_back(v2);
    poly.vecVertices.push_back(v3);
    poly.vecVertices.push_back(v4);

    m_data->addEntity(layerName, poly);

    m_scene->clearPreview();
    m_scene->dxfDraw(m_data->getLayers());
    m_step = 0;
}


void dxfDrawController::finishSplineFit()
{
    if (!m_data || !m_scene) return;
    if (m_currentState != MouseStateInView::enumMouseState_SplineFitPoint)
        return;
    if (m_splinePoints.size() < 2)
    {
        cancelSpline();
        return;
    }

    std::string layerName = getCurrentLayer().toStdString();

    EntitySpline spline;
    spline.prop.layer = layerName;
    spline.prop.color = 256;
    spline.prop.visible = true;
    spline.degree = 3;

    // 拟合点
    for (const auto& pt : m_splinePoints)
    {
        Vertex3D vertext;
        vertext = pt;
        spline.fitPoints.push_back(vertext);
    }

    
    for (const auto& pt : m_splinePoints)
    {
        Vertex3D vertext;
        vertext = pt;
        spline.controlPoints.push_back(vertext);
    }

    // 生成 clamped 节点向量
    int n = (int)spline.controlPoints.size();
    int k = spline.degree;
    int m = n + k + 1;
    spline.knots.resize(m);
    for (int i = 0; i < m; ++i)
    {
        if (i <= k)
            spline.knots[i] = 0.0;
        else if (i >= n)
            spline.knots[i] = 1.0;
        else
            spline.knots[i] = static_cast<double>(i - k) / (n - k);
    }

    m_data->addEntity(layerName, spline);

    m_scene->clearPreview();
    m_scene->dxfDraw(m_data->getLayers());

    m_splinePoints.clear();
    m_step = 0;
}

void dxfDrawController::finishSplineControl()
{
    if (!m_data || !m_scene) return;
    if (m_currentState != MouseStateInView::enumMouseState_SplineControlPoint)
        return;
    if (m_splinePoints.size() < 2)
    {
        cancelSpline();
        return;
    }

    std::string layerName = getCurrentLayer().toStdString();

    EntitySpline spline;
    spline.prop.layer = layerName;
    spline.prop.color = 256;
    spline.prop.visible = true;
    spline.degree = 3;

    // 控制点
    for (const auto& pt : m_splinePoints)
    {
        Vertex3D vertext;
        vertext = pt;
        spline.controlPoints.push_back(vertext);
    }

    // 生成 clamped 节点向量
    int n =(int)spline.controlPoints.size();
    int k = spline.degree;
    int m = n + k + 1;
    spline.knots.resize(m);
    for (int i = 0; i < m; ++i)
    {
        if (i <= k)
            spline.knots[i] = 0.0;
        else if (i >= n)
            spline.knots[i] = 1.0;
        else
            spline.knots[i] = static_cast<double>(i - k) / (n - k);
    }

    m_data->addEntity(layerName, spline);

    m_scene->clearPreview();
    m_scene->dxfDraw(m_data->getLayers());

    m_splinePoints.clear();
    m_step = 0;
}

void dxfDrawController::cancelSpline()
{
    if (m_scene)
        m_scene->clearPreview();
    m_splinePoints.clear();
    m_step = 0;
}

void dxfDrawController::finishText(QPointF scenePos)
{
    if (!m_data || !m_scene) return;
    if (m_currentState != MouseStateInView::enumMouseState_Text)
        return;

    std::string layerName = getCurrentLayer().toStdString();

    // 计算文本高度 = 矩形高度的一半
    double height = std::abs(scenePos.y() - m_startPoint.y());
    if (height < 1.0) height = 3.0;

    EntityText text;
    text.prop.layer = layerName;
    text.prop.color = 256;
    text.prop.visible = true;
    text.text = "Text";
    text.style = "STANDARD";
    text.insertPoint = m_startPoint;
    text.height = height;
    text.rotation = 0.0;
    text.alignH = 0;  // Left
    text.alignV = 0;  // Middle

    m_data->addEntity(layerName, text);

    m_scene->clearPreview();
    m_scene->dxfDraw(m_data->getLayers());
    m_step = 0;
}

void dxfDrawController::finishMText(QPointF scenePos)
{
    if (!m_data || !m_scene) return;
    if (m_currentState != MouseStateInView::enumMouseState_MText)
        return;

    std::string layerName = getCurrentLayer().toStdString();

    // 计算文本高度和宽度
    double height = std::abs(scenePos.y() - m_startPoint.y());
    if (height < 1.0) height = 3.0;

    double width = std::abs(scenePos.x() - m_startPoint.x());
    if (width < 1.0) width = 20.0;

    EntityMText mtext;
    mtext.prop.layer = layerName;
    mtext.prop.color = 256;
    mtext.prop.visible = true;
    mtext.text = "MText";
    mtext.style = "STANDARD";
    mtext.insertPoint = m_startPoint;
    mtext.height = height;
    mtext.rotation = 0.0;
    mtext.attachPoint = 1;   // TopLeft
    mtext.xAxisDir.setX(width);
    mtext.xAxisDir.setY(0);

    m_data->addEntity(layerName, mtext);

    m_scene->clearPreview();
    m_scene->dxfDraw(m_data->getLayers());
    m_step = 0;
}
