#include <cmath>

#include "DxfSelectionController.h"
#include "DxfData.h"
#include "DxfGraphicsScene.h"
#include "DxfStruct.h"

CSelectionController::CSelectionController(DxfData* pData, DxfGraphicsScene* pScene, QObject* parent)
    : QObject(parent), m_data(pData), m_scene(pScene)
{
}

CSelectionController::~CSelectionController()
{
}

void CSelectionController::HitTest(QPointF scenePos)
{
    clearSelection();
    if (!m_data || !m_scene) return;

    const auto& layers = m_data->getLayers();
    double threshold = 15.0 / m_scene->getScale();
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
        m_selectedLayer = hitLayer;
        m_nSelectedIndex = hitIndex;
        emit signalEntitySelected(hitLayer, hitIndex);
    }
    else
    {
        clearSelection();
    }
}

void CSelectionController::clearSelection()
{
    if (m_bEntitySelected)
    {
        m_bEntitySelected = false;
        m_selectedLayer.clear();
        m_nSelectedIndex = -1;
        emit signalEntityDeselected();
    }
    m_scene->clearPreview();
}
