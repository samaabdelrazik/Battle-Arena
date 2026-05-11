#ifndef ARCHER_H
#define ARCHER_H

#include "Character.h"

class Archer : public Character
{
private:
    void fireArrowAt(QPointF targetPoint);

    bool rapidFiring = false;
    int normalShotGeneration = 0;

public:
    Archer(string name);

    int specialAbility() override;

    void basicAttack(QPointF targetPoint) override;

    void rapidAttack(QPointF targetPoint);

    void stopRapidFire();
};

#endif
