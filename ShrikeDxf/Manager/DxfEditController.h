#pragma once
#include <QObject>
#include <QPointF>
#include <QString>
#include <QRectF>
#include "Tools.h"

class CDxfData;
class CDxfGraphicsScene;
class CSelectionController;

class CDxfEditController : public QObject
{
    Q_OBJECT
public:
    explicit CDxfEditController(CDxfData* pData, CDxfGraphicsScene* pScene,
        CSelectionController* pSelection, QObject* parent = nullptr);
    ~CDxfEditController();

    // ── 选中状态同步 ──
    void SetSelectedEntity(const QString& strLayer, int entityIndex);
    void ClearSelection();

    // ── 夹点命中测试 ──
    StretchGripInView HitTestGrip(QPointF scenePos) const;

    // ── 拉伸三阶段 ──
    void StartStretch(StretchGripInView grip);
    void UpdateStretch(QPointF newPos);
    void EndStretch();

    // ── 查询 ──
    bool              IsStretching()    const { return m_bStretching; }
    StretchGripInView  GetCurrentGrip()  const { return m_eCurrentGrip; }
    QString           GetSelectedLayer() const { return m_strSelectedLayer; }
    int               GetSelectedIndex() const { return m_iSelectedIndex; }

signals:
    void signalEntitySelected(const QString& strLayer, int entityIndex);
    void signalEntityDeselected();
    void signalStretchFinished();

private:
    // 内部工具：四点匹配九宫格夹点
    static StretchGripInView GripFromPoint(QPointF pt, const QRectF& bb, double tol);

    // 内部：刷新场景 + 夹点
    void RefreshSceneWithGrips();

    CDxfData* m_pData = nullptr;
    CDxfGraphicsScene* m_pScene = nullptr;
    CSelectionController* m_pSelection = nullptr;

    QString        m_strSelectedLayer;
    int            m_iSelectedIndex = -1;

    bool               m_bStretching = false;
    StretchGripInView  m_eCurrentGrip = StretchGripInView::None;

    static constexpr double kGripHitTol = 12.0;
};
