#pragma once
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QLabel>

#include "Manager/Tools.h"
#include "Manager/DxfGraphicsScene.h"
#include "Manager/DxfStruct.h"
#include "Ruler.h"

class DxfManager;

class GraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	GraphicsView();
	~GraphicsView();
	explicit GraphicsView(QWidget* mainWnd, DxfManager* pDxfManager = nullptr);

private:
	DxfManager* m_dxfManager;
	// 右键菜单
	QMenu* m_graphicsViewMenu;
	// 图元操作
	QMenu* m_graphicsOperateMenu;
	// 预览图元操作
	QMenu* m_graphicsPreviewMenu;

	// view的操作
	QAction* m_actionLockZoom;
	QAction* m_actionFilpX;
	QAction* m_actionFilpY;
	QAction* m_actionResetView;
	QAction* m_actionDrag;
	// 针对view中图元的操作
    QAction* m_actionDeleteEntity;
	QAction* m_actionCopyEntity;
	QAction* m_actionPasteEntity;
	QAction* m_actionCutEntity;
	QAction* m_actionXFile;
    QAction* m_actionYFile;
	// 针对view中预览图元的操作
	QAction* m_actionEndDrawing;
	// 尺子
	RulerH* m_rulerH;
	RulerV* m_rulerV;


	// 右键鼠标显示的针对view的操作
	// 锁定缩放
	bool m_lockZoom;
	// 沿x轴翻转
	bool m_filpAlongX;
	// 沿y轴翻转
	bool m_filpAlongY;
	// 显示鼠标位置十字线
	bool m_showPosCross;
	// 左键拖拽
	bool m_drag;

	// 右键鼠标显示的针对图元的操作 
	// 右键正在绘制预览的操作
	bool m_drawingPreview;
	// 开启复制图元中
	bool m_copyingEntity;
	// 右键正在选中图元
	bool m_selectingEntity;

	// 初始transform
	QTransform m_tranformInitial;
	// 初始场景矩阵
	QRectF m_rectInitialScene;
	QPoint m_lastPos;
	// 上次的场景矩阵
    QRectF m_rectLastScene;
	// 右键鼠标位置
    QPoint m_rightClickPos;
	

	void initMenu(QWidget* pParent);
	void initRuler();
	void initGraphicsViewAction();
	void initScene();
	void filpView();
	void updateRulers();

public:
	QWidget* m_mainWnd;

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
	// 左键按下（用于拉伸开始）
	void signalGraphicsViewLeftPressed(QPointF pos);
	// 左键释放（用于拉伸结束）
	void signalGraphicsViewLeftReleased(QPointF pos);
	// 结束预览preview信号
	void signalEndDrawingPreview();
	// 删除图元
	void signalDeleteEntity();
	// 复制图元
	void signalCopyEntity();
    // 剪切图元
	void signalCutEntity();

public slots:
	// 类外交互
	void handleRefreshGraphicsview(DxfGraphicsScene* pScene,bool bResetViewRect);
	// 正在复制图元
	void handlelCopyintEntity();
	// viewmenu动作
	void handleFilpAlongX(bool bChecked);
    void handleFilpAlongY(bool bChecked);
	void handleResetView();
	void handleDrag(bool bChecked);
	void showMenu(const QPoint& pos);
	void handleLockZoom(bool bChecked);
	// operate动作
	// 在绘制预览新图元
	void handleMouseStatusChanged(MouseStateInView mouseState);



protected:
	void wheelEvent(QWheelEvent* pEvent) override;
	void mouseMoveEvent(QMouseEvent* pEvent) override;
	void mousePressEvent(QMouseEvent* pEvent) override;
	void mouseReleaseEvent(QMouseEvent* pEvent) override;
	void resizeEvent(QResizeEvent* pEvent) override;
	void drawForeground(QPainter* pPainter, const QRectF& rect) override;
};
