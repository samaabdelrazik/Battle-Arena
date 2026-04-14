#include "battle_arenahome.h"
#include <QApplication>
#include <QGraphicsScene>
#include "Character.h"
#include "Archer.h"
#include "Mage.h"
#include "Warrior.h"
#include <QBrush>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Battle_Arenahome w;

    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600);

    Archer *archer = new Archer("Sama");
    archer->setRect(0,0,50,50);
    archer->setBrush(Qt::blue);

    Mage *mage = new Mage("player2");
    mage->setRect(0, 60, 50, 50);

    Warrior *warrior = new Warrior("player3");
    warrior->setRect(0, 120, 50, 50);

    scene->addItem(archer);
    scene->addItem(warrior);
    scene->addItem(mage);
    archer->setFlag(QGraphicsItem::ItemIsFocusable);
    archer->setFocus();

    warrior->setFlag(QGraphicsItem::ItemIsFocusable);
    warrior->setFocus();

    mage->setFlag(QGraphicsItem::ItemIsFocusable);
    mage->setFocus();

    QGraphicsView *view = new QGraphicsView(scene);

    w.show();
    QObject::connect(&w, &Battle_Arenahome::startButtonClicked, [&](){view->show(); w.close();});
    QObject::connect(&w, &Battle_Arenahome::exitButtonClicked, [&](){w.close();});
    return a.exec();
}
