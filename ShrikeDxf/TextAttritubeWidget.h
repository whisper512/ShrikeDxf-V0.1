#pragma once
#include <QWidget>

#include "ui_TextAttritubeWidget.h"
#include "DxfStruct.h"

class CTextAttritubeWidget : public QWidget
{
    Q_OBJECT

public:
    CTextAttritubeWidget(QWidget *parent = nullptr);
    ~CTextAttritubeWidget();

private:
    Ui::TextAttritubeWidget ui;

    EntityText m_text;

public slots:
    void handleNoticeTextAttribute(EntityText polyline);



};