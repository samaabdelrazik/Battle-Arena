#include "Enemy.h"
#include "Platform.h"

#include <cmath>
#include <QGraphicsScene>
#include <QBrush>
#include <QPen>

Enemy::Enemy(string name)
    : Character(name, 30, 100),
    attackCoolDown(0)
{
    setModernSpriteSheet(
        ":/sprites/minotaur_earth.png",
        96,
        96,
        0,
        3,
        0,
        1,
        2,
        2,
        4,
        5
    );

    setModernSpriteTargetSize(QSize(330, 330));
    setModernSpriteGroundOffset(83.0);
    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);

    healthBarBack = new QGraphicsRectItem(this);
    healthBarBack->setRect(0, -66, 100, 7);
    healthBarBack->setBrush(QBrush(Qt::darkGray));
    healthBarBack->setPen(QPen(Qt::black));

    healthBarFill = new QGraphicsRectItem(this);
    healthBarFill->setRect(0, -66, 100, 7);
    healthBarFill->setBrush(QBrush(Qt::green));
    healthBarFill->setPen(Qt::NoPen);
}

void Enemy::updateLocation(Character &player)
{
    updateHealthBar();

    if (!isAlive() || !player.isAlive()) {
        return;
    }

    handleGravity();

    double previousX = x();

    switch (currentState)
    {
    case State::Chase:
        handleChase(player);
        break;

    case State::Telegraph:
        handleTelegraph();
        break;

    case State::Attack:
        attack(player);
        break;

    case State::CoolDown:
        handleCooldown();
        break;

    case State::Jump:
        if (onGround) {
            currentState = State::Chase;
        }
        break;
    }

    bool movedHorizontally =
        std::abs(x() - previousX) > 0.01;

    bool facingRight =
        player.x() >= x();

    setAnimationMovementHint(movedHorizontally,
                             facingRight);

    stepSpriteAnimation();

    updateHealthBar();
}

void Enemy::handleChase(Character &player)
{
    float dx = player.x() - x();
    float dy = player.y() - y();

    float dis = sqrt(pow(dy, 2) + pow(dx, 2));

    if (dx > 5) {
        moveBy(speed, 0);
    }
    else if (dx < -5) {
        moveBy(-speed, 0);
    }

    if (jumpCooldown > 0) {
        jumpCooldown--;
    }

    if (dy < -60 && onGround && jumpCooldown == 0) {
        velocityY = jumpStrength;
        onGround = false;
        jumpCooldown = 60;
        currentState = State::Jump;
        return;
    }

    if (dis < 70) {
        currentState = State::Telegraph;
        telegraphTimer = 30;
    }
}

void Enemy::handleTelegraph()
{
    telegraphTimer--;

    if (telegraphTimer <= 0) {
        currentState = State::Attack;
    }
}

void Enemy::attack(Character &player)
{
    if (!player.isAlive()) {
        currentState = State::CoolDown;
        return;
    }

    playModernAttackAnimation(player.x() >= x());

    auto dy = std::abs(this->y() - player.y());
    auto dx = std::abs(this->x() - player.x());

    if (dx < 70 && dy < 60) {
        player.registerHit(getAttackDamage());

        float knockDir = (player.x() > x()) ? 20.0f : -20.0f;

        player.setX(player.x() + knockDir);
        player.setY(player.y() - 5);

        if (player.scene()) {
            QRectF sceneRect = player.scene()->sceneRect();

            if (player.x() < sceneRect.left()) {
                player.setX(sceneRect.left());
            }

            if (player.x() + player.rect().width() > sceneRect.right()) {
                player.setX(sceneRect.right() - player.rect().width());
            }
        }
    }

    cooldownTimer = 60;
    currentState = State::CoolDown;
}

void Enemy::handleCooldown()
{
    cooldownTimer--;

    if (cooldownTimer <= 0) {
        currentState = State::Chase;
    }
}

int Enemy::specialAbility()
{
    return getAttackDamage() * 2;
}

void Enemy::basicAttack(QPointF targetPoint)
{
    Q_UNUSED(targetPoint);
}

void Enemy::takeDamage(int damage)
{
    Character::takeDamage(damage);

    updateHealthBar();

    if (!isAlive()) {

        hide();

        if (healthBarBack) {
            healthBarBack->hide();
        }

        if (healthBarFill) {
            healthBarFill->hide();
        }
    }
}

void Enemy::handleGravity()
{
    if (velocityY < 0) {
        velocityY += gravity;
    }
    else {
        velocityY += fallGravity;
    }

    if (velocityY > maxFallSpeed) {
        velocityY = maxFallSpeed;
    }

    moveBy(0, velocityY);
    onGround = false;

    QList<QGraphicsItem*> verticalCollisions = collidingItems();

    for (QGraphicsItem *item : verticalCollisions) {

        Platform *platform = dynamic_cast<Platform*>(item);

        if (platform) {

            QRectF eRect = sceneBoundingRect();
            QRectF pRect = platform->sceneBoundingRect();

            if (velocityY >= 0 &&
                eRect.bottom() >= pRect.top() &&
                eRect.bottom() - velocityY <= pRect.top() + 5) {

                setY(pRect.top() - rect().height());

                velocityY = 0;
                onGround = true;
            }
        }
    }
}

void Enemy::updateHealthBar()
{
    if (!healthBarBack || !healthBarFill) {
        return;
    }

    if (!isAlive()) {

        healthBarBack->hide();
        healthBarFill->hide();

        return;
    }

    healthBarBack->show();
    healthBarFill->show();

    double healthPercent =
        static_cast<double>(getHealth()) /
        maxEnemyHealth;

    if (healthPercent < 0.0) {
        healthPercent = 0.0;
    }

    if (healthPercent > 1.0) {
        healthPercent = 1.0;
    }

    double barWidth = 100.0 * healthPercent;

    healthBarBack->setRect(0, -66, 100, 7);
    healthBarFill->setRect(0, -66, barWidth, 7);
}

void Enemy::resetEnemyState()
{
    resetForLevel();

    currentState = State::Chase;

    attackCoolDown = 0;
    telegraphTimer = 0;
    cooldownTimer = 0;
    jumpCooldown = 0;

    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);

    show();

    if (healthBarBack) {
        healthBarBack->show();
    }

    if (healthBarFill) {
        healthBarFill->show();
    }

    updateHealthBar();
}
