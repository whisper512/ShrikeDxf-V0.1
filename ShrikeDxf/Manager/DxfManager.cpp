#include "DxfManager.h"
#include "CommonDataManager.h"
#include "DxfInteractionDispatcher.h"
#include "DxfSelectionController.h"
#include <QMessageBox>
#include <QTimer>

CDxfManager::CDxfManager(QWidget* pMainWnd) 
    : m_pMainWnd(pMainWnd)
    , m_DxfReader(nullptr)
    , m_DxfData(nullptr)
{
    // 数据
    m_DxfData = std::make_unique<CDxfData>();
    // 读取器
    m_DxfReader = std::make_unique<CDxfReader>(m_DxfData.get());
    // 选择控制
    m_pSelectionController = std::make_unique<CSelectionController>(m_DxfData.get(), &m_DxfGraphicsScene, this);
    // 编辑控制器
    m_DxfEditController = std::make_unique<CDxfEditController>(m_DxfData.get(), &m_DxfGraphicsScene, this);
    // 新建图元绘制控制器
    m_DxfDrawController = std::make_unique<CDxfDrawController>(m_DxfData.get(), &m_DxfGraphicsScene, this);
    // 交互控制
    m_pInteractionDispatcher = std::make_unique<CDxfInteractionDispatcher>(this);
    // 设置交互控制持有的控制器
    m_pInteractionDispatcher->SetControllers(m_DxfDrawController.get(), m_DxfEditController.get(), m_pSelectionController.get());
    // 编辑类
    m_DxfEditor = std::make_unique<CDxfEditor>(m_DxfData.get());


    ConnectSignals();
}

CDxfManager::~CDxfManager()
{
    
}

