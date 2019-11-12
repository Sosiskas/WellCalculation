#include "MainView.h"

MainView::MainView(QWidget *parent) :
	QDialog(parent)
{
	_ui.setupUi(this);
}
void MainView::resizeEvent(QResizeEvent* e)
{
	QWidget::resizeEvent(e);
	emit changeSize({e->size().width(), e->size().height()});
}


