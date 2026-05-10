#include "battle_arenahome.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QProgressBar>

#include "Character.h"
#include "Archer.h"
#include "Mage.h"
#include "Warrior.h"
#include "GoodBlock.h"
#include "BadBlock.h"
#include "Platform.h"
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
    scene->setSceneRect(0, 0, 1000, 700);


    Archer *archer = new Archer("Sama");
    archer->setRect(0, 0, 50, 50);
    archer->setPos(100, 100);


   
    scene->addItem(archer);


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


    Enemy *enemy = new Enemy("enemy");
    enemy->setRect(300,0,50,50);
    enemy->setBrush(Qt::red);
    scene->addItem(enemy);


    scene->setBackgroundBrush(QBrush(QPixmap(":/images/forest.jpg").scaled(1000, 700, Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    archer->setFlag(QGraphicsItem::ItemIsFocusable);
    archer->setFocus();

    QGraphicsView *view = new QGraphicsView(scene);
    view->setWindowTitle("Battle_Arena");
    view->resize(1020, 740);
    Character* player = archer;

    QProgressBar *healthBar = new QProgressBar(view);
    QProgressBar *scoreBar = new QProgressBar(view);

    healthBar->setGeometry(20, 20, 250, 30);
    scoreBar->setGeometry(20, 60, 250, 30);

    healthBar->setRange(0, 100);
    healthBar->setValue(player->getHealth());

    scoreBar->setRange(0, 500);
    scoreBar->setValue(player->getScore());

    healthBar->setFormat("Health: %v");
    scoreBar->setFormat("Score: %v");

    healthBar->setStyleSheet(
        "QProgressBar {"
        "border: 2px solid white;"
        "border-radius: 5px;"
        "text-align: center;"
        "font-weight: bold;"
        "background-color: black;"
        "color: white;"
        "}"
        "QProgressBar::chunk {"
        "background-color: red;"
        "}"
        );

    scoreBar->setStyleSheet(
        "QProgressBar {"
        "border: 2px solid white;"
        "border-radius: 5px;"
        "text-align: center;"
        "font-weight: bold;"
        "background-color: black;"
        "color: white;"
        "}"
        "QProgressBar::chunk {"
        "background-color: green;"
        "}"
        );

    healthBar->show();
    scoreBar->show();

    QObject::connect(player, &Character::characterDied, [&](Character* dead) {
        int score = player->calculateScore();

        QMessageBox::information(nullptr, "Game Over",
                                 "You lost!\nScore: " + QString::number(score));
    });




    w.show();


    QObject::connect(&w, &Battle_Arenahome::startButtonClicked, [&](){view->show(); w.close();});
    QObject::connect(&w, &Battle_Arenahome::exitButtonClicked, [&](){w.close();});

    QTimer *timer = new QTimer();

    QObject::connect(timer, &QTimer::timeout, [=]() {
        archer->updateMovement();
        enemy->updateLocation(*player);
        healthBar->setValue(player->getHealth());

        scoreBar->setValue(player->getScore());
    });

    timer->start(16);
    return a.exec();
}
