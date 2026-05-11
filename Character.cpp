#include "Character.h"
#include "Block.h"
#include "Platform.h"

#include <iostream>
#include <QGraphicsScene>
#include <QList>
#include <QBrush>
#include <QCoreApplication>
#include <QDir>
#include <QPen>
#include <QFileInfo>
#include <QStringList>
#include <QTransform>
#include <cmath>

using namespace std;

int Character::characterCount = 0;

Character::Character(string name,
                     int attackDamage,
                     int health)
{
    this->name = name;
    this->attackDamage = attackDamage;
    this->health = health;
    this->maxHealth = health;

    characterCount++;

    baseAcceleration = acceleration;
    baseMaxMoveSpeed = maxMoveSpeed;
    baseMaxJumps = maxJumps;
}

Character::~Character()
{
    characterCount--;
}

string Character::getName() const
{
    return name;
}

int Character::getAttackDamage() const
{
    return attackDamage;
}

int Character::getHealth() const
{
    return health;
}

int Character::getScore() const
{
    return score;
}

double Character::getSpecialBar() const
{
    return specialBar;
}

int Character::getAttackCooldownFrames() const
{
    return attackCooldownFrames;
}

void Character::setAttackCooldownFrames(int frames)
{
    if (frames < 1)
    {
        frames = 1;
    }

    attackCooldownFrames = frames;
}

void Character::setMovementStats(double newAcceleration,
                                 double newMaxMoveSpeed)
{
    acceleration = newAcceleration;
    maxMoveSpeed = newMaxMoveSpeed;

    baseAcceleration = newAcceleration;
    baseMaxMoveSpeed = newMaxMoveSpeed;
}

void Character::setSpriteSheet(const QString &spritePath,
                               int columns,
                               int rows)
{
    usingModernSpriteSheet = false;

    spriteColumns = columns;
    spriteRows = rows;

    currentSpriteColumn = 1;
    spriteAnimationCounter = 0;
    walkSequenceIndex = 0;

    facingDirection = FacingDirection::OutScreen;
    pendingFacingDirection = FacingDirection::OutScreen;
    facingLockCounter = 0;
    directionTransitionCounter = 0;

    spriteSheet.load(spritePath);

    if (spriteSheet.isNull())
    {
        QString fileName = QFileInfo(spritePath).fileName();
        QString relativePath =
            "CharactersGraphics/rpgsprites1/" + fileName;

        spriteSheet.load(relativePath);
    }

    if (spriteSheet.isNull())
    {
        QString fileName = QFileInfo(spritePath).fileName();
        QString absolutePath =
            "P:/CS2 project/Battle-Arena/CharactersGraphics/rpgsprites1/" + fileName;

        spriteSheet.load(absolutePath);
    }

    if (!spriteItem)
    {
        spriteItem = new QGraphicsPixmapItem(this);
    }

    if (spriteSheet.isNull())
    {
        cout << "ERROR: Sprite failed to load: "
             << spritePath.toStdString()
             << endl;

        setBrush(Qt::blue);
        setPen(QPen(Qt::black));

        return;
    }

    cout << "Sprite loaded successfully: "
         << spritePath.toStdString()
         << endl;

    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);

    updateSpriteFrame();
}

