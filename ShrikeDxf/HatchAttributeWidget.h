#pragma once
#include <QWidget>

#include "ui_HatchAttributeWidget.h"
#include "DxfStruct.h"

class CHatchAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    CHatchAttributeWidget(QWidget* parent = nullptr);
    ~CHatchAttributeWidget();

private:
    Ui::HatchAttributeWidget ui;
    EntityHatch m_hatch;
    bool m_bUpdating = false;

    void OnValueChanged();

signals:
    void signalHatchAtttributeChanged(EntityHatch hatch);

public slots:
    void handleNoticeHatchAtttribute(EntityHatch hatch);
};
