#ifndef SPRITEANIMATOR_H
#define SPRITEANIMATOR_H

#include <QObject>
#include <QPixmap>
#include <QTimer>
#include <QMap>
#include <QString>
#include <QSize>

struct Animation
{
    int row = 0;
    int frames = 1;
    int frameDurationMs = 150;
    bool loop = true;
};

class SpriteAnimator : public QObject
{
    Q_OBJECT

private:
    QPixmap spriteSheet;

    int frameWidth = 32;
    int frameHeight = 32;
    int columnCount = 10;

    int currentFrame = 0;
    QString currentAnimationName;

    QMap<QString, Animation> animations;

    QTimer animationTimer;

    QSize targetSize = QSize(86, 86);

    bool mirrorHorizontal = false;

    void emitCurrentFrame();

private slots:
    void advanceFrame();

public:
    explicit SpriteAnimator(QObject *parent = nullptr);

    bool loadSpriteSheet(const QString &path,
                         int frameWidth,
                         int frameHeight,
                         int columnCount);

    void setTargetSize(const QSize &size);

    void addAnimation(const QString &name,
                      int row,
                      int frames,
                      int frameDurationMs,
                      bool loop = true);

    void play(const QString &name,
              bool restart = false);

    void setMirrorHorizontal(bool mirror);

    QString currentAnimation() const;

signals:
    void frameChanged(const QPixmap &pixmap);
    void animationFinished(const QString &name);
};

#endif
