#include "../GameOfLife/board.h"
#include "gameoflifegui.h"
#include "qimage.h"
#include "qfiledialog.h"
#include "cqlabel.h"

GameOfLifeGUI::GameOfLifeGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setMouseTracking(true);

	boardImage = newBoardImage();
	pixmap = QPixmap::fromImage(boardImage);
	ui.label->setPixmap(pixmap);

	connect(ui.label, SIGNAL(clicked()), this, SLOT(on_label_clicked()));

}

GameOfLifeGUI::~GameOfLifeGUI()
{

}

QImage GameOfLifeGUI::newBoardImage() {
	QString filename = QString::fromStdString("C:/Users/Linda Cai/Documents/Visual Studio 2013/Projects/GameOfLife/GameOfLifeGUI/Resources/startBoard.png");
	QImage image;
	image.load(filename);
	return image;
}

void GameOfLifeGUI::on_startButton_clicked() {

	Board b;
	ui.startButton->setText("Hello");
	for (int i = 0; i < 2048; i++) {
		//b.addLivecell((int64_t)i, (int64_t)i);
		int x1 = rand() % 512;
		int y1 = rand() % 512;


		boardImage.setPixel(x1, y1, 0xffffff);
	}
	// TODO: Scale pixmap?
	pixmap = QPixmap::fromImage(boardImage);
	ui.label->setPixmap(pixmap);
	ui.label->show();

	//QImage newImage = newBoardImage();
	//b.nextIteration();
	//for (auto& cell : b.livecells) {
	//	newImage.setPixel(cell.first, cell.second, 0xffffff);
	//}

	//pixmap = QPixmap::fromImage(newImage);
	//ui.label->setPixmap(pixmap);
	//ui.label->show();
	QPoint p = ui.label->mapFromParent(QCursor::pos());
	int x = p.x();
	int y = p.y();
	

}

void GameOfLifeGUI::on_stopButton_clicked() {
	ui.stopButton->setText("Hello");
}

void GameOfLifeGUI::on_label_clicked() {
	//QPoint p = ui.label->mapFromGlobal(QCursor::pos());
	ui.stopButton->setText("Bye");
	//ui.startButton->setText(QString("%1, %2").arg(ui.label->x).arg(ui.label->y));
}