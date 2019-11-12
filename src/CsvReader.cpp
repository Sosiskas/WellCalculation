#include "CsvReader.h"
#include <QFile>
#include <QTextStream>

int CsvReader::readFromFile(const QString& name, CsvData& paramsVector)
{
	QFile file(name);
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return -1;

	QTextStream in(&file);
	while (!in.atEnd())
	{
		QString line = in.readLine();
		QStringList list = line.split(";");
		Params temp;
		temp.MD = list[0].toDouble();
		temp.INC = list[1].toDouble();
		temp.AZM = list[2].toDouble();
		paramsVector.push_back(temp);
	}
	return 0;
}
