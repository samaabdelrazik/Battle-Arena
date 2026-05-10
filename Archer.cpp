#include "Archer.h"
#include "Arrow.h"

#include <iostream>
#include <QBrush>
#include <QGraphicsScene>
#include <cmath>

using namespace std;

Archer::Archer(string name)
    : Character(name, 25, 90)
{
    this->setBrush(Qt::blue);

    setAttackCooldownFrames(14);

    setMovementStats(1.15, 9.2);

    setModernSpriteSheet(
        ":/sprites/ranger.png",
        32,
        32,
        0,
        10,
        0,
        1,
        3
    );
}

int Archer::specialAbility()
{
    cout << getName()
         << " uses Infinite Arrow Barrage!\n";

    return getAttackDamage() * 2;
}

void Archer::basicAttack(QPointF targetPoint)
{
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

    Arrow *arrow =
        new Arrow(angle,
                  this,
                  getAttackDamage());

    arrow->setPos(centerX, centerY);

    if (scene())
    {
        scene()->addItem(arrow);
    }
}
