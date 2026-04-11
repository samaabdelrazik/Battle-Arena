#include "Warrior.h"
#include <iostream>
#include <QBrush>

using namespace std;

Warrior::Warrior(string name)
    : Character(name, 20, 150)
{
    this->setBrush(Qt::red);
}


int Warrior::specialAbility()
{
    cout << getName() << " uses Power Strike!\n";
    return getAttackDamage() * 2;
}
