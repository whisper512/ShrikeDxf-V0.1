#pragma once
#include <QWidget>

#include "ui_MTextAttritubeWidget.h"
#include "DxfStruct.h"

class CMTextAttritubeWidget : public QWidget
{
public:
	CMTextAttritubeWidget(QWidget *parent = nullptr);
    ~CMTextAttritubeWidget();

private:
	Ui::MTextAttritubeWidget ui;

	EntityMText m_Mtext;

public slots:
	void handleNoticeMTextAttribute(EntityMText Mtext);
};

