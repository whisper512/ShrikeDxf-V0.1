#include "DxfEditController.h"
#include "DxfSelectionController.h"
#include "DxfData.h"
#include "DxfGraphicsScene.h"
#include "DxfManager.h"

#include <QLineF>
#include <qDebug>
#include <cmath>


CDxfEditController::CDxfEditController(DxfData* pData, CDxfGraphicsScene* pScene,
    CSelectionController* pSelection, DxfManager* pManager, QObject* parent)
    : QObject(parent)
    , m_pData(pData)
    , m_pScene(pScene)
    , m_pSelection(pSelection)
    , m_pManager(pManager)
{
}

CDxfEditController::~CDxfEditController()
{
}


void CDxfEditController::SetSelectedEntity(const QString& strLayer, int entityIndex)
{
    m_strSelectedLayer = strLayer;
    m_iSelectedIndex = entityIndex;
    emit signalEntitySelected(strLayer, entityIndex);
}

void CDxfEditController::ClearSelection()
{
    m_strSelectedLayer.clear();
    m_iSelectedIndex = -1;
    emit signalEntityDeselected();
}


StretchGripInView CDxfEditController::HitTestGrip(QPointF scenePos) const
{
    // 无选中 → 无夹点
    if (m_iSelectedIndex < 0 || m_strSelectedLayer.isEmpty() || !m_pData)
        return StretchGripInView::None;

    // 查层
    const auto& layers = m_pData->getLayers();
    auto itLayer = layers.find(m_strSelectedLayer.toStdString());
    if (itLayer == layers.end())
        return StretchGripInView::None;

    // 查实体
    if (m_iSelectedIndex >= static_cast<int>(itLayer->second.entities.size()))
        return StretchGripInView::None;

    // 取 boundingBox
    const auto& entity = itLayer->second.entities[m_iSelectedIndex];
    QRectF bb = std::visit([](const auto& e) {
        return e.boundingBox(1.0);
        }, entity);

    if (!bb.isValid())
        return StretchGripInView::None;

    // 反查夹点
    return GripFromPoint(scenePos, bb, kGripHitTol);
}


void CDxfEditController::StartStretch(StretchGripInView grip)
{
    if (grip == StretchGripInView::None)
        return;
    if (m_iSelectedIndex < 0)
        return;

    m_bStretching = true;
    m_eCurrentGrip = grip;
}


void CDxfEditController::UpdateStretch(QPointF newPos)
{
    if (!m_bStretching || !m_pData)
        return;
    if (m_iSelectedIndex < 0 || m_strSelectedLayer.isEmpty())
        return;


    const auto& layers = m_pData->getLayers();
    auto itLayer = layers.find(m_strSelectedLayer.toStdString());
    if (itLayer == layers.end())
        return;

    if (m_iSelectedIndex >= static_cast<int>(itLayer->second.entities.size()))
        return;

    // std::visit 调实体 stretch
    auto& entity = itLayer->second.entities[m_iSelectedIndex];
    std::visit([this, newPos](auto& e) {
        using T = std::decay_t<decltype(e)>;
        T& mutableE = const_cast<T&>(e);
        mutableE.stretch(static_cast<StretchGrip>(m_eCurrentGrip), newPos);
        }, entity);

    // 实时刷新
    RefreshSceneWithGrips();
}



void CDxfEditController::EndStretch(QPointF finalPos)
{
    if (!m_bStretching)
        return;

    // 执行拉伸：直接访问图层数据修改选中图元
    if (m_pData && m_iSelectedIndex >= 0 && !m_strSelectedLayer.isEmpty())
    {
        auto& layers = m_pData->getLayers();
        auto itLayer = layers.find(m_strSelectedLayer.toStdString());
        if (itLayer != layers.end() &&
            m_iSelectedIndex < static_cast<int>(itLayer->second.entities.size()))
        {
            auto& entity = itLayer->second.entities[m_iSelectedIndex];
            std::visit([this, finalPos](auto& e) {
                e.stretch(static_cast<StretchGrip>(m_eCurrentGrip), finalPos);
                }, entity);
        }
    }

    m_bStretching = false;
    m_eCurrentGrip = StretchGripInView::None;

    if (m_pManager)
        m_pManager->RefreshScene();

    RefreshSceneWithGrips();
    emit signalStretchFinished();
}



void CDxfEditController::RefreshSceneWithGrips()
{
    if (!m_pScene || !m_pData)
        return;

    const auto& layers = m_pData->getLayers();

    // 全量重绘
    m_pScene->DxfDraw(layers);

    // 叠加夹点
    if (m_iSelectedIndex < 0 || m_strSelectedLayer.isEmpty())
        return;

    auto itLayer = layers.find(m_strSelectedLayer.toStdString());
    if (itLayer == layers.end())
        return;

    if (m_iSelectedIndex >= static_cast<int>(itLayer->second.entities.size()))
        return;

    const auto& entity = itLayer->second.entities[m_iSelectedIndex];
    QRectF bb = std::visit([](const auto& e) {
        return e.boundingBox(1.0);
        }, entity);

    if (bb.isValid())
        m_pScene->ShowGrips(bb);
}



StretchGripInView CDxfEditController::GripFromPoint(QPointF pt, const QRectF& bb, double tol)
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
