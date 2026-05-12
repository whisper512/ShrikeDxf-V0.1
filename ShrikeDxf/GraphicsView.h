#pragma once

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QLabel>

#include "Tools.h"
#include "DxfGraphicsScene.h"
#include "DxfStruct.h"
#include "Ruler.h"

class CGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	CGraphicsView();
	~CGraphicsView();

	explicit CGraphicsView(QWidget* pMainwnd);

private:
	// 右键菜单
	QMenu* m_pGraphicsViewMenu;
	QMenu* m_pGraphicsOperateMenu;
	QMenu* m_pGraphicsPreviewMenu;

	// view的操作
	QAction* m_pActionLockZoom;
	QAction* m_pActionFilpX;
	QAction* m_pActionFilpY;
	QAction* m_pActionResetView;
	QAction* m_pActionDrag;
	// 针对view中图元的操作
	QAction* m_pActionPasteEntity;
	// 针对view中预览图元的操作
	QAction* m_pActionEndDrawing;

	CRulerH* m_pRulerH;
	CRulerV* m_pRulerV;


	// 右键鼠标显示的针对view的操作*****
	// 锁定缩放
	bool m_bLockZoom;
	// 沿x轴翻转
	bool m_bFilpAlongX;
	// 沿y轴翻转
	bool m_bFilpAlongY;
	// 显示鼠标位置十字线
	bool m_bShowPosCross;
	// 左键拖拽
	bool m_bDrag;

	// 右键鼠标显示的针对图元的操作 
	// 开启复制图元中
	bool m_bCopyingEntity;

	// 右键正在绘制预览的操作
	bool m_bDrawingPreview;


	// 初始transform
	QTransform m_tranformInitial;
	// 初始场景矩阵
	QRectF m_rectInitialScene;
	QPoint m_pointLastPos;
	// 上次的场景矩阵
    QRectF m_rectLastScene;
	// 右键鼠标位置
    QPoint m_pointRightClickPos;
	

	void InitMenu(QWidget* pParent);
	void InitRuler();
	void InitGraphicsViewAction();
	void InitScene();
	void FilpView();
	void UpdateRulers();

public:
	QWidget* m_pMainWnd;

signals:
	// 粘贴信号
	void signalPaste(QPointF pos);
	// 鼠标位置,用来显示在label
	void signalMousePosString(QString strPos);
	// 鼠标位置
	void signalMousePos(QPointF pos);
	// 鼠标移动
	void signalGraphicsViewMouseMove(QPointF pos);
	// 鼠标左键点击
	void signalGraphicsViewLeftCLick(QPointF pos);
	// 鼠标右键点击
	void signalGraphicsViewRightClick(QPointF pos);
	// preview信号
	void signalEndDrawingPreview();

public slots:
	// 类外交互
	void handleRefreshGraphicsview(CDxfGraphicsScene* pScene,bool bResetViewRect);
	// 正在复制图元
	void handlelCopyintEntity();
	// viewmenu动作
	void handleFilpAlongX(bool bChecked);
    void handleFilpAlongY(bool bChecked);
	void handleResetView();
	void handleDrag(bool bChecked);
	void ShowMenu(const QPoint& pos);
	void handleLockZoom(bool bChecked);
	// operate动作
	void handlePasteEntity();
	// 在绘制预览新图元
	void handleMouseStatusChanged(enumMouseStateInView mouseState);



protected:
	void wheelEvent(QWheelEvent* pEvent) override;
	void mouseMoveEvent(QMouseEvent* pEvent) override;
	void mousePressEvent(QMouseEvent* pEvent) override;
	void mouseReleaseEvent(QMouseEvent* pEvent) override;
	void resizeEvent(QResizeEvent* pEvent) override;
	void drawForeground(QPainter* pPainter, const QRectF& rect) override;
};
