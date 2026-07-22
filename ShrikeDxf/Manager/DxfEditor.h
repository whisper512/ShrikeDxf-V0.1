#pragma once
#include <QObject>
#include "DxfStruct.h"
#include "DxfData.h"

class DxfEditor : public QObject
{
    Q_OBJECT
public:
    explicit DxfEditor(DxfData* dxfData);
    ~DxfEditor();


public:

    void editPoint(SelectedEntity* selectedEntity, EntityPoint point);
    void editLine(SelectedEntity* selectedEntity, EntityLine line);
    void editArc(SelectedEntity* selectedEntity, EntityArc arc);
    void editCircle(SelectedEntity* selectedEntity, EntityCircle circle);
    void editEllipse(SelectedEntity* selectedEntity, EntityEllipse ellipse);
    void editText(SelectedEntity* selectedEntity, EntityText text);
    void editMText(SelectedEntity* selectedEntity, EntityMText mtext);
    void editPolyline(SelectedEntity* selectedEntity, EntityPolyline polyline);
    void editLwpolyline(SelectedEntity* selectedEntity, EntityLWPolyline lwpolyline);
    void editSpline(SelectedEntity* selectedEntity, EntitySpline spline);
    void editHatch(SelectedEntity* selectedEntity, EntityHatch hatch);

private:
    // dxf数据类
    DxfData* m_dxfData;
public slots:
    

};

