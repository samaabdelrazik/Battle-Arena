#include "Platform.h"
#include <QBrush>
#include <QPen>

Platform::Platform(qreal x, qreal y, qreal width, qreal height)
{
    setRect(0, 0, width, height);
    setPos(x, y);
    setBrush(QBrush(QColor(139, 69, 19)));
    setPen(QPen(Qt::black, 2));
}
