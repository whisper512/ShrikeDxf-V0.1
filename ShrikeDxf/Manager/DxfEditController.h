#pragma once
#include <QObject>
#include <QPointF>
#include <QString>
#include <QRectF>
#include "Tools.h"

class DxfData;
class CDxfGraphicsScene;
class CSelectionController;
class DxfManager;

class DxfEditController : public QObject
{
    Q_OBJECT
public:
    explicit DxfEditController(DxfData* data, CDxfGraphicsScene* scene,
        CSelectionController* selection, DxfManager* manager, QObject* parent = nullptr);
    ~DxfEditController();

    // ── 选中状态同步 ──
    void setSelectedEntity(const QString& strLayer, int entityIndex);
    void clearSelection();

    // ── 夹点命中测试 ──
    StretchGripInView hitTestGrip(QPointF scenePos) const;

    // ── 拉伸三阶段 ──
    void startStretch(StretchGripInView grip);
    void updateStretch(QPointF newPos);
    void endStretch(QPointF finalPos);

    // ── 查询 ──
    bool              isStretching()    const { return m_stretching; }
    StretchGripInView  getCurrentGrip()  const { return m_currentGrip; }
    QString           getSelectedLayer() const { return m_selectedLayer; }
    int               getSelectedIndex() const { return m_selectedIndex; }

signals:
    void signalEntitySelected(const QString& strLayer, int entityIndex);
    void signalEntityDeselected();
    void signalStretchFinished();

private:
    // 内部工具：四点匹配九宫格夹点
    static StretchGripInView gripFromPoint(QPointF pt, const QRectF& bb, double tol);

    // 内部：刷新场景 + 夹点
    void refreshSceneWithGrips();

    DxfData* m_data = nullptr;
    CDxfGraphicsScene* m_scene = nullptr;
    CSelectionController* m_selection = nullptr;
    DxfManager* m_manager = nullptr;

    QString        m_selectedLayer;
    int            m_selectedIndex = -1;

    bool               m_stretching = false;
    StretchGripInView  m_currentGrip = StretchGripInView::None;

    static constexpr double kGripHitTol = 12.0;
};
