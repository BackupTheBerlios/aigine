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
    
	//initialisieren der windowSize variablen
	this->windowSize[0] =800;
    this->windowSize[1] =600;
	glutInitWindowSize(this->windowSize[0],this->windowSize[1]);  // festlegen der Fenstergroesse
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

	glShadeModel (GL_SMOOTH);

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



   // Ausgabe des 3d gitters
    glColor3ub(0, 255, 0);

    // Draw a 1x1 grid along the X and Z axis'
    for(float i = -50; i <= 50; i += 1)
    {
        // Start drawing some lines
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
