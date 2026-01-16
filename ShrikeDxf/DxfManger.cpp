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

        //更新通知tableview

        m_DxfLayerTableviewModel.UpdateLayerTableViewModel(m_DxfMapping.m_mapDxfEntities);
        emit signalRefreshLayerTableview(&m_DxfLayerTableviewModel);

        //绘制图形
        m_DxfGraphicsScene.DxfDraw(m_DxfMapping.m_mapDxfEntities);
        emit signalRefreshGraphicsview(&m_DxfGraphicsScene,true);
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
}


QString CDxfManger::handleChangeEntityWidget(const QString& strLayer, const QString& strEntity)
{
    QString strEntityData;

    QRegularExpression re("(\\w+)(\\d+)");
    QRegularExpressionMatch match = re.match(strEntity);
    if (match.hasMatch())
    {
        variantDxfEntity Entity = m_DxfMapping.GetEntity(strLayer, match.captured(1), match.captured(2));
        m_DxfMapping.SaveSelectedEntity(strLayer, match.captured(1), match.captured(2));
        emit signalRefreshStackedWidget(Entity);
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

int CDxfManger::handleCopyEntity(const QString& strLayer, const QString& strEntity)
{
    //获取选择的entity,临时保存
    QRegularExpression re("(\\w+)(\\d+)");
    QRegularExpressionMatch match = re.match(strEntity);
    if (match.hasMatch())
    {
        m_DxfMapping.SaveCopyingEntity(strLayer, match.captured(1), match.captured(2));
        emit signalCopyintEntity();
        return 1;
    }
    return -1;
}

void CDxfManger::handlePaste(QPointF pos)
{
    m_DxfMapping.PasteEntity(pos);
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
