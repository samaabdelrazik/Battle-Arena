#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>
#include <QObject>
#include <QPointF>
#include <QPixmap>
#include <QString>
#include <QSize>

using namespace std;

enum class FacingDirection
{
    InScreen,
    Right,
    Left,
    OutScreen
};

struct AnimationFrame
{
    int row;
    int column;

    AnimationFrame(int r = 0,
                   int c = 0)
        : row(r),
          column(c)
    {
    }
};

class Character : public QObject,
                  public QGraphicsRectItem
{
    Q_OBJECT

private:
    string name;

    int health = 0;
    int maxHealth = 0;

    int attackDamage = 0;

    int score = 0;
    int damageDealt = 0;

    bool deathSignalEmitted = false;

    static int characterCount;

    double specialBar = 0.0;
    bool specialActive = false;

    int attackCooldownFrames = 22;

    double velocityX = 0.0;

    double jumpStrength = -16.0;

    double acceleration = 0.8;
    double friction = 0.75;
    double maxMoveSpeed = 6.5;

    bool movingLeft = false;
    bool movingRight = false;

    int jumpCount = 0;
    int maxJumps = 1;

    double timeSinceLastHit = 0.0;

    bool speedBoostActive = false;
    bool jumpBoostActive = false;

    double baseAcceleration = 0.8;
    double baseMaxMoveSpeed = 6.5;
    int baseMaxJumps = 1;

    QGraphicsPixmapItem *spriteItem = nullptr;
    QPixmap spriteSheet;

    int spriteColumns = 3;
    int spriteRows = 4;

    int currentSpriteColumn = 1;
    int spriteAnimationCounter = 0;
    int walkSequenceIndex = 0;

    FacingDirection facingDirection = FacingDirection::OutScreen;
    FacingDirection pendingFacingDirection = FacingDirection::OutScreen;

    int facingLockCounter = 0;
    int directionTransitionCounter = 0;

    bool usingModernSpriteSheet = false;

    int modernFrameWidth = 32;
    int modernFrameHeight = 32;

    int modernFirstRow = 0;
    int modernRowCount = 10;

    int modernIdleRow = 0;
    int modernWalkRow = 1;
    int modernAttackRow = 3;

    int modernIdleFrameCount = 10;
    int modernWalkFrameCount = 10;
    int modernAttackFrameCount = 10;

    QSize modernSpriteTargetSize = QSize(86, 86);
    double modernSpriteGroundOffset = 0.0;

    int modernCurrentFrame = 0;
    int modernAnimationCounter = 0;

    bool modernFacingRight = true;

    bool modernMovementHintActive = false;
    bool modernMovementHintMoving = false;
    bool modernMovementHintFacingRight = true;

    bool modernAttackAnimationActive = false;
    int modernAttackFrame = 0;
    int modernAttackHoldCounter = 0;
    int modernAttackFrameDelay = 2;
    int modernAttackSlowFrameDelay = 2;
    int modernAttackFastFrameDelay = 1;

    std::vector<AnimationFrame> currentAnimationFrames;

    bool animationLooping = false;
    bool animationPlaying = false;

    int animationFrameIndex = 0;
    int animationFrameDelay = 4;
    int animationCounter = 0;
    int animationEndHoldFrames = 0;
    int animationEndHoldCounter = 0;

    int rowForFacingDirection() const;

    void updateSpriteFrame();
    void updateSpriteAnimation();
    void updateFacingFromMovement();

    void updateModernSpriteFrame(int row,
                                 int column);

    void updateModernSpriteAnimation();

protected:
    double velocityY = 0.0;

    double gravity = 0.6;
    double fallGravity = 1.0;
    double maxFallSpeed = 18.0;

    bool onGround = false;

public:
    Character(string name,
              int attackDamage,
              int health);

    virtual ~Character();

    string getName() const;

    int getAttackDamage() const;

    int getHealth() const;

    int getScore() const;

    double getSpecialBar() const;

    int getAttackCooldownFrames() const;
    void setAttackCooldownFrames(int frames);

    void setMovementStats(double newAcceleration,
                          double newMaxMoveSpeed);

    void setSpriteSheet(const QString &spritePath,
                        int columns = 3,
                        int rows = 4);

    void setModernSpriteSheet(const QString &spritePath,
                              int frameWidth,
                              int frameHeight,
                              int firstRow,
                              int rowCount,
                              int idleRow,
                              int walkRow,
                              int attackRow,
                              int idleFrameCount = 10,
                              int walkFrameCount = 10,
                              int attackFrameCount = 10);

    void playModernAttackAnimation(bool attackingRight);

    void playAnimationSequence(
        const std::vector<AnimationFrame>& frames,
        bool loop,
        int frameDelay,
        int endHoldFrames = 0
    );

    void stopAnimationSequence();

    bool isAnimationPlaying() const;

    void setModernSpriteTargetSize(const QSize &size);

    void setModernSpriteGroundOffset(double offset);

    void setFacingFromTarget(QPointF targetPoint);

    void setAnimationMovementHint(bool moving,
                                  bool facingRight);

    void clearAnimationMovementHint();

    void stepSpriteAnimation();

    virtual int attack();

    virtual int specialAbility() = 0;

    virtual void basicAttack(QPointF targetPoint) = 0;

    void takeDamage(int damage);

    bool isAlive() const;

    void increaseHealth(int amount);

    void decreaseHealth(int amount);

    void increaseScore(int amount);

    void decreaseScore(int amount);

    void increaseSpecialBar(double amount);

    void decreaseSpecialBar(double amount);

    bool canUseSpecial() const;

    bool isSpecialActive() const;

    void startSpecial();

    void stopSpecial();

    static int getCharacterCount();

    int calculateScore() const;

    void updateMovement();

    void jump();

    void activateBonuses();

    void deactivateBonuses();

    void registerHit(int damage);

    void resetForLevel();

    void keyPressEvent(QKeyEvent *event) override;

    void keyReleaseEvent(QKeyEvent *event) override;

signals:
    void characterDied(Character* deadCharacter);
};

#endif
