#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"
#include <string>
using namespace std;

class Enemy: public Character
{
public:
    int attackCoolDown;
    Enemy(string name);
    void updateLocation(const Character &player);
    void attack(Character &player);
    int specialAbility();
};

#endif // ENEMY_H
