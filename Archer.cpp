#include "Archer.h"
#include "Arrow.h"

#include <iostream>
#include <QBrush>
#include <QGraphicsScene>
#include <QTimer>
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

    startSpecial();

    std::vector<AnimationFrame> specialFrames =
    {
        AnimationFrame(3, 4),
        AnimationFrame(3, 6),
        AnimationFrame(3, 7)
    };

    playAnimationSequence(
        specialFrames,
        true,
        2
    );

    return getAttackDamage() * 2;
}

void Archer::fireArrowAt(QPointF targetPoint)
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

    Arrow *arrow =
        new Arrow(angle,
                  this,
                  getAttackDamage());

    arrow->setPos(centerX, centerY);

    scene()->addItem(arrow);
}

void Archer::basicAttack(QPointF targetPoint)
{
    if (isSpecialActive())
    {
        setFacingFromTarget(targetPoint);
        rapidFiring = false;

        if (!isAnimationPlaying())
        {
            std::vector<AnimationFrame> specialFrames =
            {
                AnimationFrame(3, 4),
                AnimationFrame(3, 6),
                AnimationFrame(3, 7)
            };

            playAnimationSequence(
                specialFrames,
                true,
                2
            );
        }
    }
    else
    {
        if (isAnimationPlaying())
        {
            return;
        }

        setFacingFromTarget(targetPoint);
        rapidFiring = false;
        normalShotGeneration++;

        std::vector<AnimationFrame> normalAttackFrames =
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
            normalAttackFrames,
            false,
            4,
            6
        );

        int releaseDelayMs =
            7 * 4 * 16;

        int shotGeneration =
            normalShotGeneration;

        QTimer::singleShot(
            releaseDelayMs,
            this,
            [this, targetPoint, shotGeneration]() {
                if (!isSpecialActive() &&
                    !rapidFiring &&
                    shotGeneration == normalShotGeneration)
                {
                    fireArrowAt(targetPoint);
                }
            }
        );

        return;
    }

    fireArrowAt(targetPoint);
}

void Archer::rapidAttack(QPointF targetPoint)
{
    setFacingFromTarget(targetPoint);

    if (!isSpecialActive() && !rapidFiring)
    {
        normalShotGeneration++;
    }

    if (!isSpecialActive() &&
        (!rapidFiring || !isAnimationPlaying()))
    {
        std::vector<AnimationFrame> rapidFrames =
        {
            AnimationFrame(3, 3),
            AnimationFrame(3, 4),
            AnimationFrame(3, 5),
            AnimationFrame(3, 6),
            AnimationFrame(3, 7)
        };

        playAnimationSequence(
            rapidFrames,
            true,
            3
        );
    }

    rapidFiring = true;

    fireArrowAt(targetPoint);
}

void Archer::stopRapidFire()
{
    if (!rapidFiring)
    {
        return;
    }

    rapidFiring = false;

    if (!isSpecialActive())
    {
        stopAnimationSequence();
    }
}
