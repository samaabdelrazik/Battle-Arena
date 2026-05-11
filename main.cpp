#include "battle_arenahome.h"
#include "CharacterSelect.h"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMessageBox>
#include <QMouseEvent>
#include <QCursor>
#include <QList>
#include <QScrollBar>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QProgressBar>

#include "Character.h"
#include "Archer.h"
#include "Mage.h"
#include "Warrior.h"
#include "GoodBlock.h"
#include "BadBlock.h"
#include "Block.h"
#include "Platform.h"
#include "Enemy.h"
#include "Arrow.h"

class ShootingFilter : public QObject
{
private:
    Character *player;
    QGraphicsView *view;

    bool mouseHeld = false;
    int normalShootCounter = 0;
    int warriorHoldCounter = 0;
    bool warriorHoldAttacking = false;

public:
    ShootingFilter(Character *player, QGraphicsView *view)
    {
        this->player = player;
        this->view = view;
    }

    void shootAttack()
    {
        if (!player || !player->isAlive())
        {
            return;
        }

        if (player->isSpecialActive() &&
            dynamic_cast<Mage*>(player))
        {
            return;
        }

        QPoint mousePos = view->mapFromGlobal(QCursor::pos());
        QPointF scenePos = view->mapToScene(mousePos);

        player->basicAttack(scenePos);
    }

    void updateNormalShooting()
    {
        bool leftButtonDown =
            QApplication::mouseButtons() & Qt::LeftButton;

        if (leftButtonDown && !mouseHeld)
        {
            mouseHeld = true;
            normalShootCounter =
                player ? player->getAttackCooldownFrames() : 0;

            warriorHoldCounter = 8;
            warriorHoldAttacking = false;

            if (player)
            {
                view->setFocus();
                view->viewport()->setFocus();
                player->setFocus();
            }
        }
        else if (!leftButtonDown && mouseHeld)
        {
            mouseHeld = false;
            normalShootCounter = 0;
            warriorHoldCounter = 0;
            warriorHoldAttacking = false;

            if (Archer *archer =
                    dynamic_cast<Archer*>(player))
            {
                archer->stopRapidFire();
            }

            if (Warrior *warrior =
                    dynamic_cast<Warrior*>(player))
            {
                warrior->stopRapidAttack();
            }

            if (Mage *mage =
                    dynamic_cast<Mage*>(player))
            {
                mage->stopRapidAttack();
            }
        }

        if (!mouseHeld || !player || !player->isAlive())
        {
            return;
        }

        normalShootCounter++;

        if (Warrior *warrior =
                dynamic_cast<Warrior*>(player))
        {
            warriorHoldCounter++;

            if (warriorHoldCounter >= 8)
            {
                QPoint mousePos =
                    view->mapFromGlobal(QCursor::pos());

                QPointF scenePos =
                    view->mapToScene(mousePos);

                if (warrior->isSpecialActive())
                {
                    warrior->basicAttack(scenePos);
                }
                else
                {
                    warrior->rapidAttack(scenePos);
                }

                warriorHoldAttacking = true;
            }

            return;
        }

        if (normalShootCounter >= player->getAttackCooldownFrames())
        {
            Archer *archer =
                dynamic_cast<Archer*>(player);

            if (archer && !archer->isSpecialActive())
            {
                QPoint mousePos =
                    view->mapFromGlobal(QCursor::pos());

                QPointF scenePos =
                    view->mapToScene(mousePos);

                archer->rapidAttack(scenePos);
            }
            else if (Warrior *warrior =
                         dynamic_cast<Warrior*>(player))
            {
                QPoint mousePos =
                    view->mapFromGlobal(QCursor::pos());

                QPointF scenePos =
                    view->mapToScene(mousePos);

                warrior->rapidAttack(scenePos);
            }
            else
            {
                if (Mage *mage =
                        dynamic_cast<Mage*>(player))
                {
                    if (!mage->isSpecialActive())
                    {
                        QPoint mousePos =
                            view->mapFromGlobal(QCursor::pos());

                        QPointF scenePos =
                            view->mapToScene(mousePos);

                        mage->rapidAttack(scenePos);
                    }
                }
                else
                {
                    shootAttack();
                }
            }

            normalShootCounter = 0;
        }
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        Q_UNUSED(obj);

        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent =
                static_cast<QMouseEvent*>(event);

            if (mouseEvent->button() == Qt::LeftButton)
            {
                mouseHeld = true;

                normalShootCounter =
                    player ? player->getAttackCooldownFrames() : 0;

                warriorHoldCounter = 8;
                warriorHoldAttacking = false;

                if (player)
                {
                    view->setFocus();
                    view->viewport()->setFocus();
                    player->setFocus();
                }

                return true;
            }
        }

        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent *mouseEvent =
                static_cast<QMouseEvent*>(event);

