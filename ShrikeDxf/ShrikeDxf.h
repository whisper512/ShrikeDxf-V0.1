#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ShrikeDxf.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QTimer.h>

#include "Menu.h"

class ShrikeDxf : public QMainWindow
{
    Q_OBJECT

public:
    ShrikeDxf(QWidget *parent = nullptr);
    ~ShrikeDxf();

	QMenuBar* GetMenuBar() { return ui.menuBar; }

    //初始化界面组件
    void InitWindowComponents();

    void InitMenuBar();
    void InitTreeView();

private:
    Ui::ShrikeDxfClass ui;

    //菜单栏
    CMenu* m_pMenu;

    //文件结构栏
    QTreeView* m_pTreeView;

};

