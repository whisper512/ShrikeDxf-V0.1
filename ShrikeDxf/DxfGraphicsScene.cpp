#include<QGraphicsItem>
#include<QGraphicsEllipseItem>
#include<QGraphicsLineItem>
#include<QPainterPath>
#include <QRegularExpression>
#include "DxfGraphicsScene.h"


CDxfGraphicsScene::CDxfGraphicsScene(QObject* parent)
    : QGraphicsScene(parent)
    ,  m_scale(1.0)
{
}

CDxfGraphicsScene::~CDxfGraphicsScene()
{

}

void CDxfGraphicsScene::DxfDraw(const std::map<std::string, stuLayer>& mapDxf)
{
    ClearScene();
    QRectF bounds = CalculateSceneBounds(mapDxf);
    if (bounds.isNull() || bounds.width() == 0 || bounds.height() == 0)
    {
        bounds = QRectF(-250, -250, 500, 500);
    }
    DrawSceneBackground(bounds);
    
    for (auto it = mapDxf.begin(); it != mapDxf.end(); ++it)
    {
        if (!it->second.isVisible)
            continue;
        for (const auto& entity : it->second.entities)
        {
            EntityType type = GetEntityType(entity);
            switch (type)
            {
            case EntityType::Point:
            {
                const auto& pt = std::get<EntityPoint>(entity);
                DrawPoint(pt);
                break;
            }
            case EntityType::Line:
                DrawLine(std::get<EntityLine>(entity));
                break;
            case EntityType::Circle:
                DrawCircle(std::get<EntityCircle>(entity));
                break;
            case EntityType::Arc:
                DrawArc(std::get<EntityArc>(entity));
                break;
            case EntityType::Ellipse:
                DrawEllipse(std::get<EntityEllipse>(entity));
                break;
            case EntityType::LWPolyline:
                DrawLWPolyline(std::get<EntityLWPolyline>(entity));
                break;
            case EntityType::Polyline:
                DrawPolyline(std::get<EntityPolyline>(entity));
                break;
            case EntityType::Spline:
                DrawSpline(std::get<EntitySpline>(entity));
                break;
            case EntityType::Text:
                DrawText(std::get<EntityText>(entity));
                break;
            case EntityType::MText:
                DrawMText(std::get<EntityMText>(entity));
                break;
            default:
                break;
            }
        }
    }
    setSceneRect(bounds);
}

void CDxfGraphicsScene::ClearScene()
{
    clear();
}

void CDxfGraphicsScene::DrawPoint(const EntityPoint& point)
{
    QColor color = Qt::black;
    const auto& colorMap = DxfColorMap::getColorMap();
    auto it = colorMap.find(point.prop.color);
    if (it != colorMap.end())
        color = it->second;
    QPen pen(color, 1.0 / m_scale);
    pen.setCosmetic(true); //线宽不受缩放影响
    qreal x = point.point.x();
    qreal y = point.point.y();
    qreal s = 1.0 / m_scale;  // 点的大小
    // 画一个小十字表示点
    // 横线
    addLine(x - s, y, x + s, y, pen);
    // 竖线
    addLine(x, y - s, x, y + s, pen);
}

void CDxfGraphicsScene::DrawLine(const EntityLine& line)
{
    if (!line.prop.visible) return;
    QColor color = GetEntityColor(line.prop);
    QPen pen(color, 1.0 / m_scale);
    pen.setCosmetic(true);
    addLine(line.startPoint.x(), line.startPoint.y(),
        line.endPoint.x(), line.endPoint.y(), pen);
}

void CDxfGraphicsScene::DrawCircle(const EntityCircle& circle)
{
    if (!circle.prop.visible) return;
    QColor color = GetEntityColor(circle.prop);
    QPen pen(color, 1.0 / m_scale);
    pen.setCosmetic(true);
    qreal r = circle.radius;
    addEllipse(circle.center.x() - r, circle.center.y() - r,
        r * 2, r * 2, pen);
}

