/* Game Engine Design 
 * GraphicPipeline.cpp  
 */

#include "GraphicPipeline.h"
#include "StateManager.h"
#include "Camera.h"

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
	this->windowSize[0] = 800;
    this->windowSize[1] = 600;

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 

	glutInitWindowSize(this->windowSize[0],this->windowSize[1]);
    glutInitWindowPosition (100, 100);
    
    // Ausgabe des Fensters
    glutCreateWindow (argv[0]);

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glutSetCursor(9);

	this->lnkCamera = new Camera(	new Point3D(5.0, 5.0, 5.0),
									new Point3D(0.0, 0.0, 0.0),
									new Point3D(0.0, 1.0, 0.0)	);								
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
   Point3D* pc = lnkCamera->getPosition();
   Point3D* lap = lnkCamera->getLookAtPosition();
   Point3D* nv = lnkCamera->getRotation();
   gluLookAt (	pc->getX(), pc->getY(), pc->getZ(),    // Position
				lap->getX(), lap->getY(), lap->getZ(), // Blickpunkt 
				nv->getX(), nv->getY(), nv->getZ());   // Senkrechtvektor 
   // Skalierungsmatrix setzten
   glScalef (1.0, 1.0, 1.0); 
   
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

/**
*
*/
Camera* GraphicPipeline::getCamera(){
	return this->lnkCamera;
}
