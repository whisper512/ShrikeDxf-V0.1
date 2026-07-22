#include "DxfEditController.h"
#include "DxfSelectionController.h"
#include "DxfData.h"
#include "DxfGraphicsScene.h"
#include "DxfManager.h"

#include <QLineF>
#include <qDebug>
#include <cmath>


DxfEditController::DxfEditController(DxfData* data, DxfGraphicsScene* scene,
    CSelectionController* selection, DxfManager* manager, QObject* parent)
    : QObject(parent)
    , m_data(data)
    , m_scene(scene)
    , m_selection(selection)
    , m_manager(manager)
{
}

DxfEditController::~DxfEditController()
{
}


void DxfEditController::setSelectedEntity(const QString& strLayer, int entityIndex)
{
    m_selectedLayer = strLayer;
    m_selectedIndex = entityIndex;
    emit signalEntitySelected(strLayer, entityIndex);
}

void DxfEditController::clearSelection()
{
    m_selectedLayer.clear();
    m_selectedIndex = -1;
    emit signalEntityDeselected();
}


StretchGripInView DxfEditController::hitTestGrip(QPointF scenePos) const
{
    // 无选中 → 无夹点
    if (m_selectedIndex < 0 || m_selectedLayer.isEmpty() || !m_data)
        return StretchGripInView::None;

    // 查层
    const auto& layers = m_data->getLayers();
    auto itLayer = layers.find(m_selectedLayer.toStdString());
    if (itLayer == layers.end())
        return StretchGripInView::None;

    // 查实体
    if (m_selectedIndex >= static_cast<int>(itLayer->second.entities.size()))
        return StretchGripInView::None;

    // 取 boundingBox
    const auto& entity = itLayer->second.entities[m_selectedIndex];
    QRectF bb = std::visit([](const auto& e) {
        return e.boundingBox(1.0);
        }, entity);

    if (!bb.isValid())
        return StretchGripInView::None;

    // 反查夹点
    return gripFromPoint(scenePos, bb, kGripHitTol);
}


void DxfEditController::startStretch(StretchGripInView grip)
{
    if (grip == StretchGripInView::None)
        return;
    if (m_selectedIndex < 0)
        return;

    m_stretching = true;
    m_currentGrip = grip;
}


void DxfEditController::updateStretch(QPointF newPos)
{
    if (!m_stretching || !m_data)
        return;
    if (m_selectedIndex < 0 || m_selectedLayer.isEmpty())
        return;


    const auto& layers = m_data->getLayers();
    auto itLayer = layers.find(m_selectedLayer.toStdString());
    if (itLayer == layers.end())
        return;

    if (m_selectedIndex >= static_cast<int>(itLayer->second.entities.size()))
        return;

    // std::visit 调实体 stretch
    auto& entity = itLayer->second.entities[m_selectedIndex];
    std::visit([this, newPos](auto& e) {
        using T = std::decay_t<decltype(e)>;
        T& mutableE = const_cast<T&>(e);
        mutableE.stretch(static_cast<StretchGrip>(m_currentGrip), newPos);
        }, entity);

    // 实时刷新
    refreshSceneWithGrips();
}



void DxfEditController::endStretch(QPointF finalPos)
{
    if (!m_stretching)
        return;

    // 执行拉伸：直接访问图层数据修改选中图元
    if (m_data && m_selectedIndex >= 0 && !m_selectedLayer.isEmpty())
    {
        auto& layers = m_data->getLayers();
        auto itLayer = layers.find(m_selectedLayer.toStdString());
        if (itLayer != layers.end() &&
            m_selectedIndex < static_cast<int>(itLayer->second.entities.size()))
        {
            auto& entity = itLayer->second.entities[m_selectedIndex];
            std::visit([this, finalPos](auto& e) {
                e.stretch(static_cast<StretchGrip>(m_currentGrip), finalPos);
                }, entity);
        }
    }

    m_stretching = false;
    m_currentGrip = StretchGripInView::None;

    if (m_manager)
        m_manager->refreshScene();

    refreshSceneWithGrips();
    emit signalStretchFinished();
}



void DxfEditController::refreshSceneWithGrips()
{
    if (!m_scene || !m_data)
        return;

    const auto& layers = m_data->getLayers();

    // 全量重绘
    m_scene->dxfDraw(layers);

    // 叠加夹点
    if (m_selectedIndex < 0 || m_selectedLayer.isEmpty())
        return;

    auto itLayer = layers.find(m_selectedLayer.toStdString());
    if (itLayer == layers.end())
        return;

    if (m_selectedIndex >= static_cast<int>(itLayer->second.entities.size()))
        return;

    const auto& entity = itLayer->second.entities[m_selectedIndex];
    QRectF bb = std::visit([](const auto& e) {
        return e.boundingBox(1.0);
        }, entity);

    if (bb.isValid())
        m_scene->showGrips(bb);
}



StretchGripInView DxfEditController::gripFromPoint(QPointF pt, const QRectF& bb, double tol)
{
    const double l = bb.left();
    const double r = bb.right();
    const double t = bb.top();
    const double b = bb.bottom();
    const double cx = bb.center().x();
    const double cy = bb.center().y();

    // 四角
    if (QLineF(pt, QPointF(l, t)).length() < tol) return StretchGripInView::TopLeft;
    if (QLineF(pt, QPointF(r, t)).length() < tol) return StretchGripInView::TopRight;
    if (QLineF(pt, QPointF(l, b)).length() < tol) return StretchGripInView::BottomLeft;
    if (QLineF(pt, QPointF(r, b)).length() < tol) return StretchGripInView::BottomRight;

    return StretchGripInView::None;
}
