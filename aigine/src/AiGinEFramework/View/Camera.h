/* Game Engine Design */



#ifndef CAMERA_H

#define CAMERA_H

#include <vector>
//#include "../Input/Mouse.h"
#include "../Model/Point3D.h"
#include "../Model/Vector3D.h"


using namespace std;

class Viewport;



/** Kamerasteuerung. */

class Camera
{


public:

	Point3D* getPosition();

	Point3D * getLookAtPosition();

	void setLookAtPosition(Point3D * lookAtPosition);

	Point3D * getRotation();
	
	/*
	 * Rotation beinhaltet die drei Winkel der Kamera für x, y,z.
	 * Die Funktion berechnet aus diesen einen Vektor und
	 * verwendet ihn in normalisierter Form als Kameravektor.
	 */
	void setRotation(Point3D * rotation, int mouseX, int mouseY, int* CurrentWinSize);

	void RotateView(float angle, float x, float y, float z);



    Camera();

    Camera(Point3D* position, Point3D* lookAtPosiont, Point3D* normalVector);

    void attachViewport(Viewport * param);

    virtual void set();


    void turnDown();



    void turnRight();



    void moveDown();


	void moveForward();


	void moveBack();


    void turnUp();



    void turnLeft();



    void moveUp();



    void moveLeft();



    void moveRight();

private:

	Point3D * rotation;
	Point3D * lookAtPosition;
    Point3D * position;

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    Viewport * lnkViewport;

	Point3D cross(Point3D * vVector1, Point3D * vVector2);
	Point3D getNormalVector(Point3D vVector);
	float magnitude(Point3D vNormal);
};

#endif //CAMERA_H