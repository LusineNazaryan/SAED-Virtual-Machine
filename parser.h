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

class CParser
{
public:
	CParser(QVector<DataToken> data_tokens_, QVector<CodeToken> code_tokens_);
	void output();

private:
	void readDataTokens(QDataStream&);
	void readCodeTokens(QDataStream&);
	uint16 getArgCode(QString argName);
private:
	QVector<DataToken> m_dataTokens;
	QVector<CodeToken> m_codeTokens;
	QHash<QString, int> nameGrammar;

};




#endif