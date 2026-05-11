#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsPixmapItem>

class Character;

class Block : public QGraphicsPixmapItem
{
public:
    Block();

    virtual void applyEffect(Character* character) = 0;

    ~Block();

private:
    QTimer *timer;
    float flipAngle =0;
};

#endif
