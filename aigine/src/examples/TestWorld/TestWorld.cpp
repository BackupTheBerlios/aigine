#include <GL/glut.h> 


#include "../../AiGinEFramework/Model/AiGinEObject.h"
#include "../../AiGinEFramework/Model/SceneManagement.h"
#include "../../AiGinEFramework/Model/AgeObjects/ageObject3DS.h"
#include "../../AiGinEFramework/Model/AgeObjects/ageObjectPrim.h"
#include "../../AiGinEFramework/Model/Translation3D.h"
#include "../../AiGinEFramework/Control/Keyboard.h"
#include "../../AiGinEFramework/Control/Mouse.h"


SceneManagement* sm;
Keyboard* kb;
Mouse* m;

void display(void)
{

    sm->display();
	
}

void reshape (int w, int h)
{
    sm->reshape(w, h);
}

/**
 * Maus Handles auf das "Mouse" Objekt umleiten.
 */
void passivemotion(int x, int y)
{
	m->passivemotion(x, y);  
}

/**
 * Tastatur Handles auf das "Keyboard" Objekt umleiten.
 */
void keyboardDown(unsigned char key, int x, int y)
{
	kb->keyAction(key, x, y);
}

void keyboardUp(unsigned char key, int x, int y)
{
	kb->keyActionUp(key, x, y);
}

void keyboardFunc() {
	if(kb->isKeyPressed('w')) {
		sm->getCamera()->moveForward(1.0);
	}
	if(kb->isKeyPressed('a')) {
		sm->getCamera()->moveLeft(1.0);
	}
	if(kb->isKeyPressed('s')) {
		sm->getCamera()->moveBack(1.0);
	}
	if(kb->isKeyPressed('d')) {
		sm->getCamera()->moveRight(1.0);
	}

	if(kb->isKeyPressed('n')) {
		sm->getCamera()->setViewFogMode(true,254.0, 236.0, 208.0, 1.0, 0.2, 100.0, 160.0, GL_LINEAR,GL_FOG_HINT, GL_DONT_CARE);//spaeter ueber einstellungsdialog bzw inidatei
	}
	if(kb->isKeyPressed('N')) {
		sm->getCamera()->setViewFogMode(false,0.0, 0.0, 0.0, 1.0, 0.2, 30.0, 100.0, GL_LINEAR,GL_FOG_HINT, GL_DONT_CARE);
	}
}

void timerFunc(int ms) {
	if(kb->keysPressed()) keyboardFunc();
	glutPostRedisplay();
	glutTimerFunc(ms,timerFunc,ms);
}

void idleFunc() {
}

int main(int argc, char** argv)
{ 
	sm = new SceneManagement();
	m = new Mouse(sm);
	kb = new Keyboard(keyboardFunc);
	sm->initOpenGL(argc, argv);
	//tests here....

	glutIgnoreKeyRepeat(1);
//	glutSetKeyRepeat();



	// Bekanntmachung der benoetigten Callback-Funktionen
	glutDisplayFunc(display); // .. fuer die Bildgenerierung
    glutReshapeFunc(reshape); // .. fuer die Aenderung der Fenstergroesse
	glutKeyboardFunc(keyboardDown); // .. fuer die Tastaturabfrage
	glutKeyboardUpFunc(keyboardUp); // .. fuer die Tastaturabfrage
	glutPassiveMotionFunc(passivemotion); // fuer die Mausabfrage
    glutTimerFunc(20,timerFunc, 20);
	glutIdleFunc(idleFunc);
    glutMainLoop(); // Kontrolle wird an GLUT uebergeben
    return 0;
} 