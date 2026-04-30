#pragma once
#include <QWidget>
#include <QRegularExpression>

#include "DxfReader.h"
#include "DxfData.h"

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

	// 暴露数据对象
	CDxfData* GetDxfData() const { return m_DxfData.get(); }
	CDxfReader* GetDxfReader() const { return m_DxfReader.get(); }


public:
	bool LoadDxfFile(const QString& strPath);
	bool SaveDxfFile(const QString& strPath);
	bool NewDxfFile();
	bool CloseDxfFile();

private:
	//mainwindow指针
	QWidget* m_pMainWnd;
	
	std::unique_ptr<CDxfData> m_DxfData;
	std::unique_ptr<CDxfReader> m_DxfReader;

	//treeview的model
	CDxfTreeviewModel m_DxfTreeviewModel;
	//图层tableview的model
	CDxfLayerTableviewModel m_DxfLayerTableviewModel;
	//graphicview的model
	CDxfGraphicsScene m_DxfGraphicsScene;

	
public:
	
public:
	


signals:
	void signalRefreshTreeview(CDxfTreeviewModel* pModel);
	void signalRefreshTreeviewAfterRead();

 public slots:
	 
};