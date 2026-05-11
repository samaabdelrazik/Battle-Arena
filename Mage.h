#ifndef MAGE_H
#define MAGE_H

#include "Character.h"

class Mage : public Character
{
private:
    void fireMagicAt(QPointF targetPoint);

    bool rapidCasting = false;
    int castGeneration = 0;

public:
    Mage(string name);

    int specialAbility() override;

    void basicAttack(QPointF targetPoint) override;

    void rapidAttack(QPointF targetPoint);

    void stopRapidAttack();

    void arcaneStormPulse();
};

#endif
