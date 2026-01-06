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
        emit RefreshTreeview(&m_DxfTreeviewModel);

        //更新通知tableview
        m_DxfLayerTableviewModel.UpdateLayerTableViewModel(m_DxfMapping.m_mapDxfEntities);
        emit RefreshLayerTableview(&m_DxfLayerTableviewModel);

        //绘制图形
        m_DxfGraphicsScene.DxfDraw(m_DxfMapping.m_mapDxfEntities);
        emit RefreshGraphicsview(&m_DxfGraphicsScene);
        return true;
    }   
    
}

void CDxfManger::ClearDxfMappingData()
{
    m_DxfMapping.m_mapDxfEntities.clear();
}

QString CDxfManger::handleGetEntityData(const QString& strLayer, const QString& strEntity)
{
    QString strEntityData;
    QString strEntityInfo;

    QRegularExpression re("(\\w+)(\\d+)");
    QRegularExpressionMatch match = re.match(strEntity);
    if (match.hasMatch())
    {
        strEntityInfo = m_DxfMapping.GetEntityInfo(strLayer, match.captured(1),match.captured(2));
        emit ReturnEntityInfo(strEntityInfo);
    }
    
    return strEntityData;

}


QString CDxfManger::handleChangeEntityWidget(const QString& strLayer, const QString& strEntity)
{
    QString strEntityData;
    QString strEntityInfo;

    QRegularExpression re("(\\w+)(\\d+)");
    QRegularExpressionMatch match = re.match(strEntity);
    if (match.hasMatch())
    {
        DxfEntity Entity = m_DxfMapping.GetEntity(strLayer, match.captured(1), match.captured(2));
        emit RefreshStackedWidget(Entity);
    }
    return strEntityData;
}
