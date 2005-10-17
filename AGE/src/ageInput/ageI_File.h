// fileControl.h: interface for the fileControl class.
//
//////////////////////////////////////////////////////////////////////


#ifndef AGEI_FILE_H
#define AGEI_FILE_H

#include <iostream>
using namespace std;

class ageI_File  
{
public:
	ageI_File();
	ageI_File(const char* fileName);
	virtual ~ageI_File();
	void openFile(char r_w_a);
	void closeFile();
	
	void readBool(bool*);
	void readInt(int*);
	void readFloat(float*);
	void readDouble(double*);
	void readChar(char*);
	void readString(string*);

	void writeBool(bool*);
	void writeInt(int*);
	void writeFloat(float*);
	void writeDouble(double*);
	void writeChar(char*);
	void writeString(string*);

private:
	const char* fileName;
	FILE* workingFile;
};

#endif //AGEI_FILE_H
