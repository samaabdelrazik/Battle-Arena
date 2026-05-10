#include "Arrow.h"
#include "Enemy.h"
#include "Character.h"

#include <QGraphicsScene>
#include <QList>
#include <cmath>

Arrow::Arrow(double angleRadians,
             Character *owner,
             int damage)
{
    this->owner = owner;
    this->damage = damage;

    setRect(0, 0, 20, 6);

    setBrush(Qt::black);

    double speed = 15.0;

    dx = cos(angleRadians) * speed;
    dy = sin(angleRadians) * speed;

    setRotation(angleRadians * 180 / M_PI);

    QTimer *timer = new QTimer(this);

    connect(timer,
            &QTimer::timeout,
            this,
            &Arrow::moveArrow);

    timer->start(16);
}

void Arrow::moveArrow()
{
    moveBy(dx, dy);

    QList<QGraphicsItem*> collisions =
        collidingItems();

    for (QGraphicsItem *item : collisions)
    {
        Enemy *enemy =
            dynamic_cast<Enemy*>(item);

        if (enemy)
        {
            enemy->takeDamage(damage);


            if (owner && !owner->isSpecialActive())
            {
                owner->increaseSpecialBar(10.0);
            }

            if (scene())
            {
                scene()->removeItem(this);
            }

            delete this;
            return;
        }
    }

    if (scene())
    {
        QRectF sceneRect = scene()->sceneRect();

        if (x() < sceneRect.left() ||
            x() > sceneRect.right() ||
            y() < sceneRect.top() ||
            y() > sceneRect.bottom())
        {
            scene()->removeItem(this);
            delete this;
        }
    }
}
