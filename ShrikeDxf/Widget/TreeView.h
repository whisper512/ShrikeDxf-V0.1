#pragma once
#include <QObject>
#include <QTreeView>
#include "Manager/DxfTreeviewModel.h"

class TreeViewManager : public QObject
{
	Q_OBJECT
public:
	TreeViewManager(QWidget* mainWnd);
	~TreeViewManager();

	void createTreeView();

private:
	//不要delete这个指针,指向父窗口,会导致析构错误
	QWidget* m_mainWnd;
	QTreeView* m_treeView;

	QMenu *m_contextMenu;
	QAction* m_actionDelete;
	QAction* m_actionCopy;

private:
	void deleteEntity();
	void copyEntity();


signals:
	void signalEntitySelected(const QString& strLayer, int entityIndex);
	void signaldeleteEntityData(const QString& strLayer, const QString& strEntity);
	void signalcopyEntityData();

public slots:
	//获取模型用于刷新
	void handleRefreshTree(DxfTreeviewModel* pModel);
	//右键显示menu
	void handleShowContextMenu(const QPoint &pos);
	//显示返回的entity的数据
	void handleReturnEntityInfo(QString strInfo );
	//获取左键选取的行
	void handleOnItemClicked(const QModelIndex& index);
	//初始化右键menu
	void initContextMenu();
	//读完图第一次刷新
    void handleRefreshTreeviewAfterRead();
};
