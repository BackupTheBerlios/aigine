// fileControl.h: interface for the fileControl class.
//
//////////////////////////////////////////////////////////////////////


#ifndef FILECONTROL_H
#define FILECONTROL_H

#include "../AiGinE.h"
using namespace std;

class fileControl  
{
public:
	fileControl();
	fileControl(const char* fileName);
	virtual ~fileControl();
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

#endif //FILECONTROL_H
