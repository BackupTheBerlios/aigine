/* Game Engine Design */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GL/glut.h>

class SceneManagement;

/**
 * "Keyboard" handelt die zugewiesenen Tastaturkommandos und für
 * die entsprechenden Methoden auf dem "SceneManagment" aus.
 *
 * TODO:
 * - reagiert derzeitig nur auf einzelne Tasten, schön wäre natürlich
 *   wenn man in meherere Richtungen (z.B. nach vorne links) laufen könnte.
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
     * Funktion für OpenGL keyboard() Pointer.
     * Prüft welche Taste gedrückt wurde und für die zugewiesene
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
