/* Game Engine Design */



#ifndef CAMERA_H

#define CAMERA_H

#include <vector>
#include "../Model/Point3D.h"

using namespace std;

class Viewport;



/** Kamerasteuerung. */

class Camera
{

public:

	Point3D* getPosition();

    Camera();

    Camera(Point3D* position);

    void attachViewport(Viewport * param);

    virtual void set();


    void turnDown();



    void turnRight();



    void moveDown();



    void turnUp();



    void turnLeft();



    void moveUp();



    void moveLeft();



    void moveRight();

private:

    Point3D * position;

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    Viewport * lnkViewport;
};

#endif //CAMERA_H