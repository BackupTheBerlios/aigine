/* Game Engine Design 
 * Mouse.cpp  
 */

#include "Mouse.h"
#include "../Model/SceneManagement.h"

//____________________________________________________________________________
Mouse::Mouse(SceneManagement* sceneMan)
{
	this->sceneMan = sceneMan;
}

//____________________________________________________________________________
void Mouse::passivemotion(int x, int y){
	this->sceneMan->setCameraRotation(x, y);
	glutPostRedisplay();
}
