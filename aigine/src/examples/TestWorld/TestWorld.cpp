#include <GL/glut.h> 


#include "../../AiGinEFramework/Model/AiGinEObject.h"
#include "../../AiGinEFramework/Model/SceneManagement.h"
#include "../../AiGinEFramework/Model/ageObject3DS.h"
#include "../../AiGinEFramework/Model/ageObjectPrim.h"
#include "../../AiGinEFramework/Model/Translation3D.h"
#include "../../AiGinEFramework/Control/Keyboard.h"
#include "../../AiGinEFramework/Control/Mouse.h"

SceneManagement* sm;
Keyboard* kb;
Mouse* m;
AiGinEObject* knie;
AiGinEObject* huefteR;
AiGinEObject* beine;
int dir;

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

void littleAnim(int ms) {
	if( knie->getRotation()->angle > 0) {
		dir = -1;
	}
	if(knie->getRotation()->angle <= -45) {
		dir = 1;
	}
	beine->getRotation()->y = 1;
	beine->getRotation()->angle = ((int)beine->getRotation()->angle) % 360 + 5;
	knie->getRotation()->angle += 2 * dir;
	huefteR->getRotation()->angle -= 2 * dir;
	cout << knie->getRotation()->angle << endl;
	sm->display();
	glutTimerFunc(ms, littleAnim, ms);

}

void testPrimObjects() {

	//Huefte
	AiGinEObject* huefte = sm->addObjectPrim(new ageObjectPrim());
	((ageObjectPrim*)huefte)->setCube(0.5);
	((ageObjectPrim*)huefte)->setColor(255,0,0);
	huefte->setTranslation(new Translation3D(0,6,0));
	huefte->setScale(new Scale3D(6,1,1));
	beine = huefte;

	//Bein_Rechts
	//Gelenk_Oben_Rechts
	AiGinEObject* gelenk_o_r = sm->addObjectPrim(new ageObjectPrim(),huefte,"child");
	((ageObjectPrim*)gelenk_o_r)->setSphere(0.25,100,100);
	((ageObjectPrim*)gelenk_o_r)->setColor(0,255,0);
	gelenk_o_r->setTranslation(new Translation3D(1.25,-0.5,0));
	gelenk_o_r->setScale(new Scale3D(1,1,1));
	gelenk_o_r->setRotation(new Rotation3D(45,1,0,0));
	huefteR = gelenk_o_r;

	//Segment_Oben_Rechts
	AiGinEObject* seg_o_r = sm->addObjectPrim(new ageObjectPrim(),gelenk_o_r,"child");
	((ageObjectPrim*)seg_o_r)->setCube(0.5);
	((ageObjectPrim*)seg_o_r)->setColor(255,0,0);
	seg_o_r->setTranslation(new Translation3D(0,-1.25,0));
	seg_o_r->setScale(new Scale3D(1,4,1));

	//Gelenk_Unten_Rechts
	AiGinEObject* gelenk_u_r = sm->addObjectPrim(new ageObjectPrim(),seg_o_r,"child");
	((ageObjectPrim*)gelenk_u_r)->setSphere(0.25,100,100);
	((ageObjectPrim*)gelenk_u_r)->setColor(0,255,0);
	gelenk_u_r->setTranslation(new Translation3D(0,-1.25,0));
	gelenk_u_r->setScale(new Scale3D(1,1,1));
	gelenk_u_r->setRotation(new Rotation3D(-45,1,0,0));
	knie = gelenk_u_r;

	//Segment_Unten_Rechts
	AiGinEObject* seg_u_r = sm->addObjectPrim(new ageObjectPrim(),gelenk_u_r,"child");
	((ageObjectPrim*)seg_u_r)->setCube(0.5);
	((ageObjectPrim*)seg_u_r)->setColor(255,0,0);
	seg_u_r->setTranslation(new Translation3D(0,-1.25,0));
	seg_u_r->setScale(new Scale3D(1,4,1));



	//Bein_Links
	//Gelenk_Oben_Links	
	AiGinEObject* gelenk_o_l = sm->addObjectPrim(new ageObjectPrim(),gelenk_o_r,"next");
	((ageObjectPrim*)gelenk_o_l)->setSphere(0.25,100,100);
	((ageObjectPrim*)gelenk_o_l)->setColor(0,255,0);
	gelenk_o_l->setTranslation(new Translation3D(-1.25,-0.5,0));
	gelenk_o_l->setScale(new Scale3D(1,1,1));

	//Segment_Oben_Links
	AiGinEObject* seg_o_l = sm->addObjectPrim(new ageObjectPrim(),gelenk_o_l,"child");
	((ageObjectPrim*)seg_o_l)->setCube(0.5);
	((ageObjectPrim*)seg_o_l)->setColor(255,0,0);
	seg_o_l->setTranslation(new Translation3D(0,-1.25,0));
	seg_o_l->setScale(new Scale3D(1,4,1));

	//Gelenk_Unten_Links
	AiGinEObject* gelenk_u_l = sm->addObjectPrim(new ageObjectPrim(),seg_o_l,"child");
	((ageObjectPrim*)gelenk_u_l)->setSphere(0.25,100,100);
	((ageObjectPrim*)gelenk_u_l)->setColor(0,255,0);
	gelenk_u_l->setTranslation(new Translation3D(0,-1.25,0));
	gelenk_u_l->setScale(new Scale3D(1,1,1));

	//Segment_Unten_Links
	AiGinEObject* seg_u_l = sm->addObjectPrim(new ageObjectPrim(),gelenk_u_l,"child");
	((ageObjectPrim*)seg_u_l)->setCube(0.5);
	((ageObjectPrim*)seg_u_l)->setColor(255,0,0);
	seg_u_l->setTranslation(new Translation3D(0,-1.25,0));
	seg_u_l->setScale(new Scale3D(1,4,1));

}


int main(int argc, char** argv)
{ 
	sm = new SceneManagement();
	m = new Mouse(sm);
	kb = new Keyboard(sm);
	sm->initOpenGL(argc, argv);

	// Aufbau des Levels
	testPrimObjects();


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
    glutTimerFunc(20,littleAnim, 20);
    glutMainLoop(); // Kontrolle wird an GLUT uebergeben
    return 0;
} 