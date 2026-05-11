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
    CDxfWriter writer(m_DxfData.get());
    bool ok = writer.SaveFile(strPath);
    if (!ok)
    {
        QMessageBox::warning(m_pMainWnd, QStringLiteral("保存失败"),
            QStringLiteral("无法保存文件:\n%1").arg(strPath));
        return false;
    }
    emit signalFileName(strPath);
    return true;

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

void CDxfManager::SynLayerModelToDxfData()
{
    int rowCount = m_DxfLayerTableviewModel.rowCount();
    if (rowCount <= 0) return;
    // 获取 DxfData 中可修改的 layers
    auto& layers = m_DxfData->GetDocument().layers;
    for (int row = 0; row < rowCount; ++row)
    {
        QStandardItem* pItemName = m_DxfLayerTableviewModel.item(row, 1);   // NAME 列
        QStandardItem* pItemColor = m_DxfLayerTableviewModel.item(row, 2);   // COLOR 列
        if (!pItemName) continue;
        std::string layerName = pItemName->text().toStdString();
        auto it = layers.find(layerName);
        if (it == layers.end()) continue;
        // 同步颜色
        if (pItemColor)
        {
            it->second.color = pItemColor->background().color();
        }
    }
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


void CDxfManager::handlePointAttributeChanged(const EntityPoint& point)
{
    m_DxfEditor.editPoint(&m_SelectedEntity, point);
    RefreshScene();
}

void CDxfManager::handleLineAttributeChanged(const EntityLine& line)
{
    m_DxfEditor.editLine(&m_SelectedEntity, line);
    RefreshScene();
}

void CDxfManager::handleCircleAttributeChanged(const EntityCircle& circle)
{
    m_DxfEditor.editCircle(&m_SelectedEntity, circle);
    RefreshScene();
}

void CDxfManager::handleArcAttributeChanged(const EntityArc& arc)
{
    m_DxfEditor.editArc(&m_SelectedEntity, arc);
    RefreshScene();
}

void CDxfManager::handleEllipseAttributeChanged(const EntityEllipse& ellipse)
{
    m_DxfEditor.editEllipse(&m_SelectedEntity, ellipse);
    RefreshScene();
}

void CDxfManager::handleSplineAttributeChanged(const EntitySpline& spline)
{
    m_DxfEditor.editSpline(&m_SelectedEntity, spline);
    RefreshScene();
}

void CDxfManager::handlePolylineAttributeChanged(const EntityPolyline& polyline)
{
    m_DxfEditor.editPolyline(&m_SelectedEntity, polyline);
    RefreshScene();
}

void CDxfManager::handleLwpolylineAttributeChanged(const EntityLWPolyline& lwpolyline)
{
    m_DxfEditor.editLwpolyline(&m_SelectedEntity, lwpolyline);
    RefreshScene();
}
void CDxfManager::handleTextAttributeChanged(const EntityText& text)
{
    m_DxfEditor.editText(&m_SelectedEntity, text);
    RefreshScene();
}

void CDxfManager::handleMTextAttributeChanged(const EntityMText& mtext)
{
    m_DxfEditor.editMText(&m_SelectedEntity, mtext);
    RefreshScene();
}

void CDxfManager::handleHatchAttributeChanged(const EntityHatch& hatch)
{
    m_DxfEditor.editHatch(&m_SelectedEntity, hatch);
    RefreshScene();
}

void CDxfManager::handleLayerAttributeChanged()
{
    SynLayerModelToDxfData();
    RefreshScene();
}
