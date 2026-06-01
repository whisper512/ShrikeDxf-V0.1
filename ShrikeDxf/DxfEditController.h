// DxfEditController.h
#pragma once
#include <QObject>
#include <QPointF>
#include <QString>

class CDxfData;
class CDxfGraphicsScene;

class CDxfEditController : public QObject
{
    Q_OBJECT
public:
    explicit CDxfEditController(CDxfData* pData, CDxfGraphicsScene* pScene, QObject* parent = nullptr);
    ~CDxfEditController();

    bool IsEntitySelected() const { return m_bEntitySelected; }
    const QString& GetSelectedLayer() const { return m_strSelectedLayer; }
    int GetSelectedIndex() const { return m_nSelectedIndex; }

    void HitTest(QPointF scenePos);
    void ClearSelection();

signals:
    void signalEntitySelected(const QString& strLayer, int entityIndex);
    void signalEntityDeselected();

private:
    CDxfData* m_pData = nullptr;
    CDxfGraphicsScene* m_pScene = nullptr;

    bool m_bEntitySelected = false;
    QString m_strSelectedLayer;
    int m_nSelectedIndex = -1;
};
