#include "Character.h"
#include <iostream>
#include <QKeyEvent>

using namespace std;

int Character::characterCount = 0;

string Character::getName() const{
    return name;
}
int Character::getAttackDamage() const{
    return attackDamage;
}
int Character::getHealth() const{
    return health;
}
Character::Character(string name, int attackDamage,int health){
    this->name = name;
    this->attackDamage = attackDamage;
    this->health = health;
    characterCount++;
}
Character::~Character(){
    characterCount--;
}
int Character::attack() const{
    return attackDamage;
}
void Character::takeDamage(int damage){
    health = health - damage;
    if (health >= 0){
        cout << name << "'s Remaining health: " << health << endl;
    }
    else{
        health = 0;
        cout << name << "'s Remaining health: " << health << " ;character is dead" << endl;
    }
}
bool Character::isAlive() const{
    if(health > 0){
        return true;
    }
        else return false;
}

int Character::getCharacterCount(){
    return characterCount;
}

int Character::getSpecialAbilityUse() const{
    return specialAbilityUse;
}

void Character::keyPressEvent(QKeyEvent *event)
{
    if(event->key()== Qt::Key_Left) moveBy(-10, 0);
    if(event->key()== Qt::Key_Right) moveBy(10,0);
    if(event->key()== Qt::Key_Up) moveBy(0, -10);
    if(event->key()== Qt::Key_Down) moveBy(0, 10);
}

