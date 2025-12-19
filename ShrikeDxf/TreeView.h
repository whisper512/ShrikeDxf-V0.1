#pragma once
#include <QObject>
#include <QTreeView>
#include "DxfTreeviewModel.h"

class CTreeView : public QObject
{
	Q_OBJECT
public:
	CTreeView(QWidget* pMainwnd);
	~CTreeView();

	void CreateTreeView();

private:
	//不要delete这个指针,指向父窗口,会导致析构错误
	QWidget* m_pMainwnd;

	QTreeView* m_pTreeView;

public slots:

	void RefreshTree(CDxfTreeviewModel* pModel);


};
