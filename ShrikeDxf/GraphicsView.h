#pragma once

#include <QObject>
#include <QGraphicsView>

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
};
