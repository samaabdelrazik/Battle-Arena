#include <QApplication>
#include <QGraphicsScene>
#include "Character.h"
#include <QBrush>
#include <QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0,0,800,600);

    Character *rect = new Character("Sama", 50, 50);
    rect->setRect(0,0,50,50);
    rect->setBrush(Qt::blue);

    scene->addItem(rect);
    rect->setFlag(QGraphicsItem::ItemIsFocusable);
    rect->setFocus();

    QGraphicsView *view = new QGraphicsView(scene);

    view->show();

    return a.exec();
}
