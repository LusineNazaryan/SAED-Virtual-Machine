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

	const QString filename = "test.asm";
	QFile fin(filename);
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
	//try
	//{
	//	CLexer lexer;

	//	lexer.work(input);

	//	QVector<SCodeToken> o = lexer.getCode();

	//	QVector<SDataToken> k = lexer.getData();
	//	lexer.p();
	//	for (const auto aa : k)
	//	{
	//		qDebug() << aa.identifierName << " line data " << aa.line << " with offset " << aa.value << " type " << static_cast<uint32>(aa.type);
	//	}

	//	for (auto a : o)
	//	{
	//		qDebug() << " line code" << a.line;
	//		qDebug() << a.opcode.instr;
	//		for (const auto i : a.argValue)
	//		{
	//			qDebug() << i;
	//		}
	//	}
	//}
	//catch (CError& e)
	//{
	//	qDebug() << e.ShowError();
	//}

	QString fileName = "test.txt";
	QFile parseFile(fileName);
	if (!parseFile.open(QIODevice::WriteOnly))
	{
		qDebug() << "error cann't open file";
	}
	


	QDataStream parseOut(&parseFile);

	CLexer lexer;
	lexer.work(input);
	QVector<SDataToken> dataTokens = lexer.getData();
	QVector<SCodeToken> codeTokens = lexer.getCode();
	CParser parser(parseOut, dataTokens, codeTokens);
	//parseOut << fileName;
	parser.work();

	parseFile.close();

	QFile fout(fileName);
	QFile fout2("output.asm");
	if (!fout.open(QIODevice::ReadOnly))
	{
		qDebug() << "error cann't open file1";
	}
	QDataStream in(&fout);

	if (!fout2.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		qDebug() << "error cann't open file2";
	}
	QTextStream in2(&fout2);


	while (!in.atEnd())
	{
		QString signature;
		int version;
		int recC;
		
		in >> signature;
		in2 << signature;
		qDebug() << "signature:		" << signature;
		in >> version;
		in2 << version;
		qDebug() << "version:	" << version;
		in >> recC;
		in2 << recC;
		qDebug() << "recordCount:	" << recC;
		/*in >> rec2;
		in2 << rec2;
		qDebug() << "record2:	" << rec2;
		in >> rec2;
		in2 << rec2;
		qDebug() << "record2:	" << rec2;
		in >> rec2;
		in2 << rec2;
		qDebug() << "record2:	" << rec2;
		in >> rec2;
		in2 << rec2;
		qDebug() << "record2:	" << rec2;
		QString mnac;
		in2 << mnac;
		qDebug() << "mnac:	" << mnac;*/
	}
	fout.close();
	fout2.close();
	
	exit(1);

	return aaa.exec();
}
