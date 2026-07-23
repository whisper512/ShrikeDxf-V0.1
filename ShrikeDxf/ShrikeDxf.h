#pragma once

#include <QtWidgets/QMainWindow>
#include <QTreeView>
#include <QFileSystemModel>
#include <QTimer.h>

#include "ui_ShrikeDxf.h"
#include "Widget/TopMenu.h"
#include "Widget/TreeView.h"
#include "Widget/GraphicsView.h"
#include "Widget/StackedWidget.h"
#include "Widget/LayerTableView.h"
#include "Widget/MoveBtnsWidget.h"
#include "Widget/CreateEntityWidget.h"

#include "Manager/CommonDataManager.h"
#include "Manager/DxfManager.h"

class ShrikeDxf : public QMainWindow
{
    Q_OBJECT

public:
    ShrikeDxf(QWidget *parent = nullptr);
    ~ShrikeDxf();

	QMenuBar* GetMenuBar() { return ui.menuBar; }

    //初始化界面组件
    void initWindowComponents();
    //初始化数据管理类
    void initDataManagers();
    //初始化menu
    void initAndCreateMenuBar();
    //初始化treeview
    void initAndcreateTreeView();
    //初始化graphicsView
    void initAndCreateGraphicsView();
    //初始化stackedWidget
    void initAndcreateStackedWidget();
    //初始化tableView
    void initAndCreateLayerTableView();
    //初始化移动按钮
    void initAndCreateMoveBtnsWidget();
    //初始化新建图元界面
    void initAndCreateCreateEntityWidget();
    //初始化位置显示label
    void initLabels();
    //链接信号和槽
    void connectSignalsAndSlots();

public:
    Ui::ShrikeDxfClass ui;

    //菜单栏
    MenuManager* m_menuManger;
    //文件结构栏
    TreeViewManager* m_treeViewManger;
    //图形界面
    GraphicsView* m_graphicsView;
    //stackedWidget
    StackedWidgetManager* m_stackedWidgetManager;
    //tableview
    LayerTableViewManager* m_layerTableViewManager;
    //移动按钮
    MoveBtnsWidget* m_moveBtnsWidget;
    //新建图元界面
    CreateEntityWidget* m_createEntityWidget;

    //显示graphics的坐标
    QLabel* m_labelPos;
    //显示当前文档名称
    QLabel* m_labelDocName;
    //显示当前工作图层
    QLabel* m_labelLayer;


    //界面通用数据管理
    CommonDataManager* m_dataManager;
    //Dxf数据管理
    DxfManager* m_dxfDataManager;

public slots:
    void handleMousePos(QString strPos);
    void handleShowDocName(QString strDocName);
    void handleShowLayerName(QString strLayerName);
};

