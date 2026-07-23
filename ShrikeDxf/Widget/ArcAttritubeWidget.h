#pragma once

#include <QWidget>

#include "ui_ArcAttritubeWidget.h"
#include "Manager/DxfStruct.h"

class ArcAttritubeWidget : public QWidget
{
    Q_OBJECT

public:
    ArcAttritubeWidget(QWidget* parent = nullptr);
    ~ArcAttritubeWidget();

private:
    Ui::ArcAttritubeWidgetClass ui;
    EntityArc m_arc;
    bool m_updating = false;

    void onSpinBoxChanged();

signals:
    void signalArcAttributeChanged(const EntityArc& arc);

public slots:
    void handleNoticeArcAttribute(EntityArc arc);
};