void Character::setModernSpriteSheet(const QString &spritePath,
                                     int frameWidth,
                                     int frameHeight,
                                     int firstRow,
                                     int rowCount,
                                     int idleRow,
                                     int walkRow,
                                     int attackRow,
                                     int idleFrameCount,
                                     int walkFrameCount,
                                     int attackFrameCount)
{
    usingModernSpriteSheet = true;

    modernFrameWidth = frameWidth;
    modernFrameHeight = frameHeight;

    modernFirstRow = firstRow;
    modernRowCount = rowCount;

    modernIdleRow = idleRow;
    modernWalkRow = walkRow;
    modernAttackRow = attackRow;

    modernIdleFrameCount =
        idleFrameCount > 0 ? idleFrameCount : 1;

    modernWalkFrameCount =
        walkFrameCount > 0 ? walkFrameCount : 1;

    modernAttackFrameCount =
        attackFrameCount > 0 ? attackFrameCount : 1;

    modernCurrentFrame = 0;
    modernAnimationCounter = 0;

    modernAttackAnimationActive = false;
    modernAttackFrame = 0;
    modernAttackHoldCounter = 0;

    QStringList possiblePaths;
    QString appDir = QCoreApplication::applicationDirPath();
    QString fileName = QFileInfo(spritePath).fileName();

    possiblePaths << spritePath;

    if (fileName == "ranger.png")
    {
        possiblePaths << "Effects/ranger spritesheet calciumtrice.png";
        possiblePaths << appDir + "/../Effects/ranger spritesheet calciumtrice.png";
        possiblePaths << appDir + "/../../Effects/ranger spritesheet calciumtrice.png";
        possiblePaths << appDir + "/../../../Effects/ranger spritesheet calciumtrice.png";
        possiblePaths << "P:/CS2 project/Battle-Arena/Effects/ranger spritesheet calciumtrice.png";
    }
    else if (fileName == "warrior.png")
    {
        possiblePaths << "Effects/warrior spritesheet calciumtrice.png";
        possiblePaths << appDir + "/../Effects/warrior spritesheet calciumtrice.png";
        possiblePaths << appDir + "/../../Effects/warrior spritesheet calciumtrice.png";
        possiblePaths << appDir + "/../../../Effects/warrior spritesheet calciumtrice.png";
        possiblePaths << "P:/CS2 project/Battle-Arena/Effects/warrior spritesheet calciumtrice.png";
    }
    else if (fileName == "wizard.png")
    {
        possiblePaths << "Effects/wizard spritesheet calciumtrice.png";
        possiblePaths << appDir + "/../Effects/wizard spritesheet calciumtrice.png";
        possiblePaths << appDir + "/../../Effects/wizard spritesheet calciumtrice.png";
        possiblePaths << appDir + "/../../../Effects/wizard spritesheet calciumtrice.png";
        possiblePaths << "P:/CS2 project/Battle-Arena/Effects/wizard spritesheet calciumtrice.png";
    }
    else if (fileName == "minotaur_earth.png")
    {
        possiblePaths << "Effects/minotaur_spritesheets/minotaurus_spritesheet_earth_original.png";
        possiblePaths << appDir + "/../Effects/minotaur_spritesheets/minotaurus_spritesheet_earth_original.png";
        possiblePaths << appDir + "/../../Effects/minotaur_spritesheets/minotaurus_spritesheet_earth_original.png";
        possiblePaths << appDir + "/../../../Effects/minotaur_spritesheets/minotaurus_spritesheet_earth_original.png";
        possiblePaths << "P:/CS2 project/Battle-Arena/Effects/minotaur_spritesheets/minotaurus_spritesheet_earth_original.png";
    }
    else if (fileName == "minotaur_frost.png")
    {
        possiblePaths << "Effects/minotaur_spritesheets/minotaurus_spritesheet_frost.png";
        possiblePaths << appDir + "/../Effects/minotaur_spritesheets/minotaurus_spritesheet_frost.png";
        possiblePaths << appDir + "/../../Effects/minotaur_spritesheets/minotaurus_spritesheet_frost.png";
        possiblePaths << appDir + "/../../../Effects/minotaur_spritesheets/minotaurus_spritesheet_frost.png";
        possiblePaths << "P:/CS2 project/Battle-Arena/Effects/minotaur_spritesheets/minotaurus_spritesheet_frost.png";
    }
    else if (fileName == "minotaur_lava.png")
    {
        possiblePaths << "Effects/minotaur_spritesheets/minotaurus_spritesheet_lava.png";
        possiblePaths << appDir + "/../Effects/minotaur_spritesheets/minotaurus_spritesheet_lava.png";
        possiblePaths << appDir + "/../../Effects/minotaur_spritesheets/minotaurus_spritesheet_lava.png";
        possiblePaths << appDir + "/../../../Effects/minotaur_spritesheets/minotaurus_spritesheet_lava.png";
        possiblePaths << "P:/CS2 project/Battle-Arena/Effects/minotaur_spritesheets/minotaurus_spritesheet_lava.png";
    }
    else if (fileName == "minotaur_lightning.png")
    {
        possiblePaths << "Effects/minotaur_spritesheets/minotaurus_spritesheet_lightning.png";
        possiblePaths << appDir + "/../Effects/minotaur_spritesheets/minotaurus_spritesheet_lightning.png";
        possiblePaths << appDir + "/../../Effects/minotaur_spritesheets/minotaurus_spritesheet_lightning.png";
        possiblePaths << appDir + "/../../../Effects/minotaur_spritesheets/minotaurus_spritesheet_lightning.png";
        possiblePaths << "P:/CS2 project/Battle-Arena/Effects/minotaur_spritesheets/minotaurus_spritesheet_lightning.png";
    }

    possiblePaths << "Effects/" + fileName;
    possiblePaths << appDir + "/Effects/" + fileName;
    possiblePaths << appDir + "/../Effects/" + fileName;
    possiblePaths << appDir + "/../../Effects/" + fileName;
    possiblePaths << appDir + "/../../../Effects/" + fileName;
    possiblePaths << "P:/CS2 project/Battle-Arena/Effects/" + fileName;

    QString loadedPath;

    for (const QString &path : possiblePaths)
    {
        spriteSheet.load(QDir::cleanPath(path));

        if (!spriteSheet.isNull())
        {
            loadedPath = path;
            break;
        }
    }

    if (!spriteItem)
    {
        spriteItem = new QGraphicsPixmapItem(this);
    }

    if (spriteSheet.isNull())
    {
        cout << "ERROR: Modern sprite failed to load: "
             << spritePath.toStdString()
             << endl;

        setBrush(Qt::darkRed);
        setPen(QPen(Qt::black));
        return;
    }

    cout << "Modern sprite loaded successfully: "
         << loadedPath.toStdString()
         << endl;

    setBrush(Qt::NoBrush);
    setPen(Qt::NoPen);

    updateModernSpriteFrame(modernIdleRow, 0);
}

