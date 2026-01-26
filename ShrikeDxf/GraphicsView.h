#pragma once

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QLabel>

#include "DxfGraphicsScene.h"
#include "DxfStruct.h"
#include "Ruler.h"

class CGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	CGraphicsView();
	~CGraphicsView();

	explicit CGraphicsView(QWidget* pMainwnd);

private:
	//右键菜单
	QMenu* m_pGraphicsViewMenu;
	QMenu* m_pGraphicsOperateMenu;

	//view的操作
	QAction* m_pActionLockZoom;
	QAction* m_pActionFilpX;
	QAction* m_pActionFilpY;
	QAction* m_pActionResetView;
	QAction* m_pActionDrag;
	//针对view中图元的操作
	QAction* m_pActionPasteEntity;

	CRulerH* m_pRulerH;
	CRulerV* m_pRulerV;


	//锁定缩放
	bool m_bLockZoom;
	//沿x轴翻转
	bool m_bFilpAlongX;
	//沿y轴翻转
	bool m_bFilpAlongY;
	//显示鼠标位置十字线
	bool m_bShowPosCross;
	//左键拖拽
	bool m_bDrag;
	//开启复制图元中
	bool m_bCopyingEntity;
	//初始transform
	QTransform m_tranformInitial;
	//初始场景矩阵
	QRectF m_rectInitialScene;
	QPoint m_pointLastPos;
	//上次的场景矩阵
    QRectF m_rectLastScene;
	//右键鼠标位置
    QPoint m_pointRightClickPos;
	

	void InitMenu(QWidget* pParent);
	void InitRuler();
	void InitGraphicsViewAction();
	void InitScene();
	void FilpView();
	void UpdateRulers();

public:
	QWidget* m_pMainWnd;

signals:
	void signalPaste(QPointF pos);
	void signalMousePos(QString strPos);

public slots:
	//类外交互
	void handleRefreshGraphicsview(CDxfGraphicsScene* pScene,bool bResetViewRect);
	//正在复制图元
	void handlelCopyintEntity();
	//viewmenu动作
	void handleFilpAlongX(bool bChecked);
    void handleFilpAlongY(bool bChecked);
	void handleResetView();
	void handleDrag(bool bChecked);
	void ShowMenu(const QPoint& pos);
	void handleLockZoom(bool bChecked);
	//operate动作
	void handlePasteEntity();

protected:
	void wheelEvent(QWheelEvent* pEvent) override;
	void mouseMoveEvent(QMouseEvent* pEvent) override;
	void mousePressEvent(QMouseEvent* pEvent) override;
	void mouseReleaseEvent(QMouseEvent* pEvent) override;
	void resizeEvent(QResizeEvent* pEvent) override;
	void drawForeground(QPainter* pPainter, const QRectF& rect) override;
};
