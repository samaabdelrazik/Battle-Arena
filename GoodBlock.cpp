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
    character->increaseHealth(20);
    character->increaseScore(10);
    character->increasePower(1);
}

GoodBlock::~GoodBlock()
{
}
