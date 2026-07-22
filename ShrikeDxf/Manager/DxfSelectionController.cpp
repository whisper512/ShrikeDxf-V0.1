#include <cmath>

#include "DxfSelectionController.h"
#include "DxfData.h"
#include "DxfGraphicsScene.h"
#include "DxfStruct.h"

SelectionController::SelectionController(DxfData* data, DxfGraphicsScene* scene, QObject* parent)
    : QObject(parent), m_data(data), m_scene(scene)
{
}

SelectionController::~SelectionController()
{
}

void SelectionController::hitTest(QPointF scenePos)
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
        m_entitySelected = true;
        m_selectedLayer = hitLayer;
        m_selectedIndex = hitIndex;
        emit signalEntitySelected(hitLayer, hitIndex);
    }
    else
    {
        clearSelection();
    }
}

void SelectionController::clearSelection()
{
    if (m_entitySelected)
    {
        m_entitySelected = false;
        m_selectedLayer.clear();
        m_selectedIndex = -1;
        emit signalEntityDeselected();
    }
    m_scene->clearPreview();
}
