#include "Block.h"
#include <QTimer>

Block::Block()
{
    setTransformOriginPoint(20,20);

    timer = new QTimer(nullptr);

    QObject::connect(timer, &QTimer::timeout, [this](){
        flipAngle += 0.1;

        QTransform transform;
        transform.translate(20, 0);
        transform.scale(sin(flipAngle), 1);
        transform.translate(-20,0);
        setTransform(transform);
    });
    timer->start(32);
}
Block::~Block()
{
    timer->stop();
    delete timer;
}
