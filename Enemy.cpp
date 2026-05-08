#include "Enemy.h"
#include "Platform.h"
Enemy::Enemy(string name): Character(name, 30, 100) ,attackCoolDown(0){setBrush(Qt::red);}

void Enemy::updateLocation(Character &player)
{
    handleGravity();
    switch (currentState)
    {
        case State::Chase:
            handleChase(player);
            break;
        case State::Telegraph:
            handleTelegraph();
            break;
        case State::Attack:
            attack(player);
            break;
        case State::CoolDown:
            handleCooldown();
            break;
        case State::Jump:
            if(onGround)
                currentState = State::Chase;
                break;
    }
}

void Enemy::handleChase( Character &player)
{
    float dx = player.x()- x();
    float dy = player.y()- y();
    float dis = sqrt(pow(dy,2)+ pow(dx, 2));

    if(dx> 5)
        moveBy(speed, 0);
    else if(dx<-5) 
        moveBy(-speed, 0);

    if(jumpCooldown >0) 
        jumpCooldown--;

    if(dy < -60 && onGround && jumpCooldown == 0)
    {
        velocityY = jumpStrength;
        onGround = false;
        jumpCooldown = 60;
        currentState = State::Jump;
        return;
    }

    if(dis<70)
    {
        currentState = State::Telegraph;
        telegraphTimer = 30;
        setBrush(Qt::yellow);
    }
}

void Enemy::handleTelegraph()
{
    telegraphTimer--;

    if(telegraphTimer%10 <5)
        setBrush(Qt::yellow);
    else
        setBrush(Qt::red);

    if(telegraphTimer<=0)
    {
        currentState = State::Attack;
        setBrush(Qt::darkRed);
    }
}

void Enemy::attack(Character &player)
{
    auto dy = abs(this->y()-player.y());
    auto dx = abs(this->x()-player.x());

    if (dx<70 && dy <60)
    {
        player.takeDamage(getAttackDamage());

        float knockDir = (player.x() > x()) ? 40.0f : -40.0f;
        player.setX(player.x() + knockDir);
        player.setY(player.y() -5);
    }
    cooldownTimer = 45;
    currentState = State::CoolDown;
}

void Enemy::handleCooldown()
{
    cooldownTimer--;
    if(cooldownTimer <= 0)
    {
        setBrush(Qt::red);
        currentState = State::Chase;
    }
}
int Enemy::specialAbility(){}

void Enemy::handleGravity()
{
    if(velocityY < 0)
    velocityY += gravity;
    else
        velocityY += fallGravity;

    if(velocityY > maxFallSpeed)
        velocityY = maxFallSpeed;

    moveBy(0, velocityY);
    onGround = false;

    QList<QGraphicsItem*> verticalCollisions = collidingItems();
    for(QGraphicsItem *item: verticalCollisions)
    {
        Platform *platform = dynamic_cast<Platform*>(item);
        if(platform)
        {
            QRectF eRect = sceneBoundingRect();
            QRectF pRect = platform->sceneBoundingRect();

            if (velocityY >= 0 &&
                eRect.bottom() >= pRect.top() &&
                eRect.bottom() - velocityY <= pRect.top() + 5)
            {
                setY(pRect.top() - rect().height());
                velocityY = 0;
                onGround  = true;
            }
        }
    }

    

}
