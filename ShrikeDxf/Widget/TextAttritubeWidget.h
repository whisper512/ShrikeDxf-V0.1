#pragma once
#include <QWidget>

#include "ui_TextAttritubeWidget.h"
#include "Manager/DxfStruct.h"

class CTextAttritubeWidget : public QWidget
{
    Q_OBJECT

public:
    CTextAttritubeWidget(QWidget* parent = nullptr);
    ~CTextAttritubeWidget();

private:
    Ui::TextAttritubeWidget ui;
    EntityText m_text;
    bool m_updating = false;

    void onValueChanged();

signals:
    void signalTextAttributeChanged(const EntityText& text);

public slots:
    void handleNoticeTextAttribute(EntityText text);
};
