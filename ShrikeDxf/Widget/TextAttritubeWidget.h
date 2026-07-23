#pragma once
#include <QWidget>

#include "ui_TextAttritubeWidget.h"
#include "Manager/DxfStruct.h"

class TextAttritubeWidget : public QWidget
{
    Q_OBJECT

public:
    TextAttritubeWidget(QWidget* parent = nullptr);
    ~TextAttritubeWidget();

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
