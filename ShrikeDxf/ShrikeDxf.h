#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ShrikeDxf.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QTimer.h>

#include "Menu.h"
#include "TreeView.h"
#include "GraphicsView.h"
#include "StackedWidget.h"
#include "TableView.h"

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
    //初始化menu
    void InitMenuBar();
    //初始化treeview
    void InitTreeView();
    //初始化graphicsView
    void InitGraphicsView();
    //初始化stackedWidget
    void InitStackedWidget();
    //初始化tableView
    void InitTableView();

    //链接信号和槽
    void ConnectSignalsAndSlots();

public:
    Ui::ShrikeDxfClass ui;

    //菜单栏
    CMenuManger* m_pMenuManger;
    //文件结构栏
    CTreeViewManger* m_pTreeViewManger;
    //图形界面
    CGraphicsView* m_pGraphicsView;
    //stackedWidget
    CStackedWidgetManger* m_pStackedWidgetManger;
    //tableview
    CTableViewManger* m_pTableViewManger;

    //界面通用数据管理
    CCommonDataManager* m_pDataManager;
    //Dxf数据管理
    CDxfManger* m_pDxfDataManger;


};