void CDxfGraphicsScene::DrawArc(const EntityArc& arc)
{
    if (!arc.prop.visible) return;
    QColor color = GetEntityColor(arc.prop);
    QPen pen(color, 1.0 / m_scale);
    pen.setCosmetic(true);
    QPainterPath path;
    qreal r = arc.radius;
    qreal cx = arc.center.x();
    qreal cy = arc.center.y();
    // DXF 角度：0=右侧，逆时针为正
    // Qt QPainterPath::arcTo: 角度从右侧开始，顺时针为正
    // 所以需要反转方向
    qreal startAngle = arc.startAngle * 180.0 / M_PI;        // 转度
    qreal sweepAngle = arc.endAngle - arc.startAngle;        // 跨度(弧度)
    sweepAngle = sweepAngle * 180.0 / M_PI;                  // 转度
    if (!arc.isCCW) {
        // 顺时针弧：反转 sweepAngle
        sweepAngle = -sweepAngle;
    }
    QRectF rect(cx - r, cy - r, r * 2, r * 2);
    path.arcMoveTo(rect, -startAngle);           // Qt 是顺时针为正，取反
    path.arcTo(rect, -startAngle, -sweepAngle);  // 取反使其逆时针
    addPath(path, pen);
}

void CDxfGraphicsScene::DrawEllipse(const EntityEllipse& ellipse)
{
    if (!ellipse.prop.visible) return;
    QColor color = GetEntityColor(ellipse.prop);
    QPen pen(color, 1.0 / m_scale);
    pen.setCosmetic(true);
    // majorAxisEndpoint 是长轴端点(相对于中心的向量)
    qreal majorLen = std::sqrt(
        ellipse.majorAxisEndpoint.x() * ellipse.majorAxisEndpoint.x() +
        ellipse.majorAxisEndpoint.y() * ellipse.majorAxisEndpoint.y());
    if (majorLen < 1e-10) return;
    qreal minorLen = majorLen * ellipse.ratio;
    // 长轴角度
    qreal angleRad = std::atan2(ellipse.majorAxisEndpoint.y(),
        ellipse.majorAxisEndpoint.x());
    QPainterPath path;
    QRectF rect(-majorLen, -minorLen, majorLen * 2, minorLen * 2);
    // 完整椭圆
    if (ellipse.startParam == 0.0 && ellipse.endParam == 2.0 * M_PI)
    {
        path.addEllipse(rect);
    }
    else
    {
        // 椭圆弧
        qreal startAngle = ellipse.startParam * 180.0 / M_PI;
        qreal sweepAngle = (ellipse.endParam - ellipse.startParam) * 180.0 / M_PI;
        path.arcMoveTo(rect, -startAngle);
        path.arcTo(rect, -startAngle, -sweepAngle);
    }
    // 旋转到长轴方向并平移到中心
    QTransform tf;
    tf.translate(ellipse.center.x(), ellipse.center.y());
    tf.rotate(angleRad * 180.0 / M_PI);
    path = tf.map(path);
    addPath(path, pen);
}

void CDxfGraphicsScene::DrawSolid(const EntitySolid& solid)
{
    // 画填充
}

void CDxfGraphicsScene::DrawHatch(const EntityHatch& hatch)
{
    // 画填充
}

void CDxfGraphicsScene::DrawSceneBackground(QRectF& rect)
{
    QPen pen(QColor("#0A0A0A"), 0.5 / m_scale);  // 蓝色半透明虚线
    pen.setCosmetic(true);
    pen.setStyle(Qt::DashLine);
    addRect(rect, pen);
}


