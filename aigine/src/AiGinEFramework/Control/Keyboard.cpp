/* Game Engine Design */

#include "Keyboard.h"



void Keyboard::keyAction(unsigned char key, int mouseX, int mouseY){
   switch (key) {
      case ('w'): // move front
		  this->sceneMan->getCamera()->moveCamera(0.5);
         glutPostRedisplay();
         break;
      case ('s'): // nach back
		  this->sceneMan->getCamera()->moveCamera(-0.5);
         glutPostRedisplay();
         break;
      case ('a'): // move left
		  this->sceneMan->getCamera()->strafeCamera(-0.5);
         glutPostRedisplay();
         break;
      case ('d'): // move right
		  this->sceneMan->getCamera()->strafeCamera(0.5);
         glutPostRedisplay();
         break;
      case 27: // ESC key code
         exit(0);
         break; 

/*      case ('y'):
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
      case('0'):
         _active_angle = &_arm_angles[0];
         break;
      case('1'):
         _active_angle = &_arm_angles[1];
         break;
      case('2'):
         _active_angle = &_arm_angles[2];
         break;
      case('3'):
         _active_angle = &_arm_angles[3];
         break;
*/

   }  

}


Keyboard::Keyboard(SceneManagement* sceneMan)
{
	this->sceneMan = sceneMan;
}
