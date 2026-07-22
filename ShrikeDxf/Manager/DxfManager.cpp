#include "DxfManager.h"
#include "CommonDataManager.h"
#include "DxfInteractionDispatcher.h"
#include "DxfSelectionController.h"
#include <QMessageBox>
#include <QTimer>

DxfManager::DxfManager(QWidget* mainWnd) 
    : m_mainWnd(mainWnd)
    , m_dxfReader(nullptr)
    , m_dxfData(nullptr)
{
    // 数据
    m_dxfData = std::make_unique<DxfData>();
    // 读取器
    m_dxfReader = std::make_unique<CDxfReader>(m_dxfData.get());
    // 选择控制
    m_selectionController = std::make_unique<CSelectionController>(m_dxfData.get(), &m_DxfGraphicsScene, this);
    // 编辑控制器
    m_dxfEditController = std::make_unique<DxfEditController>(m_dxfData.get(), &m_DxfGraphicsScene, m_selectionController.get(), this, this);
    // 新建图元绘制控制器
    m_dxfDrawController = std::make_unique<dxfDrawController>(m_dxfData.get(), &m_DxfGraphicsScene, this);
    // 交互控制
    m_interactionDispatcher = std::make_unique<DxfInteractionDispatcher>(this);
    // 设置交互控制持有的控制器
    m_interactionDispatcher->setControllers(m_dxfDrawController.get(), m_dxfEditController.get(), m_selectionController.get());
    // 编辑类
    m_dxfEditor = std::make_unique<DxfEditor>(m_dxfData.get());


    connectSignals();
}

DxfManager::~DxfManager()
{
    
}

const MouseStateInView DxfManager::getCurrentInteractionState()
{
    return m_interactionDispatcher->getCurrentState();
}


bool DxfManager::loadDxfFile(const QString& strPath)
{
    // 确保 Reader持有Data指针
    m_dxfReader->SetDataTarget(m_dxfData.get());
    // 执行读取
    bool ok = m_dxfReader->ReadFile(strPath);
    if (!ok) {
        QMessageBox::warning(m_mainWnd, QStringLiteral("打开失败"),
            QStringLiteral("无法打开或解析文件:\n%1").arg(strPath));
        return false;
    }

    // 更新treeview
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_dxfData->getLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
    emit signalRefreshTreeviewAfterRead();
    // 更新graphicsview
    m_DxfGraphicsScene.dxfDraw(m_dxfData->getLayers());
    emit signalRefreshGraphicsview(&m_DxfGraphicsScene,true);
    // 更新图层tableview
    m_DxfLayerTableviewModel.updateLayerTableViewModel(m_dxfData->getLayers());
    emit signalRefreshLayerTable(&m_DxfLayerTableviewModel);
    // 更新文件路径
    emit signalFileName(strPath);
    // 更新当前图层
    m_currentLayer = m_dxfData->getFirstLayerName();
    emit signalCurrentLayerChanged(m_currentLayer);
    return true;
}

bool DxfManager::saveDxfFile(const QString& strPath)
{
    CDxfWriter writer(m_dxfData.get());
    bool ok = writer.SaveFile(strPath);
    if (!ok)
    {
        QMessageBox::warning(m_mainWnd, QStringLiteral("保存失败"),
            QStringLiteral("无法保存文件:\n%1").arg(strPath));
        return false;
    }
    emit signalFileName(strPath);
    return true;

    return true;
}

bool DxfManager::newDxfFile()
{

    return true;
}

bool DxfManager::closeDxfFile()
{
    m_DxfTreeviewModel.clear();
    m_DxfGraphicsScene.clear();
    m_DxfLayerTableviewModel.clear();
    return false;
}

void DxfManager::deleteSelectedEntity()
{
    if (m_selectedEntity.entityIndex < 0) return;
    if (m_selectedEntity.strLayer.isEmpty()) return;

    auto* pLayer = m_dxfData->getLayer(m_selectedEntity.strLayer.toStdString());
    if (!pLayer) return;
    if (m_selectedEntity.entityIndex >= static_cast<int>(pLayer->entities.size())) return;

    // 删除图元
    pLayer->entities.erase(pLayer->entities.begin() + m_selectedEntity.entityIndex);

    // 取消选中
    deselectEntity();

    // 刷新所有视图（场景、树等）
    refreshScene();
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_dxfData->getLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
}

