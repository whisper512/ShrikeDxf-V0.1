#pragma once
#include <QWidget>

#include "ui_MTextAttritubeWidget.h"
#include "Manager/DxfStruct.h"

class MTextAttritubeWidget : public QWidget
{
    Q_OBJECT

public:
    MTextAttritubeWidget(QWidget* parent = nullptr);
    ~MTextAttritubeWidget();

private:
    Ui::MTextAttritubeWidget ui;
    EntityMText m_Mtext;
    bool m_updating = false;

    void onValueChanged();

signals:
    void signalMTextAttributeChanged(const EntityMText& Mtext);

public slots:
    void handleNoticeMTextAttribute(EntityMText Mtext);
};
