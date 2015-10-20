#pragma once

#include <QtWidgets/QMainWindow>
#include <QLabel>

class CQLabel : public QLabel {
	Q_OBJECT

public:
	CQLabel(QWidget *parent = 0);
	~CQLabel();

protected slots:
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent * ev);
	void mouseMoveEvent(QMouseEvent * ev);

signals:
	void clicked(QPoint pos);
};
