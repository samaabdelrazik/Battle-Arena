#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsRectItem>

class Platform : public QGraphicsRectItem
{
public:
    Platform(qreal x, qreal y, qreal width, qreal height);
};

#endif
