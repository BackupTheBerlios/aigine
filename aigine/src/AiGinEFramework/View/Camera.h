/* Game Engine Design
 * Camera.h
 */


#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glut.h>

class Viewport;
class Vector3D;

/**
 * Kamerasteuerung.
 * @author Danny Graef, Tobias Harpering
 * @since 2004-01-07
 */
class Camera
{


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

    /**
     * Gibt die Position der aktuellen Kamera zurueck.
     * @return Position der Kamera.
     */
    Vector3D * getPosition();

    /**
     * Gibt den Blickpunkt zurueck.
     * @return Blickpunkt der Kamera.
     */
    Vector3D * getLookAtPosition();

    /**
     * Setzt den Blickpunkt der Kamera.
     * @param lookAtPostion neuer Blickpunkt.
     */
    void setLookAtPosition(Vector3D * lookAtPosition);

    /**
     * Gibt den Normalenvektor der Kamera zurueck.
     * @return Normalenvektor der Kamera
     */
    Vector3D * getUpVector();

    /** Konstruktor */
    Camera();

    /**
     * Konstruktor mit Vektorenzuweisung.
     * @param position Kameraposition
     * @param lookAtPosition Blickpunkt der Kamera
     * @param upVector Normalenvektor der Kamera
     */
    Camera(Vector3D * position, Vector3D * lookAtPosiont, Vector3D * upVector);

    /**
     * @author
     * @return
     * @param
     */
    void attachViewport(Viewport * param);

    /**
     * @author
     * @return
     * @param
     */
    virtual void set();

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

    /** Destruktor */
    ~Camera();


private:
    /** Normalenvektor der Kamera */
    Vector3D * upVector;

    /** Blickpunkt der Kamera */
    Vector3D * lookAtPosition;

    /** Position der Kamera */
    Vector3D * position;

    /**
     * Führt eine Rotation um den angegebenen Winkel an dem durch
     * x, y, z gegebenen Achsenvektor aus.
     * @param angle Winkel um den gedreht werden soll
     * @param x, y, z Koordinaten des Achsenvektors
     */
    void rotateView(float angle, float x, float y, float z);


    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    Viewport * lnkViewport;

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