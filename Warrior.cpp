#include "Warrior.h"
#include "Enemy.h"

#include <iostream>
#include <QBrush>
#include <QGraphicsScene>
#include <QList>
#include <QTimer>

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

void Warrior::playNormalAttackAnimation(int frameDelay,
                                        int endHoldFrames)
{
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
        frameDelay,
        endHoldFrames
    );
}

void Warrior::performStrike(QPointF targetPoint,
                            bool attackingRight,
                            int damage,
                            double extraRange,
                            double knockback,
                            bool grantsSpecialBar,
                            bool damageEnemyOnce)
{
    Q_UNUSED(targetPoint);

    if (!scene() || !isAlive())
    {
        return;
    }

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
            if (damageEnemyOnce &&
                enemiesHitThisSwing.contains(enemy))
            {
                continue;
            }

            QRectF enemyRect =
                enemy->sceneBoundingRect();

            if (attackArea.intersects(enemyRect))
            {
                if (damageEnemyOnce)
                {
                    enemiesHitThisSwing.append(enemy);
                }

                enemy->takeDamage(damage);

                if (grantsSpecialBar)
                {
                    increaseSpecialBar(12.0);
                }

                double knockDirection =
                    attackingRight ? knockback : -knockback;

                enemy->setX(enemy->x() + knockDirection);
            }
        }
    }
}

void Warrior::scheduleStrikeWindow(QPointF targetPoint,
                                   bool attackingRight,
                                   int generation,
                                   int frameDelay,
                                   int damage,
                                   double extraRange,
                                   double knockback,
                                   bool grantsSpecialBar)
{
    int impactColumns[3] = {5, 6, 7};

    for (int column : impactColumns)
    {
        int impactDelayMs =
            column * frameDelay * 16;

        QTimer::singleShot(
            impactDelayMs,
            this,
            [this,
             targetPoint,
             attackingRight,
             generation,
             damage,
             extraRange,
             knockback,
             grantsSpecialBar]() {
                if (generation == attackGeneration &&
                    !isSpecialActive())
                {
                    performStrike(
                        targetPoint,
                        attackingRight,
                        damage,
                        extraRange,
                        knockback,
                        grantsSpecialBar,
                        true
                    );
                }
            }
        );
    }
}

void Warrior::finishAttackIfCurrent(int generation)
{
    if (generation != attackGeneration)
    {
        return;
    }

    warriorAttackLocked = false;
}

void Warrior::basicAttack(QPointF targetPoint)
{
    if (warriorAttackLocked)
    {
        return;
    }

    setFacingFromTarget(targetPoint);
    bool attackingRight =
        targetPoint.x() >= x() + rect().width() / 2.0;

    rapidAttacking = false;
    warriorAttackLocked = true;
    attackGeneration++;
    enemiesHitThisSwing.clear();

    int generation = attackGeneration;

    if (isSpecialActive())
    {
        std::vector<AnimationFrame> specialFrames =
        {
            AnimationFrame(3, 3),
            AnimationFrame(3, 4),
            AnimationFrame(3, 4),
            AnimationFrame(3, 5),
            AnimationFrame(3, 5),
            AnimationFrame(3, 6),
            AnimationFrame(3, 3),
            AnimationFrame(3, 4),
            AnimationFrame(3, 4),
            AnimationFrame(3, 5),
            AnimationFrame(3, 5),
            AnimationFrame(3, 6),
            AnimationFrame(3, 3),
            AnimationFrame(3, 4),
            AnimationFrame(3, 4),
            AnimationFrame(3, 5),
            AnimationFrame(3, 5),
            AnimationFrame(3, 6),
            AnimationFrame(3, 3),
            AnimationFrame(3, 4),
            AnimationFrame(3, 4),
            AnimationFrame(3, 5),
            AnimationFrame(3, 5),
            AnimationFrame(3, 6)
        };

        playAnimationSequence(
            specialFrames,
            false,
            4,
            10
        );

        for (int hit = 0; hit < 4; hit++)
        {
            int impactDelayMs =
                ((hit * 6) + 5) * 4 * 16;

            QTimer::singleShot(
                impactDelayMs,
                this,
                [this, targetPoint, attackingRight, generation]() {
                    if (generation == attackGeneration &&
                        isSpecialActive())
                    {
                        performStrike(
                            targetPoint,
                            attackingRight,
                            getAttackDamage() * 2,
                            95.0,
                            90.0,
                            false,
                            false
                        );
                    }
                }
            );
        }

        int finishDelayMs =
            (24 * 4 * 16) + (10 * 16);

        QTimer::singleShot(
            finishDelayMs,
            this,
            [this, generation]() {
                if (generation == attackGeneration)
                {
                    stopSpecial();
                    finishAttackIfCurrent(generation);
                }
            }
        );

        return;
    }

    playNormalAttackAnimation(4, 4);

    scheduleStrikeWindow(
        targetPoint,
        attackingRight,
        generation,
        4,
        getAttackDamage(),
        68.0,
        35.0,
        true
    );

    int finishDelayMs =
        (10 * 4 * 16) + (4 * 16);

    QTimer::singleShot(
        finishDelayMs,
        this,
        [this, generation]() {
            finishAttackIfCurrent(generation);
        }
    );
}

void Warrior::rapidAttack(QPointF targetPoint)
{
    setFacingFromTarget(targetPoint);
    bool attackingRight =
        targetPoint.x() >= x() + rect().width() / 2.0;

    if (!rapidAttacking)
    {
        rapidAttacking = true;
        warriorAttackLocked = false;
        attackGeneration++;

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
            2
        );
    }

    int generation = attackGeneration;
    enemiesHitThisSwing.clear();

    scheduleStrikeWindow(
        targetPoint,
        attackingRight,
        generation,
        2,
        getAttackDamage(),
        68.0,
        35.0,
        true
    );
}

void Warrior::stopRapidAttack()
{
    if (!rapidAttacking)
    {
        return;
    }

    rapidAttacking = false;
    warriorAttackLocked = false;
    attackGeneration++;
    enemiesHitThisSwing.clear();
    stopAnimationSequence();
}
