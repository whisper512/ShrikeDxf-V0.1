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
private:
    
public slots:
    

};

