// fileControl.cpp: implementation of the fileControl class.
//
//////////////////////////////////////////////////////////////////////

#include "fileControl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

fileControl::fileControl() {
		
}

fileControl::fileControl(const char* fileName) {
	this->fileName = fileName;
}

fileControl::~fileControl()
{
	
}

void fileControl::openFile(char r_w_a) {
	if (this->fileName != NULL) {
		if(r_w_a == 'r') {
			workingFile  = fopen(this->fileName,"r");
		}
		else if(r_w_a == 'w') {
			this->workingFile  = fopen(this->fileName,"w");
		}
		else if(r_w_a == 'a') {
			this->workingFile  = fopen(this->fileName,"a");
		}
		else {
			printf("Wrong use of 'Open_Files' procedure, exiting...\n") ;
			printf("Please press enter to finish\n") ;
			getchar() ;
			exit(0) ;
		}
	} 
	else {
		printf("Wrong use of 'Open_Files' procedure, exiting...\n") ;
		printf("Please press enter to finish\n") ;
		getchar() ;
		exit(0) ;
	}
}

void fileControl::closeFile() {
	fclose(this->workingFile);
}

//read-Funktionen
void fileControl::readBool(bool* b) {}
void fileControl::readInt(int* i) {
		fscanf(this->workingFile,"%i ",i);
}
void fileControl::readFloat(float* f) {
	fscanf(this->workingFile,"%f ",f);
}
void fileControl::readDouble(double* d) {
	fscanf(this->workingFile,"%f",d);
}
void fileControl::readChar(char* ch) {
	fscanf(this->workingFile,"%c ",ch);
}
void fileControl::readString(string* str) {
	fscanf(this->workingFile,"%s ",str);
}

//write-Funktionen
void fileControl::writeBool(bool*) {}
void fileControl::writeInt(int* i) {
	fprintf(this->workingFile,"%d ",*i);
}
void fileControl::writeFloat(float* f) {
	fprintf(this->workingFile,"%f ",*f);
}
void fileControl::writeDouble(double* d) {
	fprintf(this->workingFile,"%f ",*d);
}
void fileControl::writeChar(char* ch) {
	fprintf(this->workingFile,"%c ",ch);
}
void fileControl::writeString(string* str) {
	fprintf(this->workingFile,"%s ",str);
}
