#include "DxfManager.h"
#include "CommonDataManager.h"
#include <QMessageBox>



CDxfManager::CDxfManager(QWidget* pMainWnd) 
    : m_pMainWnd(pMainWnd)
    , m_DxfReader(nullptr)
    , m_DxfData(nullptr)
{
    m_DxfData = std::make_unique<CDxfData>();
    m_DxfReader = std::make_unique<CDxfReader>(m_DxfData.get());

    m_DxfEditor.m_DxfData = GetDxfData();
}

CDxfManager::~CDxfManager()
{
    
}

bool CDxfManager::LoadDxfFile(const QString& strPath)
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
    // 更新图层tableview
    m_DxfLayerTableviewModel.UpdateLayerTableViewModel(m_DxfData->GetLayers());
    emit signalRefreshLayerTable(&m_DxfLayerTableviewModel);
    // 更新文件路径
    emit signalFileName(strPath);

    
    return true;
}

bool CDxfManager::SaveDxfFile(const QString& strPath)
{


    return true;
}

bool CDxfManager::NewDxfFile()
{

    return true;
}

bool CDxfManager::CloseDxfFile()
{
    m_DxfTreeviewModel.clear();
    m_DxfGraphicsScene.clear();
    m_DxfLayerTableviewModel.clear();
    return false;
}


void CDxfManager::OnTreeViewEntitySelected(const QString& strLayer, int entityIndex)
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


void CDxfManager::handlePointAttributeChanged(EntityPoint point)
{
    m_DxfEditor.editPoint(&m_SelectedEntity, point);
    RefreshSceneCalRect();
}

void CDxfManager::handleLineAttributeChanged(EntityLine line)
{
    m_DxfEditor.editLine(&m_SelectedEntity, line);
    RefreshSceneCalRect();
}

void CDxfManager::handleCircleAttributeChanged(EntityCircle circle)
{
    m_DxfEditor.editCircle(&m_SelectedEntity, circle);
    RefreshSceneCalRect();
}

void CDxfManager::handleArcAttributeChanged(EntityArc arc)
{
    m_DxfEditor.editArc(&m_SelectedEntity, arc);
    RefreshSceneCalRect();
}

void CDxfManager::handleEllipseAttributeChanged(EntityEllipse ellipse)
{
    m_DxfEditor.editEllipse(&m_SelectedEntity, ellipse);
    RefreshSceneCalRect();
}

void CDxfManager::handleSplineAttributeChanged(EntitySpline spline)
{

}

void CDxfManager::handlePolylineAttributeChanged(EntityPolyline polyline)
{

}

void CDxfManager::handleLwpolylineAttributeChanged(EntityLWPolyline lwpolyline)
{
    m_DxfEditor.editLwpolyline(&m_SelectedEntity, lwpolyline);
    RefreshSceneCalRect();
}
void CDxfManager::handleTextAttributeChanged(EntityText text)
{

}

void CDxfManager::handleMTextAttributeChanged(EntityMText mtext)
{

}

void CDxfManager::handleHatchAttributeChanged(EntityHatch hatch)
{

}