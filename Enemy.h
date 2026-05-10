#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

#include <string>
#include <QBrush>
#include <QPointF>
#include <QGraphicsRectItem>

using namespace std;

enum class State
{
    Chase,
    Telegraph,
    Attack,
    CoolDown,
    Jump
};

class Enemy : public Character
{
private:
    State currentState = State::Chase;

    int attackCoolDown;
    int telegraphTimer = 0;
    int cooldownTimer = 0;
    int jumpCooldown = 0;

    float speed = 2.5f;
    float jumpStrength = -12.0f;

    QGraphicsRectItem *healthBarBack = nullptr;
    QGraphicsRectItem *healthBarFill = nullptr;

    int maxEnemyHealth = 100;

public:
    Enemy(string name);

    void updateLocation(Character &player);

    int specialAbility() override;
    void basicAttack(QPointF targetPoint) override;

    void takeDamage(int damage);

    void attack(Character &player);
    void handleChase(Character &player);
    void handleCooldown();
    void handleTelegraph();
    void handleGravity();

    void updateHealthBar();

    void resetEnemyState();
};

#endif
