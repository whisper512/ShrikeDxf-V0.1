#pragma once
#include <QObject>
#include <QTreeView>
#include "DxfTreeviewModel.h"

class CTreeViewManger : public QObject
{
	Q_OBJECT
public:
	CTreeViewManger(QWidget* pMainwnd);
	~CTreeViewManger();

	void CreateTreeView();

private:
	//不要delete这个指针,指向父窗口,会导致析构错误
	QWidget* m_pMainwnd;

	QTreeView* m_pTreeView;

signals:
	//通知dafmanger获取具体信息
	void GetEntityData(const QString& strLayer, const QString& strEntity);
	//通知dxfmanger图元属性界面
	void ChangeEntityWidget(const QString& strLayer, const QString& strEntity);

public slots:
	//获取模型用于刷新
	void handleRefreshTree(CDxfTreeviewModel* pModel);
	//右键显示menu
	void ShowContextMenu(const QPoint &pos);
	//显示menu
	void ShowModelData();
	//显示返回的entity的数据
	void handleReturnEntityInfo(QString strInfo );
	//获取左键选取的行
	void handleOnItemClicked(const QModelIndex& index);
};
