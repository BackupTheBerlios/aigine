/* Game Engine Design
 * Camera.h
 */


#ifndef CAMERA_H
#define CAMERA_H

//#include <GL/glut.h>

#include "../Model/Vector3D.h"
#include "../Model/LookAtObjects.h"



/**
 * Kamerasteuerung.
 * @author Danny Graef, Tobias Harpering
 * @since 2004-01-07
 */
class Camera :  public LookAtObjects {

public:
    /**
     * Ermittelt die Differenz zur vorherigen Position und
     * berechnet daraus die Drehungswinkel. Nach Ermittlung
     * der z-Achse wird dann die Drehung auf dieser und der y-Achse
     * ausgeführt.
     * @param neue x Position
     * @param neue y Position
     * @paran aktuelle Fenstergröße
     */
    void setRotation(int mouseX, int mouseY, int * CurrentWinSize);

    /** Konstruktor */
    Camera();

    /**
     * Konstruktor mit Vektorenzuweisung.
     * @param position Kameraposition
     * @param lookAtPosition Blickpunkt der Kamera
     * @param upVector Normalenvektor der Kamera
     */
    Camera(Vector3D * position, Vector3D * lookAtPosiont, Vector3D * upVector);



    /////////////////////////K E Y B O A R D   M O V E M E N T//////////////////

    /**
     * Bewegt die Kamera nach unten.
     * @param speed die Geschwindigkeit mit der die Kammera sich bewegen soll.
     */
    void moveDown(float speed);

    /**
     * Bewegt die Kamera nach vorn.
     * @param speed die Geschwindigkeit mit der die Kammera sich bewegen soll.
     */
    void moveForward(float speed);

    /**
     * Bewegt die Kamera nach hinten.
     * @param speed die Geschwindigkeit mit der die Kammera sich bewegen soll.
     */
    void moveBack(float speed);

    /**
     * Bewegt die Kamera nach oben.
     * @param speed die Geschwindigkeit mit der die Kammera sich bewegen soll.
     */
    void moveUp(float speet);

    /**
     * Bewegt die Kamera nach links.
     * @param speed die Geschwindigkeit mit der die Kammera sich bewegen soll.
     */
    void moveLeft(float speet);

    /**
     * Bewegt die Kamera nach rechts.
     * @param speed die Geschwindigkeit mit der die Kammera sich bewegen soll.
     */
    void moveRight(float speet);

	/**
     * Gibt den Normalenvektor der Kamera zurueck.
     * @return Normalenvektor der Kamera
     */
    Vector3D * getUpVector();

    /** Destruktor */
    ~Camera();

private:

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    //Viewport * lnkViewport;

    /** Normalenvektor der Kamera */
    Vector3D * upVector;

    /**
     * Bewegt die Kammera auf der x-z Ebene vorwärts und rückwärts
     * entlang der Blickrichtung.
     * @param speed die Geschwindigkeit mit der die Kammera sich bewegen soll.    
     */
    void moveCamera(float speed);

    /**
     * Bewegt die Kammera auf der x-z Ebene nach links und rechts.
     * @param speed die Geschwindigkeit mit der die Kammera sich bewegen soll.    
     */
    void strafeCamera(float speed);



};

#endif //CAMERA_H