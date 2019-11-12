#pragma once
#include "Resource.h"
#include <QString>

class CsvReader
{
public:
	int readFromFile(const QString& name, CsvData& paramsVector);
};