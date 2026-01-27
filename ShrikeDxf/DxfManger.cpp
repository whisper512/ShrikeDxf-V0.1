#include "DxfManger.h"
#include "CommonDataManger.h"
#include <QMessageBox>



CDxfManger::CDxfManger(QWidget* pMainWnd):
    m_pMainWnd(pMainWnd),
    m_isDrawingLine(false)
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

    if (iIndex != 2) {  // 不是直线绘制模式
        m_isDrawingLine = false;
    }

    switch (m_DxfMapping.iGraphicsMouseState)
    {
    case 0:
        m_DxfMapping.m_PreviewEntity.type = enumEntity_None;
        break;
    case 1:
        m_DxfMapping.m_PreviewEntity.type = enumEntity_Point;
        break;
    case 2:
        m_DxfMapping.m_PreviewEntity.type = enumEntity_Line;
        break;
    case 3:
        m_DxfMapping.m_PreviewEntity.type = enumEntity_Circle;
        break;
    case 4:
        m_DxfMapping.m_PreviewEntity.type = enumEntity_Arc;
        break;
    case 5:
        m_DxfMapping.m_PreviewEntity.type = enumEntity_Polyline;
        break;
    default:
        break;
    }
}

void CDxfManger::handleGraphicsViewMouseMove(QPointF pos)
{
    switch (m_DxfMapping.m_PreviewEntity.type)
    {
    case enumEntity_Point:
    {
        Point previewPoint(pos.x(), pos.y());
        m_DxfMapping.m_PreviewEntity.strLayer = "0";
        // 绘制预览点
        m_DxfGraphicsScene.DrawPreviewPoint(previewPoint);
        break;
    }
    case enumEntity_Line:
    {
        if (!m_isDrawingLine) 
        {
            //绘制起始点
            Point previewPoint(pos.x(), pos.y());
            m_DxfGraphicsScene.DrawPreviewPoint(previewPoint);
        }
        else {
            //绘制预览直线
            Line previewLine(Point(m_lineStartPoint.x(), m_lineStartPoint.y()), Point(pos.x(), pos.y()));
            m_DxfGraphicsScene.DrawPreviewLine(previewLine);
        }
        break;
    }
    case enumEntity_Circle:
    {
        // 实现圆预览逻辑
        // 需要记录圆心，当前鼠标位置计算半径
        break;
    }
    case enumEntity_Arc:
    {
        // 实现圆弧预览逻辑
        // 需要记录起点、终点和半径
        break;
    }
    case enumEntity_Polyline:
    {
        // 实现多段线预览逻辑
        // 需要记录所有已确定的点，当前鼠标位置作为新的点
        break;
    }
    default:
        break;
    }
    emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
}

void CDxfManger::handleGraphicsViewLeftClick(QPointF pos)
{
    switch (m_DxfMapping.m_PreviewEntity.type)
    {
    case enumEntity_Point:
    {
        Point newPoint(pos.x(), pos.y());
        //m_DxfMapping.AddPointEntity(newPoint);
        break;
    }
    case enumEntity_Line:
    {
        if (!m_isDrawingLine) {
            // 第一次点击，记录起点
            m_lineStartPoint = pos;
            m_isDrawingLine = true;
        }
        else {
            // 第二次点击，创建直线并添加到场景
            Line newLine(Point(m_lineStartPoint.x(), m_lineStartPoint.y()), Point(pos.x(), pos.y()));
            //m_DxfMapping.AddLineEntity(newLine);

            // 重置绘制状态
            m_isDrawingLine = false;

            // 刷新视图
            RefreshTreeModelAndGraphicsview();
        }
        break;
        break;
    }
    case enumEntity_Circle:
    {
        // 第一次点击记录圆心
        // 第二次点击计算半径并添加圆
        break;
    }
    case enumEntity_Arc:
    {
        // 实现圆弧的三点或两点加半径逻辑
        break;
    }
    case enumEntity_Polyline:
    {
        // 每次点击添加一个顶点
        // 右键或双击结束绘制
        break;
    }
    default:
        break;
    }
    //RefreshTreeModelAndGraphicsview();
}
