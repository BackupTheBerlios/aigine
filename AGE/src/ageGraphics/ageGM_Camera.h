/* Game Engine Design
 * Camera.h
 */


#ifndef AGEGM_CAMERA_H
#define AGEGM_CAMERA_H

//#include <GL/glut.h>

#include "ageGM_Vector3.h"
#include "ageGM_LookAtObjects.h"
#include "ageGM_LookAtObjects.h"
#include "ageGraphics/Model/ageGM_LookAtObjects.h"
#include "ageGraphics/Model/ageGM_Vector3.h"



/**
 * Kamerasteuerung.
 * @author Danny Graef, Tobias Harpering 
 * @modified by Frank Otto
 * @since 2004-01-07
 */
class ageGM_Camera :  public ageGM_LookAtObjects {

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
    ageGM_Camera();

    /**
     * Konstruktor mit Vektorenzuweisung.
     * @param position Kameraposition
     * @param lookAtPosition Blickpunkt der Kamera
     * @param upVector Normalenvektor der Kamera
     */
    ageGM_Camera(ageGM_Vector3 * position, ageGM_Vector3 * lookAtPosiont, ageGM_Vector3 * upVector);



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
    ageGM_Vector3 * getUpVector();

	/**
     * Steuert die GL-Nebel Funktion
     * @param trigger Nebel ein/aus
	 * @param red float Wert rot von RGBA
	 * @param green float Wert gruen von RGBA
	 * @param blue float Wert blau von RGBA
	 * @param alpha float Wert alpha von RGBA
	 * @param density dichte des Nebels
	 * @param start des Nebels von der Cameraposition aus 
	 * @param end Ende des Sichtbaren Bereiches vom Start aus -->keine koordinate sondern relativ zum Start
	 * @param fogmode  GL_LINEAR, GL_EXP, und GL_EXP2 Erleuterung im Quelltext! void glFogi(GL_FOG_MODE,GLint param);
	 * @param glhint_target GL_FOG_HINT | GL_LINE_SMOOTH_HINT | GL_PERSPECTIVE_CORRECTION_HINT | GL_POINT_SMOOTH_HINT | GL_POLYGON_SMOOTH_HINT 
	 * @param glhint_mode GL_FASTEST GL_NICEST GL_DONT_CARE 
     */
	void setViewFogMode(bool trigger, float red,float green ,float blue,
								  float alpha, float density, float start, float end ,GLint fogmode,
								  GLint glhint_target,GLint glhint_mode );

    /** Destruktor */
    ~ageGM_Camera();

private:

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    //Viewport * lnkViewport;

    /** Normalenvektor der Kamera */
    ageGM_Vector3 * upVector;

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

#endif //AGEGM_CAMERA_H

#include "ageGraphics/Model/ageGM_LookAtObjects.h"
#include "ageGM_Vector3.h"
#include "ageGM_LookAtObjects.h"

/**
 * Kamerasteuerung.
 * @author Danny Graef, Tobias Harpering 
 * @modified by Frank Otto
 * @since 2004-01-07
 */
class ageGM_Camera :  public ageGM_LookAtObjects {

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
    ageGM_Camera();

    /**
     * Konstruktor mit Vektorenzuweisung.
     * @param position Kameraposition
     * @param lookAtPosition Blickpunkt der Kamera
     * @param upVector Normalenvektor der Kamera
     */
    ageGM_Camera(ageGM_Vector3 * position, ageGM_Vector3 * lookAtPosiont, ageGM_Vector3 * upVector);



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
    ageGM_Vector3 * getUpVector();

	/**
     * Steuert die GL-Nebel Funktion
     * @param trigger Nebel ein/aus
	 * @param red float Wert rot von RGBA
	 * @param green float Wert gruen von RGBA
	 * @param blue float Wert blau von RGBA
	 * @param alpha float Wert alpha von RGBA
	 * @param density dichte des Nebels
	 * @param start des Nebels von der Cameraposition aus 
	 * @param end Ende des Sichtbaren Bereiches vom Start aus -->keine koordinate sondern relativ zum Start
	 * @param fogmode  GL_LINEAR, GL_EXP, und GL_EXP2 Erleuterung im Quelltext! void glFogi(GL_FOG_MODE,GLint param);
	 * @param glhint_target GL_FOG_HINT | GL_LINE_SMOOTH_HINT | GL_PERSPECTIVE_CORRECTION_HINT | GL_POINT_SMOOTH_HINT | GL_POLYGON_SMOOTH_HINT 
	 * @param glhint_mode GL_FASTEST GL_NICEST GL_DONT_CARE 
     */
	void setViewFogMode(bool trigger, float red,float green ,float blue,
								  float alpha, float density, float start, float end ,GLint fogmode,
								  GLint glhint_target,GLint glhint_mode );

    /** Destruktor */
    ~ageGM_Camera();

private:

    /**
     * @supplierCardinality 1
     * @clientCardinality 1
     */
    //Viewport * lnkViewport;

    /** Normalenvektor der Kamera */
    ageGM_Vector3 * upVector;

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


