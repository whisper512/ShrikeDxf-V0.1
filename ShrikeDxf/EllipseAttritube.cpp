#include "EllipseAttritube.h"


CEllipseAttritubeWidget::CEllipseAttritubeWidget(QWidget * parent)
{
    ui.setupUi(this);
}

CEllipseAttritubeWidget::~CEllipseAttritubeWidget()
{
}

void CEllipseAttritubeWidget::handleNoticeEllipseAttribute(EntityEllipse ellipse)
{
	ui.doubleSpinBox_x->blockSignals(true);
    ui.doubleSpinBox_y->blockSignals(true);
	ui.doubleSpinBox_majorAxisX->blockSignals(true);
    ui.doubleSpinBox_majorAxisY->blockSignals(true);
    ui.doubleSpinBox_minorAxis->blockSignals(true);
    ui.doubleSpinBox_strartAngle->blockSignals(true);
    ui.doubleSpinBox_endAngle->blockSignals(true);

	ui.doubleSpinBox_x->setValue(ellipse.center.x());
    ui.doubleSpinBox_y->setValue(ellipse.center.y());
	ui.doubleSpinBox_majorAxisX->setValue(ellipse.majorAxisEndpoint.x());
    ui.doubleSpinBox_majorAxisY->setValue(ellipse.majorAxisEndpoint.y());
	ui.doubleSpinBox_minorAxis->setValue(ellipse.ratio);
	ui.doubleSpinBox_strartAngle->setValue(ellipse.startParam);
    ui.doubleSpinBox_endAngle->setValue(ellipse.endParam);

    ui.doubleSpinBox_x->blockSignals(false);
    ui.doubleSpinBox_y->blockSignals(false);
    ui.doubleSpinBox_majorAxisX->blockSignals(false);
    ui.doubleSpinBox_majorAxisY->blockSignals(false);
    ui.doubleSpinBox_minorAxis->blockSignals(false);
    ui.doubleSpinBox_strartAngle->blockSignals(false);
    ui.doubleSpinBox_endAngle->blockSignals(false);
}