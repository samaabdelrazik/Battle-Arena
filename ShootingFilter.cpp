#pragma once

#include <QObject>
#include <QPoint>
#include "Archer.h"
#include "Warrior.h"
#include "Mage.h"
#include <QGraphicsView>
#include <QApplication>

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
