#pragma once
#include <QWidget>
#include <QRegularExpression>

#include "DxfReader.h"
#include "DxfData.h"
#include "DxfTreeviewModel.h"
#include "DxfGraphicsScene.h"
#include "DxfLayerTableviewModel.h"
#include "DxfEditor.h"
#include "DxfStruct.h"
#include "DxfWriter.h"
#include "dxfDrawController.h"
#include "DxfEditController.h"

class DxfInteractionDispatcher;
class CSelectionController;

//dxf管理类
class DxfManager : public QObject
{
	Q_OBJECT
public:
	DxfManager(QWidget* mainWnd);
	~DxfManager();

	// 暴露数据对象连接信号用
	DxfData* getDxfData() const { return m_dxfData.get(); }
	DxfReader* getDxfReader() const { return m_dxfReader.get(); }
	CDxfTreeviewModel* getTreeViewModel() { return &m_DxfTreeviewModel; }
    DxfLayerTableviewModel* getLayerTableviewModel() { return &m_DxfLayerTableviewModel; }
	DxfGraphicsScene* getScene() { return &m_DxfGraphicsScene; }
	DxfEditController* getEditController() const { return m_dxfEditController.get(); } 
	CSelectionController* getSelectionController() const { return m_selectionController.get(); }

	// 获取选中图元
	const stuSelectedEntity& getSelectedEntity() const { return m_selectedEntity; }
	// 获取交互鼠标状态
	const MouseStateInView getCurrentInteractionState();
	// 获取当前工作图层
	const QString& getCurrentLayer() const { return m_currentLayer; }

	// 刷新scene重并重置graphicsview
	void refreshSceneResetview(){
		m_DxfGraphicsScene.dxfDraw(m_dxfData->getLayers());
		emit signalRefreshGraphicsview(&m_DxfGraphicsScene, true);
	}
	// 刷新scene不重置graphicsview
	void refreshScene() {
		m_DxfGraphicsScene.dxfDraw(m_dxfData->getLayers());
		emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
	}


public:
	bool loadDxfFile(const QString& strPath);
	bool saveDxfFile(const QString& strPath);
	bool newDxfFile();
	bool closeDxfFile();

public:
	// 删除图元
	void deleteSelectedEntity();
	// 复制当前选中图元到剪贴板
	void copySelectedEntity();
	// 剪切当前选中图元（复制并删除）
	void cutSelectedEntity();
	// 粘贴剪贴板中的图元到指定场景坐标
	void pasteEntity(QPointF position);

private:
	// 连接内部信号
	void connectSignals();
	void updateSelectionDisplay();
private:
	// mainwindow指针
	QWidget* m_mainWnd;
	
	// dxf数据类
	std::unique_ptr<DxfData> m_dxfData;
	// dxf读取类
	std::unique_ptr<DxfReader> m_dxfReader;

	// treeview的model
	CDxfTreeviewModel m_DxfTreeviewModel;
	// 图层tableview的model
	DxfLayerTableviewModel m_DxfLayerTableviewModel;
	// graphicview的model
	DxfGraphicsScene m_DxfGraphicsScene;
	// dxf编辑类
	std::unique_ptr<DxfEditor> m_dxfEditor;
	// dxf绘制控制和view选中
	std::unique_ptr<dxfDrawController> m_dxfDrawController;
	// dxf编辑控制
	std::unique_ptr<DxfEditController> m_dxfEditController;
	// 交互控制调度
	std::unique_ptr<DxfInteractionDispatcher> m_interactionDispatcher;
	// 选中控制
	std::unique_ptr<CSelectionController> m_selectionController;
	// 选中图元
	stuSelectedEntity m_selectedEntity;
	// 当前工作图层
    QString m_currentLayer;
	
	// 剪贴板
	std::vector<variantDxfEntity> m_clipboard;
	QPointF m_clipboardOrigin;   // 复制时内容的中心点


public:
	// 同步图层模型数据到dxf数据结构
	void synLayerModelToDxfData();
	// 选中指定图元
	void selectEntity(const QString& strLayer, int entityIndex);
	// 取消选中
	void deselectEntity();
	// 剪贴板是否有数据
	bool hasClipboard() const { return !m_clipboard.empty(); }


signals:
	// 通知树状图刷新
	void signalRefreshTreeview(CDxfTreeviewModel* pModel);
	void signalRefreshTreeviewAfterRead();
	// 通知图层graphics刷新
	void signalRefreshGraphicsview(DxfGraphicsScene* pScene, bool bResetViewRect);
	// 选中图元发生变化
	void signalSelectedEntityChanged(const stuSelectedEntity& entity);
	// 更新dxf文件路径
	void signalFileName(const QString& filePath);
	// 更新图层属性
	void signalRefreshLayerTable(DxfLayerTableviewModel* pModel);
	// 通知当前工作图层
	void signalCurrentLayerChanged(const QString& strLayer);
	//通知graphicsview新建图元
	void signalMouseStatusChanged(MouseStateInView mouseState);

 public slots:
	 
	 // 图元属性发生变化
	 void handlePointAttributeChanged(const EntityPoint& point);
     void handleLineAttributeChanged(const EntityLine& line);
	 void handleCircleAttributeChanged(const EntityCircle& circle);
	 void handleArcAttributeChanged(const EntityArc& arc);
     void handleEllipseAttributeChanged(const EntityEllipse& ellipse);
     void handleSplineAttributeChanged(const EntitySpline& spline);
     void handlePolylineAttributeChanged(const EntityPolyline& polyline);
     void handleLwpolylineAttributeChanged(const EntityLWPolyline& lwpolyline);
	 void handleTextAttributeChanged(const EntityText& text);
     void handleMTextAttributeChanged(const EntityMText& mtext);
     void handleHatchAttributeChanged(const EntityHatch& hatch);

	 // 图层属性发生变化
	 void handleLayerAttributeChanged();
	 // 新建图元
	 void handleOnMouseStatusChanged(MouseStateInView mouseState);
	 // 鼠标移动
	 void handleMousePos(QPointF pos);
	 // 鼠标左键点击
	 void handleMouseLeftButtonClicked(QPointF pos);
	 // 鼠标右键点击
     void handleMouseRightButtonClicked(QPointF pos);
	 // 左键按下
	 void handleMouseLeftButtonPressed(QPointF pos);
	 // 左键释放
	 void handleMouseLeftButtonReleased(QPointF pos);

	 // 选中图元
	 void handleEntitySelected(const QString& strLayer, int entityIndex);
	 // 取消选中图元
	 void handleEntityDeselected();
	 // 结束绘制预览
	 void handleEndDrawingPreview();

};