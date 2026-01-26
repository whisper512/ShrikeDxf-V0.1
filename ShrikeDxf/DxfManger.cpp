#include "DxfManger.h"
#include "CommonDataManger.h"
#include <QMessageBox>



CDxfManger::CDxfManger(QWidget* pMainWnd):
    m_pMainWnd(pMainWnd)
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
