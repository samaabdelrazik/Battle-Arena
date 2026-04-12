/********************************************************************************
** Form generated from reading UI file 'battle_arenahome.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BATTLE_ARENAHOME_H
#define UI_BATTLE_ARENAHOME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Battle_Arenahome
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *Battle_Arenahome)
    {
        if (Battle_Arenahome->objectName().isEmpty())
            Battle_Arenahome->setObjectName("Battle_Arenahome");
        Battle_Arenahome->resize(800, 600);
        centralwidget = new QWidget(Battle_Arenahome);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(260, 40, 241, 41));
        QFont font;
        font.setFamilies({QString::fromUtf8("Herculanum")});
        font.setPointSize(35);
        font.setItalic(true);
        label->setFont(font);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(320, 90, 100, 32));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Herculanum")});
        font1.setItalic(true);
        pushButton->setFont(font1);
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(320, 130, 100, 32));
        pushButton_2->setFont(font1);
        Battle_Arenahome->setCentralWidget(centralwidget);
        menubar = new QMenuBar(Battle_Arenahome);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 37));
        Battle_Arenahome->setMenuBar(menubar);
        statusbar = new QStatusBar(Battle_Arenahome);
        statusbar->setObjectName("statusbar");
        Battle_Arenahome->setStatusBar(statusbar);

        retranslateUi(Battle_Arenahome);

        QMetaObject::connectSlotsByName(Battle_Arenahome);
    } // setupUi

    void retranslateUi(QMainWindow *Battle_Arenahome)
    {
        Battle_Arenahome->setWindowTitle(QCoreApplication::translate("Battle_Arenahome", "Battle_Arenahome", nullptr));
        label->setText(QCoreApplication::translate("Battle_Arenahome", "<html><head/><body><p>Battle<span style=\" color:#030303;\">-</span><span style=\" color:#690600;\">Arena</span></p></body></html>", nullptr));
        pushButton->setText(QCoreApplication::translate("Battle_Arenahome", "Start Game", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Battle_Arenahome", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Battle_Arenahome: public Ui_Battle_Arenahome {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BATTLE_ARENAHOME_H
