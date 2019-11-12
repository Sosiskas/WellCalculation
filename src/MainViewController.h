#pragma once 
#include "MainView.h"
#include "CsvReader.h"
#include "Converter.h"
#include "qpushbutton.h"
#include "QLineEdit.h"
#include <QMessageBox>
#include "qchartview.h"
#include "qvalueaxis.h"
#include "qlineseries.h"
#include "OptionTab.h"

class MainViewController
{
public:
	MainViewController();
	~MainViewController();

	void execute();
	friend class MainViewListener;
private:
	void configureTabs();
	void configureTabXZGraf();
	void configureTabXYGraf();
	void pushConvertButton();
	void informationgMessage(Signals signal);
	void resizeEvent(const Size& newSize);

	MainView* _mainWidget;
	OGL*		_ogl;
	OptionTab*	_optionTab;
	MainViewListener* _listener;
	QtCharts::QChartView* _xyView;
	QtCharts::QChartView* _xzView;

	ChartingData _dataAvAnMethod;

	QTabWidget* _tabWidget;
	QLineEdit* _lineEdit;

};

class MainViewListener : public QObject
{
public:
	MainViewListener()
	{

	}
	void setCtrl(MainViewController* ctrl)
	{
		_controller = ctrl;
	};
	void setWidget(MainView* widget)
	{
		_tabWidget = widget;
	};
	void connectSignals()
	{
		QPushButton* convertButton = _tabWidget->findChild<QPushButton*>("convertButton");

		connect(convertButton, &QPushButton::clicked, this, &MainViewListener::pushConvertButton);
		connect(_tabWidget, &MainView::changeSize, this, &MainViewListener::resizeEvent);
	};
	void pushConvertButton()
	{
		_controller->pushConvertButton();
	};
	void resizeEvent(const Size& newSize)
	{
		_controller->resizeEvent(newSize);
	};
private:
	MainView* _tabWidget;
	MainViewController* _controller;
};