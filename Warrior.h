#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"

#include <QList>

class Enemy;

class Warrior : public Character
{
private:
    bool rapidAttacking = false;
    bool warriorAttackLocked = false;
    int attackGeneration = 0;
    QList<Enemy*> enemiesHitThisSwing;

    void playNormalAttackAnimation(int frameDelay,
                                   int endHoldFrames);

    void performStrike(QPointF targetPoint,
                       bool attackingRight,
                       int damage,
                       double extraRange,
                       double knockback,
                       bool grantsSpecialBar,
                       bool damageEnemyOnce);

    void scheduleStrikeWindow(QPointF targetPoint,
                              bool attackingRight,
                              int generation,
                              int frameDelay,
                              int damage,
                              double extraRange,
                              double knockback,
                              bool grantsSpecialBar);

    void finishAttackIfCurrent(int generation);

public:
    Warrior(string name);

    int specialAbility() override;

    void basicAttack(QPointF targetPoint) override;

    void rapidAttack(QPointF targetPoint);

    void stopRapidAttack();
};

#endif
