#include "SplineAttributeWidget.h"

CSplineAttributeWidget::CSplineAttributeWidget(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    InitSplineAttributeWidget();

    // ComboBox 切换
    connect(ui.comboBox_tableType, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &CSplineAttributeWidget::OnTabChanged);

    // DoubleSpinBox
    connect(ui.doubleSpinBox_NormalVecX, &QDoubleSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_NormalVecY, &QDoubleSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_tgStartX, &QDoubleSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_tgStartY, &QDoubleSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_tgEndX, &QDoubleSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_tgEndY, &QDoubleSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_knotTolerance, &QDoubleSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_controlTolerance, &QDoubleSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);
    connect(ui.doubleSpinBox_fitTolerance, &QDoubleSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);

    // SpinBox
    connect(ui.spinBox_flags, &QSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);
    connect(ui.spinBox_degree, &QSpinBox::editingFinished, this, &CSplineAttributeWidget::OnValueChanged);

    // Table 编辑
    connect(ui.tableWidget, &QTableWidget::itemChanged,
        this, &CSplineAttributeWidget::OnTableValueChanged);
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

void CSplineAttributeWidget::OnValueChanged()
{
    if (m_bUpdating) return;

    m_pSpline.normalVec.setX(ui.doubleSpinBox_NormalVecX->value());
    m_pSpline.normalVec.setY(ui.doubleSpinBox_NormalVecY->value());
    m_pSpline.tgStart.setX(ui.doubleSpinBox_tgStartX->value());
    m_pSpline.tgStart.setY(ui.doubleSpinBox_tgStartY->value());
    m_pSpline.tgEnd.setX(ui.doubleSpinBox_tgEndX->value());
    m_pSpline.tgEnd.setY(ui.doubleSpinBox_tgEndY->value());
    m_pSpline.flags = ui.spinBox_flags->value();
    m_pSpline.degree = ui.spinBox_degree->value();
    m_pSpline.knotTolerance = ui.doubleSpinBox_knotTolerance->value();
    m_pSpline.controlTolerance = ui.doubleSpinBox_controlTolerance->value();
    m_pSpline.fitTolerance = ui.doubleSpinBox_fitTolerance->value();

    emit signalSplineAttributeChanged(m_pSpline);
}

void CSplineAttributeWidget::OnTableValueChanged()
{
    if (m_bUpdating) return;

    int index = ui.comboBox_tableType->currentIndex();
    int rows = ui.tableWidget->rowCount();

    if (index == 0)     // knots
    {
        m_pSpline.knots.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            QTableWidgetItem* item = ui.tableWidget->item(i, 0);
            m_pSpline.knots[i] = item ? item->text().toDouble() : 0.0;
        }
    }
    else if (index == 1)    // weights
    {
        m_pSpline.weights.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            QTableWidgetItem* item = ui.tableWidget->item(i, 0);
            m_pSpline.weights[i] = item ? item->text().toDouble() : 0.0;
        }
    }
    else if (index == 2)    // controlPoints
    {
        m_pSpline.controlPoints.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            QTableWidgetItem* itemX = ui.tableWidget->item(i, 0);
            QTableWidgetItem* itemY = ui.tableWidget->item(i, 1);
            double x = itemX ? itemX->text().toDouble() : 0.0;
            double y = itemY ? itemY->text().toDouble() : 0.0;
            m_pSpline.controlPoints[i] = Vertex3D(x, y);
        }
    }
    else if (index == 3)    // fitPoints
    {
        m_pSpline.fitPoints.resize(rows);
        for (int i = 0; i < rows; i++)
        {
            QTableWidgetItem* itemX = ui.tableWidget->item(i, 0);
            QTableWidgetItem* itemY = ui.tableWidget->item(i, 1);
            double x = itemX ? itemX->text().toDouble() : 0.0;
            double y = itemY ? itemY->text().toDouble() : 0.0;
            m_pSpline.fitPoints[i] = Vertex3D(x, y);
        }
    }

    emit signalSplineAttributeChanged(m_pSpline);
}

void CSplineAttributeWidget::OnTabChanged(int index)
{
    if (m_bUpdating) return;
    RefreshTable();
}

void CSplineAttributeWidget::RefreshTable()
{
    m_bUpdating = true;

    int index = ui.comboBox_tableType->currentIndex();
    ui.tableWidget->clear();

    if (index == 0)     // knots
    {
        QStringList header;
        header << "knotsVal";
        ui.tableWidget->setColumnCount(1);
        ui.tableWidget->setHorizontalHeaderLabels(header);
        ui.tableWidget->setRowCount((int)m_pSpline.knots.size());
        for (int i = 0; i < (int)m_pSpline.knots.size(); i++)
            ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(m_pSpline.knots[i])));
    }
    else if (index == 1)    // weights
    {
        QStringList header;
        header << "weightsVal";
        ui.tableWidget->setColumnCount(1);
        ui.tableWidget->setHorizontalHeaderLabels(header);
        ui.tableWidget->setRowCount((int)m_pSpline.weights.size());
        for (int i = 0; i < (int)m_pSpline.weights.size(); i++)
            ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(m_pSpline.weights[i])));
    }
    else if (index == 2)    // controlPoints
    {
        QStringList header;
        header << "controlPointsX" << "controlPointsY";
        ui.tableWidget->setColumnCount(2);
        ui.tableWidget->setHorizontalHeaderLabels(header);
        ui.tableWidget->setRowCount((int)m_pSpline.controlPoints.size());
        for (int i = 0; i < (int)m_pSpline.controlPoints.size(); i++)
        {
            ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(m_pSpline.controlPoints[i].x())));
            ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(m_pSpline.controlPoints[i].y())));
        }
    }
    else if (index == 3)    // fitPoints
    {
        QStringList header;
        header << "fitPointsX" << "fitPointsY";
        ui.tableWidget->setColumnCount(2);
        ui.tableWidget->setHorizontalHeaderLabels(header);
        ui.tableWidget->setRowCount((int)m_pSpline.fitPoints.size());
        for (int i = 0; i < (int)m_pSpline.fitPoints.size(); i++)
        {
            ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(m_pSpline.fitPoints[i].x())));
            ui.tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(m_pSpline.fitPoints[i].y())));
        }
    }

    ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_bUpdating = false;
}

void CSplineAttributeWidget::handleNoticeSplineAttribute(EntitySpline spline)
{
    m_bUpdating = true;

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

    RefreshTable();
}
