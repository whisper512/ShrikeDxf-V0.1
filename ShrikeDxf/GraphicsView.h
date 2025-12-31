#pragma once

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QLabel>

#include "DxfGraphicsScene.h"

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
	QLabel* m_pLabelMousePos;

	QAction* m_pActionLockZoom;
	QAction* m_pActionFilpX;
	QAction* m_pActionFilpY;
	QAction* m_pActionResetView;
	QAction* m_pActionShowMousePos;
	QAction* m_pActionDrag;

	//锁定缩放
	bool m_bLockZoom;
	//沿x轴翻转
	bool m_bFilpAlongX;
	//沿y轴翻转
	bool m_bFilpAlongY;
	//显示鼠标位置
	bool m_bShowMousePos;
	//左键拖拽
	bool m_bDrag;
	//初始transform
	QTransform m_tranformInitial;
	//初始场景矩阵
	QRectF m_rectInitialScene;
	QPoint m_pointLastPos;

	void InitMenu(QWidget* pParent);
	void InitAction();
	void InitPosLabel();
	void FilpView();

public:
	QWidget* m_pMainWnd;

signals:

public slots:
	void handleRefreshGraphicsview(CDxfGraphicsScene* pScene);
	void handleFilpAlongX(bool bChecked);
    void handleFilpAlongY(bool bChecked);
	void handleResetView();
	void handleShowMousePos(bool bChecked);
	void handleDrag(bool bChecked);

	void ShowMenu(const QPoint& pos);
	void handleLockZoom(bool bChecked);

protected:
	void wheelEvent(QWheelEvent* pEvent) override;
	void mouseMoveEvent(QMouseEvent* pEvent) override;
	void mousePressEvent(QMouseEvent* pEvent) override;
	void mouseReleaseEvent(QMouseEvent* pEvent) override;
};
