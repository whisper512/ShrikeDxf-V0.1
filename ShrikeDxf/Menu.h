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
	void InitMenu();
	void InitAction();
	void AddToBar();
	

private:
	QWidget* m_pParent;

	//menu
	QMenu* m_pMenuFile, * m_pMenuHelp, * m_pMenuTool, * m_pMenuSetting, * m_pMenuView;
	//ÎÄ¼þaction
	QAction* m_pActionOpen, * m_pActionSave, * m_pActionClose;
	//°ïÖúaction
	QAction* m_pActionAbout;
};

#endif // !_MENU_H