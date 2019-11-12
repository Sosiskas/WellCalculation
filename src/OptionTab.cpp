#include "OptionTab.h"

OptionTab::OptionTab(QWidget* parent)
{
	_ui.setupUi(this);
	connect(_ui.browserButton, &QPushButton::clicked, this, &OptionTab::fileDialog);
}

OptionTab::~OptionTab()
{
}

void OptionTab::fileDialog()
{
	QString dir = QFileDialog::getOpenFileName(this,
		tr("Open Csv"),
		QString(),
		tr("Csv Files (*.csv)"));
	_ui.lineEdit->setText(dir);
}
