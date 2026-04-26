#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <QGraphicsRectItem>
#include <QKeyEvent>

using namespace std;

class Character: public QObject, public QGraphicsRectItem
{

    Q_OBJECT
private:
    string name;
    int health = 0;
    int attackDamage = 0;
    int specialAbilityUse = 1;
    int damageDealt;
    static int characterCount;


    double velocityX = 0.0;
    double velocityY = 0.0;

    double gravity = 0.6;
    double fallGravity = 1.0;
    double jumpStrength = -16.0;
    double maxFallSpeed = 18.0;

    double acceleration = 0.8;
    double friction = 0.75;
    double maxMoveSpeed = 6.5;

    bool movingLeft = false;
    bool movingRight = false;
    bool onGround = false;

    int jumpCount = 0;
    int maxJumps = 1;


    double timeSinceLastHit = 0.0;
    bool speedBoostActive = false;
    bool jumpBoostActive = false;


    double baseAcceleration = 0.8;
    double baseMaxMoveSpeed = 6.5;
    int baseMaxJumps = 1;

public:
    Character(string name, int attackDamage, int health);
    virtual ~Character();

    string getName() const;
    int getAttackDamage() const;
    int getHealth() const;

    int getScore() const;
    int getSpecialAbilityUse() const;

    virtual int attack() const;
    virtual int specialAbility() = 0;
    
    virtual ~Character();
    void takeDamage(int damage);
    bool isAlive() const;

    void increaseHealth(int amount);
    void decreaseHealth(int amount);

    void increaseScore(int amount);
    void decreaseScore(int amount);

    void increasePower(int amount);
    void decreasePower(int amount);

    static int getCharacterCount();

    int calculateScore() const;
    void keyPressEvent(QKeyEvent *event);


    void updateMovement();
    void jump();

    void activateBonuses();
    void deactivateBonuses();
    void registerHit(int damage);

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    signals:
    void characterDied(Character* deadCharacter);
};

#endif
