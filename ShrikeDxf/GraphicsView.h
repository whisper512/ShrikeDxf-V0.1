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
	//ÓÒ¼ü²Ëµ¥
	QMenu* m_pGraphicsViewMenu;
	QLabel* m_pLabelMousePos;

	QAction* m_pActionLockZoom;
	QAction* m_pActionFilpX;
	QAction* m_pActionFilpY;
	QAction* m_pActionResetView;
	QAction* m_pActionShowMousePos;
	QAction* m_pActionDrag;

	//Ëø¶¨Ëõ·Å
	bool m_bLockZoom;
	//ÑØxÖá·­×ª
	bool m_bFilpAlongX;
	//ÑØyÖá·­×ª
	bool m_bFilpAlongY;
	//ÏÔÊ¾Êó±êÎ»ÖÃ
	bool m_bShowMousePos;
	//×ó¼üÍÏ×§
	bool m_bDrag;
	//³õÊ¼transform
	QTransform m_tranformInitial;
	//³õÊ¼³¡¾°¾ØÕó
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
