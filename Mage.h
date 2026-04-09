#ifndef MAGE_H
#define MAGE_H

#include "Character.h"

class Mage : public Character
{
public:
    Mage(string name);

    int attack() const override;
    int specialAbility() const;
};

#endif
