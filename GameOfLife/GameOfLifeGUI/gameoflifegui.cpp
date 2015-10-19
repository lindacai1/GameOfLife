#include "../GameOfLife/board.h"
#include "gameoflifegui.h"
#include "qimage.h"
#include "qfiledialog.h"

GameOfLifeGUI::GameOfLifeGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QString filename = QString::fromStdString("C:/Users/Linda Cai/Documents/Visual Studio 2013/Projects/GameOfLife/GameOfLifeGUI/Resources/startBoard.png");
	boardImage.load(filename);
	pixmap = QPixmap::fromImage(boardImage);
	ui.label->setPixmap(pixmap);

}

GameOfLifeGUI::~GameOfLifeGUI()
{

}

void GameOfLifeGUI::on_startButton_clicked() {
	QImage image;
	//QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.png *.jpg)"));
	//if (QString::compare(filename, QString()) != 0) {
	//	bool valid = image.load(filename);
	//	if (valid) {
	//		ui.label->setPixmap(QPixmap::fromImage(image));
	//	}
	//}
	//ui.label->setText(filename);

	ui.startButton->setText("Hello");
	for (int i = 0; i < 1000; i++) {
		boardImage.setPixel(i, i, 0xffffff);
	}
	// TODO: Scale pixmap?
	pixmap = QPixmap::fromImage(boardImage);
	ui.label->setPixmap(pixmap);
	ui.label->show();

}

void GameOfLifeGUI::on_stopButton_clicked() {
	ui.stopButton->setText("Hello");
}