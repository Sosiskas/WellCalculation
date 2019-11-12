#include "Converter.h"
#include <cmath>

void Converter::makeCoordFromData(const CsvData& Props)
{
	_dataVector = Props;
	convertAngleMethod();
	convertMinimalCurvatureMethod();
}

void Converter::convertAngleMethod()
{
	_averageAngleMethodData.resize(0);
	_averageAngleMethodData.push_back(Coordinates());

	for (int i = 1; i < _dataVector.size() - 1; i++)
	{
		Coordinates xyz;
		xyz.x = _averageAngleMethodData[i - 1].x + calculationXAvAnMethod(i);
		xyz.y = _averageAngleMethodData[i - 1].y + calculationYAvAnMethod(i);
		xyz.z = _averageAngleMethodData[i - 1].z + calculationZAvAnMethod(i);
		_averageAngleMethodData.push_back(xyz);
	}
}

void Converter::convertMinimalCurvatureMethod()
{
	_minimalCurvatureMethodData.resize(0);
	_minimalCurvatureMethodData.push_back(Coordinates());

	for (int i = 1; i < _dataVector.size() - 1; i++)
	{
		Coordinates xyz;
		xyz.x = _minimalCurvatureMethodData[i - 1].x + calculationXMinCurMethod(i)*100;
		xyz.y = _minimalCurvatureMethodData[i - 1].y + calculationYMinCurMethod(i)*100;
		xyz.z = _minimalCurvatureMethodData[i - 1].z + calculationZMinCurMethod(i)*100;
		_minimalCurvatureMethodData.push_back(xyz);
	}
}

ChartingData Converter::getDataAvAnMethod()
{
	return _averageAngleMethodData;
}

ChartingData Converter::getDataMinCurMethod()
{
	return _minimalCurvatureMethodData;
}

double Converter::getZenithMean(double a, double b)
{
	return (((a + b) / 2)* PI) / 180;
}

double Converter::calculationXAvAnMethod(int count)
{
	double sunA = sin(getZenithMean(_dataVector[count - 1].INC, _dataVector[count].INC));
	double sinB = sin(getZenithMean(_dataVector[count - 1].AZM, _dataVector[count].AZM));
	return 	(_dataVector[count].MD - _dataVector[count - 1].MD) * sunA * sinB;
}

double Converter::calculationYAvAnMethod(int count)
{
	return 	(_dataVector[count].MD - _dataVector[count - 1].MD) *
			sin(getZenithMean(_dataVector[count - 1].INC, _dataVector[count].INC)) *
			cos(getZenithMean(_dataVector[count - 1].AZM, _dataVector[count].AZM));
}

double Converter::calculationZAvAnMethod(int count)
{
	return 	(_dataVector[count].MD - _dataVector[count - 1].MD) *
		cos(getZenithMean(_dataVector[count - 1].INC, _dataVector[count].INC));
}

double Converter::calculationXMinCurMethod(int count)
{
	double result = ((_dataVector[count].MD - _dataVector[count - 1].MD) / 2) * (sin(convertFromAngleToRadian(_dataVector[count].INC)) * 
					sin(convertFromAngleToRadian(_dataVector[count].AZM)) +
					sin(convertFromAngleToRadian(_dataVector[count-1].INC)) * 
					sin(convertFromAngleToRadian(_dataVector[count-1].AZM))) * calculateRFParameter(calculationCosB(count));
	return result;
}

double Converter::calculationYMinCurMethod(int count)
{
	return ((_dataVector[count].MD - _dataVector[count - 1].MD) / 2) * (sin(convertFromAngleToRadian(_dataVector[count].INC)) *
		cos(convertFromAngleToRadian(_dataVector[count].AZM)) +
		sin(convertFromAngleToRadian(_dataVector[count - 1].INC)) *
		cos(convertFromAngleToRadian(_dataVector[count - 1].AZM))) * calculateRFParameter(calculationCosB(count));
}

double Converter::calculationZMinCurMethod(int count)
{
	return ((_dataVector[count].MD - _dataVector[count - 1].MD) / 2) * (cos(convertFromAngleToRadian(_dataVector[count].INC)) * 
		cos(convertFromAngleToRadian(_dataVector[count-1].INC))) * 
		calculateRFParameter(calculationCosB(count));
}

double Converter::convertFromAngleToRadian(double value)
{
	return value*PI / 180;
}

double Converter::convertFromRadianToAngle(double value)
{
	return value*180/PI;
}

double Converter::calculateRFParameter(double cosB)
{
	double angleB = convertFromRadianToAngle(acos(cosB));
	if (angleB == 0)
		return 0;
	double result = (2 / angleB) * tan(convertFromAngleToRadian(angleB/2));
	return result;
}

double Converter::calculationCosB(int count)
{
	double result = cos(convertFromAngleToRadian(_dataVector[count].INC - _dataVector[count-1].INC)) - 
					sin(convertFromAngleToRadian(_dataVector[count].INC)) * 
					sin(convertFromAngleToRadian(_dataVector[count-1].INC)) * 
					(1 - cos(convertFromAngleToRadian(_dataVector[count].AZM - _dataVector[count-1].AZM)));
	return result;
}
