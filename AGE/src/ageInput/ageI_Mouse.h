/* Game Engine Design */

#ifndef AGEI_MOUSE_H
#define AGEI_MOUSE_H

#include <iostream>
#include "ageGraphics/ageG_SceneManager.h"
#include "ageGraphics/ageG_SceneManager.h"
using namespace std;

class ageI_Mouse
{
public: 
	/**
	 * Constructor
	 * @param: a pointer to a SceneManagement- object who contains all further needed objects
	 *	like camara, renderer and the SceneGraph
	 */
	ageI_Mouse(ageG_SceneManager* sceneMan);

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
	ageG_SceneManager* sceneMan;
};
#endif //AGEI_MOUSE_H
