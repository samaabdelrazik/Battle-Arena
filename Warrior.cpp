#include "Warrior.h"
#include <iostream>

using namespace std;

Warrior::Warrior(string name)
    : Character(name, 20, 150)
{}

int Warrior::attack() const
{
    return getAttackDamage();
}

int Warrior::specialAbility() const
{
    cout << getName() << " uses Power Strike!\n";
    return getAttackDamage() * 2;
}
