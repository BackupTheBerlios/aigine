// IOControl.h: Schnittstelle für die Klasse IOControl.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOCONTROL_H__E26598EF_6B10_4744_9880_3C5BF684918F__INCLUDED_)
#define AFX_IOCONTROL_H__E26598EF_6B10_4744_9880_3C5BF684918F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <GL\glut.h>
#include <iostream>
using namespace std;

//#define int ANGULAR_SCALE = 5.0;
// eine Winkel struktur
	typedef struct {
		float x;
		float y;
		float z;
	} _angles;
class IOControl  
{
private:
	

	// 0 = global, 1+2+3 = entsprechendes Armelement
	//_angles** _arm_angles;
	_angles* _angle;
	_angles* _temp_angle;
	_angles* _active_angle;

public:
	IOControl(); //std contructor

	void keyboard(unsigned char key, int x, int y);
	
	//Getter und Setter

	_angles* getAngle() ;
	void setAngle(_angles* allocAngle);
/*	
	_angles** getArm_angles();
	void setArm_angles(_angles** arm_angle_ptr);
*/
	_angles* getTemp_angle() ;
	void setTemp_angle(_angles* angle_ptr);
	
	//_angles* getActive_angle() ;
	//void setActive_angle(_angles* angle_ptr);

	virtual ~IOControl(); //std destructor

};

#endif // !defined(AFX_IOCONTROL_H__E26598EF_6B10_4744_9880_3C5BF684918F__INCLUDED_)
