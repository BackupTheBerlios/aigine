/* Game Engine Design */

#ifndef KEYBOARD_H
#define KEYBOARD_H
#pragma warning (disable: 4786)

#include <GL/glut.h>
#include <map>
using namespace std;

//class SceneManagement;

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
	Keyboard(void (*func)());

    /**
     * Funktion f�r OpenGL keyboard() Pointer.
     * Pr�ft welche Taste gedr�ckt wurde und f�r die zugewiesene
	   * Methode auf.
     */
    void keyAction(unsigned char key, int x, int y);
	void keyActionUp(unsigned char key, int x, int y);
	bool isKeyPressed(unsigned char key);
	bool keysPressed();
private:
	/**
	 * Zugewiesenes "SceneManagment".
	 */
	int keyCount;
	void (*function)();
	map<char , int> keys;
};
#endif //KEYBOARD_H
