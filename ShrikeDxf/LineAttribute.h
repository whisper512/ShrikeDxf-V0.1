#pragma once

#include <QWidget>
#include "ui_LineAttribute.h"
#include "DxfStruct.h"

class CLineAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    CLineAttributeWidget(QWidget* parent = nullptr);
    ~CLineAttributeWidget();

private:
    Ui::CLineAttribute ui;
    EntityLine m_line;
    bool m_bUpdating = false;

    void onAnySpinChanged();

signals:
    // 通知线属性改变
    void signalLineAttributeChanged(EntityLine line);

public slots:
    // 设置线属性
    void handleNoticeLineAttribute(EntityLine line);
};
