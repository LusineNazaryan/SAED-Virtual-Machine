#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <QTextStream>
#include <QVector>
#include <qdebug.h>
#include "instructions.h"
#include <QDataStream>
#include <QFileInfo>

using uint16 = unsigned short;

enum class ESectionType 
{
	Data, Code, DataTable, CodeTable
};

struct SHeaderTable{
	SHeaderTable()
	{
	}
	SHeaderTable(ESectionType tt, int oo, int ss) : type(tt), offset(oo), size(ss)
	{
	}
	ESectionType type;
	int offset;
	int size;
};

struct SHeader{
	SHeader()
	{
	}
	SHeader(QString ss, int vv) :signature(ss), version(vv)
	{
	}
	QString signature;
	int version;
	int recordCount;
	QVector<SHeaderTable> recordTable;
};
/*
struct dataPair{
	int position;
	EType type;
};
*/
class CParser
{
public:
	CParser(QDataStream& out, QVector<SDataToken> dataTokens, QVector<SCodeToken> codeTokens);
	void work();

private:
	void writeHeader(QDataStream&);
	SHeader createHeader(QDataStream&);
	int writeDataSection(QDataStream&);
	int writeCodeSection(QDataStream&);
	int writeDataTableSection(QDataStream&);
	int writeCodeTableSection(QDataStream&);
//	uint16 getArgCode(QString argName);
private:
	SHeader m_SHeader;
	QVector<SDataToken> m_dataTokens;
	QVector<SCodeToken> m_codeTokens;
	QHash<QString, int> m_nameGrammar;
	QDataStream& m_out;

};




#endif