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
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameOfLifeGUIClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GameOfLifeGUIClass)
    {
        if (GameOfLifeGUIClass->objectName().isEmpty())
            GameOfLifeGUIClass->setObjectName(QStringLiteral("GameOfLifeGUIClass"));
        GameOfLifeGUIClass->resize(600, 400);
        menuBar = new QMenuBar(GameOfLifeGUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        GameOfLifeGUIClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GameOfLifeGUIClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GameOfLifeGUIClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(GameOfLifeGUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        GameOfLifeGUIClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(GameOfLifeGUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GameOfLifeGUIClass->setStatusBar(statusBar);

        retranslateUi(GameOfLifeGUIClass);

        QMetaObject::connectSlotsByName(GameOfLifeGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *GameOfLifeGUIClass)
    {
        GameOfLifeGUIClass->setWindowTitle(QApplication::translate("GameOfLifeGUIClass", "GameOfLifeGUI", 0));
    } // retranslateUi

};

namespace Ui {
    class GameOfLifeGUIClass: public Ui_GameOfLifeGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEOFLIFEGUI_H
