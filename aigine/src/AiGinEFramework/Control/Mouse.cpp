/* Game Engine Design */


#include "Mouse.h"
#include "../GraphicEngine/SceneManagement.h"

Mouse::Mouse(SceneManagement* sceneMan)
{
	this->sceneMan = sceneMan;
}


void Mouse::passivemotion(int x, int y){
	this->sceneMan->setCameraRotation(x, y);
	glutPostRedisplay();
}
