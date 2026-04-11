#include "Mage.h"
#include <iostream>
#include <QBrush>

using namespace std;

Mage::Mage(string name)
    : Character(name, 35, 80)
{
    this->setBrush(Qt::yellow);
}


int Mage::specialAbility()
{
    cout << getName() << " uses Arcane Storm!\n";
    return getAttackDamage() * 3;
}
