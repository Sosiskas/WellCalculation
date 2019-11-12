#include <QtWidgets/QApplication>
#include "MainViewController.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainViewController w;
	w.execute();
	return 0;
}
