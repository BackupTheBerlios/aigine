/* Game Engine Design */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GL/glut.h>

class SceneManagement;

/**
 * "Keyboard" handelt die zugewiesenen Tastaturkommandos und f�r
 * die entsprechenden Methoden auf dem "SceneManagment" aus.
 *
 * TODO:
 * - reagiert derzeitig nur auf einzelne Tasten, sch�n w�re nat�rlich
 *   wenn man in meherere Richtungen (z.B. nach vorne links) laufen k�nnte.
 *
 * @author Danny Graef, Tobias Harpering, Frank Otto
 * @date 2004-01-06
 */
class Keyboard
{
public:
	/**
	 * Konstruktor
	 */
	Keyboard(SceneManagement* sceneMan);

    /**
     * Funktion f�r OpenGL keyboard() Pointer.
     * Pr�ft welche Taste gedr�ckt wurde und f�r die zugewiesene
	   * Methode auf.
     */
    void keyAction(unsigned char key, int x, int y);

private:
	/**
	 * Zugewiesenes "SceneManagment".
	 */
	SceneManagement* sceneMan;
};
#endif //KEYBOARD_H
