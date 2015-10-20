#include "cqlabel.h"
#include <QMouseEvent>

CQLabel::CQLabel(QWidget *parent) : QLabel(parent) {}
CQLabel::~CQLabel() {}

void CQLabel::mousePressEvent(QMouseEvent *ev) {
	emit clicked(ev->pos());
}

void CQLabel::mouseReleaseEvent(QMouseEvent * ev) {

}
void CQLabel::mouseMoveEvent(QMouseEvent * ev) {

}