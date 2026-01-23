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
	//通知tree读完图刷新
	void signalRefreshTreeviewAfterRead();
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
	//通知移动步长和旋转角度的初始值
	void signalSetStepLengthAndAngle(const double& dStepLength, const double& dRotationAngle);
	//当前的当前选中的entity的类型
	void signalSelectedEntityType(int nType);
	//更新label显示文件名
	void signalShowFileName(QString strFileName);


 public slots:
	QString handleSaveSelectedEntity(const QString& strLayer, const QString& strEntity);
	int handleDeleteEntity(const QString& strLayer, const QString& strEntity);
	int handleCopyEntity(const QString& strLayer, const QString& strEntity);
	void handlePaste(QPointF pos);

	void handleLayerModelChanged();

	//point界面属性发生变化
	void handlePointAttributeChanged(Point point);
	//line界面属性发生变化
	void handleLineAttributeChanged(Line line);
	//circle界面属性发生变化
	void handleCircleAttributeChanged(Circle circle);
    //arc界面属性发生变化
    void handleArcAttributeChanged(Arc arc);
	//polyline界面属性发生变化
    void handlePolylineAttributeChanged(Polyline polyline);

	//向上按钮按下
	void handleOnBtnUpClicked();
    //向下按钮按下
	void handleOnBtnDownClicked();
    //向左按钮按下
	void handleOnBtnLeftClicked();
    //向右按钮按下
    void handleOnBtnRightClicked();
	//顺时针旋转
    void handleOnBtnCWClicked();
    //逆时针旋转
    void handleOnBtnCCWClicked();
	//设置移动步长
	void handleOnStepLengthOrAngleChanged(double dStepLength, double dRotationAngle);

};