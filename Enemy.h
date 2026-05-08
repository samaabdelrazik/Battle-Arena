#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"
#include <string>
#include <QBrush>
using namespace std;

enum class State{Chase, Telegraph, Attack, CoolDown, Jump};


class Enemy: public Character
{
private:
    State currentState = State::Chase;
    int attackCoolDown;
    int telegraphTimer = 0;
    int cooldownTimer  = 0;
    int jumpCooldown   = 0;
    float speed        = 2.5f;
    float jumpStrength = -12.0f;

public:
    
    Enemy(string name);
    void updateLocation( Character &player);
    
    int specialAbility();

    void attack( Character &player);
    void handleChase( Character &player);
    void handleCooldown();
    void handleTelegraph();
    void handleGravity();
};

#endif // ENEMY_H
