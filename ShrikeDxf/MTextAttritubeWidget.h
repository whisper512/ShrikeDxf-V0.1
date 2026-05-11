#pragma once
#include <QWidget>

#include "ui_MTextAttritubeWidget.h"
#include "DxfStruct.h"

class CMTextAttritubeWidget : public QWidget
{
    Q_OBJECT

public:
    CMTextAttritubeWidget(QWidget* parent = nullptr);
    ~CMTextAttritubeWidget();

private:
    Ui::MTextAttritubeWidget ui;
    EntityMText m_Mtext;
    bool m_bUpdating = false;

    void OnValueChanged();

signals:
    void signalMTextAttributeChanged(const EntityMText& Mtext);

public slots:
    void handleNoticeMTextAttribute(EntityMText Mtext);
};
