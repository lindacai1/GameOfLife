#include "gameoflifegui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GameOfLifeGUI w;
	w.show();
	return a.exec();
}
