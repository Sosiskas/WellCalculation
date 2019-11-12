#pragma once

#include "ui_Widget.h"
#include <QGLQuard.h>
#include <QMainWindow>
#include <QResizeEvent>
#include <QDialog>

class MainView : public QDialog
{
	Q_OBJECT

public:
	explicit MainView(QWidget *parent = Q_NULLPTR);
	
private slots:
	void resizeEvent(QResizeEvent *event);

private:

	Ui::Form _ui;

signals:
	void changeSize(const Size& newSize);
};