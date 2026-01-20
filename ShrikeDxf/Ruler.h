#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class CRulerH : public QWidget
{
    Q_OBJECT

private:
    //尺子的左边起始
    double m_dOrigin;
    //缩放因子
    double m_dRulerZoom;
    //尺子起始值
    double m_dStart;
    //尺子结束值
    double m_dEnd;
    //刻度最小值
    double m_dStepMin;
    //刻度最大值
    double m_dStepMax;
    bool m_bOutOfRange;

public:
    explicit CRulerH(QWidget* parent = nullptr);
    ~CRulerH();
    void SetOrigin(double origin);
    void SetRulerZoom(double zoom);
    void SetRange(double min, double max);
    void SetStepRange(double min, double max);

    double CalculateStepSize() const;
    bool IsMajorTickMark(long long count, double step) const;
    QString FormatTickValue(double value, int decimalPlaces) const;
    int CalculateDecimalPlaces(double step) const;
protected:
    void paintEvent(QPaintEvent* event) override;

};

class CRulerV : public QWidget
{
    Q_OBJECT

private:
    //尺子的上方起始
    double m_dOrigin;
    //缩放因子
    double m_dRulerZoom;
    //尺子起始值
    double m_dStart;
    //尺子结束值
    double m_dEnd;
    //刻度最小值
    double m_dStepMin;
    //刻度最大值
    double m_dStepMax;
    bool m_bOutOfRange;

public:
    explicit CRulerV(QWidget* parent = nullptr);
    ~CRulerV();
    void SetOrigin(double origin);
    void SetRulerZoom(double zoom);
    void SetRange(double min, double max);
    void SetStepRange(double min, double max);

    double CalculateStepSize() const;
    bool IsMajorTickMark(long long count, double step) const;
    QString FormatTickValue(double value, int decimalPlaces) const;
    int CalculateDecimalPlaces(double step) const;
protected:
    void paintEvent(QPaintEvent* event) override;
};