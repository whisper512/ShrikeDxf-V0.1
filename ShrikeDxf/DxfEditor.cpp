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
