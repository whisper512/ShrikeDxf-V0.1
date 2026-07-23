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
	void InitAction();
	void AddToBar();
	void ConnectSolt();

private:
	QWidget* m_pParent;

	//menu
	QMenu* m_pMenuFile, * m_pMenuHelp, * m_pMenuSetting, * m_pMenuView;
	//文件action
	QAction* m_pActionOpen, * m_pActionSave, * m_pActionClose, * m_pActionNew, * m_pActionSaveAs;
	//外观action
	//QAction* m_pActionLightTheme, *m_pActionDarkTheme;
	//帮助action
	QAction* m_pActionAbout;

private:
	//槽函数
	void OnOpen();
	void OnSave();
    void OnClose();
    void OnNew();
    void OnSaveAs();
};

#endif // !_MENU_H