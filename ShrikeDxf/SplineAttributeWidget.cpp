#include "SplineAttributeWidget.h"

CSplineAttributeWidget::CSplineAttributeWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	InitSplineAttributeWidget();

	connect(ui.comboBox_tableType, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &CSplineAttributeWidget::OnTabChanged);
}

CSplineAttributeWidget::~CSplineAttributeWidget()
{
}

void CSplineAttributeWidget::InitSplineAttributeWidget()
{
	QTimer::singleShot(0, this, [this]() {
		ui.comboBox_tableType->addItem("knots");
		ui.comboBox_tableType->addItem("weights");
        ui.comboBox_tableType->addItem("controlPoints");
        ui.comboBox_tableType->addItem("fitPoints");
	});
}

void CSplineAttributeWidget::OnTabChanged(int index)
{
	QStringList header;

	if (index == 0)
	{
		//knots
		header << "knotsVal";
		ui.tableWidget->setColumnCount(1);
		ui.tableWidget->setHorizontalHeaderLabels(header);
		ui.tableWidget->setRowCount((int)m_pSpline.knots.size()); // 添加设置行数
		if (m_pSpline.knots.size() > 0)
		{
			for (int i = 0; i < m_pSpline.knots.size(); i++)
			{
				QTableWidgetItem* itemVal = new QTableWidgetItem(QString::number(m_pSpline.knots[i]));
				ui.tableWidget->setItem(i, 0, itemVal);
			}
		}
	}
	else if (index == 1)
	{
		//weights
		header << "weightsVal";
		ui.tableWidget->setColumnCount(1);
		ui.tableWidget->setHorizontalHeaderLabels(header);
		ui.tableWidget->setRowCount((int)m_pSpline.weights.size()); // 添加设置行数
		if (m_pSpline.weights.size() > 0)
		{
			for (int i = 0; i < m_pSpline.weights.size(); i++)
			{
				QTableWidgetItem* itemVal = new QTableWidgetItem(QString::number(m_pSpline.weights[i]));
				ui.tableWidget->setItem(i, 0, itemVal);
			}
		}
	}
	else if (index == 2)
	{
		//controlPoints
		header << "controlPointsX" << "controlPointsY";
		ui.tableWidget->setColumnCount(2);
		ui.tableWidget->setHorizontalHeaderLabels(header);
		ui.tableWidget->setRowCount((int)m_pSpline.controlPoints.size()); // 添加设置行数
		if (m_pSpline.controlPoints.size() > 0)
		{
			for (int i = 0; i < m_pSpline.controlPoints.size(); i++)
			{
				QTableWidgetItem* itemValX = new QTableWidgetItem(QString::number(m_pSpline.controlPoints[i].x()));
				QTableWidgetItem* itemValY = new QTableWidgetItem(QString::number(m_pSpline.controlPoints[i].y()));
				ui.tableWidget->setItem(i, 0, itemValX);
				ui.tableWidget->setItem(i, 1, itemValY);
			}
		}
	}
	else if (index == 3)
	{
		//fitPoints
		header << "fitPointsX" << "fitPointsY";
		ui.tableWidget->setColumnCount(2);
		ui.tableWidget->setRowCount((int)m_pSpline.fitPoints.size());
		ui.tableWidget->setHorizontalHeaderLabels(header);
		if (m_pSpline.fitPoints.size() > 0)
		{
			for (int i = 0; i < m_pSpline.fitPoints.size(); i++)
			{
				QTableWidgetItem* itemValX = new QTableWidgetItem(QString::number(m_pSpline.fitPoints[i].x()));
				QTableWidgetItem* itemValY = new QTableWidgetItem(QString::number(m_pSpline.fitPoints[i].y()));
				ui.tableWidget->setItem(i, 0, itemValX);
				ui.tableWidget->setItem(i, 1, itemValY);
			}
		}
	}

	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}


void CSplineAttributeWidget::handleNoticeSplineAttribute(EntitySpline spline)
{
	ui.doubleSpinBox_NormalVecX->blockSignals(true);
	ui.doubleSpinBox_NormalVecY->blockSignals(true);
	ui.doubleSpinBox_tgStartX->blockSignals(true);
	ui.doubleSpinBox_tgStartY->blockSignals(true);
	ui.doubleSpinBox_tgEndX->blockSignals(true);
	ui.doubleSpinBox_tgEndY->blockSignals(true);
	ui.spinBox_flags->blockSignals(true);
	ui.spinBox_degree->blockSignals(true);
	ui.doubleSpinBox_knotTolerance->blockSignals(true);
	ui.doubleSpinBox_controlTolerance->blockSignals(true);
	ui.doubleSpinBox_fitTolerance->blockSignals(true);

	m_pSpline = spline;
	ui.doubleSpinBox_NormalVecX->setValue(spline.normalVec.x());
    ui.doubleSpinBox_NormalVecY->setValue(spline.normalVec.y());
	ui.doubleSpinBox_tgStartX->setValue(spline.tgStart.x());
    ui.doubleSpinBox_tgStartY->setValue(spline.tgStart.y());
	ui.doubleSpinBox_tgEndX->setValue(spline.tgEnd.x());
    ui.doubleSpinBox_tgEndY->setValue(spline.tgEnd.y());
	ui.spinBox_flags->setValue(spline.flags);
    ui.spinBox_degree->setValue(spline.degree);
	ui.doubleSpinBox_knotTolerance->setValue(spline.knotTolerance);
	ui.doubleSpinBox_controlTolerance->setValue(spline.controlTolerance);
	ui.doubleSpinBox_fitTolerance->setValue(spline.fitTolerance);

	ui.doubleSpinBox_NormalVecX->blockSignals(false);
	ui.doubleSpinBox_NormalVecY->blockSignals(false);
	ui.doubleSpinBox_tgStartX->blockSignals(false);
	ui.doubleSpinBox_tgStartY->blockSignals(false);
	ui.doubleSpinBox_tgEndX->blockSignals(false);
	ui.doubleSpinBox_tgEndY->blockSignals(false);
	ui.spinBox_flags->blockSignals(false);
	ui.spinBox_degree->blockSignals(false);
	ui.doubleSpinBox_knotTolerance->blockSignals(false);
	ui.doubleSpinBox_controlTolerance->blockSignals(false);
	ui.doubleSpinBox_fitTolerance->blockSignals(false);

	OnTabChanged(0);
}