/* Game Engine Design */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GL/glut.h>
#include "../GraphicEngine/SceneManagement.h"

class SceneManagement;

class Keyboard
{
public:    

    Keyboard(SceneManagement* sceneMan);

    void keyAction(unsigned char key, int x, int y);
private:    
    SceneManagement* sceneMan;
};
#endif //KEYBOARD_H
