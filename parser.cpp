#include "parser.h"
#include <iostream>
#include <cmath>
using namespace std;

CParser::CParser(QDataStream& out, QVector<SDataToken> dataTokens, QVector<SCodeToken> codeTokens) :
m_out(out), m_dataTokens(dataTokens), m_codeTokens(codeTokens)
{
}

void CParser::work()
{
	writeHeader(m_out);

	if (!m_dataTokens.isEmpty())
	{
		writeDataSection(m_out);
	}
	writeCodeSection(m_out);
	if (!m_dataTokens.isEmpty())
	{
		writeDataTableSection(m_out);
	}
	writeCodeTableSection(m_out);
}

SHeader CParser::createHeader(QDataStream& out)
{
	QString signature = "SAED/VirtualMachine/ExeFile";
	int version = 1;
	int offset = sizeof signature + sizeof(int);

	SHeader header(signature, version);

	if (!m_dataTokens.isEmpty())
	{
		SHeaderTable headerTable(ESectionType::Data, offset, m_dataTokens.size());
		header.recordTable.push_back(headerTable);
		offset += writeDataSection(out);
	}

	{
		SHeaderTable headerTable(ESectionType::Code, offset, m_codeTokens.size());
		header.recordTable.push_back(headerTable);
		offset += writeCodeSection(out);
	}

	if (!m_dataTokens.isEmpty())
	{
		SHeaderTable headerTable(ESectionType::DataTable, offset, m_dataTokens.size());
		header.recordTable.push_back(headerTable);
		offset += writeDataTableSection(out);
	}
	
	/*
	{
	SHeaderTable headerTable(ESectionType::CodeTable, offset, m_dataTokens.size());
	header.arrayTable.push_back(headerTable);
	offset += writeCodeTableSection(m_out);
	}
	
	*/
	header.recordCount = header.recordTable.size();
	return header;
}

void CParser::writeHeader(QDataStream& out)
{
	SHeader header = createHeader(out);

	out << header.signature;
	out << header.version;
	out << header.recordCount;

	for (int i = 0; i < header.recordCount; ++i)
	{
		out << static_cast<int>(header.recordTable[i].type);
		out << header.recordTable[i].offset;
		out << header.recordTable[i].size;
	}
}

int CParser::writeCodeSection(QDataStream& out)
{
	int offset = 0;			//size of codeSection

	for (int i = 0; i < m_codeTokens.size(); i++)
	{
		out << m_codeTokens[i].opcode.opcode;
		offset += sizeof m_codeTokens[i].opcode.opcode;
		for (int j = 0; j < m_codeTokens[i].argValue.size(); j++)
		{
			out << m_codeTokens[i].argValue[j];
			offset += sizeof m_codeTokens[i].argValue[j];
		}
	}
	return offset;
}

int CParser::writeDataSection(QDataStream& out)
{
	int offset = 0;		//size of dataSection
	uint8 zero = 0;
	int remainder = 0;
	for (int i = 0; i < m_dataTokens.size(); i++)
	{
		m_nameGrammar[m_dataTokens[i].identifierName] = offset;
		int dataType = static_cast<int>(m_dataTokens[i].type);
		if (dataType)
		{
			int typeToSize = static_cast<int> (pow(2, dataType));
			remainder = offset % typeToSize;
			offset += typeToSize - remainder;
		}
		else 
		{
			remainder = 0;
		}
		while (remainder != 0)
		{
			out << zero;
			remainder--;
		}
		out << m_dataTokens[i].value;
		offset += pow(2, dataType);
		
		/*cout << static_cast<int>(m_dataTokens[i].type);
		qDebug() << m_dataTokens[i].identifierName;
		qDebug() << m_dataTokens[i].value;*/
	}
	return offset;

}

int CParser::writeDataTableSection(QDataStream& out)
{
	int offset = 0;
	for (int i = 0; i < m_dataTokens.size(); i++)
	{                                                
		int dataType = static_cast<int>(m_dataTokens[i].type);
		out << dataType;
		out << m_dataTokens[i].identifierName;
	//	out << m_dataTokens[i].value;
		out << m_dataTokens[i].line;
		offset += sizeof(int) + m_dataTokens[i].identifierName.size() +
				/*dataType +*/ sizeof(int);
	}
	return offset;
}

int CParser::writeCodeTableSection(QDataStream& out)
{
	int offset = 0;
	/*for (int i = 0; i < m_codeTokens.size(); i++)
	{
		out << m_codeTokens[i].opcode.opcode;

	}*/
	return offset;
}



