/* Game Engine Design
 * Keyboard.cpp
 */

#include "Keyboard.h"
#include "../Model/SceneManagement.h"

//____________________________________________________________________________
void Keyboard::keyAction(unsigned char key, int mouseX, int mouseY){
	if(key ==  27) exit(0); // ESC for program quit
	if(key == 'w') this->sceneMan->getCamera()->moveForward(1.0);
	if(key == 's') this->sceneMan->getCamera()->moveBack(1.0);
	if(key == 'a') this->sceneMan->getCamera()->moveLeft(1.0);
	if(key == 'd') this->sceneMan->getCamera()->moveRight(1.0);
	if(key == 'n') this->sceneMan->getCamera()->setViewFogMode(true,254.0, 236.0, 208.0, 1.0, 0.2, 100.0, 160.0, GL_LINEAR,GL_FOG_HINT, GL_DONT_CARE);//spaeter ueber einstellungsdialog bzw inidatei
    if(key == 'N') this->sceneMan->getCamera()->setViewFogMode(false,0.0, 0.0, 0.0, 1.0, 0.2, 30.0, 100.0, GL_LINEAR,GL_FOG_HINT, GL_DONT_CARE);

//	glutPostRedisplay();
}

//____________________________________________________________________________
Keyboard::Keyboard(SceneManagement* sceneMan)
{
	this->sceneMan = sceneMan;
}
