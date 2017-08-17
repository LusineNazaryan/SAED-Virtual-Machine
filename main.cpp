#include <QtCore/QCoreApplication>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QTextStream>
#include <QFileInfo>

#include "lexer.h"
#include "instructions.h"
#include "parser.h"

int main(int argc, char *argv[])
{
	QCoreApplication aaa(argc, argv);

	QString FILENAME = "test.asm";
	QFile fin(FILENAME);
	QFileInfo f_info(fin);

	if (f_info.completeSuffix() != "asm")
	{
		//terminate
	}

	if (!fin.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Cannot open" << f_info.absoluteFilePath() <<
			"file doesn't exist";
	}

	QTextStream input(&fin);

	Lexer lex(input);

	QVector<CodeToken> o = lex.codeOut();

	QVector<DataToken> k = lex.dataOut();


	for (auto aa : k)
	{
		qDebug() << aa.name << " line data" << aa.line;
	}

	for (auto a : o)
	{
		qDebug() << a.name << " line code" << a.line;
		for (auto i : a.arg_vlaue)
		{
			qDebug() << i;
		}
	}

	CParser parser(k, o);
	parser.output();

	exit(1);
	return aaa.exec();
}
