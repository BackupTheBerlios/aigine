/* Game Engine Design */

#include "Keyboard.h"
#include "../GraphicEngine/SceneManagement.h"

void Keyboard::keyAction(unsigned char key, int x, int y){
   switch (key) {
      case ('x'):
		  this->sceneMan->getCamera()->moveDown();
         glutPostRedisplay();
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
Keyboard::Keyboard(SceneManagement* sceneMan){
	this->sceneMan = sceneMan;
}
