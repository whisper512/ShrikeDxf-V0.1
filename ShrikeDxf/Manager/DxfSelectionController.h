#pragma once
#include <QObject>
#include <QPointF>
#include <QString>

class DxfData;
class CDxfGraphicsScene;

// 选择控制器
class CSelectionController : public QObject
{
    Q_OBJECT
public:
    explicit CSelectionController(DxfData* pData, CDxfGraphicsScene* pScene, QObject* parent = nullptr);
    ~CSelectionController();

    // 是否已选中图元
    bool IsEntitySelected() const { return m_bEntitySelected; }
    int getSelectedIndex() const { return m_nSelectedIndex; }
    const QString& getSelectedLayer() const { return m_selectedLayer; }

    // 命中测试
    void HitTest(QPointF scenePos);
    // 清除选择
    void clearSelection();

signals:
    // 通知选中的图元
    void signalEntitySelected(const QString& strLayer, int entityIndex);
    // 通知取消选中
    void signalEntityDeselected();

private:
    DxfData* m_data = nullptr;
    CDxfGraphicsScene* m_scene = nullptr;
    // 是否选中
    bool m_bEntitySelected = false;
    // 选中的图元图层
    QString m_selectedLayer;
    // 选中的图元索引
    int m_nSelectedIndex = -1;
};
