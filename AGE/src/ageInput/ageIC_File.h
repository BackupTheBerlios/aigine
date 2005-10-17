// fileControl.h: interface for the fileControl class.
//
//////////////////////////////////////////////////////////////////////


#ifndef AGEIC_FILE_H
#define AGEIC_FILE_H

#include <iostream>
using namespace std;

class ageIC_File  
{
public:
	ageIC_File();
	ageIC_File(const char* fileName);
	virtual ~ageIC_File();
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

#endif //AGEIC_FILE_H
