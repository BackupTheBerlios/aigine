/* Game Engine Design */

#ifndef MOUSE_H
#define MOUSE_H

#include "../AiGinE.h"

class SceneManagement;

class Mouse
{
public: 
	/**
	 * Constructor
	 * @param: a pointer to a SceneManagement- object who contains all further needed objects
	 *	like camara, renderer and the SceneGraph
	 */
	Mouse(SceneManagement* sceneMan);

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
	SceneManagement* sceneMan;
};
#endif //MOUSE_H
