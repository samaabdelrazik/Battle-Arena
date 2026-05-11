#include "BadBlock.h"
#include "Character.h"

#include <QBrush>

BadBlock::BadBlock()
{
    setPixmap(QPixmap(":/images/bomb.png").scaled(40,40));
}

void BadBlock::applyEffect(Character* character)
{
    character->decreaseHealth(10);
    character->decreaseScore(10);
    character->decreaseSpecialBar(5.0);
}

BadBlock::~BadBlock()
{
}
