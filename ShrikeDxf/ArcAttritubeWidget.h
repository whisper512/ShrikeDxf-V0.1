#pragma once

#include <QWidget>

#include "ui_ArcAttritubeWidget.h"
#include "DxfStruct.h"

class CArcAttritubeWidget : public QWidget
{
    Q_OBJECT

public:
    CArcAttritubeWidget(QWidget* parent = nullptr);
    ~CArcAttritubeWidget();

private:
    Ui::ArcAttritubeWidgetClass ui;
    EntityArc m_arc;
    bool m_bUpdating = false;

    void OnSpinBoxChanged();

signals:
    void signalArcAttributeChanged(const EntityArc& arc);

public slots:
    void handleNoticeArcAttribute(EntityArc arc);
};
