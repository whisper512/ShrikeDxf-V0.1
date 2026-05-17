#pragma once
#include "EntityBase.h"
#include <QRegularExpression>
#include <QGraphicsItem>

// ─── MText ──────────────────────────────────────────────────
struct EntityMText
{
    EntityProp prop;
    Vertex3D   insertPoint;
    Vertex3D   xAxisDir;
    std::string text;
    double     height = 0.0;
    double     rotation = 0.0;
    double     widthFactor = 1.0;
    double     lineSpacing = 1.0;
    std::string style = "STANDARD";
    int     attachPoint = 1;
    int     textDir = 1;
    int     lineSpaceStyle = 1;

    // 计算边界
    QRectF boundingBox(double padding = 0.0) const
    {
        QFont font(QString::fromStdString(style));
        if (height < 1.0) font.setPixelSize(1);
        else font.setPixelSize(static_cast<int>(height));
        QString textStr = QString::fromStdString(text);
        textStr.replace("\\P", "\n");
        textStr.remove(QRegularExpression("\\\\[A-Za-z][^;]*;"));
        QGraphicsTextItem tempItem;
        tempItem.setPlainText(textStr);
        tempItem.setFont(font);
        QRectF lr = tempItem.boundingRect();
        double ax, ay;
        switch (attachPoint) {
        case 1: ax = lr.left();            ay = lr.top();             break;
        case 2: ax = lr.center().x();      ay = lr.top();             break;
        case 3: ax = lr.right();           ay = lr.top();             break;
        case 4: ax = lr.left();            ay = lr.center().y();      break;
        case 5: ax = lr.center().x();      ay = lr.center().y();      break;
        case 6: ax = lr.right();           ay = lr.center().y();      break;
        case 7: ax = lr.left();            ay = lr.bottom();          break;
        case 8: ax = lr.center().x();      ay = lr.bottom();          break;
        case 9: ax = lr.right();           ay = lr.bottom();          break;
        default: ax = lr.left();           ay = lr.top();             break;
        }
        double setX = insertPoint.x() - ax;
        double setY = insertPoint.y() + ay;
        double left = setX + lr.left();
        double top = setY - lr.bottom();
        double right = setX + lr.right();
        double bottom = setY - lr.top();
        return QRectF(left - padding, top - padding,
            right - left + padding * 2,
            bottom - top + padding * 2);
    }
    double distanceTo(double px, double py) const {
        QFont font(QString::fromStdString(style));
        if (height < 1.0) font.setPixelSize(1);
        else font.setPixelSize(static_cast<int>(height));
        QString textStr = QString::fromStdString(text);
        textStr.replace("\\P", "\n");
        textStr.remove(QRegularExpression("\\\\[A-Za-z][^;]*;"));
        QGraphicsTextItem tempItem;
        tempItem.setPlainText(textStr);
        tempItem.setFont(font);
        QRectF lr = tempItem.boundingRect();
        double ax, ay;
        switch (attachPoint) {
        case 1: ax = lr.left();            ay = lr.top();             break;
        case 2: ax = lr.center().x();      ay = lr.top();             break;
        case 3: ax = lr.right();           ay = lr.top();             break;
        case 4: ax = lr.left();            ay = lr.center().y();      break;
        case 5: ax = lr.center().x();      ay = lr.center().y();      break;
        case 6: ax = lr.right();           ay = lr.center().y();      break;
        case 7: ax = lr.left();            ay = lr.bottom();          break;
        case 8: ax = lr.center().x();      ay = lr.bottom();          break;
        case 9: ax = lr.right();           ay = lr.bottom();          break;
        default: ax = lr.left();           ay = lr.top();             break;
        }
        double setX = insertPoint.x() - ax;
        double setY = insertPoint.y() + ay;
        double left = setX + lr.left();
        double top = setY - lr.bottom();
        double right = setX + lr.right();
        double bottom = setY - lr.top();
        double dx = 0, dy = 0;
        if (px < left) dx = left - px;
        else if (px > right) dx = px - right;
        if (py < top) dy = top - py;
        else if (py > bottom) dy = py - bottom;
        return EntUtil::rectDistanceToPoint(px, py, left, top, right, bottom);
    }
};