#pragma once
#include <QWidget>

#include "ui_EllipseAttritubeWidget.h"
#include "Manager/DxfStruct.h"

class EllipseAttritubeWidget : public QWidget
{
    Q_OBJECT

public:
    EllipseAttritubeWidget(QWidget* parent = nullptr);
    ~EllipseAttritubeWidget();

private:
    Ui::CEllipesAttributeWidget ui;
    EntityEllipse m_ellipse;
    bool m_updating = false;

    void onSpinBoxChanged();

signals:
    void signalEllipseAttributeChanged(const EntityEllipse& ellipse);

public slots:
    void handleNoticeEllipseAttribute(EntityEllipse polyline);
};
