#ifndef COMBATEFFECT_H
#define COMBATEFFECT_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QRect>
#include <QSize>
#include <QTimer>
#include <QVector>
#include <QString>

class CombatEffect : public QObject,
                     public QGraphicsPixmapItem
{
    Q_OBJECT

private:
    QPixmap spriteSheet;
    QVector<QRect> frames;

    int currentFrame = 0;
    int frameDurationMs = 45;

    QSize displaySize;

    bool mirrorHorizontal = false;

    QTimer *animationTimer = nullptr;

    void updateFrame();

private slots:
    void advanceFrame();

public:
    CombatEffect(const QString &spriteSheetPath,
                 const QVector<QRect> &sourceFrames,
                 const QSize &targetSize,
                 int frameDurationMs,
                 QGraphicsItem *parentItem = nullptr);

    void setMirrorHorizontal(bool mirror);
    void start();
};

#endif
