#include "Ruler.h"
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QFont>
#include <QTransform>
#include <QtMath>

CRulerH::CRulerH(QWidget* parent) : QWidget(parent),
    m_dOrigin(0.0),
    m_dRulerZoom(1.0),
    m_dStart(0.0),
    m_dEnd(5000.0),
    m_dStepMin(1.0),
    m_dStepMax(500.0),
    m_bOutOfRange(false)
{
    setFixedHeight(20);
}

void CRulerH::SetOrigin(double origin)
{
    m_dOrigin = origin;
    update();
}

void CRulerH::SetRulerZoom(double zoom)
{
    m_dRulerZoom = zoom;
    update();
}

void CRulerH::SetRange(double min, double max)
{
    m_dStart = min;
    m_dEnd = max;
    update();
}

void CRulerH::SetStepRange(double min, double max)
{
    m_dStepMin = min;
    m_dStepMax = max;
    update();
}

double CRulerH::CalculateStepSize() const
{
    if (m_dRulerZoom <= 0 || width() <= 0)
    {
        return 1.0;
    }

    // 计算视图范围
    double dViewRange = width() / m_dRulerZoom;

    // 计算目标步长，确保最多30个主刻度
    double TargetStep = dViewRange / 30.0;

    // 使用对数计算最接近的标准步长
    double logStep = std::log10(TargetStep);
    double power = std::floor(logStep);
    double base = std::pow(10.0, power);

    double NormalizedStep = TargetStep / base;

    // 选择最接近的标准步长
    if (NormalizedStep >= 5.0) return 5.0 * base;
    else if (NormalizedStep >= 2.0) return 2.0 * base;
    else return 1.0 * base;

}

int CRulerH::CalculateDecimalPlaces(double step) const
{
    if (step >= 1.0) return 0;

    // 计算需要的小数位数
    double logStep = std::log10(step);
    int places = static_cast<int>(-std::floor(logStep));

    // 限制小数位数
    return qMin(places, 6);
}

bool CRulerH::IsMajorTickMark(long long count, double step) const
{
    return (count % 10 == 0);
}

QString CRulerH::FormatTickValue(double value, int decimalPlaces) const
{
    // 格式化刻度值，避免科学计数法
    QString text = QString::number(value, 'f', decimalPlaces);

    // 移除末尾多余的0
    if (decimalPlaces > 0) 
    {
        while (text.endsWith('0')) 
        {
            text.chop(1);
        }
        if (text.endsWith('.')) 
        {
            text.chop(1);
        }
    }
    return text;
}

void CRulerH::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#e0e0e0"));
    
    // 计算刻度间隔
    double step = CalculateStepSize();

    // 计算需要的小数位数
    int decimalPlaces = CalculateDecimalPlaces(step);

    // 计算第一个刻度的位置
    double startValue = std::ceil(m_dStart / step) * step;

    // 绘制刻度
    painter.setPen(Qt::black);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);;

    for (double value = startValue; value <= m_dEnd; value += step)
    {
        // 将场景坐标转换为视图坐标
        double xPos = (value - m_dOrigin) * m_dRulerZoom;

        // 只绘制在视图范围内的刻度
        if (xPos < 0 || xPos > width())
            continue;

        
        long long count = qRound64(value / step);
        bool isMajor = IsMajorTickMark(count, step);

        // 绘制刻度线，从上往下
        int tickHeight = isMajor ? 8 : 3;
        painter.drawLine(QPointF(xPos, 0), QPointF(xPos, tickHeight));

        // 绘制刻度值（仅主刻度）
        if (isMajor)
        {
            QString text = FormatTickValue(value, decimalPlaces);
            QRectF textRect = painter.boundingRect(QRectF(), Qt::AlignLeft | Qt::AlignTop, text);
            textRect.moveCenter(QPointF(xPos + 5, height()-5 ));
            painter.drawText(textRect, Qt::AlignCenter, text);
        }
    }
}
