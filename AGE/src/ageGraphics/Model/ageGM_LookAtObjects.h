/* Game Engine Design */

#ifndef _LOOKATOBJECTS_H
#define _LOOKATOBJECTS_H


#include <GL/glut.h>

#include "ageGraphics/Control/ageGC_Object.h"
#include "ageGM_Vector3.h"
#include "ageGraphics/Control/ageGC_Object.h"



/**
* @author Tobias Harpering
* @description Das LookAtObject ist das 'Parent-Objekt für alle Objecte, die
* einen Vieweport und eine LookAt-Variable
*/

class ageGM_LookAtObjects : public ageGC_Object   {
public:  
	
	/**Konstruktor*/
	ageGM_LookAtObjects();
	ageGM_LookAtObjects(ageGM_Vector3 * lookAt);
    /**
	* @return 
    * @param
    */
   // void attachViewport(Viewport * param);
	/**
     * Gibt den Blickpunkt zurueck.
     * @return Blickpunkt (Richtungsvektor).
     */
    ageGM_Vector3 * getLookAtPosition();

    /**
     * Setzt den Blickpunkt (Richtungsvector.
     * @param lookAtPostion neuer Blickpunkt.
     */
    void setLookAtPosition(ageGM_Vector3 * lookAtPosition);
	
	 /**
     * Führt eine Rotation um den angegebenen Winkel an dem durch
     * x, y, z gegebenen Achsenvektor aus.
     * @param angle Winkel um den gedreht werden soll
     * @param x, y, z Koordinaten des Achsenvektors
     */
    void rotateView(float angle, float x, float y, float z);

private:
   

	/** Blickpunkt des LookAtObjectes */
	ageGM_Vector3 * lookAtPosition;

};
#endif //_LOOKATOBJECTS_H
