/* Game Engine Design 
 * GraphicPipeline.cpp  
 */

#include "ageGV_GraphicPipeline.h"
//#include "StateManager.h"
#include "ageGraphics/Model/ageGM_Camera.h"
#include "ageGraphics/Model/ageGM_Vector3.h"
#include "ageGraphics/Model/ageGM_Light.h"

//____________________________________________________________________________
ageGV_GraphicPipeline::ageGV_GraphicPipeline(int argc, char** argv)
{
	GraphicPipeline();
  initOpenGL(argc, argv);
}

//____________________________________________________________________________
ageGV_GraphicPipeline::ageGV_GraphicPipeline()
{
	this->windowSize = new int ;
	//this->InitMask();		//	zuerst die bilder mask.zip entpacken!!!
}

//____________________________________________________________________________
ageGV_GraphicPipeline::~ageGV_GraphicPipeline()
{
}

//____________________________________________________________________________
void ageGV_GraphicPipeline::initOpenGL(int argc, char** argv){
	this->windowSize[0] = 800;
    this->windowSize[1] = 600;

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 

	glutInitWindowSize(this->windowSize[0],this->windowSize[1]);
    glutInitWindowPosition (50, 50);
    
    // Ausgabe des Fensters
    glutCreateWindow (argv[0]);

	glShadeModel (GL_SMOOTH);


	this->lnkLight = new Light(this); //Konstruktor mit den standart Werten:
												   //  Vector3D position, Vector3D lookAtPosition

	float * lightGreen_defuse = new float[4];
	lightGreen_defuse[0] = 1.0;
	lightGreen_defuse[1] = 1.0;
	lightGreen_defuse[2] = 1.0;
	lightGreen_defuse[3] = 1.0;        // leichtes gruen	

	Translation3D * lightpos = new Translation3D(30.0,-30.0,1.0); //die position des lichtes +10 in alle richtungen
	this->lnkLight->setDiffuse(lightGreen_defuse);  // leichtes gruen
	this->lnkLight->setTranslation(lightpos);

	float * ambient = new float[4];
	ambient[0] = 0.2;
	ambient[1] = 0.2;
	ambient[2] = 0.2;
	ambient[3] = 1.0; 
	this->lnkLight->setAmbient(ambient);
	

	this->enableLight(lnkLight,0);
	/**/
	//Translation3D * lightpos = new Translation3D(3.0,-30.0,1.0); //die position des lichtes +10 in alle richtungen

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	glutSetCursor(GLUT_CURSOR_NONE);

	// das Licht initiieren und enablen wird dann im licht gemacht
	
	this->lnkCamera = new Camera(	new Vector3D(5.0, 25.0, 5.0), new Vector3D(0.0, 25.0, 0.0), new Vector3D(0.0, 1.0, 0.0)	);								
}

//____________________________________________________________________________
void ageGV_GraphicPipeline::reshape(int width, int height)
{
   glViewport (0, 0, (GLsizei) width, (GLsizei) height);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 700.0);
   glMatrixMode (GL_MODELVIEW);

   // set the windowSize variable
   this->windowSize[0] =width;
   this->windowSize[1] =height;
}

//____________________________________________________________________________
void ageGV_GraphicPipeline::initDisplay(){	
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
   // TODO: als Onjekt in ScenenGraph h�ngen
   glColor3ub(0, 255, 0);
   // Draw a 1x1 grid along the X and Z axis'
   for(float i = -50; i <= 50; i += 10)
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
int * ageGV_GraphicPipeline::getWindowSize() {
	return this->windowSize;
}

//____________________________________________________________________________
ageGM_Camera* ageGV_GraphicPipeline::getCamera(){
	return this->lnkCamera;
}

//____________________________________________________________________________
void ageGV_GraphicPipeline::setCameraRotation(int mouseX, int mouseY) {
	this->lnkCamera->setRotation(mouseX, mouseY, this->windowSize);

}

void ageGV_GraphicPipeline::addLightAt(ageGM_Vector3 * position, ageGM_Vector3 * lookAt) {
	
	
}
//____________________________________________________________________________
char intToChar(int integer)
{
 return (char)integer+48;
}
//___enableLight________________________________________________________________
/**setzt alle relevanten Gl befehle f�r das Licht
* @todo im moment kann man nur ganz normale lichter setzten. um zb ein spot zu setzten muss ich noch 
* mal rann...:-(
*/

int ageGV_GraphicPipeline::enableLight(ageGM_Light * currentLight, int index) {
	int isSet = false;					
	string lightCount = "GL_LIGHT";
	lightCount += intToChar(index);

	 glEnable(GL_LIGHTING);

	
	float * lightPosition = new float[4] ;
	lightPosition[0] = currentLight->getTranslation()->x;
	lightPosition[1] = currentLight->getTranslation()->y;
	lightPosition[2] = currentLight->getTranslation()->z;
	lightPosition[3] = currentLight->getPos_direct();

   // farbiges licht hinzufuegen
   glLightfv(GL_LIGHT0, GL_POSITION, lightPosition );

   if(currentLight->getDiffuse() != NULL) 
	   //glLightfv(index, GL_DIFFUSE, currentLight->getDiffuse());
     glLightfv(GL_LIGHT0, GL_DIFFUSE, currentLight->getDiffuse());
	
   if(currentLight->getAmbient() != NULL) 
		//glLightfv(index, GL_AMBIENT, currentLight->getAmbient());
    glLightfv(GL_LIGHT0, GL_AMBIENT, currentLight->getAmbient());
   
   if(currentLight->getSpecular() != NULL) 
		//glLightfv(index, GL_SPECULAR, currentLight->getSpecular());
    glLightfv(GL_LIGHT0, GL_SPECULAR, currentLight->getSpecular());

   //for (int i = 0; i < index; i++) {
		//glEnable(index);
   glEnable(GL_LIGHT0);
   //}
	return isSet;
}