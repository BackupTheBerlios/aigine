/* Game Engine Design */


#include <iostream>
using namespace std;
#include "Mouse.h"
#include <math.h>

/**
* Constructor
* @param: a pointer to a SceneManagement- object who contains all further needed objects
*	like camara, renderer and the SceneGraph
*/
Mouse::Mouse(SceneManagement* sceneMan)
{
	this->sceneMan = sceneMan;
	angleX = 0.0;
	angleY = 1.0;
	angleZ = 0.0;
	
	this->old_x = -1;
	this->old_y = -1;
}


void Mouse::passivemotion(int x, int y){
	int * winSize = this->sceneMan->getWindowSize();

	cout<<"x : "<< x <<"   y : "<<y<<endl;
	cout<<"windowSize : "<<winSize[0] <<" | "<<winSize[1]<<endl;
	
	Point3D *rotation = new Point3D(this->angleX, this->angleY, this->angleZ);

	//this->sceneMan->getCamera()->setNormalVector(rotation_vector);
	this->sceneMan->getCamera()->setRotation(rotation, x, y, winSize);
	this->old_x = x;
	this->old_y = y;
	glutPostRedisplay();
}
