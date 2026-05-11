#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsRectItem>
#include <QTimer>

class Enemy;
class Character;

class Arrow : public QObject,
              public QGraphicsRectItem
{
    Q_OBJECT

private:
    double dx;
    double dy;

    int damage;

    Character *owner;

public:
    Arrow(double angleRadians,
          Character *owner,
          int damage = 25);

public slots:
    void moveArrow();
};

#endif
