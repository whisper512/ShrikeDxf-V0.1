#pragma once
#include "DxfStruct.h"
#include "DxfData.h"
#include <QObject>

class CDxfEditor : public QObject
{
    Q_OBJECT
public:
	CDxfEditor();
    ~CDxfEditor();

public:
    // dxf数据类
    CDxfData* m_DxfData;

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
    
public slots:
    

};

