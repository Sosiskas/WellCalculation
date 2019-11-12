#pragma once
#include "ui_OptionTab.h"
#include <QWidget>
#include <QFileDialog>

class OptionTab : public QWidget
{
	Q_OBJECT
public:
	OptionTab(QWidget* parent = 0);
	~OptionTab();
private:
	void fileDialog();
	Ui::OptionTab _ui;
};