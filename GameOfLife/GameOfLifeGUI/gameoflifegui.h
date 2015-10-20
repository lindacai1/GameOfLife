#ifndef GAMEOFLIFEGUI_H
#define GAMEOFLIFEGUI_H

#include <QtWidgets/QMainWindow>
#include "ui_gameoflifegui.h"
#include "../GameOfLife/board.h"

class GameOfLifeGUI : public QMainWindow
{
	Q_OBJECT

public:
	GameOfLifeGUI(QWidget *parent = 0);
	~GameOfLifeGUI();

private:
	static const int pixelSize = 4;

	Ui::GameOfLifeGUIClass ui;
	QImage boardImage;
	QPixmap pixmap;
	Board board;
	QImage newBoardImage();
	QTimer *timer;
	void updateUI();
	void setPixel(int x, int y, int offset = pixelSize);

private slots:
void on_startButton_clicked(bool x);
	void on_stopButton_clicked(bool x);
	void on_label_clicked();
	void on_nextButton_clicked(bool x);
	void on_randBoardButton_clicked(bool x);
	void update();
};

#endif // GAMEOFLIFEGUI_H
