#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class RulerH : public QWidget
{
    Q_OBJECT

private:
    //尺子的左边起始
    double m_origin;
    //缩放因子
    double m_rulerZoom;
    //尺子起始值
    double m_start;
    //尺子结束值
    double m_end;
    //刻度最小值
    double m_stepMin;
    //刻度最大值
    double m_stepMax;
    //graphicsview中scene的鼠标位置
    double m_mouseXPos;

public:
    explicit RulerH(QWidget* parent = nullptr);
    ~RulerH();
    void setOrigin(double origin);
    void setRulerZoom(double zoom);
    void setRange(double min, double max);
    void setMousePos(double pos);
    //计算尺子间隔
    double calculateStepSize() const;
    //判断是否为主刻度
    bool isMajorTickMark(long long count, double step) const;
    QString formatTickValue(double value, int decimalPlaces) const;
    int calculateDecimalPlaces(double step) const;
protected:
    void paintEvent(QPaintEvent* event) override;

};

class RulerV : public QWidget
{
    Q_OBJECT

private:
    //尺子的上方起始
    double m_origin;
    //缩放因子
    double m_rulerZoom;
    //尺子起始值
    double m_start;
    //尺子结束值
    double m_end;
    //刻度最小值
    double m_stepMin;
    //刻度最大值
    double m_stepMax;
    double m_mouseYPos;

public:
    explicit RulerV(QWidget* parent = nullptr);
    ~RulerV();
    void setOrigin(double origin);
    void setRulerZoom(double zoom);
    void setRange(double min, double max);
    void setMousePos(double pos);

    double calculateStepSize() const;
    bool isMajorTickMark(long long count, double step) const;
    QString formatTickValue(double value, int decimalPlaces) const;
    int calculateDecimalPlaces(double step) const;
protected:
    void paintEvent(QPaintEvent* event) override;
};