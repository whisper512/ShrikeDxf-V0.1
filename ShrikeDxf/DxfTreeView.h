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

	QMenu *m_pContextMenu;
	QAction* m_pActionDelete;

private:
	void DeleteEntity();


signals:
	//通知dxfmanger图元属性界面
	void signalChangeEntityWidget(const QString& strLayer, const QString& strEntity);
	//通知dxfmanger删除图元
	void signalDeleteEntityData(const QString& strLayer, const QString& strEntity);

public slots:
	//获取模型用于刷新
	void handleRefreshTree(CDxfTreeviewModel* pModel);
	//右键显示menu
	void handleShowContextMenu(const QPoint &pos);
	//显示返回的entity的数据
	void handleReturnEntityInfo(QString strInfo );
	//获取左键选取的行
	void handleOnItemClicked(const QModelIndex& index);
	//初始化右键menu
	void InitContextMenu();
};
