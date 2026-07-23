#pragma once

#include <QWidget>
#include "ui_LineAttribute.h"
#include "Manager/DxfStruct.h"

class LineAttributeWidget : public QWidget
{
    Q_OBJECT

public:
    LineAttributeWidget(QWidget* parent = nullptr);
    ~LineAttributeWidget();

private:
    Ui::CLineAttribute ui;
    EntityLine m_line;
    bool m_updating = false;

    void onAnySpinChanged();

signals:
    // 通知线属性改变
    void signalLineAttributeChanged(const EntityLine& line);

public slots:
    // 设置线属性
    void handleNoticeLineAttribute(EntityLine line);
};