void Character::playModernAttackAnimation(bool attackingRight)
{
    if (!usingModernSpriteSheet)
    {
        return;
    }

    modernFacingRight = attackingRight;

    if (modernAttackAnimationActive)
    {
        if (modernAttackFrameDelay > modernAttackFastFrameDelay)
        {
            modernAttackFrameDelay--;
        }

        return;
    }

    modernAttackAnimationActive = true;
    modernAttackFrame = 0;
    modernAttackHoldCounter = 0;
    modernAttackFrameDelay = modernAttackSlowFrameDelay;

    updateModernSpriteFrame(modernAttackRow,
                            modernAttackFrame);
}

void Character::setModernSpriteTargetSize(const QSize &size)
{
    if (!size.isValid() ||
        size.width() <= 0 ||
        size.height() <= 0)
    {
        return;
    }

    modernSpriteTargetSize = size;

    if (usingModernSpriteSheet)
    {
        updateModernSpriteFrame(modernIdleRow,
                                modernCurrentFrame);
    }
}

void Character::setModernSpriteGroundOffset(double offset)
{
    modernSpriteGroundOffset = offset;

    if (usingModernSpriteSheet)
    {
        updateModernSpriteFrame(modernIdleRow,
                                modernCurrentFrame);
    }
}

void Character::setAnimationMovementHint(bool moving,
                                         bool facingRight)
{
    modernMovementHintActive = true;
    modernMovementHintMoving = moving;
    modernMovementHintFacingRight = facingRight;
}

void Character::clearAnimationMovementHint()
{
    modernMovementHintActive = false;
}

void Character::stepSpriteAnimation()
{
    updateModernSpriteAnimation();
}

int Character::rowForFacingDirection() const
{
    if (facingDirection == FacingDirection::InScreen)
    {
        return 0;
    }

    if (facingDirection == FacingDirection::Right)
    {
        return 1;
    }

    if (facingDirection == FacingDirection::Left)
    {
        return 3;
    }

    return 2;
}

