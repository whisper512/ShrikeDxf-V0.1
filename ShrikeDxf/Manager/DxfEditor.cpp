#include "DxfEditor.h"

DxfEditor::DxfEditor(DxfData* dxfData)
    : m_dxfData(dxfData)
{
    
}

DxfEditor::~DxfEditor()
{
}

void DxfEditor::editPoint(SelectedEntity* selectedEntity, EntityPoint point)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::Point)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 ||selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntityPoint>(entity))
        return;

    auto& pt = std::get<EntityPoint>(entity);
    pt = point;
    selectedEntity->entity = pt;  // 更新选中图元数据
    
}

void DxfEditor::editLine(SelectedEntity* selectedEntity, EntityLine line)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::Line)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 || selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntityLine>(entity))
        return;

    auto& selectedLine = std::get<EntityLine>(entity);
    selectedLine = line;
    selectedEntity->entity = selectedLine;
}

void DxfEditor::editArc(SelectedEntity* selectedEntity, EntityArc arc)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::Arc)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 || selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntityArc>(entity))
        return;

    auto& selectedArc = std::get<EntityArc>(entity);
    selectedArc = arc;
    selectedEntity->entity = selectedArc;
}

void DxfEditor::editCircle(SelectedEntity* selectedEntity, EntityCircle circle)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::Circle)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 || selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntityCircle>(entity))
        return;

    auto& selectedCircle = std::get<EntityCircle>(entity);
    selectedCircle = circle;
    selectedEntity->entity = selectedCircle;
}

void DxfEditor::editEllipse(SelectedEntity* selectedEntity, EntityEllipse ellipse)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::Ellipse)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 || selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntityEllipse>(entity))
        return;

    auto& selectedEllipse = std::get<EntityEllipse>(entity);
    selectedEllipse = ellipse;
    selectedEntity->entity = selectedEllipse;
}

void DxfEditor::editText(SelectedEntity* selectedEntity, EntityText text)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::Text)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 || selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntityText>(entity))
        return;

    auto& selectedText = std::get<EntityText>(entity);
    selectedText = text;
    selectedEntity->entity = selectedText;
}

void DxfEditor::editMText(SelectedEntity* selectedEntity, EntityMText mtext)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::MText)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 || selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntityMText>(entity))
        return;

    auto& selectedMText = std::get<EntityMText>(entity);
    selectedMText = mtext;
    selectedEntity->entity = selectedMText;
}

void DxfEditor::editPolyline(SelectedEntity* selectedEntity, EntityPolyline polyline)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::Polyline)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 || selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntityPolyline>(entity))
        return;

    auto& selectedPolyline = std::get<EntityPolyline>(entity);
    selectedPolyline = polyline;
    selectedEntity->entity = selectedPolyline;
}

void DxfEditor::editLwpolyline(SelectedEntity* selectedEntity, EntityLWPolyline lwpolyline)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::LWPolyline)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 || selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntityLWPolyline>(entity))
        return;

    auto& selectedLWPolyline = std::get<EntityLWPolyline>(entity);
    selectedLWPolyline = lwpolyline;
    selectedEntity->entity = selectedLWPolyline;
}

void DxfEditor::editSpline(SelectedEntity* selectedEntity, EntitySpline spline)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::Spline)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 || selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntitySpline>(entity))
        return;

    auto& selectedSpline = std::get<EntitySpline>(entity);
    selectedSpline = spline;
    selectedEntity->entity = selectedSpline;
}

void DxfEditor::editHatch(SelectedEntity* selectedEntity, EntityHatch hatch)
{
    if (!selectedEntity || !m_dxfData)
        return;

    if (selectedEntity->type != EntityType::Hatch)
        return;

    Layer* pLayer = m_dxfData->getLayer(selectedEntity->layer.toStdString());
    if (!pLayer)
        return;

    if (selectedEntity->entityIndex < 0 || selectedEntity->entityIndex >= static_cast<int>(pLayer->entities.size()))
        return;

    auto& entity = pLayer->entities[selectedEntity->entityIndex];
    if (!std::holds_alternative<EntityHatch>(entity))
        return;

    auto& selectedHatch = std::get<EntityHatch>(entity);
    selectedHatch = hatch;
    selectedEntity->entity = selectedHatch;
}