void CDxfGraphicsScene::DrawLWPolyline(const EntityLWPolyline& polyline)
{
    if (!polyline.prop.visible || polyline.vertices.empty()) return;
    QColor color = GetEntityColor(polyline.prop);
    QPen pen(color, 1.0 / m_scale);
    pen.setCosmetic(true);
    pen.setJoinStyle(Qt::RoundJoin);
    QPainterPath path;
    const auto& verts = polyline.vertices;
    int n = static_cast<int>(verts.size());
    // 移动到第一个点
    path.moveTo(verts[0].point.x(), verts[0].point.y());
    for (int i = 0; i < n; ++i)
    {
        int next = (i + 1) % n;
        if (!polyline.isClosed() && next == 0) break;
        double bulge = verts[i].bulge;
        double x1 = verts[i].point.x();
        double y1 = verts[i].point.y();
        double x2 = verts[next].point.x();
        double y2 = verts[next].point.y();
        if (std::abs(bulge) < 1e-10)
        {
            // 直线段
            path.lineTo(x2, y2);
        }
        else
        {
            // 凸度弧段：bulge = tan(sweepAngle/4)
            double theta = 4.0 * std::atan(std::abs(bulge));
            double cx, cy, rx, ry, startAng, sweepAng;
            // 计算弧参数
            double dx = x2 - x1;
            double dy = y2 - y1;
            double dist = std::sqrt(dx * dx + dy * dy);
            double d = dist / 2.0;
            double h = d / std::tan(theta / 2.0);  // 弧高
            // 中点
            double mx = (x1 + x2) / 2.0;
            double my = (y1 + y2) / 2.0;
            // 垂直方向
            double vx = -dy / dist;
            double vy = dx / dist;
            if (bulge > 0) {
                // 逆时针凸起
                cx = mx + vx * h;
                cy = my + vy * h;
            }
            else {
                cx = mx - vx * h;
                cy = my - vy * h;
            }
            rx = std::sqrt(d * d + h * h);
            ry = rx;
            // 起始角和跨角
            startAng = std::atan2(y1 - cy, x1 - cx) * 180.0 / M_PI;
            double endAng = std::atan2(y2 - cy, x2 - cx) * 180.0 / M_PI;
            sweepAng = endAng - startAng;
            // 归一化
            if (bulge > 0) {
                // 逆时针
                if (sweepAng < 0) sweepAng += 360.0;
            }
            else {
                if (sweepAng > 0) sweepAng -= 360.0;
            }
            QRectF arcRect(cx - rx, cy - ry, rx * 2, ry * 2);
            path.arcTo(arcRect, -startAng, -sweepAng);
        }
    }
    if (polyline.isClosed())
        path.closeSubpath();
    addPath(path, pen);
}


void CDxfGraphicsScene::DrawPolyline(const EntityPolyline& polyline)
{
    if (!polyline.prop.visible || polyline.vertices.empty()) return;
    QColor color = GetEntityColor(polyline.prop);
    QPen pen(color, 1.0 / m_scale);
    pen.setCosmetic(true);
    pen.setJoinStyle(Qt::RoundJoin);
    QPainterPath path;
    const auto& verts = polyline.vertices;
    int n = static_cast<int>(verts.size());
    path.moveTo(verts[0].point.x(), verts[0].point.y());
    for (int i = 0; i < n; ++i)
    {
        int next = (i + 1) % n;
        if (!polyline.isClosed() && next == 0) break;
        double bulge = verts[i].bulge;
        double x1 = verts[i].point.x();
        double y1 = verts[i].point.y();
        double x2 = verts[next].point.x();
        double y2 = verts[next].point.y();
        if (std::abs(bulge) < 1e-10)
        {
            path.lineTo(x2, y2);
        }
        else
        {
            // 凸度弧转换（同 LWPolyline）
            double theta = 4.0 * std::atan(std::abs(bulge));
            double dx = x2 - x1;
            double dy = y2 - y1;
            double dist = std::sqrt(dx * dx + dy * dy);
            double d = dist / 2.0;
            double h = d / std::tan(theta / 2.0);
            double mx = (x1 + x2) / 2.0;
            double my = (y1 + y2) / 2.0;
            double vx = -dy / dist;
            double vy = dx / dist;
            double cx, cy;
            if (bulge > 0) { cx = mx + vx * h; cy = my + vy * h; }
            else { cx = mx - vx * h; cy = my - vy * h; }
            double rx = std::sqrt(d * d + h * h);
            double ry = rx;
            double startAng = std::atan2(y1 - cy, x1 - cx) * 180.0 / M_PI;
            double endAng = std::atan2(y2 - cy, x2 - cx) * 180.0 / M_PI;
            double sweepAng = endAng - startAng;
            if (bulge > 0) { if (sweepAng < 0) sweepAng += 360.0; }
            else { if (sweepAng > 0) sweepAng -= 360.0; }
            QRectF arcRect(cx - rx, cy - ry, rx * 2, ry * 2);
            path.arcTo(arcRect, -startAng, -sweepAng);
        }
    }
    if (polyline.isClosed())
        path.closeSubpath();
    addPath(path, pen);
}

