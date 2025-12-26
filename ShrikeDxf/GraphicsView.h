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


public:
	QWidget* m_pMainWnd;

public slots:
	void handleRefreshGraphicsview(CDxfGraphicsScene* pScene);

protected:
	void wheelEvent(QWheelEvent* pEvent) override;

};
