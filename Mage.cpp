#include "Mage.h"
#include <iostream>

using namespace std;

Mage::Mage(string name)
    : Character(name, 35, 80)
{}

int Mage::attack() const
{
    return getAttackDamage();
}

int Mage::specialAbility() const
{
    cout << getName() << " uses Arcane Storm!\n";
    return getAttackDamage() * 3;
}
