/* Game Engine Design */

#ifndef MOUSE_H
#define MOUSE_H

#include <GL/glut.h>
#include <iostream>
using namespace std;

#include "../GraphicEngine/SceneManagement.h"
#include "../Model/Point3D.h"

class Mouse
{
public:    
	Mouse(SceneManagement* sceneMan);
    void passivemotion(int x, int y);
	SceneManagement* sceneMan;

private:
	float angleX ; //angle for the x - achsis of the camera
	float angleY ;
	float angleZ ;

	int old_x;
	int old_y;
	
};
#endif //MOUSE_H
