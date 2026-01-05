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

signals:
	//用来通知treeview
	void RefreshTreeview(CDxfTreeviewModel* pModel);
	//用来返回获取到的entity信息
	void ReturnEntityInfo(QString strInfo);
	//通知graphicsview
	void RefreshGraphicsview(CDxfGraphicsScene* pScene);
	//通知layerTableview
	void RefreshLayerTableview(CDxfLayerTableviewModel* pModel);

 public slots:
	QString handleGetEntityData(const QString& strLayer, const QString& strEntity);
	QString handleChangeEntityWidget(const QString& strLayer, const QString& strEntity);
};