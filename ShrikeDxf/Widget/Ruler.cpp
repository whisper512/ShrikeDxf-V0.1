#include "Ruler.h"
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QFont>
#include <QTransform>
#include <QtMath>

RulerH::RulerH(QWidget* parent) : QWidget(parent),
    m_origin(0.0),
    m_rulerZoom(1.0),
    m_start(0.0),
    m_end(5000.0),
    m_stepMin(1.0),
    m_stepMax(500.0)
{
}

RulerH::~RulerH()
{
}

RulerV::RulerV(QWidget* parent) : QWidget(parent),
    m_origin(0.0),
    m_rulerZoom(1.0),
    m_start(0.0),
    m_end(5000.0),
    m_stepMin(1.0),
    m_stepMax(500.0)
{
}

RulerV::~RulerV()
{
}

void RulerH::setOrigin(double origin)
{
    m_origin = origin;
    update();
}

void RulerV::setOrigin(double origin)
{
    m_origin = origin;
    update();
}

void RulerH::setRulerZoom(double zoom)
{
    m_rulerZoom = zoom;
    update();
}

void RulerV::setRulerZoom(double zoom)
{
    //view的Y轴翻转
    m_rulerZoom = zoom;
    update();
}

void RulerH::setRange(double min, double max)
{
    m_start = min;
    m_end = max;
    update();
}

void RulerV::setRange(double min, double max)
{
    m_start = min;
    m_end = max;
    update();
}

void RulerH::setMousePos(double pos)
{
    m_mouseXPos = pos;
    update();
}

void RulerV::setMousePos(double pos)
{
    m_mouseYPos = pos;
    update();
}

double RulerH::calculateStepSize() const
{
    if (width() <= 0)
    {
        return 1.0;
    }

    // 视图范围
    double dViewRange = width() / m_rulerZoom;
    double TargetStep = dViewRange / 35.0;

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

double RulerV::calculateStepSize() const
{
    if (height() <= 0)
    {
        return 1.0;
    }
    double dViewRange = height() / (-m_rulerZoom);
    double TargetStep = dViewRange / 35.0;
    double logStep = std::log10(TargetStep);
    double power = std::floor(logStep);
    double base = std::pow(10.0, power);

    double NormalizedStep = TargetStep / base;

    // 选择最接近的标准步长
    if (NormalizedStep >= 5.0) return 5.0 * base;
    else if (NormalizedStep >= 2.0) return 2.0 * base;
    else return 1.0 * base;
}

int RulerH::calculateDecimalPlaces(double step) const
{
    if (step >= 1.0) return 0;
    // 计算需要的小数位数
    double logStep = std::log10(step);
    int places = static_cast<int>(-std::floor(logStep));
    // 限制小数位数
    return qMin(places, 6);
}

int RulerV::calculateDecimalPlaces(double step) const
{
    if (step >= 1.0) return 0;
    double logStep = std::log10(step);
    int places = static_cast<int>(-std::floor(logStep));
    return qMin(places, 6);
}

bool RulerH::isMajorTickMark(long long count, double step) const
{
    return (count % 10 == 0);
}

bool RulerV::isMajorTickMark(long long count, double step) const
{
    return (count % 10 == 0);
}

QString RulerH::formatTickValue(double value, int decimalPlaces) const
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

QString RulerV::formatTickValue(double value, int decimalPlaces) const
{
    QString text = QString::number(value, 'f', decimalPlaces);
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

void RulerH::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#FFFFFF"));
    
    double step = calculateStepSize();

    // 计算需要的小数位数
    int decimalPlaces = calculateDecimalPlaces(step);

    // 计算第一个刻度的位置
    double startValue = std::ceil(m_start / step) * step;

    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.color() = Qt::black;
    painter.setPen(pen);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);;

    for (double value = startValue; value <= m_end; value += step)
    {
        // 将场景坐标转换为视图坐标
        double xPos = (value - m_origin) * m_rulerZoom;
        // 只绘制在视图范围内的刻度
        if (xPos < 0 || xPos > width())
            continue;

        long long count = qRound64(value / step);
        bool isMajor = isMajorTickMark(count, step);

        //刻度的长度
        int iTickHeight;
        //小刻度长度交错
        if (count % 2 == 0) 
        {
            iTickHeight = isMajor ? 10 : 6;
        }
        else 
        {
            iTickHeight = isMajor ? 10 : 4;
        }
        painter.drawLine(QPointF(xPos, height() - iTickHeight), QPointF(xPos, height()));

        //主刻度
        if (isMajor)
        {
            QString text = formatTickValue(value, decimalPlaces);
            QRectF textRect = painter.boundingRect(QRectF(), Qt::AlignLeft | Qt::AlignTop, text);

            // 调整文本位置，确保不超出边界
            double textX = xPos;
            // 如果文本左边界超出标尺左边界，调整位置
            if (textX - textRect.width() / 2 < 0)
            {
                textX = textRect.width() / 2;
            }
            // 如果文本右边界超出标尺右边界，调整位置
            else if (textX + textRect.width() / 2 > width())
            {
                textX = width() - textRect.width() / 2;
            }
            textRect.moveCenter(QPointF(textX, 5));
            painter.drawText(textRect, Qt::AlignCenter, text);
        }
    }
    //画出鼠标位置的红线,鼠标坐标是scene中的坐标需要转换
    QPen redPen(Qt::red);
    redPen.setWidth(1);
    painter.setPen(redPen);
    double widgetMouseX = (m_mouseXPos - m_origin) * m_rulerZoom;
    painter.drawLine(QPointF(widgetMouseX, 0), QPointF(widgetMouseX, height()));
}

void RulerV::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#FFFFFF"));

    double step = calculateStepSize();
    int decimalPlaces = calculateDecimalPlaces(step);
    double startValue = std::ceil(m_start / step) * step;

    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.color() = Qt::black;
    painter.setPen(pen);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);

    for (double value = startValue; value <= m_end; value += step)
    {
        double yPos =height() - ((value - m_origin) * (-m_rulerZoom));
        if (yPos < 0 || yPos > height())
        {
            continue;
        }
        long long count = qRound64(value / step);
        bool isMajor = isMajorTickMark(count, step);

        int tickHeight;
        if (count % 2 == 0) 
        {
            tickHeight = isMajor ? 10 : 6;
        }
        else 
        {
            tickHeight = isMajor ? 8 : 4;
        }
       
        painter.drawLine(QPointF(width() - tickHeight, yPos), QPointF(width(), yPos));
        // 绘制主刻度值
        if (isMajor)
        {
            QString text = formatTickValue(value, decimalPlaces);
            QRectF textRect = painter.boundingRect(QRectF(), Qt::AlignLeft | Qt::AlignTop, text);

            double textY = yPos;
            if (textY - textRect.height() / 2 < 0)
            {
                textY = textRect.height() / 2;
            }
            else if (textY + textRect.height() / 2 > height())
            {
                textY = height() - textRect.height() / 2;
            }
            
            textRect.moveCenter(QPointF(width() / 2, textY));
            painter.save();
            painter.translate(textRect.center());
            painter.rotate(-90);
            painter.drawText(QRectF(-textRect.width() / 2, -textRect.height() / 2 - 5, textRect.width(), textRect.height()), Qt::AlignCenter, text);
            painter.restore();
        }
    }
    
    QPen redPen(Qt::red);
    redPen.setWidth(1);
    painter.setPen(redPen);
    double WidgetMouseY = height() - ((m_mouseYPos - m_origin) * (-m_rulerZoom));
    painter.drawLine(QPointF(0, WidgetMouseY), QPointF(width(), WidgetMouseY));
}