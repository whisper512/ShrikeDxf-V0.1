#pragma once

#include <QWidget>

#include "ui_LineAttribute.h"
#include "DxfStruct.h"



class CLineAttributeWidget : public QWidget
{
	Q_OBJECT

public:
	CLineAttributeWidget(QWidget *parent = nullptr);
	~CLineAttributeWidget();

private:
	Ui::CLineAttribute ui;
	
	EntityLine m_line;

	void OnSpinBoxStartChanged();
    void OnSpinBoxEndChanged();
	void OnAngleSpinBoxChanged();
	void OnLengthSpinBoxChanged();
	void RefreshData();
	void LockOrReleaseStartSpainBoxSignal(bool lock);
    void LockOrReleaseEndSpainBoxSignal(bool lock);


signals:
		void signalLineAttributeChanged(EntityLine line);

public slots:
	void handleNoticeLineAttribute(EntityLine line);

};