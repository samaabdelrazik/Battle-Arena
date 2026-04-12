#ifndef BATTLE_ARENAHOME_H
#define BATTLE_ARENAHOME_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Battle_Arenahome;
}
QT_END_NAMESPACE

class Battle_Arenahome : public QMainWindow
{
    Q_OBJECT

public:
    Battle_Arenahome(QWidget *parent = nullptr);
    ~Battle_Arenahome();

private:
    Ui::Battle_Arenahome *ui;
};
#endif // BATTLE_ARENAHOME_H
