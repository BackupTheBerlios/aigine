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
class ageI_Keyboard
{
public:
	/**
	 * Konstruktor
	 */
	ageI_Keyboard(void (*func)());

    /**
     * Funktion für OpenGL keyboard() Pointer.
     * Prüft welche Taste gedrückt wurde und für die zugewiesene
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
