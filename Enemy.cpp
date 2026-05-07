#include "Enemy.h"

Enemy::Enemy(string name): Character(name, 30, 100) ,attackCoolDown(0){}

void Enemy::updateLocation(const Character &player)
{
    if(this->x() < player.x())
        this->setX(x()+1);

    else if(this->x()> player.x())
        this->setX(x()-1);

    if(this->y() < player.y())
        this->setY(y()+1);

    else if(this->y()> player.y())
        this->setY(y()-1);
}

void Enemy::attack(Character &player)
{
    auto dy = abs(this->y()-player.y());
    auto dx = abs(this->x()-player.x());

    if (attackCoolDown<=0)
    {
        if(dy <=1 && dx<=1){
            player.takeDamage(this->Character::attack());
            this->setX(this->x()+40);
            attackCoolDown =20;
        }
    }
    else
        attackCoolDown--;


}
int Enemy::specialAbility(){}
