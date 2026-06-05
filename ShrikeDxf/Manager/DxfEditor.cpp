#include "DxfEditor.h"

CDxfEditor::CDxfEditor() 
	: m_DxfData(nullptr)
{
}

CDxfEditor::~CDxfEditor()
{
}

void CDxfEditor::editPoint(stuSelectedEntity* selectedEntity, EntityPoint point)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::Point)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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

void CDxfEditor::editLine(stuSelectedEntity* selectedEntity, EntityLine line)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::Line)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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

void CDxfEditor::editArc(stuSelectedEntity* selectedEntity, EntityArc arc)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::Arc)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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

void CDxfEditor::editCircle(stuSelectedEntity* selectedEntity, EntityCircle circle)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::Circle)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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

void CDxfEditor::editEllipse(stuSelectedEntity* selectedEntity, EntityEllipse ellipse)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::Ellipse)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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

void CDxfEditor::editText(stuSelectedEntity* selectedEntity, EntityText text)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::Text)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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

void CDxfEditor::editMText(stuSelectedEntity* selectedEntity, EntityMText mtext)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::MText)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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

void CDxfEditor::editPolyline(stuSelectedEntity* selectedEntity, EntityPolyline polyline)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::Polyline)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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

void CDxfEditor::editLwpolyline(stuSelectedEntity* selectedEntity, EntityLWPolyline lwpolyline)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::LWPolyline)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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

void CDxfEditor::editSpline(stuSelectedEntity* selectedEntity, EntitySpline spline)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::Spline)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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

void CDxfEditor::editHatch(stuSelectedEntity* selectedEntity, EntityHatch hatch)
{
    if (!selectedEntity || !m_DxfData)
        return;

    if (selectedEntity->type != EntityType::Hatch)
        return;

    stuLayer* pLayer = m_DxfData->GetLayer(selectedEntity->strLayer.toStdString());
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
