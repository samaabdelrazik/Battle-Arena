#ifndef ARCHER_H
#define ARCHER_H

#include "Character.h"

class Archer : public Character
{
public:
    Archer(string name);
    int specialAbility()  override;
};

#endif
