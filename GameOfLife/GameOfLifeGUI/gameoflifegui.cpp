#include "gameoflifegui.h"
#include "qimage.h"
#include "qfiledialog.h"
#include "cqlabel.h"
#include <qtimer.h>

#define WHITE 0xffffff

static const int imageSize = 512;

GameOfLifeGUI::GameOfLifeGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setMouseTracking(true);

	boardImage = newBoardImage();
	pixmap = QPixmap::fromImage(boardImage);
	updateUI();

	connect(ui.label, SIGNAL(clicked()), this, SLOT(on_label_clicked()));
	timer = NULL;
	delay = 150;

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

void GameOfLifeGUI::on_startButton_clicked(bool x) {

	if (!timer) {
		timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this, SLOT(update()));
		timer->start(delay);
	}
	
	updateUI();
}

void GameOfLifeGUI::on_stopButton_clicked(bool x) {
	
	if (timer) {
		delete timer;
		timer = NULL;
	}

}

void GameOfLifeGUI::update() {
	on_nextButton_clicked(true);
}

void GameOfLifeGUI::on_randBoardButton_clicked(bool x) {
	for (int i = 0; i < 512; i++) {
		int x1 = rand() % imageSize / pixelSize;
		int y1 = rand() % imageSize / pixelSize;

		board.addLivecell((int64_t)x1, (int64_t)y1);
		setPixel(x1, y1);
	}

	updateUI();
}



void GameOfLifeGUI::on_label_clicked() {
	QPoint p = ui.label->mapFromGlobal(QCursor::pos());
	int x = p.x() / pixelSize;
	int y = p.y() / pixelSize;
	ui.coordLabel->setText(QString("%1, %2").arg(x).arg(y));
	board.addLivecell((int64_t)x, (int64_t)y);
	setPixel(x, y);
	updateUI();
}

void GameOfLifeGUI::on_nextButton_clicked(bool x) {
	boardImage = newBoardImage();
	board = board.nextIteration();
	for (auto& cell : board.livecells) {
		setPixel(cell.first, cell.second);
	}
	
	updateUI();
}

void GameOfLifeGUI::updateUI() {
	pixmap = QPixmap::fromImage(boardImage);
	ui.label->setPixmap(pixmap);
	ui.label->show();
}

void GameOfLifeGUI::setPixel(int x, int y, int offset) {
	x *= offset;
	y *= offset;
	for (int i = x; i < x + offset; ++i) {
		for (int j = y; j < y + offset; ++j) {
			boardImage.setPixel(i, j, WHITE);
		}
	}
}