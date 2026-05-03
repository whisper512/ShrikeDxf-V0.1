#pragma once
#include <QWidget>

#include "ui_EllipseAttritubeWidget.h"
#include "DxfStruct.h"

class CEllipseAttritubeWidget : public QWidget
{
    Q_OBJECT
    
public:
    CEllipseAttritubeWidget(QWidget *parent = nullptr);
    ~CEllipseAttritubeWidget();

private:
    Ui::CEllipesAttributeWidget ui;
    EntityEllipse m_ellipse;


public slots:
    void handleNoticeEllipseAttribute(EntityEllipse polyline);
};