            if (mouseEvent->button() == Qt::LeftButton)
            {
                mouseHeld = false;
                normalShootCounter = 0;
                warriorHoldCounter = 0;
                warriorHoldAttacking = false;

                view->setFocus();
                view->viewport()->setFocus();

                Archer *archer =
                    dynamic_cast<Archer*>(player);

                if (archer)
                {
                    archer->stopRapidFire();
                }

                Warrior *warrior =
                    dynamic_cast<Warrior*>(player);

                if (warrior)
                {
                    warrior->stopRapidAttack();
                }

                Mage *mage =
                    dynamic_cast<Mage*>(player);

                if (mage)
                {
                    mage->stopRapidAttack();
                }

                return true;
            }
        }

        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent =
                static_cast<QKeyEvent*>(event);

            if (keyEvent->key() == Qt::Key_R)
            {
                if (!keyEvent->isAutoRepeat())
                {
                    if (player &&
                        player->canUseSpecial() &&
                        !player->isSpecialActive())
                    {
                        Archer *archer =
                            dynamic_cast<Archer*>(player);

                        if (archer)
                        {
                            archer->specialAbility();
                        }
                        else if (Warrior *warrior =
                                     dynamic_cast<Warrior*>(player))
                        {
                            QPoint mousePos =
                                view->mapFromGlobal(QCursor::pos());

                            QPointF scenePos =
                                view->mapToScene(mousePos);

                            warrior->stopRapidAttack();
                            warrior->startSpecial();
                            warrior->basicAttack(scenePos);
                        }
                        else
                        {
                            if (Mage *mage =
                                    dynamic_cast<Mage*>(player))
                            {
                                mage->stopRapidAttack();
                            }

                            player->startSpecial();
                        }
                    }
                }

                if (player)
                {
                    player->setFocus();
                }

                return true;
            }
        }

        return false;
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Battle_Arenahome home;
    CharacterSelect characterSelect;

    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 1000, 700);

    Character *player = nullptr;
    Archer *archerPlayer = nullptr;
    Mage *magePlayer = nullptr;

    QList<Enemy*> activeEnemies;

    int totalEnemiesSpawned = 0;
    int totalEnemiesKilled = 0;
    int framesSurvived = 0;

    const int MAX_TOTAL_ENEMIES = 20;
    const int MAX_ACTIVE_ENEMIES = 5;

    Platform *ground = new Platform(0, 650, 1000, 50);
    Platform *platform1 = new Platform(180, 520, 180, 20);
    Platform *platform2 = new Platform(450, 420, 180, 20);
    Platform *platform3 = new Platform(720, 320, 180, 20);

    scene->addItem(ground);
    scene->addItem(platform1);
    scene->addItem(platform2);
    scene->addItem(platform3);

    auto spawnBlocks = [&]() {
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
    };

    spawnBlocks();

    QGraphicsTextItem *specialText =
        new QGraphicsTextItem("Special: 0%");
    specialText->setDefaultTextColor(Qt::white);
    specialText->setFont(QFont("Arial", 14, QFont::Bold));
    specialText->setPos(20, 15);
    specialText->setZValue(1000);
    scene->addItem(specialText);

    QGraphicsRectItem *specialBarBack =
        new QGraphicsRectItem(20, 45, 220, 20);
    specialBarBack->setBrush(QBrush(Qt::darkGray));
    specialBarBack->setPen(QPen(Qt::white, 2));
    specialBarBack->setZValue(1000);
    scene->addItem(specialBarBack);

    QGraphicsRectItem *specialBarFill =
        new QGraphicsRectItem(22, 47, 0, 16);
    specialBarFill->setBrush(QBrush(Qt::green));
    specialBarFill->setPen(Qt::NoPen);
    specialBarFill->setZValue(1001);
    scene->addItem(specialBarFill);

    QGraphicsTextItem *specialStatusText =
        new QGraphicsTextItem("");
    specialStatusText->setDefaultTextColor(Qt::yellow);
    specialStatusText->setFont(QFont("Arial", 12, QFont::Bold));
    specialStatusText->setPos(20, 70);
    specialStatusText->setZValue(1000);
    scene->addItem(specialStatusText);

    QGraphicsTextItem *enemyCounterText =
        new QGraphicsTextItem("Enemies Defeated: 0 / 20");
    enemyCounterText->setDefaultTextColor(Qt::red);
    enemyCounterText->setFont(QFont("Arial", 14, QFont::Bold));
    enemyCounterText->setPos(700, 15);
    enemyCounterText->setZValue(1000);
    scene->addItem(enemyCounterText);

    QGraphicsTextItem *timerText =
        new QGraphicsTextItem("Time Left: 60");
    timerText->setDefaultTextColor(Qt::cyan);
    timerText->setFont(QFont("Arial", 14, QFont::Bold));
    timerText->setPos(700, 45);
    timerText->setZValue(1000);
    scene->addItem(timerText);

    QGraphicsView *view = new QGraphicsView(scene);
    view->setWindowTitle("Battle_Arena");
    view->resize(1020, 740);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFocusPolicy(Qt::StrongFocus);
    view->viewport()->setFocusPolicy(Qt::StrongFocus);

    QTimer *timer = new QTimer();
    ShootingFilter *filter = nullptr;

    auto createPlayer =
        [&](const QString &characterType) {
        if (player)
        {
            scene->removeItem(player);
            delete player;

            player = nullptr;
            archerPlayer = nullptr;
            magePlayer = nullptr;
        }

        if (characterType == "Archer")
        {
            player = new Archer("Player");
            archerPlayer = dynamic_cast<Archer*>(player);
            magePlayer = nullptr;
        }
        else if (characterType == "Mage")
        {
            player = new Mage("Player");
            archerPlayer = nullptr;
            magePlayer = dynamic_cast<Mage*>(player);
        }
        else
        {
            player = new Warrior("Player");
            archerPlayer = nullptr;
            magePlayer = nullptr;
        }

        player->setRect(0, 0, 50, 50);
        player->setPos(100, 600);
        player->setFlag(QGraphicsItem::ItemIsFocusable);
        player->setZValue(20);

        scene->addItem(player);
        player->setFocus();
    };

    auto spawnEnemy = [&]() {
        if (totalEnemiesSpawned >= MAX_TOTAL_ENEMIES ||
            !player)
        {
            return;
        }

        QString enemySprites[4] = {
            ":/sprites/minotaur_earth.png",
            ":/sprites/minotaur_frost.png",
            ":/sprites/minotaur_lava.png",
            ":/sprites/minotaur_lightning.png"
        };

        Enemy *enemy =
            new Enemy("enemy",
                      enemySprites[totalEnemiesSpawned % 4],
                      totalEnemiesSpawned);
        enemy->setRect(0, 0, 70, 70);

        int slot = activeEnemies.size();

        double spawnYPositions[5] = {
            580,
            580,
            450,
            350,
            250
        };

        double spawnY =
            spawnYPositions[slot % 5];

        double playerX = player->x();

        if (playerX < 500)
        {
            enemy->setPos(950 - slot * 70, spawnY);
        }
        else
        {
            enemy->setPos(0 + slot * 70, spawnY);
        }

        scene->addItem(enemy);
        activeEnemies.append(enemy);

        totalEnemiesSpawned++;
    };

    auto spawnInitialEnemies = [&]() {
        while (activeEnemies.size() < MAX_ACTIVE_ENEMIES &&
               totalEnemiesSpawned < MAX_TOTAL_ENEMIES)
        {
            spawnEnemy();
        }
    };

    auto clearArrowsAndBlocks = [&]() {
        QList<QGraphicsItem*> items = scene->items();

        for (QGraphicsItem *item : items)
        {
            Arrow *arrow =
                dynamic_cast<Arrow*>(item);

            Block *block =
                dynamic_cast<Block*>(item);

            if (arrow || block)
            {
                scene->removeItem(item);
                delete item;
            }
        }
    };

    auto clearEnemies = [&]() {
        for (Enemy *enemy : activeEnemies)
        {
            scene->removeItem(enemy);
            delete enemy;
        }

        activeEnemies.clear();
    };

    auto restartLevel = [&]() {
        clearArrowsAndBlocks();
        clearEnemies();

        totalEnemiesSpawned = 0;
        totalEnemiesKilled = 0;
        framesSurvived = 0;

        enemyCounterText->setPlainText(
            "Enemies Defeated: 0 / 20");

        timerText->setPlainText("Time Left: 60");

        if (player)
        {
            player->resetForLevel();
            player->setPos(100, 600);
            player->setFlag(QGraphicsItem::ItemIsFocusable);
            player->setFocus();
        }

        spawnBlocks();
        spawnInitialEnemies();

        timer->start(16);
    };

    auto startArena =
        [&](const QString &characterType) {
        createPlayer(characterType);

        if (filter)
        {
            a.removeEventFilter(filter);
            view->removeEventFilter(filter);
            view->viewport()->removeEventFilter(filter);

            delete filter;
            filter = nullptr;
        }

        filter = new ShootingFilter(player, view);

        a.installEventFilter(filter);
        view->installEventFilter(filter);
        view->viewport()->installEventFilter(filter);

        QObject::connect(player,
                         &Character::characterDied,
                         [&](Character* dead) {
            Q_UNUSED(dead);

            timer->stop();

            QMessageBox::information(nullptr,
                                     "Game Over",
                                     "You lost!");

            restartLevel();
        });

        restartLevel();

        characterSelect.close();
        view->show();

        view->setFocus();
        view->viewport()->setFocus();
        player->setFocus();
    };

    QObject::connect(timer,
                     &QTimer::timeout,
                     [&]() {
        if (!player)
        {
            return;
        }

        framesSurvived++;

        int secondsLeft =
            60 - (framesSurvived / 60);

        timerText->setPlainText(
            "Time Left: " +
            QString::number(secondsLeft));
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

        if (secondsLeft <= 0)
        {
            timer->stop();

            QMessageBox::information(
                nullptr,
                "Victory",
                "You survived for 1 minute!");

            restartLevel();

            return;
        }

        player->updateMovement();

        for (int i = activeEnemies.size() - 1;
             i >= 0;
             i--)
        {
            Enemy *enemy = activeEnemies[i];

            enemy->updateLocation(*player);

            if (!enemy->isAlive())
            {
                totalEnemiesKilled++;

                enemyCounterText->setPlainText(
                    "Enemies Defeated: " +
                    QString::number(totalEnemiesKilled) +
                    " / 20");

                activeEnemies.removeAt(i);

                scene->removeItem(enemy);
                delete enemy;

                if (totalEnemiesKilled >= MAX_TOTAL_ENEMIES)
                {
                    timer->stop();

                    QMessageBox::information(
                        nullptr,
                        "Victory",
                        "You defeated all 20 enemies!");

                    restartLevel();

                    return;
                }

                spawnEnemy();
            }
        }

        if (filter)
        {
            filter->updateNormalShooting();
        }

        double specialPercent =
            player->getSpecialBar();

        if (specialPercent < 0)
        {
            specialPercent = 0;
        }

        if (specialPercent > 100)
        {
            specialPercent = 100;
        }

        double barWidth =
            216.0 * (specialPercent / 100.0);

        specialBarFill->setRect(22, 47, barWidth, 16);

        specialText->setPlainText(
            "Special: " +
            QString::number((int)specialPercent) +
            "%");

        if (player->isSpecialActive())
        {
            specialStatusText->setPlainText(
                "SPECIAL ACTIVE");
        }
        else if (player->canUseSpecial())
        {
            specialStatusText->setPlainText(
                "READY - Press R");
        }
        else
        {
            specialStatusText->setPlainText("");
        }

        static int archerSpecialFireCounter = 0;

        if (archerPlayer &&
            archerPlayer->isSpecialActive())
        {
            archerSpecialFireCounter++;

            archerPlayer->decreaseSpecialBar(0.16);

            if (archerPlayer->getSpecialBar() <= 0)
            {
                archerPlayer->stopSpecial();
                archerPlayer->stopAnimationSequence();
                archerSpecialFireCounter = 0;
            }
            else if (archerSpecialFireCounter >= 7)
            {
                QPoint mousePos =
                    view->mapFromGlobal(QCursor::pos());

                QPointF scenePos =
                    view->mapToScene(mousePos);

                archerPlayer->basicAttack(scenePos);

                archerSpecialFireCounter = 0;
            }
        }
        else
        {
            archerSpecialFireCounter = 0;
        }

        static int magePulseCounter = 0;
        static int magePulseDelayCounter = 0;

        if (magePlayer &&
            magePlayer->isSpecialActive())
        {
            magePulseDelayCounter++;

            if (magePulseCounter == 0 ||
                magePulseDelayCounter >= 30)
            {
                magePlayer->arcaneStormPulse();

                magePlayer->decreaseSpecialBar(20.0);

                magePulseCounter++;
                magePulseDelayCounter = 0;

                if (magePulseCounter >= 5 ||
                    magePlayer->getSpecialBar() <= 0)
                {
                    magePlayer->stopSpecial();

                    magePulseCounter = 0;
                    magePulseDelayCounter = 0;
                }
            }
        }
        else
        {
            magePulseCounter = 0;
            magePulseDelayCounter = 0;
        }

        player->setFocus();
    });

    QObject::connect(&home,
                     &Battle_Arenahome::startButtonClicked,
                     [&]() {
        home.close();
        characterSelect.show();
    });

    QObject::connect(&home,
                     &Battle_Arenahome::exitButtonClicked,
                     [&]() {
        home.close();
    });

    QObject::connect(&characterSelect,
                     &CharacterSelect::archerMaleSelected,
                     [&]() {
        startArena("Archer");
    });

    QObject::connect(&characterSelect,
                     &CharacterSelect::archerFemaleSelected,
                     [&]() {
        startArena("Archer");
    });

    QObject::connect(&characterSelect,
                     &CharacterSelect::mageMaleSelected,
                     [&]() {
        startArena("Mage");
    });

    QObject::connect(&characterSelect,
                     &CharacterSelect::mageFemaleSelected,
                     [&]() {
        startArena("Mage");
    });

    QObject::connect(&characterSelect,
                     &CharacterSelect::warriorMaleSelected,
                     [&]() {
        startArena("Warrior");
    });

    QObject::connect(&characterSelect,
                     &CharacterSelect::warriorFemaleSelected,
                     [&]() {
        startArena("Warrior");
    });

    home.show();

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