void CDxfGraphicsScene::DrawSpline(const EntitySpline& spline)
{
    if (!spline.prop.visible ||
        (spline.controlPoints.empty() && spline.fitPoints.empty())) return;
    QColor color = GetEntityColor(spline.prop);
    QPen pen(color, 1.0 / m_scale);
    pen.setCosmetic(true);
    QPainterPath path;
    // 优先用拟合点做 Catmull-Rom 插值（平滑曲线）
    if (spline.fitPoints.size() >= 2)
    {
        const auto& pts = spline.fitPoints;
        int n = static_cast<int>(pts.size());
        path.moveTo(pts[0].x(), pts[0].y());
        // Catmull-Rom 插值：每两个点之间用 cubicTo 插值
        // 需要前后各一个辅助点
        for (int i = 0; i < n - 1; ++i)
        {
            // 取前后各一点作为控制点
            QPointF p0 = (i == 0) ? pts[0].toQPointF() : pts[i - 1].toQPointF();
            QPointF p1 = pts[i].toQPointF();
            QPointF p2 = pts[i + 1].toQPointF();
            QPointF p3 = (i + 2 >= n) ? pts[n - 1].toQPointF() : pts[i + 2].toQPointF();
            // Catmull-Rom → 三次贝塞尔控制点
            double t = 0.5;  // 张力参数
            QPointF cp1 = p1 + (p2 - p0) * t / 3.0;
            QPointF cp2 = p2 - (p3 - p1) * t / 3.0;
            path.cubicTo(cp1, cp2, p2);
        }
    }
    else if (spline.controlPoints.size() >= 2)
    {
        int degree = (spline.degree > 0) ? spline.degree : 3;
        int n = static_cast<int>(spline.controlPoints.size());
        if (n <= degree)
        {
            // 控制点太少，直接连线
            path.moveTo(spline.controlPoints[0].x(), spline.controlPoints[0].y());
            for (int i = 1; i < n; ++i)
                path.lineTo(spline.controlPoints[i].x(), spline.controlPoints[i].y());
        }
        else
        {
            // 构建节点向量
            std::vector<double> knots = spline.knots;
            if (knots.empty())
            {
                // 标准 clamped 节点：前 degree+1 个 = 0，后 degree+1 个 = 1
                int m = n + degree + 1;
                knots.resize(m);
                for (int i = 0; i < m; ++i)
                {
                    if (i <= degree)
                        knots[i] = 0.0;
                    else if (i >= n)
                        knots[i] = 1.0;
                    else
                        knots[i] = static_cast<double>(i - degree) / (n - degree);
                }
            }
            // 采样 100 个点
            int numSamples = 200;
            double uMin = knots[degree];
            double uMax = knots[n];
            if (uMax <= uMin) {
                uMin = knots.front();
                uMax = knots.back();
            }
            auto getPoint = [&](double u) -> QPointF
                {
                    double x = 0, y = 0, wSum = 0;
                    bool hasWeight = !spline.weights.empty();
                    for (int j = 0; j < n; ++j)
                    {
                        double basis = BSplineBasis(j, degree, u, knots);
                        if (basis < 1e-15) continue;
                        double w = hasWeight ? spline.weights[j] : 1.0;
                        x += spline.controlPoints[j].x() * w * basis;
                        y += spline.controlPoints[j].y() * w * basis;
                        wSum += w * basis;
                    }
                    if (hasWeight && std::abs(wSum) > 1e-15) {
                        return QPointF(x / wSum, y / wSum);
                    }
                    return QPointF(x, y);
                };
            bool first = true;
            for (int i = 0; i <= numSamples; ++i)
            {
                double u = uMin + (uMax - uMin) * i / numSamples;
                QPointF pt = getPoint(u);
                if (first) {
                    path.moveTo(pt);
                    first = false;
                }
                else {
                    path.lineTo(pt);
                }
            }
        }
    }
    addPath(path, pen);
}


