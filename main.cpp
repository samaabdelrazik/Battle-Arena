#include "battle_arenahome.h"
#include <QApplication>
#include <QGraphicsScene>
#include "Character.h"
#include "Archer.h"
#include "Mage.h"
#include "Warrior.h"
#include "Enemy.h"
#include <QBrush>
#include <QGraphicsView>
#include <QMessageBox>
#include <QTimer>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Battle_Arenahome w;

    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600);

    Archer *archer = new Archer("Sama");
    archer->setRect(0,0,50,50);
    archer->setBrush(Qt::blue);

    Enemy *enemy = new Enemy("enemy");
    enemy->setRect(300,0,50,50);
    enemy->setBrush(Qt::red);

    scene->addItem(archer);
    scene->addItem(enemy);

    archer->setFlag(QGraphicsItem::ItemIsFocusable);
    archer->setFocus();

    Character* player = archer;

    QObject::connect(player, &Character::characterDied, [&](Character* dead) {
        int score = player->calculateScore();

        QMessageBox::information(nullptr, "Game Over",
                                 "You lost!\nScore: " + QString::number(score));
    });


    QGraphicsView *view = new QGraphicsView(scene);

    w.show();
    QObject::connect(&w, &Battle_Arenahome::startButtonClicked, [&](){view->show(); w.close();});
    QObject::connect(&w, &Battle_Arenahome::exitButtonClicked, [&](){w.close();});
    QTimer *timer = new QTimer();

    QObject::connect(timer, &QTimer::timeout, [=]() {
        enemy->updateLocation(*player);
        enemy->attack(*player);
    });

    timer->start(20);
    return a.exec();
}
