/********************************************************************************
** Form generated from reading UI file 'gameoflifegui.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEOFLIFEGUI_H
#define UI_GAMEOFLIFEGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameOfLifeGUIClass
{
public:
    QWidget *centralWidget;
    QPushButton *startButton;
    QPushButton *stopButton;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GameOfLifeGUIClass)
    {
        if (GameOfLifeGUIClass->objectName().isEmpty())
            GameOfLifeGUIClass->setObjectName(QStringLiteral("GameOfLifeGUIClass"));
        GameOfLifeGUIClass->resize(654, 674);
        centralWidget = new QWidget(GameOfLifeGUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        startButton = new QPushButton(centralWidget);
        startButton->setObjectName(QStringLiteral("startButton"));
        startButton->setGeometry(QRect(100, 40, 75, 23));
        stopButton = new QPushButton(centralWidget);
        stopButton->setObjectName(QStringLiteral("stopButton"));
        stopButton->setGeometry(QRect(480, 40, 75, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(70, 80, 512, 512));
        GameOfLifeGUIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GameOfLifeGUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 654, 21));
        GameOfLifeGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GameOfLifeGUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GameOfLifeGUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(GameOfLifeGUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GameOfLifeGUIClass->setStatusBar(statusBar);

        retranslateUi(GameOfLifeGUIClass);
        QObject::connect(startButton, SIGNAL(clicked()), GameOfLifeGUIClass, SLOT(on_startButton_clicked()));
        QObject::connect(stopButton, SIGNAL(clicked()), GameOfLifeGUIClass, SLOT(on_stopButton_clicked()));
        QObject::connect(label, SIGNAL(clicked()), GameOfLifeGUIClass, SLOT(on_label_clicked()));

        QMetaObject::connectSlotsByName(GameOfLifeGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *GameOfLifeGUIClass)
    {
        GameOfLifeGUIClass->setWindowTitle(QApplication::translate("GameOfLifeGUIClass", "GameOfLifeGUI", 0));
        startButton->setText(QApplication::translate("GameOfLifeGUIClass", "Start", 0));
        stopButton->setText(QApplication::translate("GameOfLifeGUIClass", "Stop", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class GameOfLifeGUIClass: public Ui_GameOfLifeGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEOFLIFEGUI_H
