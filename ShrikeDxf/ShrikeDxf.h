#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ShrikeDxf.h"
#include <QTreeView>
#include <QFileSystemModel>

#include "Menu.h"

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

    CMenu* pMenu;

    //文件结构栏
    QTreeView* m_pTreeView;

   
    

};

