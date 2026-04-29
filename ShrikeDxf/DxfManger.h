#pragma once
#include <QWidget>
#include <QRegularExpression>

#include "libdxfrw.h"
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
	bool NewDxfFile();
	bool CloseDxfFile();
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

	//dxf版本
	//DL_Codes::version m_dxfVersion;
	
public:
	//预览相关
	Line m_linePreview;
	QPointF m_pointCircleCenter; // 存储圆心位置
	QPointF m_pointDiameterStart; // 存储直径起点位置
	QPointF m_pointArcCenter;  // 存储弧线圆心
	QPointF m_pointArcStart;   // 存储弧线起点
	Arc m_arcPreview;
	QPointF m_ArcFirstPoint;   // 存储弧线的起点
	QPointF m_ArcSecondPoint;  // 存储弧线的中间点
public:
	//当前工作图层
    QString m_strCurrentLayer;
	QColor GetCurrentLayerColor();
	QString GetCurrentLayerName();
private:
	//刷新树状图和绘图的model
	void RefreshTreelAndGraphicsAndLayertable();

signals:
	//-----------
	//用来通知treeview
	void signalRefreshTreeview(CDxfTreeviewModel* pModel);
	//通知tree读完图刷新
	void signalRefreshTreeviewAfterRead();
	//用来返回获取到的entity信息
	void signalReturnEntityInfo(QString strInfo);
	//-----------

	//-----------
	//通知graphicsview，加载新的scene,加载scene后要不要重新计算边界
	void signalRefreshGraphicsview(CDxfGraphicsScene* pScene, bool bResetViewRect);
	//-----------

	//-----------
	//通知layerTableview
	void signalRefreshLayerTableview(CDxfLayerTableviewModel* pModel);
	//-----------

	//-----------
	//通知stackedwidget
	void signalRefreshStackedWidget(variantDxfEntity dxfentity);
	//-----------

	//-----------
	//通知graphics正在复制图元
	void signalCopyintEntity();
	//通知graphics正在预览
    void signalStartPreviewEntity(int index);
	//-----------

	//-----------
	//通知移动步长和旋转角度的初始值
	void signalSetStepLengthAndAngle(const double& dStepLength, const double& dRotationAngle);
	//当前的当前选中的entity的类型
	void signalSelectedEntityType(int nType);
	//更新label显示文件名
	void signalShowFileName(QString strFileName);
	//通知主界面修改label
	void signalCurrentLayer(QString strLayer);
	//通知createbtn切换状态
	void signalChangeCreateBtnStatus(int index);
	//-----------

 public slots:
	 //-----treeview的槽函数-----
	 //保存tree鼠标点选的图元
	QString handleSaveSelectedEntity(const QString& strLayer, const QString& strEntity);
	//tree删除图元
	int handleDeleteEntity(const QString& strLayer, const QString& strEntity);
	//通知graphics复制图元
	int handleCopyEntity();
	//粘贴图元
	void handlePaste(QPointF pos);
	//-----------
	
	//-----tablelayer的槽函数-----
	//tablelayer的模型发生改变,刷新
	void handleLayerModelChanged();
	//工作图层改变
	void handleChangeCurrentLayer(QString strLayer);
	//-----------

	//-----stackwidget的槽函数-----
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
	//-----------

	//-----movewidget的槽函数-----
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
	//-----------

	//-----graphicsview的槽函数-----
	//创建图元按钮改变
	void handleMouseStatus(int index);
	void handleGraphicsViewMouseMove(QPointF pos);
	void handleGraphicsViewLeftClick(QPointF pos);
	void handleGraphicsViewRightClick(QPointF pos);
	//graphicsview结束绘画预览
	void handleEndDrawPreview();
	//-----------


};