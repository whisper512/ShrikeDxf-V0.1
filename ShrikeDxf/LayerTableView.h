#pragma once
#include <QObject>
#include <QTableView>

#include "DxfLayerTableviewModel.h"

class CLayerTableViewManger : public QObject
{
	Q_OBJECT
public:
	CLayerTableViewManger(QWidget* pMainwnd);
	~CLayerTableViewManger();

	void CreateTableView();

private:
	void InitTableView();

private:
	QWidget* m_pMainWnd;
	QTableView* m_pTableView;

signals:
	void signalLayerModelChanged();

public slots:
	void handleRefreshLayerTableview(CDxfLayerTableviewModel* pModel);
	void handleTableViewClicked(const QModelIndex& index);
};
