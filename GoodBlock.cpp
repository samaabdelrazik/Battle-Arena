#include "GoodBlock.h"
#include "Character.h"

#include <QBrush>
#include <QTimer>

GoodBlock::GoodBlock()
{
    setPixmap(QPixmap(":/images/coin.png").scaled(40,40));
}

void GoodBlock::applyEffect(Character* character)
{
    character->increaseHealth(10);
    character->increaseScore(10);
    character->increaseSpecialBar(15.0);
}

GoodBlock::~GoodBlock()
{
}
