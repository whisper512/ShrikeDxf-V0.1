#pragma once

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QScrollBar>

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
	
	QAction* m_pActionLockZoom;
	QAction* m_pActionFilpX;
	QAction* m_pActionFilpY;

	//Ëø¶¨Ëõ·Å
	bool m_bLockZoom;
	//ÑØxÖá·­×ª
	bool m_bFilpAlongX;
	//ÑØyÖá·­×ª
	bool m_bFilpAlongY;

	void InitMenu(QWidget* pParent);
	void InitAction();

public:
	QWidget* m_pMainWnd;

signals:

public slots:
	void handleRefreshGraphicsview(CDxfGraphicsScene* pScene);
	void handleFilpAlongX(bool bChecked);
    void handleFilpAlongY(bool bChecked);

	void ShowMenu(const QPoint& pos);
	void handleLockZoom(bool bChecked);

protected:
	void wheelEvent(QWheelEvent* pEvent) override;

};
