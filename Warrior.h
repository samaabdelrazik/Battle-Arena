#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"

class Warrior : public Character
{
public:
    Warrior(string name);

    int specialAbility() override;

    void basicAttack(QPointF targetPoint) override;
};

#endif
