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

	int * winSize = NULL;
	int * winCenter = new int;
	
	 cout<<"x : "<< x <<"   y : "<<y<<endl;
	 winSize = this->sceneMan->getWindowSize();
	 cout<<"windowSize : "<<winSize[0] <<" | "<<winSize[1]<<endl;
	
	 //center of the corent gl window
	 winCenter[0] = winSize[0] /2;
	 winCenter[1] = winSize[1] /2;


	 // TODO: eine exacte mathematische bereichnung des rotations winkels ausgehend von dem 
	 // standpunkt der kammera. in diesem 1. versuch wird lediglich der Vector verschoben der die blickrichtung 
	 // angibt. is aber scheisse wie gesagt um n bischen mathe kommen wa anscheindend nich rum
	 if (this->old_x < 0  ) {
		this->old_x = x;
	 }

	 if (this->old_y < 0  ) {
		this->old_y = y;
	 }

	 if(this->old_x < x ) {
		this->angleX += 0.1;
	 }

	 if(this->old_x > x ) {
		this->angleX -= 0.1;
	 }

	 if(this->old_y < y ) {
		this->angleY += 0.1;
	 }

	 if(this->old_y > y ) {
		this->angleY -= 0.1;
	 }





	// nicht der normalVector wird hier verschoben ( hab nicht gepeilt wie der funktioniert) sondern der 
	 // lookAtVector


	Point3D *rotation_vector = new Point3D(this->angleX, this->angleY, this->angleZ);

	
	//this->sceneMan->getCamera()->setNormalVector(rotation_vector);
	this->sceneMan->getCamera()->setLookAtPosition(rotation_vector);
		
}
