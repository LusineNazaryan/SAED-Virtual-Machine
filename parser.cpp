#include "parser.h"

CParser::CParser(QVector<DataToken> data_tokens_, QVector<CodeToken> code_tokens_) :
m_dataTokens(data_tokens_), m_codeTokens(code_tokens_) {}

void CParser::output()
{
	QString FILENAME = "test.asm";
	QFile fout(FILENAME);
	if (!fout.open(QIODevice::WriteOnly))
	{
		qDebug() << "ERROR!!!";
	}
	QDataStream out(&fout);

	if (!m_dataTokens.isEmpty())
	{
		readDataTokens(out);
	}
	readCodeTokens(out);
}

void CParser::readCodeTokens(QDataStream& out)
{
	Opcode o_opcode;
	QVector<uint16> args;

	for (QVector<CodeToken>::iterator it = m_codeTokens.begin(); it != m_codeTokens.end(); ++it)
	{
		QString instrName = (*it).name;
		Pair pair = inst[instrName];
		o_opcode.instr = static_cast<uint16>(pair.instr_code);
		int argCount = inst[instrName].argument_count;
		while (argCount)
		{
			args[argCount - 1] = static_cast<uint16>((*it).arg_type[argCount - 1]);
			--argCount;
		}
		o_opcode.arg1Type = 0;
		o_opcode.arg2Type = 0; 
		o_opcode.arg3Type = 0;
		if (!args.isEmpty())
		{
			o_opcode.arg1Type = args.first();
			args.pop_back();
			if (!args.isEmpty())
			{
				o_opcode.arg1Type = args.first();
				args.pop_back();
				if (!args.isEmpty())
				{
					o_opcode.arg1Type = args.first();
					args.pop_back();
				}
			}
		}
		
		o_opcode.argSize = static_cast<uint16>(pair.argument_type);
		// write opcode in .exe
		out << o_opcode.opcode;
		
		if (!(*it).arg_vlaue[0].isEmpty())
		{
			QString arg = (*it).arg_vlaue[0];
			out << getArgCode(arg);

			// write getArgCode(arg) in .exe

			if (!(*it).arg_vlaue[1].isEmpty())
			{
				QString arg = (*it).arg_vlaue[1];
				out << getArgCode(arg);
				// write getArgCode(arg) in .exe

				if (!(*it).arg_vlaue[2].isEmpty())
				{
					QString arg = (*it).arg_vlaue[2];
					out << getArgCode(arg);
					// write getArgCode(arg) in .exe
				}
			}
		}

	}

}

void CParser::readDataTokens(QDataStream& out)
{
	int name = 5;
	for (QVector<DataToken>::iterator it = m_dataTokens.begin(); it != m_dataTokens.end(); ++it)
	{
		//write it.size
		out << static_cast<int>((*it).size);
		//write name
		out << name;
		nameGrammar[(*it).name] = name;
		if (!(*it).value.isNull())
		{
			//write it.value
			out << (*it).value;
			name += 4;
		}
		name += 4;
	}
}

uint16 CParser::getArgCode(QString argName)
{
	int regNum = argName.right(argName.size() - 1).toInt();

	if ((argName[0] == 'R') && (regNum >= 0 && regNum <= 1023))
	{
		return regNum;
	}
	else if ((argName[0] == 'A') && (regNum >= 0 && regNum <= 3))
	{
		return regNum;
	}
	else if (argName == "IP")
	{
		return 4;
	}
	else if (argName == "TR")
	{
		return 5;
	}
	else if (argName == "SP")
	{
		return 6;
	}
	else if (argName == "SF")
	{
		return 7;
	}
	else if (argName == "FLAGS")
	{
		return 8;
	}
	else
	{
		return nameGrammar[argName];
	}

}

