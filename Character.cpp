#include "Character.h"
#include "Block.h"
#include "Platform.h"

#include <iostream>
#include <QGraphicsScene>
#include <QList>
#include <cmath>

using namespace std;

int Character::characterCount = 0;

Character::Character(string name, int attackDamage, int health)
{
    this->name = name;
    this->attackDamage = attackDamage;
    this->health = health;
    characterCount++;


    baseAcceleration = acceleration;
    baseMaxMoveSpeed = maxMoveSpeed;
    baseMaxJumps = maxJumps;
}

Character::~Character()
{
    characterCount--;
}

string Character::getName() const
{
    return name;
}

int Character::getAttackDamage() const
{
    return attackDamage;
}

int Character::getHealth() const
{
    return health;
}

int Character::getScore() const
{
    return score;
}

int Character::getSpecialAbilityUse() const
{
    return specialAbilityUse;
}
int Character::attack(){
    damageDealt += attackDamage;
    return attackDamage;
}
void Character::takeDamage(int damage){
    health -= damage;

    if (health > 0){
        cout << name << "'s Remaining health: " << health << endl;
    }
    else{
        health = 0;
        cout << name << "'s Remaining health: " << health << " ;character is dead" << endl;

        emit characterDied(this);
    }

}

bool Character::isAlive() const{
    if(health > 0){
        return true;
    }

    cout << name << " gained health. Current health: " << health << endl;
}

void Character::decreaseHealth(int amount)
{
    takeDamage(amount);
}

void Character::increaseScore(int amount)
{
    score += amount;
    cout << name << " gained score. Current score: " << score << endl;
}

void Character::decreaseScore(int amount)
{
    score -= amount;

    if (score < 0) {
        score = 0;
    }

    cout << name << " lost score. Current score: " << score << endl;
}

void Character::increasePower(int amount)
{
    specialAbilityUse += amount;
    cout << name << " power increased. Current power: " << specialAbilityUse << endl;
}

void Character::decreasePower(int amount)
{
    specialAbilityUse -= amount;

    if (specialAbilityUse < 0) {
        specialAbilityUse = 0;
    }

    cout << name << " power decreased. Current power: " << specialAbilityUse << endl;
}

int Character::getCharacterCount()
{
    return characterCount;
}

void Character::jump()
{
    if (jumpCount < maxJumps) {
        velocityY = jumpStrength;
        onGround = false;
        jumpCount++;
    }
}

void Character::activateBonuses()
{
    if (!speedBoostActive) {
        speedBoostActive = true;
        acceleration = baseAcceleration + 0.4;
        maxMoveSpeed = baseMaxMoveSpeed + 2.0;
        cout << name << " activated BonusSpeed!" << endl;
    }

    if (!jumpBoostActive) {
        jumpBoostActive = true;
        maxJumps = 2;
        cout << name << " activated BonusJump!" << endl;
    }
}

void Character::deactivateBonuses()
{
    if (speedBoostActive || jumpBoostActive) {
        cout << name << "'s bonuses were removed." << endl;
    }

    speedBoostActive = false;
    jumpBoostActive = false;

    acceleration = baseAcceleration;
    maxMoveSpeed = baseMaxMoveSpeed;
    maxJumps = baseMaxJumps;

    if (jumpCount > maxJumps) {
        jumpCount = maxJumps;
    }
}

void Character::registerHit(int damage)
{
    takeDamage(damage);
    timeSinceLastHit = 0.0;
    deactivateBonuses();
}

void Character::updateMovement()
{

    timeSinceLastHit += 0.016;

    if (timeSinceLastHit >= 5.0) {
        activateBonuses();
    }


    if (movingLeft) {
        velocityX -= acceleration;
    }
    if (movingRight) {
        velocityX += acceleration;
    }


    if (!movingLeft && !movingRight) {
        velocityX *= friction;

        if (abs(velocityX) < 0.2) {
            velocityX = 0;
        }
    }


    if (velocityX > maxMoveSpeed) {
        velocityX = maxMoveSpeed;
    }
    if (velocityX < -maxMoveSpeed) {
        velocityX = -maxMoveSpeed;
    }


    if (velocityX != 0) {
        moveBy(velocityX, 0);

        QList<QGraphicsItem*> horizontalCollisions = collidingItems();
        for (QGraphicsItem *item : horizontalCollisions) {
            Platform *platform = dynamic_cast<Platform*>(item);
            if (platform) {
                QRectF charRect = sceneBoundingRect();
                QRectF platformRect = platform->sceneBoundingRect();

                bool standingOnTop =
                    charRect.bottom() <= platformRect.top() + 5 &&
                    charRect.bottom() >= platformRect.top() - 5;

                if (!standingOnTop) {
                    moveBy(-velocityX, 0);
                    velocityX = 0;
                    break;
                }
            }
        }
    }


    if (velocityY < 0) {
        velocityY += gravity;
    } else {
        velocityY += fallGravity;
    }

    if (velocityY > maxFallSpeed) {
        velocityY = maxFallSpeed;
    }


    moveBy(0, velocityY);
    onGround = false;

    QList<QGraphicsItem*> verticalCollisions = collidingItems();
    for (QGraphicsItem *item : verticalCollisions) {
        Platform *platform = dynamic_cast<Platform*>(item);
        if (platform) {
            QRectF charRect = sceneBoundingRect();
            QRectF platformRect = platform->sceneBoundingRect();


            if (velocityY >= 0 &&
                charRect.bottom() >= platformRect.top() &&
                charRect.bottom() - velocityY <= platformRect.top() + 5) {

                setY(platformRect.top() - rect().height());
                velocityY = 0;
                onGround = true;
                jumpCount = 0;
            }

            else if (velocityY < 0 &&
                     charRect.top() <= platformRect.bottom() &&
                     charRect.top() - velocityY >= platformRect.bottom() - 5) {

                setY(platformRect.bottom());
                velocityY = 0;
            }
        }
    }


    if (scene()) {
        QRectF sceneRect = scene()->sceneRect();

        if (x() < sceneRect.left()) {
            setX(sceneRect.left());
            velocityX = 0;
        }

        if (x() + rect().width() > sceneRect.right()) {
            setX(sceneRect.right() - rect().width());
            velocityX = 0;
        }
    }


    QList<QGraphicsItem*> collidedItems = collidingItems();
    for (QGraphicsItem *item : collidedItems) {
        Block *block = dynamic_cast<Block*>(item);
        if (block) {
            block->applyEffect(this);

            if (scene()) {
                scene()->removeItem(block);
            }

            delete block;
            break;
        }
    }
}

int Character::calculateScore() const {
    return damageDealt * 10;
}

void Character::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left) {
        movingLeft = true;
    }
    else if (event->key() == Qt::Key_Right) {
        movingRight = true;
    }
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Space) {
        jump();
    }
}

void Character::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left) {
        movingLeft = false;
    }
    else if (event->key() == Qt::Key_Right) {
        movingRight = false;
    }
}
