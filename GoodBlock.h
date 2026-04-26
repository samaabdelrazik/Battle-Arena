#ifndef GOODBLOCK_H
#define GOODBLOCK_H

#include "Block.h"

class GoodBlock : public Block
{
public:
    GoodBlock();

    void applyEffect(Character* character) override;
};

#endif
