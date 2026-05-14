#include "Platform.h"
#include <QBrush>
#include <QPen>
#include <QPainter>

Platform::Platform(qreal x, qreal y, qreal width, qreal height)
{
    setRect(0, 0, width, height);
    setPos(x, y);
    setBrush(QBrush(QColor(139, 69, 19)));
    setPen(QPen(Qt::black, 2));
}

void Platform::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->fillRect(QRect(0, 8, rect().width(), rect().height()-8), QColor(139, 69, 19));


    painter->fillRect(QRect(0, 0, rect().width(), 8), QColor(34, 139, 34));

    painter->setPen(QPen(QColor(80, 200, 80), 2));
    painter->drawLine(0, 0, rect().width(), 0);

}
