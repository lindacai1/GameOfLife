#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_gameoflifegui.h"
#include <QLabel>

class CQLabel : public QLabel {
	Q_OBJECT

public:
	CQLabel(QWidget *parent = 0);
	~CQLabel();
	int x, y;

protected slots:
	void mousePressEvent(QMouseEvent *ev);

signals:
	void clicked(QPoint pos);
};