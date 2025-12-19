#pragma once
#include <QWidget>

#include "DxfMapping.h"
#include "DxfTreeviewModel.h"


//dxf管理类,包含映射,操作，绘图
class CDxfManger : public QObject
{
	Q_OBJECT
public:
	CDxfManger(QWidget* pMainWnd);
	~CDxfManger();

public:
	bool LoadDxfFile(const QString& strPath);

private:
	//mainwindow指针
	QWidget* m_pMainWnd;
	//dxf映射类
	CDxfMapping m_DxfMapping;
	//用于显示treeview的model
	CDxfTreeviewModel m_DxfTreeviewModel;

signals:
	//用来通知treeview
	void RefreshTreeview(CDxfTreeviewModel* pModel);


};

