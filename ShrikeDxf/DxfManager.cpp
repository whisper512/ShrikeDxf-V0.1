#include "DxfManager.h"
#include "CommonDataManger.h"
#include <QMessageBox>



CDxfManger::CDxfManger(QWidget* pMainWnd) 
    : m_pMainWnd(pMainWnd)
    , m_DxfReader(nullptr)
    , m_DxfData(nullptr)
{
    m_DxfData = std::make_unique<CDxfData>();
    m_DxfReader = std::make_unique<CDxfReader>(m_DxfData.get());
}

CDxfManger::~CDxfManger()
{
    
}

bool CDxfManger::LoadDxfFile(const QString& strPath)
{
    // 确保 Reader持有Data指针
    m_DxfReader->SetDataTarget(m_DxfData.get());
    // 执行读取
    bool ok = m_DxfReader->ReadFile(strPath);
    if (!ok) {
        QMessageBox::warning(m_pMainWnd, QStringLiteral("打开失败"),
            QStringLiteral("无法打开或解析文件:\n%1").arg(strPath));
        return false;
    }

    // 更新treeview
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfData->GetLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
    emit signalRefreshTreeviewAfterRead();
    // 更新graphicsview
    m_DxfGraphicsScene.DxfDraw(m_DxfData->GetLayers());
    emit signalRefreshGraphicsview(&m_DxfGraphicsScene,true);
    return true;
}

bool CDxfManger::SaveDxfFile(const QString& strPath)
{


    return true;
}

bool CDxfManger::NewDxfFile()
{

    return true;
}

bool CDxfManger::CloseDxfFile()
{
    m_DxfTreeviewModel.clear();
    m_DxfGraphicsScene.clear();
    m_DxfLayerTableviewModel.clear();
    return false;
}


void CDxfManger::OnTreeViewEntitySelected(const QString& strLayer, int entityIndex)
{
    m_SelectedEntity.strLayer = strLayer;
    m_SelectedEntity.entityIndex = entityIndex;
    m_SelectedEntity.type = EntityType::None;

    if (entityIndex >= 0)
    {
        // 从DxfData中根据图层名+索引找到对应的实体
        const auto& layers = m_DxfData->GetLayers();
        auto it = layers.find(strLayer.toStdString());
        if (it != layers.end() && entityIndex < (int)it->second.entities.size())
        {
            m_SelectedEntity.entity = it->second.entities[entityIndex];
            m_SelectedEntity.type = GetEntityType(m_SelectedEntity.entity);
        }
    }
    // 发出信号通知关注者
    emit signalSelectedEntityChanged(m_SelectedEntity);
}