void Character::setFacingFromTarget(QPointF targetPoint)
{
    double centerX =
        x() + rect().width() / 2.0;

    double centerY =
        y() + rect().height() / 2.0;

    double dx = targetPoint.x() - centerX;
    double dy = targetPoint.y() - centerY;

    modernFacingRight = dx >= 0;

    double verticalThreshold = 25.0;

    if (dy > verticalThreshold)
    {
        facingDirection = FacingDirection::OutScreen;
    }
    else if (dy < -verticalThreshold)
    {
        facingDirection = FacingDirection::InScreen;
    }
    else
    {
        if (dx >= 0)
        {
            facingDirection = FacingDirection::Right;
        }
        else
        {
            facingDirection = FacingDirection::Left;
        }
    }

    pendingFacingDirection = facingDirection;
    directionTransitionCounter = 0;
    facingLockCounter = 18;

    currentSpriteColumn = 1;
    walkSequenceIndex = 0;

    if (!usingModernSpriteSheet)
    {
        updateSpriteFrame();
    }
}

void Character::updateFacingFromMovement()
{
    if (facingLockCounter > 0)
    {
        facingLockCounter--;
        return;
    }

    if (directionTransitionCounter > 0)
    {
        directionTransitionCounter--;

        if (directionTransitionCounter == 0)
        {
            facingDirection = pendingFacingDirection;
        }

        return;
    }

    bool wantsLeft =
        velocityX < -0.3;

    bool wantsRight =
        velocityX > 0.3;

    if (!wantsLeft && !wantsRight)
    {
        return;
    }

    FacingDirection wantedDirection =
        wantsRight ? FacingDirection::Right : FacingDirection::Left;

    modernFacingRight = wantsRight;

    if (facingDirection == wantedDirection)
    {
        return;
    }

    bool changingBetweenLeftAndRight =
        (facingDirection == FacingDirection::Left &&
         wantedDirection == FacingDirection::Right) ||
        (facingDirection == FacingDirection::Right &&
         wantedDirection == FacingDirection::Left);

    if (changingBetweenLeftAndRight)
    {
        facingDirection = FacingDirection::InScreen;
        pendingFacingDirection = wantedDirection;
        directionTransitionCounter = 5;
    }
    else
    {
        facingDirection = wantedDirection;
        pendingFacingDirection = wantedDirection;
        directionTransitionCounter = 0;
    }
}

void Character::updateSpriteFrame()
{
    if (!spriteItem || spriteSheet.isNull())
    {
        return;
    }

    int frameWidth = spriteSheet.width() / spriteColumns;
    int frameHeight = spriteSheet.height() / spriteRows;

    int row = rowForFacingDirection();

    QRect sourceRect(
        currentSpriteColumn * frameWidth,
        row * frameHeight,
        frameWidth,
        frameHeight
    );

    QPixmap frame =
        spriteSheet.copy(sourceRect);

    QPixmap scaledFrame =
        frame.scaled(70,
                     70,
                     Qt::KeepAspectRatio,
                     Qt::SmoothTransformation);

    spriteItem->setPixmap(scaledFrame);

    double spriteX =
        (rect().width() - scaledFrame.width()) / 2.0;

    double spriteY =
        rect().height() -
        scaledFrame.height() +
        modernSpriteGroundOffset;

    spriteItem->setPos(spriteX, spriteY);
    spriteItem->setZValue(10);
}

void Character::updateModernSpriteFrame(int row,
                                        int column)
{
    if (!spriteItem || spriteSheet.isNull())
    {
        return;
    }

    int realRow =
        modernFirstRow + row;

    if (realRow < modernFirstRow)
    {
        realRow = modernFirstRow;
    }

    if (realRow >= modernFirstRow + modernRowCount)
    {
        realRow = modernFirstRow + modernRowCount - 1;
    }

    QRect sourceRect(
        column * modernFrameWidth,
        realRow * modernFrameHeight,
        modernFrameWidth,
        modernFrameHeight
    );

    QPixmap frame =
        spriteSheet.copy(sourceRect);

    if (!modernFacingRight)
    {
        frame = frame.transformed(
            QTransform().scale(-1, 1),
            Qt::SmoothTransformation
        );
    }

    QPixmap scaledFrame =
        frame.scaled(modernSpriteTargetSize,
                     Qt::KeepAspectRatio,
                     Qt::SmoothTransformation);

    spriteItem->setPixmap(scaledFrame);

    double spriteX =
        (rect().width() - scaledFrame.width()) / 2.0;

    double spriteY =
        rect().height() -
        scaledFrame.height() +
        modernSpriteGroundOffset;

    spriteItem->setPos(spriteX, spriteY);
    spriteItem->setZValue(10);
}

