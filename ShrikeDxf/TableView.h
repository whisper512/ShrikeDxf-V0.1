#pragma once
#include <QObject>
#include <QTableView>

class CTableViewManger : public QObject
{
public:
	CTableViewManger(QWidget* pMainwnd);
	~CTableViewManger();

	void CreateTableView();
private:
	QWidget* m_pMainWnd;
	QTableView* m_pTableView;
};
