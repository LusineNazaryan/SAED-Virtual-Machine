#ifndef REGISTER_H
#define REGISTER_H

#include <QByteArray>
#include <QVector>

struct Register
{
	QByteArray R;	//0 - 1023
	QVector<int> A;	//0 - 3
	int IP;	//4
	int TR;	//5
	int SP;	//6
	int SF;	//7
	int FLAGS;	//8

	Register()
	{
		R.resize(1024);
		A.resize(4);
	}
};



#endif