void Character::updateSpriteAnimation()
{
    if (!spriteItem || spriteSheet.isNull())
    {
        return;
    }

    if (usingModernSpriteSheet)
    {
        updateModernSpriteAnimation();
        return;
    }

    updateFacingFromMovement();

    bool isMoving =
        std::abs(velocityX) > 0.3;

    if (isMoving)
    {
        spriteAnimationCounter++;

        if (spriteAnimationCounter >= 8)
        {
            int walkSequence[4] = {0, 1, 2, 1};

            walkSequenceIndex++;

            if (walkSequenceIndex >= 4)
            {
                walkSequenceIndex = 0;
            }

            currentSpriteColumn =
                walkSequence[walkSequenceIndex];

            spriteAnimationCounter = 0;
        }
    }
    else
    {
        currentSpriteColumn = 1;
        spriteAnimationCounter = 0;
        walkSequenceIndex = 0;
    }

    updateSpriteFrame();
}

void Character::playAnimationSequence(
    const std::vector<AnimationFrame>& frames,
    bool loop,
    int frameDelay,
    int endHoldFrames)
{
    if (frames.empty())
    {
        return;
    }

    currentAnimationFrames = frames;

    animationLooping = loop;
    animationPlaying = true;

    animationFrameIndex = 0;
    animationCounter = 0;

    animationFrameDelay = frameDelay;
    animationEndHoldFrames = endHoldFrames;
    animationEndHoldCounter = 0;

    updateModernSpriteFrame(
        currentAnimationFrames[0].row,
        currentAnimationFrames[0].column
    );
}

void Character::stopAnimationSequence()
{
    animationPlaying = false;

    animationFrameIndex = 0;
    animationCounter = 0;
    animationEndHoldFrames = 0;
    animationEndHoldCounter = 0;

    currentAnimationFrames.clear();
}

bool Character::isAnimationPlaying() const
{
    return animationPlaying;
}

void Character::updateModernSpriteAnimation()
{
    if (!usingModernSpriteSheet)
    {
        return;
    }

    if (animationPlaying)
    {
        animationCounter++;

        if (animationCounter >= animationFrameDelay)
        {
            animationCounter = 0;

            animationFrameIndex++;

            if (animationFrameIndex >=
                static_cast<int>(currentAnimationFrames.size()))
            {
                if (animationLooping)
                {
                    animationFrameIndex = 0;
                }
                else
                {
                    animationFrameIndex =
                        static_cast<int>(currentAnimationFrames.size()) - 1;

                    if (animationEndHoldCounter < animationEndHoldFrames)
                    {
                        animationEndHoldCounter++;
                    }
                    else
                    {
                        animationPlaying = false;
                    }
                }
            }
        }

        if (!currentAnimationFrames.empty())
        {
            const AnimationFrame& frame =
                currentAnimationFrames[animationFrameIndex];

            updateModernSpriteFrame(
                frame.row,
                frame.column
            );
        }

        return;
    }

    bool moving =
        modernMovementHintActive
            ? modernMovementHintMoving
            : (movingLeft || movingRight);

    if (!modernMovementHintActive)
    {
        if (movingLeft && !movingRight)
        {
            modernFacingRight = false;
        }
        else if (movingRight && !movingLeft)
        {
            modernFacingRight = true;
        }
        else if (velocityX < -0.3)
        {
            modernFacingRight = false;
        }
        else if (velocityX > 0.3)
        {
            modernFacingRight = true;
        }
    }

    modernAnimationCounter++;

    bool facingRight =
        modernMovementHintActive
            ? modernMovementHintFacingRight
            : modernFacingRight;

    if (modernMovementHintActive)
    {
        modernFacingRight = facingRight;
    }

    int row =
        moving
            ? modernWalkRow
            : modernIdleRow;

    int frameCount =
        moving
            ? modernWalkFrameCount
            : modernIdleFrameCount;

    int speed =
        moving
            ? 5
            : 12;

    if (modernAnimationCounter >= speed)
    {
        modernAnimationCounter = 0;

        modernCurrentFrame++;

        if (modernCurrentFrame >= frameCount)
        {
            modernCurrentFrame = 0;
        }
    }

    int displayColumn = modernCurrentFrame;

    if (!facingRight)
    {
        displayColumn =
            (frameCount - 1) - modernCurrentFrame;
    }

    updateModernSpriteFrame(
        row,
        displayColumn
    );
}

