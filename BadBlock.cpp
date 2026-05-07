#include "BadBlock.h"
#include "Character.h"
#include <QBrush>

BadBlock::BadBlock()
{
    setBrush(Qt::red);
}

void BadBlock::applyEffect(Character* character)
{
    character->decreaseHealth(15);
    character->decreaseScore(5);
    character->decreasePower(1);
}
