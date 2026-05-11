#include "Enemy.h"
#include "Platform.h"

#include <cmath>
#include <QGraphicsScene>
#include <QBrush>
#include <QPen>
#include <algorithm>

Enemy::Enemy(string name,
             const QString &spritePath,
             int formationIndex)
    : Character(name, 30, 100),
    attackCoolDown(0),
    formationIndex(formationIndex)
{
    setModernSpriteSheet(
        spritePath,
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

    setModernSpriteTargetSize(QSize(200, 200));
    setModernSpriteGroundOffset(48.0);
    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);
    setZValue(5);

    healthBarBack = new QGraphicsRectItem(this);
    healthBarBack->setRect(0, -42, 70, 7);
    healthBarBack->setBrush(QBrush(Qt::darkGray));
    healthBarBack->setPen(QPen(Qt::black));

    healthBarFill = new QGraphicsRectItem(this);
    healthBarFill->setRect(0, -42, 70, 7);
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
        handleTelegraph(player);
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

    separateFromOtherEnemies();

    bool movedHorizontally =
        std::abs(x() - previousX) > 0.01;

    bool facingRight =
        shouldFaceRight(player);

    setAnimationMovementHint(movedHorizontally,
                             facingRight);

    stepSpriteAnimation();

    updateHealthBar();
}

void Enemy::handleChase(Character &player)
{
    float enemyCenterX =
        x() + rect().width() / 2.0;

    float targetCenterX =
        targetAttackCenterX(player);

    float dx = targetCenterX - enemyCenterX;
    float dy = player.y() - y();

    float stoppingDistance = 6.0f;

    if (dx > stoppingDistance) {
        moveBy(speed, 0);
    }
    else if (dx < -stoppingDistance) {
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

    if (canAttackPlayer(player)) {
        currentState = State::Telegraph;
        telegraphTimer = 30;
    }
}

void Enemy::handleTelegraph(Character &player)
{
    Q_UNUSED(player);

    telegraphTimer--;

    if (telegraphTimer <= 0) {
        attackTimer = 0;
        attackDamageApplied = false;
        currentState = State::Attack;
    }
}

void Enemy::attack(Character &player)
{
    if (!player.isAlive()) {
        currentState = State::CoolDown;
        return;
    }

    const int attackFrameDelay = 5;
    const int damageFrameTick = attackFrameDelay * 3;
    const int attackFinishTick = attackFrameDelay * 5 + 6;

    if (attackTimer == 0) {
        const double enemyCenterX =
            x() + rect().width() / 2.0;

        const double playerCenterY =
            player.y() + player.rect().height() / 2.0;

        setFacingFromTarget(
            QPointF(enemyCenterX + (shouldFaceRight(player) ? 100.0 : -100.0),
                    playerCenterY)
        );

        playAnimationSequence(
            {
                AnimationFrame(2, 0),
                AnimationFrame(2, 1),
                AnimationFrame(2, 2),
                AnimationFrame(2, 3),
                AnimationFrame(2, 4)
            },
            false,
            attackFrameDelay,
            4
        );
    }

    attackTimer++;

    if (!attackDamageApplied &&
        attackTimer >= damageFrameTick) {
        applyAttackDamage(player);
        attackDamageApplied = true;
    }

    if (attackTimer >= attackFinishTick) {
        attackTimer = 0;
        attackDamageApplied = false;
        cooldownTimer = 60;
        currentState = State::CoolDown;
    }
}

bool Enemy::canAttackPlayer(Character &player) const
{
    auto enemyCenterX =
        x() + rect().width() / 2.0;

    auto playerCenterX =
        player.x() + player.rect().width() / 2.0;

    auto dy = std::abs(this->y() - player.y());
    auto dx = std::abs(enemyCenterX - playerCenterX);

    double attackCenterGap =
        (rect().width() + player.rect().width()) / 2.0 + 35.0;

    return dx < attackCenterGap && dy < 75;
}

double Enemy::targetAttackCenterX(Character &player) const
{
    const double enemyCenterX =
        x() + rect().width() / 2.0;

    const double playerCenterX =
        player.x() + player.rect().width() / 2.0;

    const double attackSide =
        enemyCenterX < playerCenterX ? -1.0 : 1.0;

    const double baseGap =
        (rect().width() + player.rect().width()) / 2.0 + 18.0;

    const double spacingOffset =
        (formationIndex % 3) * 10.0;

    return playerCenterX + attackSide * (baseGap + spacingOffset);
}

bool Enemy::shouldFaceRight(Character &player) const
{
    const double enemyCenterX =
        x() + rect().width() / 2.0;

    const double playerCenterX =
        player.x() + player.rect().width() / 2.0;

    return playerCenterX >= enemyCenterX;
}

void Enemy::applyAttackDamage(Character &player)
{
    if (!canAttackPlayer(player)) {
        return;
    }

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

void Enemy::separateFromOtherEnemies()
{
    if (!scene()) {
        return;
    }

    const QRectF myRect = sceneBoundingRect();
    const double myCenterX = myRect.center().x();
    const double myCenterY = myRect.center().y();
    const double minimumCenterGap = rect().width() + 18.0;

    for (QGraphicsItem *item : scene()->items()) {
        Enemy *other = dynamic_cast<Enemy*>(item);

        if (!other || other == this || !other->isAlive()) {
            continue;
        }

        const QRectF otherRect = other->sceneBoundingRect();

        if (!myRect.intersects(otherRect)) {
            continue;
        }

        const double otherCenterX = otherRect.center().x();
        const double otherCenterY = otherRect.center().y();

        if (std::abs(myCenterY - otherCenterY) > rect().height()) {
            continue;
        }

        const double dx = myCenterX - otherCenterX;
        const double overlap = minimumCenterGap - std::abs(dx);

        if (overlap <= 0.0) {
            continue;
        }

        const double direction =
            dx < 0.0 ? -1.0 :
            dx > 0.0 ? 1.0 :
            this < other ? -1.0 : 1.0;

        setX(x() + direction * (overlap / 2.0 + 1.0));

        if (scene()) {
            const QRectF sceneRect = scene()->sceneRect();
            const double minX = sceneRect.left();
            const double maxX = sceneRect.right() - rect().width();
            setX(std::clamp(x(), minX, maxX));
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

    double barWidth = 70.0 * healthPercent;

    healthBarBack->setRect(0, -42, 70, 7);
    healthBarFill->setRect(0, -42, barWidth, 7);
}

void Enemy::resetEnemyState()
{
    resetForLevel();

    currentState = State::Chase;

    attackCoolDown = 0;
    telegraphTimer = 0;
    cooldownTimer = 0;
    jumpCooldown = 0;
    attackTimer = 0;
    attackDamageApplied = false;

    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);
    setZValue(5);

    show();

    if (healthBarBack) {
        healthBarBack->show();
    }

    if (healthBarFill) {
        healthBarFill->show();
    }

    updateHealthBar();
}
