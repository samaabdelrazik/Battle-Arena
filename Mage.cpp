#include "Mage.h"
#include "Arrow.h"
#include "Enemy.h"

#include <iostream>
#include <QBrush>
#include <QGraphicsScene>
#include <QList>
#include <QGraphicsEllipseItem>
#include <QPen>
#include <QTimer>
#include <cmath>

using namespace std;

Mage::Mage(string name)
    : Character(name, 35, 110)
{
    this->setBrush(Qt::yellow);

    setAttackCooldownFrames(22);

    setMovementStats(0.8, 6.5);

    setModernSpriteSheet(
        ":/sprites/wizard.png",
        32,
        32,
        5,
        5,
        0,
        1,
        3
    );
}

int Mage::specialAbility()
{
    cout << getName()
         << " uses Arcane Storm!\n";

    return getAttackDamage() * 2;
}

void Mage::basicAttack(QPointF targetPoint)
{
    if (!scene())
    {
        return;
    }

    setFacingFromTarget(targetPoint);

    bool attackingRight =
        targetPoint.x() >= x() + rect().width() / 2.0;

    playModernAttackAnimation(attackingRight);

    double centerX =
        x() + rect().width() / 2;

    double centerY =
        y() + rect().height() / 2;

    double dx = targetPoint.x() - centerX;
    double dy = targetPoint.y() - centerY;

    double angle = atan2(dy, dx);

    Arrow *magicProjectile =
        new Arrow(angle,
                  this,
                  getAttackDamage());

    magicProjectile->setRect(0, 0, 28, 28);
    magicProjectile->setBrush(QBrush(Qt::magenta));
    magicProjectile->setPos(centerX, centerY);

    scene()->addItem(magicProjectile);
}

void Mage::arcaneStormPulse()
{
    if (!scene())
    {
        return;
    }

    playModernAttackAnimation(true);

    double centerX =
        x() + rect().width() / 2;

    double centerY =
        y() + rect().height() / 2;

    double stormRadius = 170.0;
    int stormDamage = specialAbility();

    QGraphicsEllipseItem *stormEffect =
        new QGraphicsEllipseItem(
            centerX - stormRadius,
            centerY - stormRadius,
            stormRadius * 2,
            stormRadius * 2
        );

    stormEffect->setBrush(QBrush(QColor(160, 0, 255, 60)));
    stormEffect->setPen(QPen(QColor(200, 100, 255), 3));
    stormEffect->setZValue(500);

    scene()->addItem(stormEffect);

    QList<QGraphicsItem*> allItems = scene()->items();

    for (QGraphicsItem *item : allItems)
    {
        Enemy *enemy =
            dynamic_cast<Enemy*>(item);

        if (enemy && enemy->isAlive())
        {
            double enemyCenterX =
                enemy->x() + enemy->rect().width() / 2;

            double enemyCenterY =
                enemy->y() + enemy->rect().height() / 2;

            double distX = enemyCenterX - centerX;
            double distY = enemyCenterY - centerY;

            double distance =
                sqrt(distX * distX + distY * distY);

            if (distance <= stormRadius)
            {
                enemy->takeDamage(stormDamage);
            }
        }
    }

    QTimer::singleShot(120, [stormEffect]() {
        if (stormEffect->scene())
        {
            stormEffect->scene()->removeItem(stormEffect);
        }

        delete stormEffect;
    });
}
