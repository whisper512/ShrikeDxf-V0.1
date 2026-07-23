#include "SplineAttributeWidget.h"

SplineAttributeWidget::SplineAttributeWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    initSplineAttributeWidget();

    // ComboBox 切换
    connect(ui.comboBox_tableType, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &SplineAttributeWidget::onTabChanged);

    // DoubleSpinBox
    connect(ui.doubleSpinBox_NormalVecX, &QDoubleSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);
    connect(ui.doubleSpinBox_NormalVecY, &QDoubleSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);
    connect(ui.doubleSpinBox_tgStartX, &QDoubleSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);
    connect(ui.doubleSpinBox_tgStartY, &QDoubleSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);
    connect(ui.doubleSpinBox_tgEndX, &QDoubleSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);
    connect(ui.doubleSpinBox_tgEndY, &QDoubleSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);
    connect(ui.doubleSpinBox_knotTolerance, &QDoubleSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);
    connect(ui.doubleSpinBox_controlTolerance, &QDoubleSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);
    connect(ui.doubleSpinBox_fitTolerance, &QDoubleSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);

    // SpinBox
    connect(ui.spinBox_flags, &QSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);
    connect(ui.spinBox_degree, &QSpinBox::editingFinished, this, &SplineAttributeWidget::onValueChanged);

    // Table 编辑
    connect(ui.tableWidget, &QTableWidget::itemChanged,
        this, &SplineAttributeWidget::onTableValueChanged);
}

SplineAttributeWidget::~SplineAttributeWidget()
{
}

void SplineAttributeWidget::initSplineAttributeWidget()
{
    QTimer::singleShot(0, this, [this]() {
        ui.comboBox_tableType->addItem("knots");
        ui.comboBox_tableType->addItem("weights");
        ui.comboBox_tableType->addItem("controlPoints");
        ui.comboBox_tableType->addItem("fitPoints");
        });
}

void SplineAttributeWidget::onValueChanged()
{
    if (m_updating) return;

    m_spline.normalVec.setX(ui.doubleSpinBox_NormalVecX->value());
    m_spline.normalVec.setY(ui.doubleSpinBox_NormalVecY->value());
    m_spline.tgStart.setX(ui.doubleSpinBox_tgStartX->value());
    m_spline.tgStart.setY(ui.doubleSpinBox_tgStartY->value());
    m_spline.tgEnd.setX(ui.doubleSpinBox_tgEndX->value());
    m_spline.tgEnd.setY(ui.doubleSpinBox_tgEndY->value());
    m_spline.flags = ui.spinBox_flags->value();
    m_spline.degree = ui.spinBox_degree->value();
    m_spline.knotTolerance = ui.doubleSpinBox_knotTolerance->value();
    m_spline.controlTolerance = ui.doubleSpinBox_controlTolerance->value();
    m_spline.fitTolerance = ui.doubleSpinBox_fitTolerance->value();

    emit signalSplineAttributeChanged(m_spline);
}

void SplineAttributeWidget::onTableValueChanged()
{
    if (m_updating) return;

    int index = ui.comboBox_tableType->currentIndex();
    int rows = ui.tableWidget->rowCount();

    if (index == 0)     // knots
    {
        m_spline.knots.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            QTableWidgetItem* item = ui.tableWidget->item(i, 0);
            m_spline.knots[i] = item ? item->text().toDouble() : 0.0;
        }
    }
    else if (index == 1)    // weights
    {
        m_spline.weights.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            QTableWidgetItem* item = ui.tableWidget->item(i, 0);
            m_spline.weights[i] = item ? item->text().toDouble() : 0.0;
        }
    }
    else if (index == 2)    // controlPoints
    {
        m_spline.controlPoints.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            QTableWidgetItem* itemX = ui.tableWidget->item(i, 0);
            QTableWidgetItem* itemY = ui.tableWidget->item(i, 1);
            double x = itemX ? itemX->text().toDouble() : 0.0;
            double y = itemY ? itemY->text().toDouble() : 0.0;
            m_spline.controlPoints[i] = Vertex3D(x, y);
        }
    }
    else if (index == 3)    // fitPoints
    {
        m_spline.fitPoints.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            QTableWidgetItem* itemX = ui.tableWidget->item(i, 0);
            QTableWidgetItem* itemY = ui.tableWidget->item(i, 1);
            double x = itemX ? itemX->text().toDouble() : 0.0;
            double y = itemY ? itemY->text().toDouble() : 0.0;
            m_spline.fitPoints[i] = Vertex3D(x, y);
        }
    }

    emit signalSplineAttributeChanged(m_spline);
}

void SplineAttributeWidget::onTabChanged(int index)
{
    if (m_updating) return;
    refreshTable();
}

void SplineAttributeWidget::refreshTable()
{
    m_updating = true;

    int index = ui.comboBox_tableType->currentIndex();
    ui.tableWidget->clear();

    if (index == 0)     // knots
    {
        QStringList header;
        header << "knotsVal";
        ui.tableWidget->setColumnCount(1);
        ui.tableWidget->setHorizontalHeaderLabels(header);
        ui.tableWidget->setRowCount((int)m_spline.knots.size());
        for (int i = 0; i < (int)m_spline.knots.size(); i++)
            ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(m_spline.knots[i])));
    }
    else if (index == 1)    // weights
    {
        QStringList header;
        header << "weightsVal";
        ui.tableWidget->setColumnCount(1);
        ui.tableWidget->setHorizontalHeaderLabels(header);
        ui.tableWidget->setRowCount((int)m_spline.weights.size());
        for (int i = 0; i < (int)m_spline.weights.size(); i++)
            ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(m_spline.weights[i])));
    }
    else if (index == 2)    // controlPoints
    {
        QStringList header;
        header << "controlPointsX" << "controlPointsY";
        ui.tableWidget->setColumnCount(2);
        ui.tableWidget->setHorizontalHeaderLabels(header);
        ui.tableWidget->setRowCount((int)m_spline.controlPoints.size());
        for (int i = 0; i < (int)m_spline.controlPoints.size(); i++)
        {
            ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(m_spline.controlPoints[i].x())));
            ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(m_spline.controlPoints[i].y())));
        }
    }
    else if (index == 3)    // fitPoints
    {
        QStringList header;
        header << "fitPointsX" << "fitPointsY";
        ui.tableWidget->setColumnCount(2);
        ui.tableWidget->setHorizontalHeaderLabels(header);
        ui.tableWidget->setRowCount((int)m_spline.fitPoints.size());
        for (int i = 0; i < (int)m_spline.fitPoints.size(); i++)
        {
            ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(m_spline.fitPoints[i].x())));
            ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(m_spline.fitPoints[i].y())));
        }
    }

    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_updating = false;
}

void SplineAttributeWidget::handleNoticeSplineAttribute(EntitySpline spline)
{
    m_updating = true;

    m_spline = spline;
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

    refreshTable();
}
