/* Game Engine Design */

#ifndef KEYBOARD_H
#define KEYBOARD_H
#pragma warning (disable: 4786)

#include <GL/glut.h>
#include <map>
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
class Keyboard
{
public:
	/**
	 * Konstruktor
	 */
	Keyboard(void (*func)());

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
	map<char , int> keys;
};
#endif //KEYBOARD_H
