#pragma once
#include <QWidget>

#include "ui_TextAttritubeWidget.h"
#include "DxfStruct.h"

class CTextAttritubeWidget : public QWidget
{
    Q_OBJECT

public:
    CTextAttritubeWidget(QWidget* parent = nullptr);
    ~CTextAttritubeWidget();

private:
    Ui::TextAttritubeWidget ui;
    EntityText m_text;
    bool m_bUpdating = false;

    void OnValueChanged();

signals:
    void signalTextAttributeChanged(EntityText text);

public slots:
    void handleNoticeTextAttribute(EntityText text);
};
