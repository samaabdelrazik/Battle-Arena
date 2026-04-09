#include "Archer.h"
#include <iostream>

using namespace std;

Archer::Archer(string name)
    : Character(name, 25, 100)
{}

int Archer::attack() const
{
    return getAttackDamage();
}

int Archer::specialAbility() const
{
    cout << getName() << " uses Double Shot!\n";
    return getAttackDamage() * 2;
}
