#include "battle_arenahome.h"
#include "ui_battle_arenahome.h"

Battle_Arenahome::Battle_Arenahome(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Battle_Arenahome)
{
    ui->setupUi(this);
}

Battle_Arenahome::~Battle_Arenahome()
{
    delete ui;
}


void Battle_Arenahome::on_startButton_clicked()
{
    emit startButtonClicked();
}

void Battle_Arenahome::on_exitButton_clicked()
{
    emit exitButtonClicked();
}

