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


//dxf管理类
class CDxfManager : public QObject
{
	Q_OBJECT
public:
	CDxfManager(QWidget* pMainWnd);
	~CDxfManager();

	// 暴露数据对象连接信号用
	CDxfData* GetDxfData() const { return m_DxfData.get(); }
	CDxfReader* GetDxfReader() const { return m_DxfReader.get(); }
	CDxfTreeviewModel* GetTreeViewModel() { return &m_DxfTreeviewModel; }
    CDxfLayerTableviewModel* GetLayerTableviewModel() { return &m_DxfLayerTableviewModel; }
	CDxfGraphicsScene* GetScene() { return &m_DxfGraphicsScene; }

	// 获取选中图元
	const stuSelectedEntity& GetSelectedEntity() const { return m_SelectedEntity; }
	// 刷新scene重新计算边界
	void RefreshSceneCalRect(){
		m_DxfGraphicsScene.DxfDraw(m_DxfData->GetLayers());
		emit signalRefreshGraphicsview(&m_DxfGraphicsScene, true);
	}
	// 刷新scene但不计算边界
	void RefreshScene() {
		m_DxfGraphicsScene.DxfDraw(m_DxfData->GetLayers());
		emit signalRefreshGraphicsview(&m_DxfGraphicsScene, false);
	}

public:
	bool LoadDxfFile(const QString& strPath);
	bool SaveDxfFile(const QString& strPath);
	bool NewDxfFile();
	bool CloseDxfFile();
private:
	// mainwindow指针
	QWidget* m_pMainWnd;
	
	// dxf数据类
	std::unique_ptr<CDxfData> m_DxfData;
	// dxf读取类
	std::unique_ptr<CDxfReader> m_DxfReader;

	// treeview的model
	CDxfTreeviewModel m_DxfTreeviewModel;
	// 图层tableview的model
	CDxfLayerTableviewModel m_DxfLayerTableviewModel;
	// graphicview的model
	CDxfGraphicsScene m_DxfGraphicsScene;
	// dxf编辑类
	CDxfEditor m_DxfEditor;
	// 选中图元
	stuSelectedEntity m_SelectedEntity;
	
	
public:
	


signals:
	// 通知树状图刷新
	void signalRefreshTreeview(CDxfTreeviewModel* pModel);
	void signalRefreshTreeviewAfterRead();
	// 通知图层graphics刷新
	void signalRefreshGraphicsview(CDxfGraphicsScene* pScene, bool bResetViewRect);
	// 选中图元发生变化
	void signalSelectedEntityChanged(const stuSelectedEntity& entity);
	// 更新dxf文件路径
	void signalFileName(const QString& filePath);
	// 更新图层属性
	void signalRefreshLayerTable(CDxfLayerTableviewModel* pModel);


 public slots:
	 // treeview选中图元发生变化
	 void OnTreeViewEntitySelected(const QString& strLayer, int entityIndex);

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
};