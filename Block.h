#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsRectItem>

class Character;

class Block : public QGraphicsRectItem
{
public:
    Block();

    virtual void applyEffect(Character* character) = 0;
};

#endif
