// IOControl.cpp: Implementierung der Klasse IOControl.
//
//////////////////////////////////////////////////////////////////////

#include "IOControl.h"

//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

IOControl::IOControl()
{
	// Armwinkel allozieren und initialisieren
   
      _angle = (_angles*) malloc(sizeof(_angles));
      _angle->x = 0.0;
      _angle->y = 0.0;
      _angle->z = 0.0;
   
   // setze aktuelle Rotation auf global
   _active_angle = _angle;
}

IOControl::~IOControl()
{

}



void IOControl::keyboard(unsigned char key, int x, int y)
{

	switch (key) {
      case 27: // ESC key code
         exit(0);
         break;
      case ('x'):
         _active_angle->x++;
         glutPostRedisplay();
         break;
      case ('y'):
         //_y_angular++;
         _active_angle->y++;
         glutPostRedisplay();
         break;
      case ('z'):
         _active_angle->z++;
         glutPostRedisplay();
         break;
      case ('X'):
         //_x_angular--;
         _active_angle->x--;
         glutPostRedisplay();
         break;
      case ('Y'):
         //_y_angular--;
         _active_angle->y--;
         glutPostRedisplay();
         break;
      case ('Z'):
         _active_angle->z--;
         //_z_angular--;
         glutPostRedisplay();
         break;
     /* case('0'):
         _active_angle = _arm_angles[0];
         break;
      case('1'):
         _active_angle = _arm_angles[1];
         break;
      case('2'):
         _active_angle = _arm_angles[2];
         break;
      case('3'):
         _active_angle = _arm_angles[3];
         break;
		 */
      // Texturanpassungen
     case ('s'):
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
         glutPostRedisplay();
         break;
      case ('S'):
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
         glutPostRedisplay();
         break;
      case ('t'):
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
         glutPostRedisplay();
         break;
      case ('T'):
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
         glutPostRedisplay();
         break;
      case ('h'):
		 //print out help-datei
      	 system("less README.TXT");
         printf("\n");
   }
}
//Getter und Setter

_angles* IOControl::getAngle() {
	return _angle;
}
/**
*	Diese Struktur ist wie n Zepfchen... völlig für den arsch. hatt keine lust da jetzt grosartig
*	umzustrukturieren. wird ja eh alles ausgelagert...
*/
void IOControl::setAngle(_angles* allocAngle) {
	_angle = allocAngle;
}

/*	
_angels** IOControl::getArm_angles() {
	return _arm_angles;
}
void IOControl::setArm_angles(_angles** arm_angle_ptr) {
	_arm_angles = arm_angle_ptr;	
}
*/
_angles* IOControl::getTemp_angle() {
	return _temp_angle;
}

void IOControl::setTemp_angle(_angles* angle_ptr) {
	this->_temp_angle = angle_ptr;

}	








