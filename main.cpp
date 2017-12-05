#include <QtCore/QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QTextStream>
#include <QFileInfo>

#include "compiler.h"

int main(int argc, char *argv[])
{
	QCoreApplication aaa(argc, argv);

	CCompiler compiler;
	compiler.work("test.asm","test.txt");

	
	exit(1);
	return aaa.exec();
}
