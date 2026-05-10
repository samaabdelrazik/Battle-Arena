#include "BadBlock.h"
#include "Character.h"
#include <QBrush>

BadBlock::BadBlock()
{
    setPixmap(QPixmap(":/images/bomb.png").scaled(40,40));
}

void BadBlock::applyEffect(Character* character)
{
    character->decreaseHealth(15);
    character->decreaseScore(5);
    character->decreasePower(1);
}

BadBlock::~BadBlock()
{
}
