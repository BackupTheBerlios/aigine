// AigineThread.cpp: Implementierung der Klasse AigineThread.
//
//////////////////////////////////////////////////////////////////////

#include "AigineThread.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion
//////////////////////////////////////////////////////////////////////

AigineThread::AigineThread()
{

}
/**
* �bergabe des eindeutigen namens des zu erstellenden Threads
* @autor: Tobias Harpering
*/

AigineThread::AigineThread(char* name)
{
	this->_name = name;
}

//was wird noch gebraucht?






// Getter und Setter

char* AigineThread::getName() {
	return _name;
}

void AigineThread::setName(char* name) {
	this->_name = name;
}



////////////////////////////////////////////////////////////////////
//Destruktion
////////////////////////////////////////////////////////////////////
AigineThread::~AigineThread()
{

}
