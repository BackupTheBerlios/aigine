/* Game Engine Design */

#include "GraphicPipeline.h"
#include "StateManager.h"
#include "Camera.h"

/**
*Constructor
*/
GraphicPipeline::GraphicPipeline()
{	
	this->windowSize = new int ;
}



/**
*	
*/
void GraphicPipeline::reshape(int width, int height)
{
   glViewport (0, 0, (GLsizei) width, (GLsizei) height);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 50.0);
   glMatrixMode (GL_MODELVIEW);
   
   // set the windowSize variable
   
   this->windowSize[0] =width;
   this->windowSize[1] =height;
}


int * GraphicPipeline::getWindowSize() {
	return this->windowSize;
}


/**
*	
*/
void GraphicPipeline::initOpenGL(int argc, char** argv){
    glutInit(&argc, argv); // initialisiert GLUT und übergibt die Parameter
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // mit Tiefeneffekt fuer Licht

    glutInitWindowSize(800,600);  // festlegen der Fenstergroesse
	//initialisieren der windowSize variablen
	this->windowSize[0] =800;
    this->windowSize[1] =600;
    glutInitWindowPosition (100, 100); // Positionierung des Fensters
    glutCreateWindow (argv[0]); // Ausgabe des Fensters

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	// Kamera initialisieren
	glutSetCursor(9);

	this->lnkCamera = new Camera(new Point3D(5.0, 5.0, 5.0),new Point3D(0.0, 0.0, 0.0),new Point3D(0.0,1.0,0.0));

}


/**
*	
*/
void GraphicPipeline::initDisplay(){

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // weiss
   //GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // weiss
   GLfloat mat_diffuse[] = { 0.8, 1.0, 0.8, 1.0 }; // leichtes gruen
   //GLfloat mat_diffuse1[] = { 1.0, 0.4, 0.4, 1.0 }; // ein wenig rot
   //GLfloat mat_shininess[] = { 10.0 }; // fuer Helligkeit der Reflextion
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; // position in homogenen koordinaten
   //GLfloat light_position1[] = { -3.0, 3.0, 3.0, 1.0 };
     // da 4. = 0 werden die ersten drei Koordinaten unendlich
	
   //glShadeModel (GL_FLAT); // Shading wird auf flach gesetzt
	glShadeModel (GL_SMOOTH);

	/* Materialeigenschaften der Oberfläche */
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); // Spiegelreflextion des Lichts
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse); // Divuse Reflextion
	//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess); // Groesse der Reflektion setzen
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// rotes licht hinzufuegen
   glLightfv(GL_LIGHT1, GL_POSITION, light_position);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, mat_diffuse); 

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   //glEnable(GL_LIGHT1);
   glEnable(GL_DEPTH_TEST);

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Bildspeicher mit Tiefeneffekt loeschen
   glColor3f (1.0, 1.0, 1.0); // setzt die Farbe auf weiss
   glLoadIdentity (); /* clear the matrix */ // Transformationsmatrix wird auf
     // .. die Einheitsmatrix gesetzt   
           /* viewing transformation  */
   Point3D* pc = lnkCamera->getPosition();
   Point3D* lap = lnkCamera->getLookAtPosition();
   Point3D* nv = lnkCamera->getRotation();
   gluLookAt (pc->getX(), pc->getY(), pc->getZ(), lap->getX(), lap->getY(), lap->getZ(), nv->getX(), nv->getY(), nv->getZ()); // Betrachterposition
   glScalef (1.0, 1.0, 1.0); /* modeling transformation */ // Skalierungsmatrix // ..  setzten
}

/**
*	
*/
Camera* GraphicPipeline::getCamera(){
	return this->lnkCamera;
}


/**
*Destructor
*/
GraphicPipeline::~GraphicPipeline()
{
}
