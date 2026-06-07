#include <cmath>

#include "DxfSelectionController.h"
#include "DxfData.h"
#include "DxfGraphicsScene.h"
#include "DxfStruct.h"

CSelectionController::CSelectionController(CDxfData* pData, CDxfGraphicsScene* pScene, QObject* parent)
    : QObject(parent), m_pData(pData), m_pScene(pScene)
{
}

CSelectionController::~CSelectionController()
{
}

void CSelectionController::HitTest(QPointF scenePos)
{
    ClearSelection();
    if (!m_pData || !m_pScene) return;

    const auto& layers = m_pData->GetLayers();
    double threshold = 15.0 / m_pScene->GetScale();
    if (threshold < 0.5) threshold = 0.5;

    QString hitLayer;
    int hitIndex = -1;
    double minDist = threshold;
    double px = scenePos.x(), py = scenePos.y();

    for (auto it = layers.rbegin(); it != layers.rend(); ++it)
    {
        if (!it->second.isVisible) continue;
        const auto& entities = it->second.entities;
        for (int i = static_cast<int>(entities.size()) - 1; i >= 0; --i)
        {
            const auto& entity = entities[i];
            if (GetEntityType(entity) == EntityType::Hatch)
                continue;
            double dist = std::visit([px, py](const auto& e) {
                return e.distanceTo(px, py);
                }, entity);
            if (dist >= 0 && dist < minDist)
            {
                minDist = dist;
                hitLayer = QString::fromStdString(it->first);
                hitIndex = i;
            }
        }
    }

    if (hitIndex >= 0)
    {
        m_bEntitySelected = true;
        m_strSelectedLayer = hitLayer;
        m_nSelectedIndex = hitIndex;
        emit signalEntitySelected(hitLayer, hitIndex);
    }
    else
    {
        ClearSelection();
    }
}

void CSelectionController::ClearSelection()
{
    if (m_bEntitySelected)
    {
        m_bEntitySelected = false;
        m_strSelectedLayer.clear();
        m_nSelectedIndex = -1;
        emit signalEntityDeselected();
    }
    m_pScene->ClearPreview();
}