QRectF CDxfGraphicsScene::CalculateSceneBounds(const std::map<std::string, stuLayer>& mapDxf)
{
    if (mapDxf.empty())
        return QRectF();

    bool first = true;
    qreal minX = 0, minY = 0, maxX = 0, maxY = 0;

    auto updateBounds = [&](qreal x, qreal y)
        {
            if (first) {
                minX = maxX = x;
                minY = maxY = y;
                first = false;
            }
            else {
                minX = qMin(minX, x);
                maxX = qMax(maxX, x);
                minY = qMin(minY, y);
                maxY = qMax(maxY, y);
            }
        };

    for (const auto& [name, layer] : mapDxf)
    {
        for (const auto& entity : layer.entities)
        {
            EntityType type = GetEntityType(entity);
            switch (type)
            {
            case EntityType::Point:
            {
                const auto& pt = std::get<EntityPoint>(entity);
                updateBounds(pt.point.x(), pt.point.y());
                break;
            }
            case EntityType::Line:
            {
                const auto& line = std::get<EntityLine>(entity);
                updateBounds(line.startPoint.x(), line.startPoint.y());
                updateBounds(line.endPoint.x(), line.endPoint.y());
                break;
            }
            case EntityType::Circle:
            {
                const auto& circle = std::get<EntityCircle>(entity);
                qreal r = circle.radius;
                updateBounds(circle.center.x() - r, circle.center.y() - r);
                updateBounds(circle.center.x() + r, circle.center.y() + r);
                break;
            }
            case EntityType::Arc:
            {
                const auto& arc = std::get<EntityArc>(entity);
                qreal r = arc.radius;
                updateBounds(arc.center.x() - r, arc.center.y() - r);
                updateBounds(arc.center.x() + r, arc.center.y() + r);
                break;
            }
            case EntityType::Ellipse:
            {
                const auto& ellipse = std::get<EntityEllipse>(entity);
                qreal majorLen = std::sqrt(
                    ellipse.majorAxisEndpoint.x() * ellipse.majorAxisEndpoint.x() +
                    ellipse.majorAxisEndpoint.y() * ellipse.majorAxisEndpoint.y());
                qreal minorLen = majorLen * ellipse.ratio;
                qreal maxRadius = qMax(majorLen, minorLen);
                updateBounds(ellipse.center.x() - maxRadius, ellipse.center.y() - maxRadius);
                updateBounds(ellipse.center.x() + maxRadius, ellipse.center.y() + maxRadius);
                break;
            }
            case EntityType::LWPolyline:
            {
                const auto& poly = std::get<EntityLWPolyline>(entity);
                for (const auto& v : poly.vertices)
                    updateBounds(v.point.x(), v.point.y());
                break;
            }
            case EntityType::Polyline:
            {
                const auto& poly = std::get<EntityPolyline>(entity);
                for (const auto& v : poly.vertices)
                    updateBounds(v.point.x(), v.point.y());
                break;
            }
            case EntityType::Spline:
            {
                const auto& spline = std::get<EntitySpline>(entity);
                for (const auto& cp : spline.controlPoints)
                    updateBounds(cp.x(), cp.y());
                for (const auto& fp : spline.fitPoints)
                    updateBounds(fp.x(), fp.y());
                break;
            }
            case EntityType::Text:
            {
                const auto& txt = std::get<EntityText>(entity);
                updateBounds(txt.insertPoint.x(), txt.insertPoint.y());
                // 文字有宽度和高度，粗略估算范围
                double w = txt.height * txt.text.length() * 0.6;
                double h = txt.height;
                updateBounds(txt.insertPoint.x() + w, txt.insertPoint.y() + h);
                break;
            }
            case EntityType::MText:
            {
                const auto& mtext = std::get<EntityMText>(entity);
                updateBounds(mtext.insertPoint.x(), mtext.insertPoint.y());
                // 粗略估算多行文本范围
                double lineCount = 1;
                for (char c : mtext.text)
                    if (c == '\\' || c == '\n') lineCount++;
                double w = mtext.height * mtext.text.length() * 0.4;
                double h = mtext.height * lineCount * 1.5;
                updateBounds(mtext.insertPoint.x() + w, mtext.insertPoint.y() + h);
                break;
            }
            default:
                break;
            }
        }
    }

    if (first)
        return QRectF();

    // 加一些边距
    qreal margin = 50;
    return QRectF(minX - margin, minY - margin,
        maxX - minX + margin * 2,
        maxY - minY + margin * 2);
}


void CDxfGraphicsScene::DrawText(const EntityText& text)
{
    if (!text.prop.visible) return;
    QColor color = GetEntityColor(text.prop);
    double fontSize = text.height * m_scale;
    if (fontSize < 0.1) fontSize = 3.0;
    QFont font(QString::fromStdString(text.style));
    font.setPixelSize(static_cast<int>(fontSize));
    // 文本显示
    QGraphicsTextItem* pItem = addText(QString::fromStdString(text.text), font);
    pItem->setDefaultTextColor(color);
    // 位置和旋转
    pItem->setPos(text.insertPoint.x(), text.insertPoint.y());
    pItem->setRotation(text.rotation * 180.0 / M_PI);
    // 垂直翻转以补偿 Qt Y-down vs DXF Y-up
    pItem->setTransform(QTransform::fromScale(1.0, -1.0), true);
    // 对齐方式
    Qt::Alignment align = Qt::AlignLeft | Qt::AlignVCenter;
    if (text.alignH == 1) align = Qt::AlignLeft | Qt::AlignVCenter;
    else if (text.alignH == 2) align = Qt::AlignCenter;
    else if (text.alignH == 3) align = Qt::AlignRight | Qt::AlignVCenter;
    // 垂直对齐
    if (text.alignV == 1) align |= Qt::AlignTop;
    else if (text.alignV == 2) align |= Qt::AlignVCenter;
    else if (text.alignV == 3) align |= Qt::AlignBottom;
    pItem->setTextWidth(-1);
}


