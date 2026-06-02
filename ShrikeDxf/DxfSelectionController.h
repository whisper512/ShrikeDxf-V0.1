#pragma once
#include <QObject>
#include <QPointF>
#include <QString>

class CDxfData;
class CDxfGraphicsScene;

class CSelectionController : public QObject
{
    Q_OBJECT
public:
    explicit CSelectionController(CDxfData* pData, CDxfGraphicsScene* pScene, QObject* parent = nullptr);
    ~CSelectionController();

    // 是否已选中图元
    bool IsEntitySelected() const { return m_bEntitySelected; }
    const QString& GetSelectedLayer() const { return m_strSelectedLayer; }
    int GetSelectedIndex() const { return m_nSelectedIndex; }

    // 命中测试
    void HitTest(QPointF scenePos);
    // 清除选择
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
