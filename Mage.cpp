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
        2,
        3
    );
}

int Mage::specialAbility()
{
    cout << getName()
         << " uses Arcane Storm!\n";

    return getAttackDamage() * 2;
}

void Mage::fireMagicAt(QPointF targetPoint)
{
    if (!scene() || !isAlive())
    {
        return;
    }

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

void Mage::basicAttack(QPointF targetPoint)
{
    if (!scene())
    {
        return;
    }

    if (isAnimationPlaying())
    {
        return;
    }

    setFacingFromTarget(targetPoint);
    rapidCasting = false;
    castGeneration++;

    std::vector<AnimationFrame> normalFrames =
    {
        AnimationFrame(3, 0),
        AnimationFrame(3, 1),
        AnimationFrame(3, 2),
        AnimationFrame(3, 3),
        AnimationFrame(3, 4),
        AnimationFrame(3, 5),
        AnimationFrame(3, 6),
        AnimationFrame(3, 7),
        AnimationFrame(3, 8),
        AnimationFrame(3, 9)
    };

    playAnimationSequence(
        normalFrames,
        false,
        4,
        4
    );

    int generation = castGeneration;
    int releaseDelayMs =
        5 * 4 * 16;

    QTimer::singleShot(
        releaseDelayMs,
        this,
        [this, targetPoint, generation]() {
            if (generation == castGeneration &&
                !rapidCasting &&
                !isSpecialActive())
            {
                fireMagicAt(targetPoint);
            }
        }
    );
}

void Mage::rapidAttack(QPointF targetPoint)
{
    setFacingFromTarget(targetPoint);

    if (!rapidCasting)
    {
        rapidCasting = true;
        castGeneration++;

        std::vector<AnimationFrame> rapidFrames =
        {
            AnimationFrame(3, 0),
            AnimationFrame(3, 1),
            AnimationFrame(3, 2),
            AnimationFrame(3, 3),
            AnimationFrame(3, 4),
            AnimationFrame(3, 5),
            AnimationFrame(3, 6),
            AnimationFrame(3, 7),
            AnimationFrame(3, 8),
            AnimationFrame(3, 9)
        };

        playAnimationSequence(
            rapidFrames,
            true,
            2
        );
    }

    fireMagicAt(targetPoint);
}

void Mage::stopRapidAttack()
{
    if (!rapidCasting)
    {
        return;
    }

    rapidCasting = false;
    castGeneration++;
    stopAnimationSequence();
}

void Mage::arcaneStormPulse()
{
    if (!scene())
    {
        return;
    }

    std::vector<AnimationFrame> specialFrames =
    {
        AnimationFrame(1, 0),
        AnimationFrame(1, 1),
        AnimationFrame(1, 2),
        AnimationFrame(1, 3),
        AnimationFrame(1, 4),
        AnimationFrame(1, 5),
        AnimationFrame(1, 6),
        AnimationFrame(1, 7),
        AnimationFrame(1, 8),
        AnimationFrame(1, 9)
    };

    playAnimationSequence(
        specialFrames,
        false,
        3,
        2
    );

    int pulseDelayMs =
        6 * 3 * 16;

    QTimer::singleShot(
        pulseDelayMs,
        this,
        [this]() {
            if (!scene() || !isSpecialActive())
            {
                return;
            }

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
    );
}
