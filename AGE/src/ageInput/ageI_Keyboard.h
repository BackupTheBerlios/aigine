/* Game Engine Design */

#ifndef AGEI_KEYBOARD_H
#define AGEI_KEYBOARD_H
#pragma warning (disable: 4786)

#include <GL/glut.h>
#include <map>
#include "stl_map.h"
#include "stl/stl_map.h"
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
class ageI_Keyboard
{
public:
	/**
	 * Konstruktor
	 */
	ageI_Keyboard(void (*func)());

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
	map<_Key, _Tp, _Compare, _Alloc> keys;
};
#endif //AGEI_KEYBOARD_H
