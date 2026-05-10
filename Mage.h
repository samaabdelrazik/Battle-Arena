#ifndef MAGE_H
#define MAGE_H

#include "Character.h"

class Mage : public Character
{
public:
    Mage(string name);

    int specialAbility() override;

    void basicAttack(QPointF targetPoint) override;

    void arcaneStormPulse();
};

#endif
