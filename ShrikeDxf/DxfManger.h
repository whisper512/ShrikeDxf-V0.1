#pragma once
#include <QWidget>
#include <QRegularExpression>


#include "DxfMapping.h"
#include "DxfTreeviewModel.h"
#include "DxfGraphicsScene.h"
#include "DxfLayerTableviewModel.h"


//dxf管理类,包含映射,操作，绘图
class CDxfManger : public QObject
{
	Q_OBJECT
public:
	CDxfManger(QWidget* pMainWnd);
	~CDxfManger();

public:
	bool LoadDxfFile(const QString& strPath);
	bool SaveDxfFile(const QString& strPath);
	void ClearDxfMappingData();

private:
	//mainwindow指针
	QWidget* m_pMainWnd;
	//dxf映射类
	CDxfMapping m_DxfMapping;
	//treeview的model
	CDxfTreeviewModel m_DxfTreeviewModel;
	//图层tableview的model
	CDxfLayerTableviewModel m_DxfLayerTableviewModel;
	//graphicview的model
	CDxfGraphicsScene m_DxfGraphicsScene;

private:
	//刷新树状图和绘图的model
	void RefreshTreeModelAndGraphicsview();

signals:
	//用来通知treeview
	void signalRefreshTreeview(CDxfTreeviewModel* pModel);
	//用来返回获取到的entity信息
	void signalReturnEntityInfo(QString strInfo);
	//通知graphicsview
	void signalRefreshGraphicsview(CDxfGraphicsScene* pScene, bool bResetViewRect);
	//通知layerTableview
	void signalRefreshLayerTableview(CDxfLayerTableviewModel* pModel);
	//通知stackedwidget
	void signalRefreshStackedWidget(variantDxfEntity dxfentity);
	//通知graphics正在复制图元
	void signalCopyintEntity();

 public slots:
	QString handleChangeEntityWidget(const QString& strLayer, const QString& strEntity);
	int handleDeleteEntity(const QString& strLayer, const QString& strEntity);
	int handleCopyEntity(const QString& strLayer, const QString& strEntity);
	void handlePaste(QPointF pos);
	//point界面属性发送变化
	void handlePointAttributeChanged(Point point);
};