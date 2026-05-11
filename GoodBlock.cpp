#include "GoodBlock.h"
#include "Character.h"

#include <QBrush>
#include <QTimer>
#include <QDirIterator>

GoodBlock::GoodBlock()
{
    QPixmap pix(":/images/coin.png");

    if (pix.isNull()) {
        qWarning() << "IMAGE LOAD FAILED! Path tried: :/images/coin.png";
        // Let's see what is actually inside your resources right now
        QDirIterator it(":", QDirIterator::Subdirectories);
        while (it.hasNext()) {
            qDebug() << "Available resource:" << it.next();
        }
    } else {
        setPixmap(pix.scaled(40,40));
    }
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
