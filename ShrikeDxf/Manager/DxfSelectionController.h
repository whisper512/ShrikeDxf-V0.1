#pragma once
#include <QObject>
#include <QPointF>
#include <QString>

class DxfData;
class DxfGraphicsScene;

// 选择控制器
class SelectionController : public QObject
{
    Q_OBJECT
public:
    explicit SelectionController(DxfData* data, DxfGraphicsScene* scene, QObject* parent = nullptr);
    ~SelectionController();

    // 是否已选中图元
    bool IsEntitySelected() const { return m_entitySelected; }
    int getSelectedIndex() const { return m_selectedIndex; }
    const QString& getSelectedLayer() const { return m_selectedLayer; }

    // 命中测试
    void hitTest(QPointF scenePos);
    // 清除选择
    void clearSelection();

signals:
    // 通知选中的图元
    void signalEntitySelected(const QString& strLayer, int entityIndex);
    // 通知取消选中
    void signalEntityDeselected();

private:
    DxfData* m_data = nullptr;
    DxfGraphicsScene* m_scene = nullptr;
    // 是否选中
    bool m_entitySelected = false;
    // 选中的图元图层
    QString m_selectedLayer;
    // 选中的图元索引
    int m_selectedIndex = -1;
};
