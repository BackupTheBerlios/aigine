/* Game Engine Design */

#ifndef _LOOKATOBJECTS_H
#define _LOOKATOBJECTS_H


#include <GL/glut.h>

#include "AiGinEObject.h"
#include "Vector3D.h"



/**
* @author Tobias Harpering
* @description Das LookAtObject ist das 'Parent-Objekt für alle Objecte, die
* einen Vieweport und eine LookAt-Variable
*/

class LookAtObjects : public AiGinEObject   {
public:  
	
	/**Konstruktor*/
	LookAtObjects();
	LookAtObjects(Vector3D * lookAt);
    /**
	* @return 
    * @param
    */
   // void attachViewport(Viewport * param);
	/**
     * Gibt den Blickpunkt zurueck.
     * @return Blickpunkt (Richtungsvektor).
     */
    Vector3D * getLookAtPosition();

    /**
     * Setzt den Blickpunkt (Richtungsvector.
     * @param lookAtPostion neuer Blickpunkt.
     */
    void setLookAtPosition(Vector3D * lookAtPosition);
	
	 /**
     * Führt eine Rotation um den angegebenen Winkel an dem durch
     * x, y, z gegebenen Achsenvektor aus.
     * @param angle Winkel um den gedreht werden soll
     * @param x, y, z Koordinaten des Achsenvektors
     */
    void rotateView(float angle, float x, float y, float z);

private:
   

	/** Blickpunkt des LookAtObjectes */
	Vector3D * lookAtPosition;

};
#endif //_LOOKATOBJECTS_H
