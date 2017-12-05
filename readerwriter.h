#ifndef READERWRITER_H
#define READERWRITER_H

#include "lexer.h"
#include "instructions.h"
#include "header.h"

#include <QTextStream>
#include <QVector>
#include <qdebug.h>
#include <QDataStream>
#include <QFileInfo>

class CReaderWriter
{
public:
	CReaderWriter();

	void addSection(QByteArray buf);
	void write(QString filename);

	QByteArray read(QString sFileExe);

private:
	void createHeader();
	QByteArray writeHeader(QString signature, int32 version);
	void addSectionToHeader(int i, QByteArray buf);
	int getOffset(int i);

private:
	int32 m_headerSize;
	int32 m_nVersion;
	QString m_sSignature;
	SHeader m_SHeader;
	QByteArray m_buffer;
};




#endif