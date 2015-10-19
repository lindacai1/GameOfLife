#include "cqlabel.h"
#include <QMouseEvent>

CQLabel::CQLabel(QWidget *parent) : QLabel(parent) {}
CQLabel::~CQLabel() {}

void CQLabel::mousePressEvent(QMouseEvent *ev) {
	this->x = ev->x();
	this->y = ev->y();
	emit clicked(ev->pos());
}