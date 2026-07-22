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

    void editPoint(stuSelectedEntity* selectedEntity, EntityPoint point);
    void editLine(stuSelectedEntity* selectedEntity, EntityLine line);
    void editArc(stuSelectedEntity* selectedEntity, EntityArc arc);
    void editCircle(stuSelectedEntity* selectedEntity, EntityCircle circle);
    void editEllipse(stuSelectedEntity* selectedEntity, EntityEllipse ellipse);
    void editText(stuSelectedEntity* selectedEntity, EntityText text);
    void editMText(stuSelectedEntity* selectedEntity, EntityMText mtext);
    void editPolyline(stuSelectedEntity* selectedEntity, EntityPolyline polyline);
    void editLwpolyline(stuSelectedEntity* selectedEntity, EntityLWPolyline lwpolyline);
    void editSpline(stuSelectedEntity* selectedEntity, EntitySpline spline);
    void editHatch(stuSelectedEntity* selectedEntity, EntityHatch hatch);

private:
    // dxf数据类
    DxfData* m_dxfData;
public slots:
    

};

