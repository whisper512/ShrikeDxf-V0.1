#pragma once
#include <QObject>
#include <QTableView>

#include "Manager/DxfLayerTableviewModel.h"

class LayerTableViewManager : public QObject
{
	Q_OBJECT
public:
	LayerTableViewManager(QWidget* mainWnd);
	~LayerTableViewManager();

	void createTableView();

private:
	void initTableView();

private:
	QWidget* m_mainWnd;
	QTableView* m_tableView;

signals:
	void signalLayerModelChanged();
	void signalChangeCurrentLayer(QString strCurLayer);

public slots:
	void handleRefreshLayerTableview(DxfLayerTableviewModel* pModel);

private slots:
	void handleTableViewClicked(const QModelIndex& index);
};
