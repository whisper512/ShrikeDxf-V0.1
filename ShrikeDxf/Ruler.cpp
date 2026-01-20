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

CRulerH::~CRulerH()
{
}

CRulerV::CRulerV(QWidget* parent) : QWidget(parent),
    m_dOrigin(0.0),
    m_dRulerZoom(1.0),
    m_dStart(0.0),
    m_dEnd(5000.0),
    m_dStepMin(1.0),
    m_dStepMax(500.0),
    m_bOutOfRange(false)
{
}

CRulerV::~CRulerV()
{
}

void CRulerH::SetOrigin(double origin)
{
    m_dOrigin = origin;
    update();
}

void CRulerV::SetOrigin(double origin)
{
    m_dOrigin = origin;
    update();
}

void CRulerH::SetRulerZoom(double zoom)
{
    m_dRulerZoom = zoom;
    update();
}

void CRulerV::SetRulerZoom(double zoom)
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

void CRulerV::SetRange(double min, double max)
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

void CRulerV::SetStepRange(double min, double max)
{
    m_dStepMin = min;
    m_dStepMax = max;
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

double CRulerV::CalculateStepSize() const
{
    if (m_dRulerZoom <= 0 || height() <= 0)
    {
        return 1.0;
    }
    double dViewRange = height() / m_dRulerZoom;
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

int CRulerH::CalculateDecimalPlaces(double step) const
{
    if (step >= 1.0) return 0;

    // 计算需要的小数位数
    double logStep = std::log10(step);
    int places = static_cast<int>(-std::floor(logStep));

    // 限制小数位数
    return qMin(places, 6);
}

int CRulerV::CalculateDecimalPlaces(double step) const
{
    if (step >= 1.0) return 0;
    double logStep = std::log10(step);
    int places = static_cast<int>(-std::floor(logStep));
    return qMin(places, 6);
}

bool CRulerH::IsMajorTickMark(long long count, double step) const
{
    return (count % 10 == 0);
}

bool CRulerV::IsMajorTickMark(long long count, double step) const
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

QString CRulerV::FormatTickValue(double value, int decimalPlaces) const
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

    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.color() = Qt::black;
    painter.setPen(pen);
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


        int tickHeight;
        if (count % 2 == 0) {
            tickHeight = isMajor ? 10 : 6;
        }
        else {
            tickHeight = isMajor ? 10 : 4;  // 其他刻度使用短长度
        }
        painter.drawLine(QPointF(xPos, height() - tickHeight), QPointF(xPos, height()));

        // 绘制刻度值（仅主刻度）
        if (isMajor)
        {
            QString text = FormatTickValue(value, decimalPlaces);
            QRectF textRect = painter.boundingRect(QRectF(), Qt::AlignLeft | Qt::AlignTop, text);

            // 调整文本位置，确保不超出边界
            double textX = xPos;
            // 如果文本左边界超出标尺左边界，调整位置
            if (textX - textRect.width() / 2 < 0)
                textX = textRect.width() / 2;
            // 如果文本右边界超出标尺右边界，调整位置
            else if (textX + textRect.width() / 2 > width())
                textX = width() - textRect.width() / 2;

            textRect.moveCenter(QPointF(textX, 5)); // 文本位置改为顶部
            painter.drawText(textRect, Qt::AlignCenter, text);
        }
    }
}

void CRulerV::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#e0e0e0"));

    double step = CalculateStepSize();

    int decimalPlaces = CalculateDecimalPlaces(step);

    double startValue = std::ceil(m_dStart / step) * step;

    QPen pen = painter.pen();
    pen.setWidth(1);
    pen.color() = Qt::black;
    painter.setPen(pen);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);;

    for (double value = startValue; value <= m_dEnd; value += step)
    {
        double yPos = (value - m_dOrigin) * m_dRulerZoom;

        if (yPos < 0 || yPos > height())
            continue;


        long long count = qRound64(value / step);
        bool isMajor = IsMajorTickMark(count, step);

        int tickHeight;
        // 根据count的值决定刻度线长度，实现交错效果
        if (count % 2 == 0) {
            tickHeight = isMajor ? 10 : 6;  // 每5个刻度使用中等长度
        }
        else {
            tickHeight = isMajor ? 8 : 4;  // 其他刻度使用短长度
        }
        // 刻度线从左侧改为右侧
        painter.drawLine(QPointF(width() - tickHeight, yPos), QPointF(width(), yPos));

        // 绘制主刻度值
        if (isMajor)
        {
            QString text = FormatTickValue(value, decimalPlaces);
            QRectF textRect = painter.boundingRect(QRectF(), Qt::AlignLeft | Qt::AlignTop, text);

            // 调整文本位置，确保不超出边界
            double textY = yPos;
            // 如果文本上边界超出标尺上边界，调整位置
            if (textY - textRect.height() / 2 < 0)
                textY = textRect.height() / 2;
            // 如果文本下边界超出标尺下边界，调整位置
            else if (textY + textRect.height() / 2 > height())
                textY = height() - textRect.height() / 2;

            textRect.moveCenter(QPointF(width() / 2, textY)); // 文本位置改为左侧
            painter.save();
            painter.translate(textRect.center());
            painter.rotate(-90);
            painter.drawText(QRectF(-textRect.width() / 2 , -textRect.height() / 2 - 5, textRect.width(), textRect.height()), Qt::AlignCenter, text);
            painter.restore();
        }
    }
}