#include "BadBlock.h"
#include "Character.h"

#include <QBrush>

BadBlock::BadBlock()
{
    setRect(0, 0, 40, 40);
    setBrush(QBrush(Qt::red));
}

void BadBlock::applyEffect(Character* character)
{
    character->decreaseHealth(10);
    character->decreaseScore(10);
    character->decreaseSpecialBar(5.0);
}
