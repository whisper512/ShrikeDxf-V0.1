#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ShrikeDxf.h"
#include <QTreeView>
#include <QFileSystemModel>
#include <QTimer.h>

#include "TopMenu.h"
#include "TreeView.h"
#include "GraphicsView.h"
#include "StackedWidget.h"
#include "LayerTableView.h"
#include "MoveBtnsWidget.h"
#include "CreateEntityWidget.h"

#include "CommonDataManager.h"
#include "DxfManager.h"

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
    void InitAndCreateMenuBar();
    //初始化treeview
    void InitAndCreateTreeView();
    //初始化graphicsView
    void InitAndCreateGraphicsView();
    //初始化stackedWidget
    void InitAndCreateStackedWidget();
    //初始化tableView
    void InitAndCreateLayerTableView();
    //初始化移动按钮
    void InitAndCreateMoveBtnsWidget();
    //初始化新建图元界面
    void InitAndCreateCreateEntityWidget();

    //初始化位置显示label
    void InitLabels();

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
    CLayerTableViewManger* m_pLayerTableViewManger;
    //移动按钮
    CMoveBtnsWidget* m_pMoveBtnsWidget;
    //新建图元界面
    CCreateEntityWidget* m_pCreateEntityWidget;

    //显示graphics的坐标
    QLabel* m_pLabelPos;
    //显示当前文档名称
    QLabel* m_pLabelDocName;
    //显示当前工作图层
    QLabel* m_pLabelLayer;


    //界面通用数据管理
    CCommonDataManager* m_pDataManager;
    //Dxf数据管理
    CDxfManager* m_pDxfDataManger;

public slots:
    void handleMousePos(QString strPos);
    void handleShowDocName(QString strDocName);
    void handleShowLayerName(QString strLayerName);
};

