#pragma once

#include <QWidget>

#include "ui_LineAttribute.h"
#include "Primitive.h"


class CLineAttributeWidget : public QWidget
{
	Q_OBJECT

public:
	CLineAttributeWidget(QWidget *parent = nullptr);
	~CLineAttributeWidget();

private:
	Ui::CLineAttribute ui;
	
	Line m_line;

	void OnSpinBoxStartChanged();
    void OnSpinBoxEndChanged();
	void OnAngleSpinBoxChanged();
	void OnLengthSpinBoxChanged();
	void RefreshData();
	void LockOrReleaseStartSpainBoxSignal(bool lock);
    void LockOrReleaseEndSpainBoxSignal(bool lock);


signals:
		void signalLineAttributeChanged(Line line);

public slots:
	void handleNoticeLineAttribute(Line line);

};