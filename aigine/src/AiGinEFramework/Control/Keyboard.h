/* Game Engine Design */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GL/glut.h>
//#include <iostream>
//using namespace std;

#include "../GraphicEngine/SceneManagement.h"
#include "../Model/Point3D.h"

//class SceneManagement;
class Point3D;

class Keyboard
{
public:
	Keyboard(SceneManagement* sceneMan);

    void keyAction(unsigned char key, int x, int y);    
	SceneManagement* sceneMan;
};
#endif //KEYBOARD_H