void DxfManager::copySelectedEntity()
{
    if (m_selectedEntity.entityIndex < 0 || m_selectedEntity.strLayer.isEmpty())
        return;

    const auto& layers = m_dxfData->getLayers();
    auto it = layers.find(m_selectedEntity.strLayer.toStdString());
    if (it == layers.end() ||
        m_selectedEntity.entityIndex >= static_cast<int>(it->second.entities.size()))
        return;

    const auto& srcEntity = it->second.entities[m_selectedEntity.entityIndex];

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

void DxfManager::cutSelectedEntity()
{
    copySelectedEntity();      // 先复制到剪贴板
    deleteSelectedEntity();    // 再删除
}

void DxfManager::pasteEntity(QPointF position)
{
    if (m_clipboard.empty())
        return;

    // 计算平移量
    QPointF offset = position - m_clipboardOrigin;

    // 获取当前工作图层，并确保存在
    std::string layerName = m_currentLayer.toStdString();
    stuLayer& layer = m_dxfData->ensureLayer(layerName);

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
    // 剪切板内容移除
    m_clipboard.clear();

    // 刷新视图和树
    refreshScene();
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_dxfData->getLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
}


void DxfManager::connectSignals()
{
    QTimer::singleShot(0, this, [this]() {
        connect(m_selectionController.get(), &CSelectionController::signalEntitySelected, this, &DxfManager::handleEntitySelected);
        connect(m_selectionController.get(), &CSelectionController::signalEntityDeselected, this, &DxfManager::handleEntityDeselected);
        });
}

void DxfManager::updateSelectionDisplay()
{
    m_DxfGraphicsScene.removeGrips();
    if (m_selectedEntity.entityIndex < 0) return;

    const auto& layers = m_dxfData->getLayers();
    auto it = layers.find(m_selectedEntity.strLayer.toStdString());
    if (it == layers.end()) return;
    if (m_selectedEntity.entityIndex < 0 ||
        m_selectedEntity.entityIndex >= static_cast<int>(it->second.entities.size()))
        return;

    const auto& entity = it->second.entities[m_selectedEntity.entityIndex];
    QRectF bounds = std::visit([](const auto& e) { return e.boundingBox(1.0); }, entity);
    if (bounds.isValid())
        m_DxfGraphicsScene.showGrips(bounds);
}


void DxfManager::synLayerModelToDxfData()
{
    int rowCount = m_DxfLayerTableviewModel.rowCount();
    if (rowCount <= 0) return;
    // 获取 DxfData 中可修改的 layers
    auto& layers = m_dxfData->getDocument().layers;
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

void DxfManager::selectEntity(const QString& strLayer, int entityIndex)
{
    // 更新选中信息
    m_selectedEntity.strLayer = strLayer;
    m_selectedEntity.entityIndex = entityIndex;
    m_selectedEntity.type = EntityType::None;

    const auto& layers = m_dxfData->getLayers();
    auto it = layers.find(strLayer.toStdString());
    if (it != layers.end() && entityIndex >= 0 && entityIndex < static_cast<int>(it->second.entities.size()))
    {
        m_selectedEntity.entity = it->second.entities[entityIndex];
        m_selectedEntity.type = GetEntityType(m_selectedEntity.entity);
    }

    if (m_dxfEditController)
        m_dxfEditController->setSelectedEntity(strLayer, entityIndex);
    // 更新选中框
    updateSelectionDisplay();

    // 通知其他关注者
    emit signalSelectedEntityChanged(m_selectedEntity);
}

void DxfManager::deselectEntity()
{
    m_selectedEntity = stuSelectedEntity();
    m_DxfGraphicsScene.removeGrips();

    emit signalSelectedEntityChanged(m_selectedEntity);
}



void DxfManager::handlePointAttributeChanged(const EntityPoint& point)
{
    m_dxfEditor->editPoint(&m_selectedEntity, point);
    refreshScene();
}

void DxfManager::handleLineAttributeChanged(const EntityLine& line)
{
    m_dxfEditor->editLine(&m_selectedEntity, line);
    refreshScene();
}

void DxfManager::handleCircleAttributeChanged(const EntityCircle& circle)
{
    m_dxfEditor->editCircle(&m_selectedEntity, circle);
    refreshScene();
}

void DxfManager::handleArcAttributeChanged(const EntityArc& arc)
{
    m_dxfEditor->editArc(&m_selectedEntity, arc);
    refreshScene();
}

void DxfManager::handleEllipseAttributeChanged(const EntityEllipse& ellipse)
{
    m_dxfEditor->editEllipse(&m_selectedEntity, ellipse);
    refreshScene();
}

void DxfManager::handleSplineAttributeChanged(const EntitySpline& spline)
{
    m_dxfEditor->editSpline(&m_selectedEntity, spline);
    refreshScene();
}

void DxfManager::handlePolylineAttributeChanged(const EntityPolyline& polyline)
{
    m_dxfEditor->editPolyline(&m_selectedEntity, polyline);
    refreshScene();
}

void DxfManager::handleLwpolylineAttributeChanged(const EntityLWPolyline& lwpolyline)
{
    m_dxfEditor->editLwpolyline(&m_selectedEntity, lwpolyline);
    refreshScene();
}
void DxfManager::handleTextAttributeChanged(const EntityText& text)
{
    m_dxfEditor->editText(&m_selectedEntity, text);
    refreshScene();
}

void DxfManager::handleMTextAttributeChanged(const EntityMText& mtext)
{
    m_dxfEditor->editMText(&m_selectedEntity, mtext);
    refreshScene();
}

void DxfManager::handleHatchAttributeChanged(const EntityHatch& hatch)
{
    m_dxfEditor->editHatch(&m_selectedEntity, hatch);
    refreshScene();
}

void DxfManager::handleLayerAttributeChanged()
{
    synLayerModelToDxfData();
    refreshScene();
}

void DxfManager::handleOnMouseStatusChanged(MouseStateInView mouseState)
{
    if (m_interactionDispatcher)
    {
        m_interactionDispatcher->setMouseStatus(mouseState);
    }
    // 发送给graphicsview控制右键小菜单
    emit signalMouseStatusChanged(mouseState);
}

void DxfManager::handleMousePos(QPointF pos)
{
    if (m_interactionDispatcher)
    {
        // 交互调度类
        m_interactionDispatcher->onMouseMove(pos);
    }
}

void DxfManager::handleMouseLeftButtonClicked(QPointF pos)
{
    if (m_interactionDispatcher)
    {
        m_interactionDispatcher->onLeftClick(pos);
    }
    
    // 更新tree的model刷新treeview
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_dxfData->getLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
}

void DxfManager::handleMouseRightButtonClicked(QPointF pos)
{
    if (m_interactionDispatcher)
    {
        m_interactionDispatcher->onRightClick(pos);
    }

    // 更新tree的model刷新treeview
    m_DxfTreeviewModel.UpdateLayoutItemModel(m_dxfData->getLayers());
    emit signalRefreshTreeview(&m_DxfTreeviewModel);
}

void DxfManager::handleMouseLeftButtonPressed(QPointF pos)
{
    if (m_interactionDispatcher)
    {
        m_interactionDispatcher->onLeftPress(pos);
    }
}

void DxfManager::handleMouseLeftButtonReleased(QPointF pos)
{
    if (m_interactionDispatcher)
    {
        m_interactionDispatcher->onLeftRelease(pos);
    }
}


void DxfManager::handleEntitySelected(const QString& strLayer, int entityIndex)
{
    selectEntity(strLayer, entityIndex);
}

void DxfManager::handleEntityDeselected()
{
    deselectEntity();
}

void DxfManager::handleEndDrawingPreview()
{
    if (m_interactionDispatcher)
        m_interactionDispatcher->setMouseStatus(MouseStateInView::enumMouseState_None);
}
