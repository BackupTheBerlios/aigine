/* Game Engine Design 
 * GraphicPipeline.cpp  
 */

#include "GraphicPipeline.h"
//#include "StateManager.h"
#include "Camera.h"
#include "../Model/Vector3D.h"
#include "../View/Light.h"

//____________________________________________________________________________
GraphicPipeline::GraphicPipeline()
{
	this->windowSize = new int ;
}

//____________________________________________________________________________
GraphicPipeline::~GraphicPipeline()
{
}

//____________________________________________________________________________
void GraphicPipeline::initOpenGL(int argc, char** argv){
	this->windowSize[0] = 500;
    this->windowSize[1] = 500;

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 

	glutInitWindowSize(this->windowSize[0],this->windowSize[1]);
    glutInitWindowPosition (100, 100);
    
    // Ausgabe des Fensters
    glutCreateWindow (argv[0]);

	/* // Definition der Lichteigenschaften
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // weiss
   //GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // weiss
   GLfloat mat_diffuse[] = { 0.8, 1.0, 0.8, 1.0 }; // leichtes gruen
   GLfloat mat_diffuse1[] = { 1.0, 0.4, 0.4, 1.0 }; // ein wenig rot
   GLfloat mat_shininess[] = { 10.0 }; // fuer Helligkeit der Reflextion
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; // position in homogenen koordinaten
   GLfloat light_position1[] = { -3.0, 3.0, 3.0, 1.0 };
     // da 4. = 0 werden die ersten drei Koordinaten unendlich /**/

	this->lnkLight = new Light(); //Konstruktor mit den standart Werten:
												   //  Vector3D position, Vector3D lookAtPosition
	
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glutSetCursor(9);

	// das Licht initiieren und enablen wird dann im licht gemacht
	
	

	this->lnkCamera = new Camera(	new Vector3D(5.0, 5.0, 5.0), new Vector3D(0.0, 0.0, 0.0), new Vector3D(0.0, 1.0, 0.0)	);								
}

//____________________________________________________________________________
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

//____________________________________________________________________________
void GraphicPipeline::initDisplay(){	
   glShadeModel (GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);
   // Bildspeicher mit Tiefeneffekt loeschen
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
   // Transformationsmatrix wird auf die Einheitsmatrix gesetzt
   glLoadIdentity ();
   // Kameraposition setzen
   Vector3D* pc = lnkCamera->getTranslation();
   Vector3D* lap = lnkCamera->getLookAtPosition();
   Vector3D* nv = lnkCamera->getUpVector();
   gluLookAt (	pc->x, pc->y, pc->z,    // Position
				lap->x, lap->y, lap->z, // Blickpunkt 
				nv->x, nv->y, nv->z);   // Senkrechtvektor 
   // Skalierungsmatrix setzten
   glScalef (1.0, 1.0, 1.0); 
   
   //Es werde Licht
	
   
   // Ausgabe des 3d gitters
   // TODO: als Onjekt in ScenenGraph hängen
   glColor3ub(0, 255, 0);
   // Draw a 1x1 grid along the X and Z axis'
   for(float i = -50; i <= 50; i += 1)
   {
        glBegin(GL_LINES);
        // Do the horizontal lines (along the X)
            glVertex3f(-50, 0, i);
            glVertex3f(50, 0, i);
        // Do the vertical lines (along the Z)
            glVertex3f(i, 0, -50);
            glVertex3f(i, 0, 50);
        // Stop drawing lines
        glEnd();
   }
}

//____________________________________________________________________________
int * GraphicPipeline::getWindowSize() {
	return this->windowSize;
}

//____________________________________________________________________________
Camera* GraphicPipeline::getCamera(){
	return this->lnkCamera;
}

//____________________________________________________________________________
void GraphicPipeline::setCameraRotation(int mouseX, int mouseY) {
	this->lnkCamera->setRotation(mouseX, mouseY, this->windowSize);

}

void GraphicPipeline::addLightAt(Vector3D * position, Vector3D * lookAt) {
	
	
}