#pragma once
#include <vector>
#define PI 3.14159265 
#include <QString>

enum Signals
{
	FILE_IS_EMPTY,
	GOOD_READING,
	DIRECTORY_IS_EMPTY
};

struct Size
{
	int width;
	int height;
};

static const QString MESSAGE_FILE_IS_EMPTY = "Файл пуст. Построение графиков невозможно.";
static const QString MESSAGE_GOOD_READING = "Успешное чтение";
static const QString MESSAGE_GOOD_CONVERTION = "Успешная конвертация";
static const QString MESSAGE_ERROR_CONVERSION = "Ошибка данных csv файла";
static const QString MESSAGE_DIRECTORY_EMPTY = "Ввудите путь до файла";

struct Params
{
	double MD;
	double INC;
	double AZM;
};

struct Coordinates
{
	Coordinates()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	float x;
	float y;
	float z;
};

typedef std::vector<Coordinates> ChartingData;
typedef std::vector<Params> CsvData;