#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <QGraphicsRectItem>
using namespace std;

class Character: public QGraphicsRectItem
{
private:
    string name;
    int health = 0;
    int attackDamage = 0;
    int specialAbilityUse = 1;
    int score = 0;
    static int characterCount;
public:
    int getAttackDamage() const;
    int getHealth() const;
    string getName() const;
    Character(string name, int attackDamage,int health);
    virtual ~Character();
    virtual int attack() const;
    void takeDamage(int damage);
    bool isAlive() const;

    // virtual int specialAbility() = 0;
    static int getCharacterCount();
    int getSpecialAbilityUse() const;
    void keyPressEvent(QKeyEvent *event);
};

#endif // CHARACTER_H

