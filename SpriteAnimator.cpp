#include "SpriteAnimator.h"

#include <QTransform>
#include <QRect>
#include <iostream>

SpriteAnimator::SpriteAnimator(QObject *parent)
    : QObject(parent)
{
    connect(&animationTimer,
            &QTimer::timeout,
            this,
            &SpriteAnimator::advanceFrame);
}

bool SpriteAnimator::loadSpriteSheet(const QString &path,
                                     int frameWidth,
                                     int frameHeight,
                                     int columnCount)
{
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->columnCount = columnCount;

    spriteSheet.load(path);

    if (spriteSheet.isNull())
    {
        std::cout << "ERROR: SpriteAnimator failed to load: "
                  << path.toStdString()
                  << std::endl;

        return false;
    }

    std::cout << "SpriteAnimator loaded: "
              << path.toStdString()
              << std::endl;

    return true;
}

void SpriteAnimator::setTargetSize(const QSize &size)
{
    targetSize = size;
    emitCurrentFrame();
}

void SpriteAnimator::addAnimation(const QString &name,
                                  int row,
                                  int frames,
                                  int frameDurationMs,
                                  bool loop)
{
    Animation animation;
    animation.row = row;
    animation.frames = frames;
    animation.frameDurationMs = frameDurationMs;
    animation.loop = loop;

    animations[name] = animation;
}

void SpriteAnimator::play(const QString &name,
                          bool restart)
{
    if (!animations.contains(name))
    {
        std::cout << "ERROR: Missing animation: "
                  << name.toStdString()
                  << std::endl;

        return;
    }

    if (!restart &&
        currentAnimationName == name &&
        animationTimer.isActive())
    {
        return;
    }

    currentAnimationName = name;
    currentFrame = 0;

    Animation animation = animations[name];

    emitCurrentFrame();

    animationTimer.start(animation.frameDurationMs);
}

void SpriteAnimator::setMirrorHorizontal(bool mirror)
{
    if (mirrorHorizontal == mirror)
    {
        return;
    }

    mirrorHorizontal = mirror;
    emitCurrentFrame();
}

QString SpriteAnimator::currentAnimation() const
{
    return currentAnimationName;
}

void SpriteAnimator::emitCurrentFrame()
{
    if (spriteSheet.isNull() ||
        !animations.contains(currentAnimationName))
    {
        return;
    }

    Animation animation = animations[currentAnimationName];

    if (currentFrame < 0)
    {
        currentFrame = 0;
    }

    if (currentFrame >= animation.frames)
    {
        currentFrame = animation.frames - 1;
    }

    QRect sourceRect(
        currentFrame * frameWidth,
        animation.row * frameHeight,
        frameWidth,
        frameHeight
    );

    QPixmap frame =
        spriteSheet.copy(sourceRect);

    if (mirrorHorizontal)
    {
        frame = frame.transformed(
            QTransform().scale(-1, 1),
            Qt::SmoothTransformation
        );
    }

    QPixmap scaled =
        frame.scaled(targetSize,
                     Qt::KeepAspectRatio,
                     Qt::SmoothTransformation);

    emit frameChanged(scaled);
}

void SpriteAnimator::advanceFrame()
{
    if (!animations.contains(currentAnimationName))
    {
        return;
    }

    Animation animation = animations[currentAnimationName];

    currentFrame++;

    if (currentFrame >= animation.frames)
    {
        if (animation.loop)
        {
            currentFrame = 0;
        }
        else
        {
            animationTimer.stop();
            emit animationFinished(currentAnimationName);
            return;
        }
    }

    emitCurrentFrame();
}