int Character::attack()
{
    damageDealt += attackDamage;

    return attackDamage;
}

void Character::takeDamage(int damage)
{
    if (!isAlive())
    {
        return;
    }

    health -= damage;

    if (health > 0)
    {
        cout << name
             << " remaining health: "
             << health << endl;
    }
    else
    {
        health = 0;

        cout << name
             << " died" << endl;

        if (!deathSignalEmitted)
        {
            deathSignalEmitted = true;
            emit characterDied(this);
        }
    }
}

bool Character::isAlive() const
{
    return health > 0;
}

void Character::increaseHealth(int amount)
{
    health += amount;

    if (health > maxHealth)
    {
        health = maxHealth;
    }
}

void Character::decreaseHealth(int amount)
{
    takeDamage(amount);
}

void Character::increaseScore(int amount)
{
    score += amount;
}

void Character::decreaseScore(int amount)
{
    score -= amount;

    if (score < 0)
    {
        score = 0;
    }
}

void Character::increaseSpecialBar(double amount)
{
    specialBar += amount;

    if (specialBar > 100.0)
    {
        specialBar = 100.0;
    }
}

void Character::decreaseSpecialBar(double amount)
{
    specialBar -= amount;

    if (specialBar < 0.0)
    {
        specialBar = 0.0;
    }
}

bool Character::canUseSpecial() const
{
    return specialBar >= 100.0;
}

bool Character::isSpecialActive() const
{
    return specialActive;
}

void Character::startSpecial()
{
    if (canUseSpecial())
    {
        specialActive = true;
    }
}

void Character::stopSpecial()
{
    specialActive = false;
    specialBar = 0.0;
}

int Character::getCharacterCount()
{
    return characterCount;
}

int Character::calculateScore() const
{
    return damageDealt * 10 + score;
}

void Character::jump()
{
    if (jumpCount < maxJumps)
    {
        velocityY = jumpStrength;
        onGround = false;
        jumpCount++;
    }
}

void Character::activateBonuses()
{
    if (!speedBoostActive)
    {
        speedBoostActive = true;

        acceleration = baseAcceleration + 0.4;
        maxMoveSpeed = baseMaxMoveSpeed + 2.0;
    }

    if (!jumpBoostActive)
    {
        jumpBoostActive = true;
        maxJumps = 2;
    }
}

void Character::deactivateBonuses()
{
    speedBoostActive = false;
    jumpBoostActive = false;

    acceleration = baseAcceleration;
    maxMoveSpeed = baseMaxMoveSpeed;
    maxJumps = baseMaxJumps;

    if (jumpCount > maxJumps)
    {
        jumpCount = maxJumps;
    }
}

void Character::registerHit(int damage)
{
    takeDamage(damage);

    timeSinceLastHit = 0.0;

    decreaseSpecialBar(5.0);

    deactivateBonuses();
}

void Character::resetForLevel()
{
    health = maxHealth;

    score = 0;
    damageDealt = 0;

    specialBar = 0.0;
    specialActive = false;

    deathSignalEmitted = false;

    velocityX = 0.0;
    velocityY = 0.0;

    movingLeft = false;
    movingRight = false;

    jumpCount = 0;

    timeSinceLastHit = 0.0;

    currentSpriteColumn = 1;
    spriteAnimationCounter = 0;
    walkSequenceIndex = 0;

    modernCurrentFrame = 0;
    modernAnimationCounter = 0;
    modernAttackAnimationActive = false;
    modernAttackFrame = 0;
    modernAttackHoldCounter = 0;
    modernAttackFrameDelay = modernAttackSlowFrameDelay;
    animationEndHoldFrames = 0;
    animationEndHoldCounter = 0;
    modernFacingRight = true;
    modernSpriteGroundOffset = 0.0;
    modernMovementHintActive = false;
    modernMovementHintMoving = false;
    modernMovementHintFacingRight = true;

    facingDirection = FacingDirection::OutScreen;
    pendingFacingDirection = FacingDirection::OutScreen;
    facingLockCounter = 0;
    directionTransitionCounter = 0;

    deactivateBonuses();

    if (usingModernSpriteSheet)
    {
        updateModernSpriteFrame(modernIdleRow, 0);
    }
    else
    {
        updateSpriteFrame();
    }
}

