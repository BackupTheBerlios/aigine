#include <GL/glut.h> 


#include "../../AiGinEFramework/Model/AiGinEObject.h"
#include "../../AiGinEFramework/Model/SceneManagement.h"
#include "../../AiGinEFramework/AgeObjects/ageObject3DS.h"
#include "../../AiGinEFramework/AgeObjects/ageObjectPrim.h"
#include "../../AiGinEFramework/Model/Translation3D.h"
#include "../../AiGinEFramework/Control/Keyboard.h"
#include "../../AiGinEFramework/Control/Mouse.h"

#include "robot.h"

SceneManagement* sm;
Keyboard* kb;
Mouse* m;

robot* robi;

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
void keyboard(unsigned char key, int x, int y)
{
	kb->keyAction(key, x, y);
}

void timerFunc(int ms) {
	robi->animRobot();
	sm->display();
	glutTimerFunc(ms,timerFunc,ms);
}

int main(int argc, char** argv)
{ 
	sm = new SceneManagement();
	m = new Mouse(sm);
	kb = new Keyboard(sm);
	sm->initOpenGL(argc, argv);

	// Aufbau des Levels
	//	testPrimObjects();

	robi = new robot(sm);
	robi->drawRobot();
	robi->getModel()->setTranslation(new Translation3D(-20,0,-20));
//	sm->printGraph();
	
	// ... weitere Objekte laden
	
	// Bekanntmachung der benoetigten Callback-Funktionen
	glutDisplayFunc(display); // .. fuer die Bildgenerierung
    glutReshapeFunc(reshape); // .. fuer die Aenderung der Fenstergroesse
	glutKeyboardFunc(keyboard); // .. fuer die Tastaturabfrage
	glutPassiveMotionFunc(passivemotion); // fuer die Mausabfrage
    glutTimerFunc(20,timerFunc, 20);
    glutMainLoop(); // Kontrolle wird an GLUT uebergeben
    return 0;
} 