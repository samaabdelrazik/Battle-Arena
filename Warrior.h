#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"

class Warrior : public Character
{
public:
    Warrior(string name);

    int attack() const override;
    int specialAbility() const;
};

#endif
