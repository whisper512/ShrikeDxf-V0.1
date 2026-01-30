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

QString CDxfManger::GetCurrentLayerName()
{
    return m_strCurrentLayer;
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

void CDxfManger::handleMouseStatus(int index)
{
    m_DxfGraphicsScene.m_MouseState = (enumMouseStateInView)index;
    m_DxfGraphicsScene.clearPreviewEntityData();
    m_DxfGraphicsScene.ChangePreviewEntityByMouseState();
    //通知view预览绘图
    emit signalStartPreviewEntity(index);
}

void CDxfManger::handleGraphicsViewMouseMove(QPointF pos)
{
    m_DxfGraphicsScene.MouseMove(pos,GetCurrentLayerColor());
    //通知view刷新绘图
    emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
}

void CDxfManger::handleGraphicsViewLeftClick(QPointF pos)
{
    m_DxfGraphicsScene.MouseLeftClick(pos, GetCurrentLayerColor());
    switch (m_DxfGraphicsScene.m_PreviewEntity.type)
    {
    case enumEntity_Point:
    {
        //添加点
        Point point(pos.x(), pos.y());
        m_DxfMapping.addPointToLayer(point, GetCurrentLayerName());
        RefreshTreeModelAndGraphicsview();
        break;
    }
    case enumPreviewEntity_Line:
    {
        m_DxfMapping.addLineToLayer(m_DxfGraphicsScene.m_linePreview,GetCurrentLayerName());
        break;
    }
    case enumPreviewEntity_Center_Radius_Circle:
    {
        m_DxfMapping.addCircleToLayer(m_DxfGraphicsScene.m_circlePreview, GetCurrentLayerName());
        break;
    }
    case enumPreviewEntity_Diameter_Circle:
    {
        m_DxfMapping.addCircleToLayer(m_DxfGraphicsScene.m_circlePreview, GetCurrentLayerName());
        break;
    }
    case enumPreviewEntity_Center_Endpoint_Arc:
    {
        if (m_DxfGraphicsScene.m_pointArcCenter.isNull() && m_DxfGraphicsScene.m_pointArcStart.isNull())
        {
            m_DxfMapping.addArcToLayer(m_DxfGraphicsScene.m_arcPreview, GetCurrentLayerName());
        }
        break;
    }
    default:
        break;
    }
    RefreshTreeModelAndGraphicsview();
}

void CDxfManger::handleGraphicsViewRightClick(QPointF pos)
{
    m_DxfGraphicsScene.MouseRightClick(pos);
}

void CDxfManger::handleEndDrawPreview()
{
    //处理多段线
    if (m_DxfGraphicsScene.m_MouseState == enumMouseStateInView_PreviewPolyline && !m_DxfGraphicsScene.m_vecPolylinePoints.isEmpty())
    {
        m_DxfMapping.addPolylineToLayer(m_DxfGraphicsScene.m_vecPolylinePoints, GetCurrentLayerName());
        m_DxfGraphicsScene.m_vecPolylinePoints.clear();
    }

    m_DxfGraphicsScene.m_MouseState = enumMouseStateInView_None;
    //结束预览绘图,通知btn切换状态
    emit signalChangeCreateBtnStatus((int)m_DxfGraphicsScene.m_MouseState);
}