const enumMouseStateInView CDxfManager::GetCurrentInteractionState()
{
    return m_pInteractionDispatcher->GetCurrentState();
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
    // 更新当前图层
    m_strCurrentLayer = m_DxfData->GetFirstLayerName();
    emit signalCurrentLayerChanged(m_strCurrentLayer);
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

void CDxfManager::DeleteSelectedEntity()
{
    if (m_SelectedEntity.entityIndex < 0) return;
    if (m_SelectedEntity.strLayer.isEmpty()) return;

    auto* pLayer = m_DxfData->GetLayer(m_SelectedEntity.strLayer.toStdString());
    if (!pLayer) return;
    if (m_SelectedEntity.entityIndex >= static_cast<int>(pLayer->entities.size())) return;

    // 删除图元
    pLayer->entities.erase(pLayer->entities.begin() + m_SelectedEntity.entityIndex);

    // 取消选中
    DeselectEntity();

    // 刷新所有视图（场景、树等）
    RefreshScene();
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfData->GetLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
}

void CDxfManager::CopySelectedEntity()
{
    if (m_SelectedEntity.entityIndex < 0 || m_SelectedEntity.strLayer.isEmpty())
        return;

    const auto& layers = m_DxfData->GetLayers();
    auto it = layers.find(m_SelectedEntity.strLayer.toStdString());
    if (it == layers.end() ||
        m_SelectedEntity.entityIndex >= static_cast<int>(it->second.entities.size()))
        return;

    const auto& srcEntity = it->second.entities[m_SelectedEntity.entityIndex];

    // 拷贝到剪贴板
    m_clipboard.clear();
    m_clipboard.push_back(srcEntity);      // variant 直接复制，深拷贝

    // 计算该实体的包围盒中心，作为粘贴偏移基准
    QRectF bbox = std::visit([](const auto& e) { return e.boundingBox(1.0); }, srcEntity);
    if (bbox.isValid())
        m_clipboardOrigin = bbox.center();
    else
        m_clipboardOrigin = QPointF(0, 0);
}

void CDxfManager::CutSelectedEntity()
{
    CopySelectedEntity();      // 先复制到剪贴板
    DeleteSelectedEntity();    // 再删除
}

void CDxfManager::PasteEntity(QPointF position)
{
    if (m_clipboard.empty())
        return;

    // 计算平移量
    QPointF offset = position - m_clipboardOrigin;

    // 获取当前工作图层，并确保存在
    std::string layerName = m_strCurrentLayer.toStdString();
    stuLayer& layer = m_DxfData->EnsureLayer(layerName);

    // 平移并添加实体
    for (const auto& entity : m_clipboard)
    {
        variantDxfEntity newEntity = entity;   // 拷贝
        // 对每种类型执行平移
        std::visit([offset](auto& e) {
            e.translate(offset.x(), offset.y());
            }, newEntity);
        layer.entities.push_back(std::move(newEntity));
    }

    // 刷新视图和树
    RefreshScene();
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfData->GetLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
}


void CDxfManager::ConnectSignals()
{
    QTimer::singleShot(0, this, [this]() {
        connect(m_pSelectionController.get(), &CSelectionController::signalEntitySelected, this, &CDxfManager::handleEntitySelected);
        connect(m_pSelectionController.get(), &CSelectionController::signalEntityDeselected, this, &CDxfManager::handleEntityDeselected);
        });
}

void CDxfManager::UpdateSelectionDisplay()
{
    m_DxfGraphicsScene.RemoveGrips();
    if (m_SelectedEntity.entityIndex < 0) return;

    const auto& layers = m_DxfData->GetLayers();
    auto it = layers.find(m_SelectedEntity.strLayer.toStdString());
    if (it == layers.end()) return;
    if (m_SelectedEntity.entityIndex < 0 ||
        m_SelectedEntity.entityIndex >= static_cast<int>(it->second.entities.size()))
        return;

    const auto& entity = it->second.entities[m_SelectedEntity.entityIndex];
    QRectF bounds = std::visit([](const auto& e) { return e.boundingBox(1.0); }, entity);
    if (bounds.isValid())
        m_DxfGraphicsScene.ShowGrips(bounds);
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

void CDxfManager::SelectEntity(const QString& strLayer, int entityIndex)
{
    // 更新选中信息
    m_SelectedEntity.strLayer = strLayer;
    m_SelectedEntity.entityIndex = entityIndex;
    m_SelectedEntity.type = EntityType::None;

    const auto& layers = m_DxfData->GetLayers();
    auto it = layers.find(strLayer.toStdString());
    if (it != layers.end() && entityIndex >= 0 && entityIndex < static_cast<int>(it->second.entities.size()))
    {
        m_SelectedEntity.entity = it->second.entities[entityIndex];
        m_SelectedEntity.type = GetEntityType(m_SelectedEntity.entity);
    }

    // 更新选中框
    UpdateSelectionDisplay();

    // 通知其他关注者
    emit signalSelectedEntityChanged(m_SelectedEntity);
}

void CDxfManager::DeselectEntity()
{
    m_SelectedEntity = stuSelectedEntity();
    m_DxfGraphicsScene.RemoveGrips();
    emit signalSelectedEntityChanged(m_SelectedEntity);
}



void CDxfManager::handlePointAttributeChanged(const EntityPoint& point)
{
    m_DxfEditor->editPoint(&m_SelectedEntity, point);
    RefreshScene();
}

void CDxfManager::handleLineAttributeChanged(const EntityLine& line)
{
    m_DxfEditor->editLine(&m_SelectedEntity, line);
    RefreshScene();
}

void CDxfManager::handleCircleAttributeChanged(const EntityCircle& circle)
{
    m_DxfEditor->editCircle(&m_SelectedEntity, circle);
    RefreshScene();
}

void CDxfManager::handleArcAttributeChanged(const EntityArc& arc)
{
    m_DxfEditor->editArc(&m_SelectedEntity, arc);
    RefreshScene();
}

void CDxfManager::handleEllipseAttributeChanged(const EntityEllipse& ellipse)
{
    m_DxfEditor->editEllipse(&m_SelectedEntity, ellipse);
    RefreshScene();
}

void CDxfManager::handleSplineAttributeChanged(const EntitySpline& spline)
{
    m_DxfEditor->editSpline(&m_SelectedEntity, spline);
    RefreshScene();
}

void CDxfManager::handlePolylineAttributeChanged(const EntityPolyline& polyline)
{
    m_DxfEditor->editPolyline(&m_SelectedEntity, polyline);
    RefreshScene();
}

void CDxfManager::handleLwpolylineAttributeChanged(const EntityLWPolyline& lwpolyline)
{
    m_DxfEditor->editLwpolyline(&m_SelectedEntity, lwpolyline);
    RefreshScene();
}
void CDxfManager::handleTextAttributeChanged(const EntityText& text)
{
    m_DxfEditor->editText(&m_SelectedEntity, text);
    RefreshScene();
}

void CDxfManager::handleMTextAttributeChanged(const EntityMText& mtext)
{
    m_DxfEditor->editMText(&m_SelectedEntity, mtext);
    RefreshScene();
}

void CDxfManager::handleHatchAttributeChanged(const EntityHatch& hatch)
{
    m_DxfEditor->editHatch(&m_SelectedEntity, hatch);
    RefreshScene();
}

void CDxfManager::handleLayerAttributeChanged()
{
    SynLayerModelToDxfData();
    RefreshScene();
}

void CDxfManager::handleOnMouseStatusChanged(enumMouseStateInView mouseState)
{
    if (m_pInteractionDispatcher)
    {
        m_pInteractionDispatcher->SetMouseStatus(mouseState);
    }
    // 发送给graphicsview控制右键小菜单
    emit signalMouseStatusChanged(mouseState);
}

void CDxfManager::handleMousePos(QPointF pos)
{
    if (m_pInteractionDispatcher)
    {
        // 交互调度类
        m_pInteractionDispatcher->OnMouseMove(pos);
    }
}

void CDxfManager::handleMouseLeftButtonClicked(QPointF pos)
{
    if (m_pInteractionDispatcher)
    {
        m_pInteractionDispatcher->OnLeftClick(pos);
    }
    
    // 更新tree的model刷新treeview
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfData->GetLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
}

void CDxfManager::handleMouseRightButtonClicked(QPointF pos)
{
    if (m_pInteractionDispatcher)
    {
        m_pInteractionDispatcher->OnRightClick(pos);
    }

    // 更新tree的model刷新treeview
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_DxfData->GetLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
}

void CDxfManager::handleEntitySelected(const QString& strLayer, int entityIndex)
{
    SelectEntity(strLayer, entityIndex);
}

void CDxfManager::handleEntityDeselected()
{
    DeselectEntity();
}

void CDxfManager::handleEndDrawingPreview()
{
    if (m_pInteractionDispatcher)
        m_pInteractionDispatcher->SetMouseStatus(enumMouseStateInView::enumMouseState_None);
}
