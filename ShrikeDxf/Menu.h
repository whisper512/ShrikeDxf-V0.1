#pragma once
#ifndef _MENU_H
#define _MENU_H

#include <QMenuBar.h>
#include <QMenu.h>
#include <QWidget>

class CMenu : public QWidget
{
	public:
	explicit CMenu(QWidget* parent = nullptr);
	~CMenu();
	
	void InitMenuBar();

	

private:
	QWidget* m_pParent;

	//Ö÷²Ëµ¥
	QMenu* m_pMenuFile, * m_pMenuHelp, * m_pMenuTool, * m_pMenuSetting;
	QAction* m_pActionOpen, * m_pActionSave, * m_pActionClose;
	QAction* m_pActionAbout;
	

};

#endif // !_MENU_H