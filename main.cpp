#include "battle_arenahome.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

#include "Character.h"
#include "Archer.h"
#include "Mage.h"
#include "Warrior.h"
#include "GoodBlock.h"
#include "BadBlock.h"
#include "Platform.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Battle_Arenahome w;

    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1000, 700);


    Archer *archer = new Archer("Sama");
    archer->setRect(0, 0, 50, 50);
    archer->setPos(100, 100);

    Mage *mage = new Mage("player2");
    mage->setRect(0, 0, 50, 50);
    mage->setPos(200, 100);

    Warrior *warrior = new Warrior("player3");
    warrior->setRect(0, 0, 50, 50);
    warrior->setPos(300, 100);

    scene->addItem(archer);
    scene->addItem(mage);
    scene->addItem(warrior);


    Platform *ground = new Platform(0, 650, 1000, 50);
    Platform *platform1 = new Platform(180, 520, 180, 20);
    Platform *platform2 = new Platform(450, 420, 180, 20);
    Platform *platform3 = new Platform(720, 320, 180, 20);

    scene->addItem(ground);
    scene->addItem(platform1);
    scene->addItem(platform2);
    scene->addItem(platform3);


    GoodBlock *good1 = new GoodBlock();
    good1->setPos(240, 480);
    scene->addItem(good1);

    GoodBlock *good2 = new GoodBlock();
    good2->setPos(510, 380);
    scene->addItem(good2);


    BadBlock *bad1 = new BadBlock();
    bad1->setPos(780, 280);
    scene->addItem(bad1);

    BadBlock *bad2 = new BadBlock();
    bad2->setPos(600, 610);
    scene->addItem(bad2);


    archer->setFlag(QGraphicsItem::ItemIsFocusable);
    archer->setFocus();

    QGraphicsView *view = new QGraphicsView(scene);
    view->setWindowTitle("Battle_Arena");
    view->resize(1020, 740);


    QTimer *timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [=]() {
        archer->updateMovement();
        mage->updateMovement();
        warrior->updateMovement();
    });
    timer->start(16);

    w.show();

    QObject::connect(&w, &Battle_Arenahome::startButtonClicked, [&]() {
        view->show();
        w.close();
    });

    QObject::connect(&w, &Battle_Arenahome::exitButtonClicked, [&]() {
        w.close();
    });

    return a.exec();
}