void CDxfGraphicsScene::DrawMText(const EntityMText& mtext)
{
    if (!mtext.prop.visible) return;
    QColor color = GetEntityColor(mtext.prop);
    double fontSize = mtext.height * m_scale;
    if (fontSize < 0.1) fontSize = 3.0;
    QFont font(QString::fromStdString(mtext.style));
    font.setPixelSize(static_cast<int>(fontSize));
    // 替换 DXF 换行符 \P 为 Qt 换行符 \n
    QString text = QString::fromStdString(mtext.text);
    text.replace("\\P", "\n");
    // 还可以处理其他 DXF 格式化代码（如 \L \Q \H 等，但先只处理换行）
    text.remove(QRegularExpression("\\\\[A-Za-z][^;]*;"));
    QGraphicsTextItem* pItem = addText(text, font);
    pItem->setDefaultTextColor(color);
    // 位置
    pItem->setPos(mtext.insertPoint.x(), mtext.insertPoint.y());
    pItem->setRotation(mtext.rotation * 180.0 / M_PI);
    // 垂直翻转以补偿 Qt Y-down vs DXF Y-up
    pItem->setTransform(QTransform::fromScale(1.0, -1.0), true);
    // 设置文本宽度以支持自动换行
    double textWidth = std::sqrt(
        mtext.xAxisDir.x() * mtext.xAxisDir.x() +
        mtext.xAxisDir.y() * mtext.xAxisDir.y());
    if (textWidth > 0)
        pItem->setTextWidth(textWidth);
    // 附着点
    QPointF offset(0, 0);
    QRectF rect = pItem->boundingRect();
    qreal w = rect.width();
    qreal h = rect.height();
    switch (mtext.attachPoint)
    {
    case 1: offset = QPointF(0, 0); break;           // TL
    case 2: offset = QPointF(-w / 2, 0); break;      // TC
    case 3: offset = QPointF(-w, 0); break;          // TR
    case 4: offset = QPointF(0, -h / 2); break;      // ML
    case 5: offset = QPointF(-w / 2, -h / 2); break; // MC
    case 6: offset = QPointF(-w, -h / 2); break;     // MR
    case 7: offset = QPointF(0, -h); break;          // BL
    case 8: offset = QPointF(-w / 2, -h); break;     // BC
    case 9: offset = QPointF(-w, -h); break;         // BR
    default: break;
    }
    pItem->setPos(mtext.insertPoint.x() + offset.x(),
        mtext.insertPoint.y() + offset.y());
}


double CDxfGraphicsScene::BSplineBasis(int i, int k, double u, const std::vector<double>& knots)
{
    if (k == 0)
    {
        return (u >= knots[i] && u < knots[i + 1]) ? 1.0 : 0.0;
    }
    else
    {
        double a = 0, b = 0;

        double denom1 = knots[i + k] - knots[i];
        if (std::abs(denom1) > 1e-15)
            a = (u - knots[i]) / denom1 * BSplineBasis(i, k - 1, u, knots);

        double denom2 = knots[i + k + 1] - knots[i + 1];
        if (std::abs(denom2) > 1e-15)
            b = (knots[i + k + 1] - u) / denom2 * BSplineBasis(i + 1, k - 1, u, knots);

        return a + b;
    }
}


QColor CDxfGraphicsScene::GetEntityColor(const EntityProp& prop) const
{
    QColor color = Qt::black;
    if (prop.color == 256) {

        return Qt::black;
    }
    const auto& colorMap = DxfColorMap::getColorMap();
    auto it = colorMap.find(prop.color);
    if (it != colorMap.end())
        color = it->second;
    return color;
}

