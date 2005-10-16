/* Game Engine Design */

#ifndef AGEIC_MOUSE_H
#define AGEIC_MOUSE_H

#include <iostream>
#include "ageGraphics/ageGC_SceneManager.h"
using namespace std;

class ageIC_Mouse
{
public: 
	/**
	 * Constructor
	 * @param: a pointer to a SceneManagement- object who contains all further needed objects
	 *	like camara, renderer and the SceneGraph
	 */
	ageIC_Mouse(ageGC_SceneManager* sceneMan);

	/**
	 * mouse movement depends to the normal vector of the camara object
	 * @param x position of mouse
	 * @param y position of mouse
     */
    void passivemotion(int x, int y);

private:

	/**
	 * Referenz für den Kamerazugiff.
	 */ 
	ageGC_SceneManager* sceneMan;
};
#endif //AGEIC_MOUSE_H
