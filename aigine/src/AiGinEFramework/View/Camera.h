/* Game Engine Design */



#ifndef CAMERA_H

#define CAMERA_H

class Viewport;



/** Kamerasteuerung. */

class Camera
{

public:



    Vector * getPosition();



    Camera();



    /** [6] or [5] if you don't think you don't need the near */

    void extractPlanes(Plane param);



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

    /**
     * @supplierCardinality 1
     * @clientCardinality 1 
     */
    Viewport * lnkViewport;
};

#endif //CAMERA_H