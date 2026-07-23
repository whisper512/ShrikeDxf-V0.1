#pragma once
#ifndef _MENU_H
#define _MENU_H

#include <QMenuBar.h>
#include <QMenu.h>
#include <QWidget>

class MenuManager : public QWidget
{
	public:
	explicit MenuManager(QWidget* parent = nullptr);
	~MenuManager();
	
	void initMenuBar();
	void initMenu();
	void initAction();
	void addToBar();
	void connectSlot();

private:
	QWidget* m_parent;

	//menu
	QMenu* m_menuFile, * m_menuHelp, * m_menuSetting, * m_menuView;
	//文件action
	QAction* m_actionOpen, * m_actionSave, * m_actionClose, * m_actionNew, * m_actionSaveAs;
	//外观action
	//QAction* m_pActionLightTheme, *m_pActionDarkTheme;
	//帮助action
	QAction* m_actionAbout;

private:
	//槽函数
	void onOpen();
	void onSave();
    void onClose();
    void onNew();
    void onSaveAs();
};

#endif // !_MENU_H