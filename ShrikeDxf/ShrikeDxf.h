#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ShrikeDxf.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QTimer.h>

#include "Menu.h"

#include "CommonDataManger.h"
#include "DxfManger.h"

class ShrikeDxf : public QMainWindow
{
    Q_OBJECT

public:
    ShrikeDxf(QWidget *parent = nullptr);
    ~ShrikeDxf();

	QMenuBar* GetMenuBar() { return ui.menuBar; }

    //初始化界面组件
    void InitWindowComponents();
    //初始化数据管理类
    void InitDataManagers();

    void InitMenuBar();
    void InitTreeView();

public:
    Ui::ShrikeDxfClass ui;

    //菜单栏
    CMenu* m_pMenu;
    //界面通用数据管理
    CCommonDataManager* m_pDataManager;
    //Dxf数据管理
    CDxfManger* m_pDxfDataManger;
    //文件结构栏
    QTreeView* m_pTreeView;

};

