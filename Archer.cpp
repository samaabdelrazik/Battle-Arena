#include "Archer.h"
#include <iostream>
#include <QBrush>

using namespace std;

Archer::Archer(string name)
    : Character(name, 25, 100)
{
    this->setBrush(Qt::blue);
}


int Archer::specialAbility()
{
    cout << getName() << " uses Double Shot!\n";
    return getAttackDamage() * 2;
}
