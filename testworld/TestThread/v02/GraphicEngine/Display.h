// Display.h: Schnittstelle für die Klasse Display.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISPLAY_H__7E2E5D21_24D5_41BF_8129_E60AE5F8717B__INCLUDED_)
#define AFX_DISPLAY_H__7E2E5D21_24D5_41BF_8129_E60AE5F8717B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <GL/glut.h>
#include <iostream>
using namespace std;

#include "IOControl.h"

#define ANGULAR_SCALE 5.0

typedef struct Line
{
	GLfloat start[3];
    GLfloat end[3];
    GLfloat angle[3];
} Line;





class Display  
{
public:

	Display(IOControl *ioTmp);

	float get_x();
	float get_y();
	float get_z();
	void reshape(int w,int h);
	void display_cube();
	static void init(void);
	Display();
	

	virtual ~Display();

private:
	//die Control muss hier zwischen gespeichert werden um 
	//auf die winkel information zugreifen zu können
	IOControl *_IoTmp; 

	float _x ;
	float _y ;
	float _z ;

};

#endif // !defined(AFX_DISPLAY_H__7E2E5D21_24D5_41BF_8129_E60AE5F8717B__INCLUDED_)
