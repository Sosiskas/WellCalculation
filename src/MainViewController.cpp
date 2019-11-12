#include "MainViewController.h"

MainViewController::MainViewController()
{
	_mainWidget = new MainView();
	_listener = new MainViewListener();
	_ogl = new OGL();
	_optionTab = new OptionTab();
	_tabWidget = _mainWidget->findChild<QTabWidget*>("tabWidget");

	configureTabs();

	_listener->setCtrl(this);
	_listener->setWidget(_mainWidget);
	_listener->connectSignals();

	_lineEdit = _optionTab->findChild<QLineEdit*>("lineEdit");
}

MainViewController::~MainViewController()
{
	delete _mainWidget;
	delete _listener;
}

void MainViewController::execute()
{
	_mainWidget->exec();
}

void MainViewController::configureTabs()
{
	_tabWidget->clear();
	_tabWidget->addTab(_ogl, "gl");
	_tabWidget->addTab(_optionTab, "Options");
}

void MainViewController::configureTabXZGraf()
{
	_xzView = new QtCharts::QChartView;

	QtCharts::QValueAxis* xAxis = new QtCharts::QValueAxis;
	xAxis->setRange(-6000, 6000);
	xAxis->setTitleText("X");
	xAxis->setTitleBrush(Qt::magenta);
	xAxis->setLabelsColor(Qt::magenta);

	QtCharts::QValueAxis* yAxis = new QtCharts::QValueAxis;
	yAxis->setRange(-14000, 0);
	yAxis->setTitleText("Z");
	yAxis->setTitleBrush(Qt::yellow);
	yAxis->setLabelsColor(Qt::yellow);

	_xzView->chart()->setTheme(QtCharts::QChart::ChartThemeBlueCerulean);

	QPen XZPen;
	XZPen.setColor(QColor(255, 0, 0));
	XZPen.setWidth(3);

	QtCharts::QLineSeries* xzGraf = new QtCharts::QLineSeries;
	xzGraf->setName("XZ View");
	xzGraf->setPen(XZPen);
	xzGraf->setUseOpenGL(true);

	_xzView->chart()->addSeries(xzGraf);
	_xzView->chart()->setAxisX(xAxis, xzGraf);
	_xzView->chart()->setAxisY(yAxis, xzGraf);
	
	for (int i = 0; i < _dataAvAnMethod.size(); i++)
	{
		xzGraf->append(_dataAvAnMethod[i].x, -_dataAvAnMethod[i].z);
	}

	_tabWidget->addTab(_xzView, "XZ");
}

void MainViewController::configureTabXYGraf()
{
	_xyView = new QtCharts::QChartView;

	QtCharts::QValueAxis* xAxis = new QtCharts::QValueAxis;
	xAxis->setRange(-6000, 6000);
	xAxis->setTitleText("X");  
	xAxis->setTitleBrush(Qt::magenta);  
	xAxis->setLabelsColor(Qt::magenta);   

	QtCharts::QValueAxis* yAxis = new QtCharts::QValueAxis;
	yAxis->setRange(-6000, 6000);
	yAxis->setTitleText("Y");
	yAxis->setTitleBrush(Qt::yellow);
	yAxis->setLabelsColor(Qt::yellow);

	_xyView->chart()->setTheme(QtCharts::QChart::ChartThemeBlueCerulean);

	QPen XYPen;
	XYPen.setColor(QColor(255, 0, 0));
	XYPen.setWidth(3);

	QtCharts::QLineSeries* xyGraf = new QtCharts::QLineSeries;
	xyGraf->setName("XY View");
	xyGraf->setPen(XYPen);
	xyGraf->setUseOpenGL(true);

	_xyView->chart()->addSeries(xyGraf);        
	_xyView->chart()->setAxisX(xAxis, xyGraf);  
	_xyView->chart()->setAxisY(yAxis, xyGraf);
	
	for (int i = 0; i < _dataAvAnMethod.size(); i++)
	{
		xyGraf->append(_dataAvAnMethod[i].x, _dataAvAnMethod[i].y);
	}

	_tabWidget->addTab(_xyView, "XY");
}

void MainViewController::pushConvertButton()
{
	configureTabs();
	QString path = _lineEdit->text();
	if (path.isEmpty())
	{
		informationgMessage(Signals::DIRECTORY_IS_EMPTY);
		return;
	}

	CsvData data;
	if (CsvReader().readFromFile(path, data) == -1)
	{
		informationgMessage(Signals::FILE_IS_EMPTY);
		return;
	}

	//informationgMessage(Signals::GOOD_READING);

	Converter converter;
	converter.makeCoordFromData(data);

	_dataAvAnMethod = converter.getDataAvAnMethod();
	_ogl->setData(_dataAvAnMethod);
	_ogl->setMinimalCurvatureMethodData(converter.getDataMinCurMethod());
	configureTabXYGraf();
	configureTabXZGraf();
}

void MainViewController::informationgMessage(Signals signal)
{
	QMessageBox msgBox(_mainWidget);
	msgBox.setText("");
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.setDefaultButton(QMessageBox::Ok);

	switch (signal)
	{
	case FILE_IS_EMPTY:
	{
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setInformativeText(MESSAGE_FILE_IS_EMPTY);
	} break;
	case GOOD_READING:
	{
		msgBox.setIcon(QMessageBox::Information);
		msgBox.setInformativeText(MESSAGE_GOOD_READING);
	} break;
	case DIRECTORY_IS_EMPTY:
	{
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setInformativeText(MESSAGE_DIRECTORY_EMPTY);
	} break;
	default:
		break;
	}
	
	int ret = msgBox.exec();
}

void MainViewController::resizeEvent(const Size& newSize)
{
	_ogl->resize(newSize.width, newSize.height);
}
