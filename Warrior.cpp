#include "Warrior.h"
#include "Enemy.h"

#include <iostream>
#include <QBrush>
#include <QGraphicsScene>
#include <QList>
#include <cmath>

using namespace std;

Warrior::Warrior(string name)
    : Character(name, 50, 160)
{
    this->setBrush(Qt::red);

    setAttackCooldownFrames(34);

    setMovementStats(0.45, 4.2);

    setModernSpriteSheet(
        ":/sprites/warrior.png",
        32,
        32,
        0,
        10,
        0,
        1,
        3
    );
}

int Warrior::specialAbility()
{
    cout << getName()
         << " uses Power Strike!\n";

    return getAttackDamage() * 2;
}

void Warrior::basicAttack(QPointF targetPoint)
{
    setFacingFromTarget(targetPoint);

    if (!scene())
    {
        return;
    }

    bool attackingRight =
        targetPoint.x() >= x() + rect().width() / 2.0;

    playModernAttackAnimation(attackingRight);

    bool usingPowerStrike =
        isSpecialActive();

    double extraRange =
        usingPowerStrike ? 95.0 : 48.0;

    int damage =
        usingPowerStrike ? specialAbility() : getAttackDamage();

    double knockback =
        usingPowerStrike ? 90.0 : 35.0;

    QRectF attackArea;

    if (attackingRight)
    {
        attackArea = QRectF(
            x() + rect().width(),
            y() - 35,
            extraRange,
            rect().height() + 70
        );
    }
    else
    {
        attackArea = QRectF(
            x() - extraRange,
            y() - 35,
            extraRange,
            rect().height() + 70
        );
    }

    QList<QGraphicsItem*> allItems =
        scene()->items();

    for (QGraphicsItem *item : allItems)
    {
        Enemy *enemy =
            dynamic_cast<Enemy*>(item);

        if (enemy && enemy->isAlive())
        {
            QRectF enemyRect =
                enemy->sceneBoundingRect();

            if (attackArea.intersects(enemyRect))
            {
                enemy->takeDamage(damage);

                if (!usingPowerStrike)
                {
                    increaseSpecialBar(12.0);
                }

                double knockDirection =
                    attackingRight ? knockback : -knockback;

                enemy->setX(enemy->x() + knockDirection);
            }
        }
    }

    if (usingPowerStrike)
    {
        stopSpecial();
    }
}
