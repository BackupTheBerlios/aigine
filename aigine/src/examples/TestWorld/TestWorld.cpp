#include <GL/glut.h> 


#include "../../AiGinEFramework/Model/AiGinEObject.h"
#include "../../AiGinEFramework/Model/SceneManagement.h"
#include "../../AiGinEFramework/Model/ageObject3DS.h"
#include "../../AiGinEFramework/Model/ageObjectPrim.h"
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


/*	
	//AiGinEObject * root = sm->addObject3DS("teekanne.3DS");

	//ageObject3DS* rootGelenk = new ageObject3DS("rootGelenk.3DS");
	AiGinEObject * root = sm->addObject3DS("rootGelenk.3DS");
	root->setScale(new Scale3D(0.03,0.03,0.03));
	root->setTranslation(new Translation3D(0,0,0));

	AiGinEObject * arm01 = sm->addObject3DS("robot01.3DS",root,"child");
	arm01->setTranslation(new Translation3D(0,1,0));
	arm01->setScale( new Scale3D(0.03,0.03,0.03));

	AiGinEObject * gelenk01 = sm->addObject3DS("gelenk01.3DS",arm01,"child");
	gelenk01->setTranslation(new Translation3D(0,-0.5,0));
	gelenk01->setScale( new Scale3D(0.007,0.007,0.007));
	
	AiGinEObject * arm02 = sm->addObject3DS("robot02.3DS",gelenk01,"child");
	arm02->setTranslation(new Translation3D(0,2.2,0));
	arm02->setScale( new Scale3D(0.03,0.03,0.03));
 /*   
	AiGinEObject * gelenk02 = sm->addObject3DS("gelenk02.3DS",arm02,"child");
	gelenk02->setTranslation(new Translation3D(0,-50,0));
	gelenk02->setScale( new Scale3D(0.25,0.25,0.25));
    
	AiGinEObject * arm03 = sm->addObject3DS("robot03.3DS",gelenk02,"child");
	arm03->setTranslation(new Translation3D(0,105,0));
	arm03->setScale( new Scale3D(4,4,4));
/**/

	
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