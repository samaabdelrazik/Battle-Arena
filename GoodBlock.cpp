#include "GoodBlock.h"
#include "Character.h"

#include <QBrush>

GoodBlock::GoodBlock()
{
    setRect(0, 0, 40, 40);
    setBrush(QBrush(Qt::green));
}

void GoodBlock::applyEffect(Character* character)
{
    character->increaseHealth(10);
    character->increaseScore(10);
    character->increaseSpecialBar(15.0);
}
