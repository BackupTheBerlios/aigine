/* Game Engine Design */

#include "GraphicPipeline.h"
#include "StateManager.h"
#include "Camera.h"

void GraphicPipeline::reshape(int width, int height)
{
   glViewport (0, 0, (GLsizei) width, (GLsizei) height);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 50.0);
   glMatrixMode (GL_MODELVIEW);
}

void GraphicPipeline::initOpenGL(int argc, char** argv){
    glutInit(&argc, argv); // initialisiert GLUT und übergibt die Parameter
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // mit Tiefeneffekt fuer Licht

    glutInitWindowSize (500, 500);  // festlegen der Fenstergroesse
    glutInitWindowPosition (100, 100); // Positionierung des Fensters
    glutCreateWindow (argv[0]); // Ausgabe des Fensters

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	// Kamera initialisieren
	this->lnkCamera = new Camera(new Point3D(5.0, 5.0, -5.0));
}

GraphicPipeline::~GraphicPipeline()
{
}

GraphicPipeline::GraphicPipeline()
{
}
void GraphicPipeline::initDisplay(){
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Bildspeicher mit Tiefeneffekt loeschen
   glColor3f (1.0, 1.0, 1.0); // setzt die Farbe auf weiss
   glLoadIdentity (); /* clear the matrix */ // Transformationsmatrix wird auf
     // .. die Einheitsmatrix gesetzt   
           /* viewing transformation  */
   Point3D* pc = lnkCamera->getPosition();
   gluLookAt (pc->getX(), pc->getY(), pc->getZ(), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Betrachterposition
   glScalef (1.0, 1.0, 1.0); /* modeling transformation */ // Skalierungsmatrix
     // ..  setzten
}
Camera* GraphicPipeline::getCamera(){
	return this->lnkCamera;
}
