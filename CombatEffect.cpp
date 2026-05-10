#include "CombatEffect.h"

#include <QTransform>
#include <QGraphicsScene>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>
#include <iostream>

CombatEffect::CombatEffect(const QString &spriteSheetPath,
                           const QVector<QRect> &sourceFrames,
                           const QSize &targetSize,
                           int frameDurationMs,
                           QGraphicsItem *parentItem)
    : QObject(),
      QGraphicsPixmapItem(parentItem)
{
    frames = sourceFrames;
    displaySize = targetSize;
    this->frameDurationMs = frameDurationMs;

    QStringList possiblePaths;

    possiblePaths << spriteSheetPath;
    possiblePaths << ":/effects/Attack_Effects.png";
    possiblePaths << ":/effects/Effects/Attack_Effects.png";
    possiblePaths << "Effects/Attack_Effects.png";
    possiblePaths << "../Effects/Attack_Effects.png";
    possiblePaths << "../../Effects/Attack_Effects.png";
    possiblePaths << "../../../Effects/Attack_Effects.png";
    possiblePaths << "P:/CS2 project/Battle-Arena/Effects/Attack_Effects.png";

    QString appDir = QCoreApplication::applicationDirPath();

    possiblePaths << appDir + "/Effects/Attack_Effects.png";
    possiblePaths << appDir + "/../Effects/Attack_Effects.png";
    possiblePaths << appDir + "/../../Effects/Attack_Effects.png";
    possiblePaths << appDir + "/../../../Effects/Attack_Effects.png";

    QString loadedPath;

    for (const QString &path : possiblePaths)
    {
        spriteSheet.load(path);

        if (!spriteSheet.isNull())
        {
            loadedPath = path;
            break;
        }
    }

    if (spriteSheet.isNull())
    {
        std::cout << "ERROR: Combat effect failed to load from all paths."
                  << std::endl;
    }
    else
    {
        std::cout << "Combat effect loaded successfully from: "
                  << loadedPath.toStdString()
                  << std::endl;
    }

    setZValue(800);

    animationTimer = new QTimer(this);

    connect(animationTimer,
            &QTimer::timeout,
            this,
            &CombatEffect::advanceFrame);
}

void CombatEffect::setMirrorHorizontal(bool mirror)
{
    mirrorHorizontal = mirror;
    updateFrame();
}

void CombatEffect::start()
{
    currentFrame = 0;
    updateFrame();

    if (!frames.isEmpty())
    {
        animationTimer->start(frameDurationMs);
    }
}

void CombatEffect::updateFrame()
{
    if (spriteSheet.isNull() || frames.isEmpty())
    {
        return;
    }

    if (currentFrame < 0 || currentFrame >= frames.size())
    {
        return;
    }

    QPixmap frame = spriteSheet.copy(frames[currentFrame]);

    if (mirrorHorizontal)
    {
        frame = frame.transformed(
            QTransform().scale(-1, 1),
            Qt::SmoothTransformation
        );
    }

    QPixmap scaledFrame =
        frame.scaled(displaySize,
                     Qt::KeepAspectRatio,
                     Qt::SmoothTransformation);

    setPixmap(scaledFrame);
}

void CombatEffect::advanceFrame()
{
    currentFrame++;

    if (currentFrame >= frames.size())
    {
        animationTimer->stop();

        if (scene())
        {
            scene()->removeItem(this);
        }

        deleteLater();
        return;
    }

    updateFrame();
}
