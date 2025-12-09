#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ShrikeDxf.h"

#include <QMenuBar.h>
#include <QMenu.h>
#include <QAction.h>
#include <QTreeView>
#include <QFileSystemModel>


class ShrikeDxf : public QMainWindow
{
    Q_OBJECT

public:
    ShrikeDxf(QWidget *parent = nullptr);
    ~ShrikeDxf();

	QMenuBar* GetMenuBar() { return ui.menuBar; }

    void InitWindow();
    void InitMenuBar();
    void InitTreeView();

private:
    Ui::ShrikeDxfClass ui;

    //主菜单
    QMenu* m_pMenuFile, * m_pMenuHelp, * m_pMenuTool, * m_pMenuSetting;
	QAction* m_pActionOpen, * m_pActionSave, * m_pActionClose;
	QAction* m_pActionAbout;

    //文件结构栏
    QTreeView* m_pTreeView;

    //QFileSystemModel* model = new QFileSystemModel;
    

};

