// DxfEditController.h
#pragma once
#include <QObject>
#include <QPointF>
#include <QString>

class CDxfData;
class CDxfGraphicsScene;
class CSelectionController;

class CDxfEditController : public QObject
{
    Q_OBJECT
public:
    explicit CDxfEditController(CDxfData* pData, CDxfGraphicsScene* pScene, QObject* parent = nullptr);
    ~CDxfEditController();


signals:
    void signalEntitySelected(const QString& strLayer, int entityIndex);
    void signalEntityDeselected();

private:
    CDxfData* m_pData = nullptr;
    CDxfGraphicsScene* m_pScene = nullptr;
    CSelectionController* m_pSelection = nullptr;
};
