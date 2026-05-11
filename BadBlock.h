#ifndef BADBLOCK_H
#define BADBLOCK_H

#include "Block.h"
#include <QTimer>

class BadBlock : public Block
{
public:
    BadBlock();

    void applyEffect(Character* character) override;
    ~BadBlock();

};

#endif
