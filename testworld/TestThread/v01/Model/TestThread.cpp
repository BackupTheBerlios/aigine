// TestThread.cpp: Implementierung der Klasse TestThread.
//
//////////////////////////////////////////////////////////////////////

#include "TestThread.h"










//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

TestThread::TestThread()
{

}

TestThread::TestThread(char* name)
{
	this->name = name;
	cout << "construct ->" << this->name <<  " :: " << this << endl;
}

TestThread::~TestThread()
{

}

void* TestThread::Thread()
{
	cout << "start ->" << this->name <<  " :: " << this << endl;
	return this;
}
