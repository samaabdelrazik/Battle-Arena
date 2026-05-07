#include "GoodBlock.h"
#include "Character.h"
#include <QBrush>

GoodBlock::GoodBlock()
{
    setBrush(Qt::green);
}

void GoodBlock::applyEffect(Character* character)
{
    character->increaseHealth(20);
    character->increaseScore(10);
    character->increasePower(1);
}
