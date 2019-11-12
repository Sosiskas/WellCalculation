#pragma once
#include "Resource.h"

class Converter
{
public:
	void makeCoordFromData(const CsvData& Props);
	ChartingData getDataAvAnMethod();
	ChartingData getDataMinCurMethod();
private:
	void convertAngleMethod();
	void convertMinimalCurvatureMethod();

	double getZenithMean(double a, double b);
	double calculationXAvAnMethod(int count);
	double calculationYAvAnMethod(int count);
	double calculationZAvAnMethod(int count);

	double calculationXMinCurMethod(int count);
	double calculationYMinCurMethod(int count);
	double calculationZMinCurMethod(int count);

	double convertFromAngleToRadian(double value);
	double convertFromRadianToAngle(double value);
	double calculateRFParameter(double cosB);
	double calculationCosB(int count);
	CsvData _dataVector;
	ChartingData _averageAngleMethodData;
	ChartingData _minimalCurvatureMethodData;
};