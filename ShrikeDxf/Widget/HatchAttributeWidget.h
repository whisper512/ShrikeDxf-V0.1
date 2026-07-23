#pragma once
#include <QWidget>

#include "ui_HatchAttributeWidget.h"
#include "Manager/DxfStruct.h"

class HatchAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    HatchAttributeWidget(QWidget* parent = nullptr);
    ~HatchAttributeWidget();

private:
    Ui::HatchAttributeWidget ui;
    EntityHatch m_hatch;
    bool m_updating = false;

    void onValueChanged();

signals:
    void signalHatchAtttributeChanged(const EntityHatch& hatch);

public slots:
    void handleNoticeHatchAtttribute(EntityHatch hatch);
};
