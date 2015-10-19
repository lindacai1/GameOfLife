#ifndef GAMEOFLIFEGUI_H
#define GAMEOFLIFEGUI_H

#include <QtWidgets/QMainWindow>
#include "ui_gameoflifegui.h"

class GameOfLifeGUI : public QMainWindow
{
	Q_OBJECT

public:
	GameOfLifeGUI(QWidget *parent = 0);
	~GameOfLifeGUI();

private:
	Ui::GameOfLifeGUIClass ui;
	QImage boardImage;
	QPixmap pixmap;
	QImage newBoardImage();

private slots:
	void on_startButton_clicked();
	void on_stopButton_clicked();
	void on_label_clicked();
};

#endif // GAMEOFLIFEGUI_H