void Character::updateMovement()
{
    if (!isAlive())
    {
        return;
    }

    increaseSpecialBar(0.08);

    timeSinceLastHit += 0.016;

    if (timeSinceLastHit >= 5.0)
    {
        activateBonuses();
    }

    if (movingLeft)
    {
        velocityX -= acceleration;
    }

    if (movingRight)
    {
        velocityX += acceleration;
    }

    if (!movingLeft && !movingRight)
    {
        velocityX *= friction;

        if (abs(velocityX) < 0.2)
        {
            velocityX = 0;
        }
    }

    if (velocityX > maxMoveSpeed)
    {
        velocityX = maxMoveSpeed;
    }

    if (velocityX < -maxMoveSpeed)
    {
        velocityX = -maxMoveSpeed;
    }

    if (velocityX != 0)
    {
        moveBy(velocityX, 0);

        QList<QGraphicsItem*> horizontalCollisions =
            collidingItems();

        for (QGraphicsItem *item :
             horizontalCollisions)
        {
            Platform *platform =
                dynamic_cast<Platform*>(item);

            if (platform)
            {
                QRectF charRect =
                    sceneBoundingRect();

                QRectF platformRect =
                    platform->sceneBoundingRect();

                bool standingOnTop =
                    charRect.bottom()
                        <= platformRect.top() + 5 &&
                    charRect.bottom()
                        >= platformRect.top() - 5;

                if (!standingOnTop)
                {
                    moveBy(-velocityX, 0);
                    velocityX = 0;
                    break;
                }
            }
        }
    }

    if (velocityY < 0)
    {
        velocityY += gravity;
    }
    else
    {
        velocityY += fallGravity;
    }

    if (velocityY > maxFallSpeed)
    {
        velocityY = maxFallSpeed;
    }

    moveBy(0, velocityY);

    onGround = false;

    QList<QGraphicsItem*> verticalCollisions =
        collidingItems();

    for (QGraphicsItem *item :
         verticalCollisions)
    {
        Platform *platform =
            dynamic_cast<Platform*>(item);

        if (platform)
        {
            QRectF charRect =
                sceneBoundingRect();

            QRectF platformRect =
                platform->sceneBoundingRect();

            if (velocityY >= 0 &&
                charRect.bottom()
                    >= platformRect.top() &&
                charRect.bottom() - velocityY
                    <= platformRect.top() + 5)
            {
                setY(platformRect.top()
                     - rect().height());

                velocityY = 0;
                onGround = true;
                jumpCount = 0;
            }
        }
    }

    if (scene())
    {
        QRectF sceneRect =
            scene()->sceneRect();

        if (x() < sceneRect.left())
        {
            setX(sceneRect.left());
            velocityX = 0;
        }

        if (x() + rect().width()
            > sceneRect.right())
        {
            setX(sceneRect.right()
                 - rect().width());

            velocityX = 0;
        }
    }

    QList<QGraphicsItem*> collidedItems =
        collidingItems();

    for (QGraphicsItem *item :
         collidedItems)
    {
        Block *block =
            dynamic_cast<Block*>(item);

        if (block)
        {
            block->applyEffect(this);

            if (scene())
            {
                scene()->removeItem(block);
            }

            delete block;
            break;
        }
    }

    updateSpriteAnimation();
}

void Character::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left ||
        event->key() == Qt::Key_A)
    {
        movingLeft = true;
    }
    else if (event->key() == Qt::Key_Right ||
             event->key() == Qt::Key_D)
    {
        movingRight = true;
    }
    else if (event->key() == Qt::Key_Up ||
             event->key() == Qt::Key_W)
    {
        jump();
    }
}

void Character::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left ||
        event->key() == Qt::Key_A)
    {
        movingLeft = false;
    }
    else if (event->key() == Qt::Key_Right ||
             event->key() == Qt::Key_D)
    {
        movingRight = false;
    }
}
