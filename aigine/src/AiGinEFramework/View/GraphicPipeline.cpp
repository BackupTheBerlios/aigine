/* Game Engine Design 
 * GraphicPipeline.cpp  
 */

#include "GraphicPipeline.h"
//#include "StateManager.h"
#include "Camera.h"
#include "../Model/Vector3D.h"
#include "../View/Light.h"

//____________________________________________________________________________
GraphicPipeline::GraphicPipeline(int argc, char** argv)
{
	GraphicPipeline();
  initOpenGL(argc, argv);
}

//____________________________________________________________________________
GraphicPipeline::GraphicPipeline()
{
	this->windowSize = new int ;
	//this->InitMask();		//	zuerst die bilder mask.zip entpacken!!!
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
void GraphicPipeline::reshape(int width, int height)
{
   glViewport (0, 0, (GLsizei) width, (GLsizei) height);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 400.0);
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
//____________________________________________________________________________
char intToChar(int integer)
{
 return (char)integer+48;
}
//___enableLight________________________________________________________________
/**setzt alle relevanten Gl befehle für das Licht
* @todo im moment kann man nur ganz normale lichter setzten. um zb ein spot zu setzten muss ich noch 
* mal rann...:-(
*/

int GraphicPipeline::enableLight(Light * currentLight, int index) {
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

//____________________________Image_MASK methods______________________________

/** Laed die Bilder Der Image Mask*/
void GraphicPipeline::InitMask() {

	CreateTexture(g_Texture, "CrossHairMask.bmp", 0);	// Load the cross hair mask used for transparency
	CreateTexture(g_Texture, "CrossHair.bmp", 1);

	//hier Bilder laden ,ich weiss ist nicht perfect aber erstmal eine Loesung
}

void GraphicPipeline::OrthoMode(int left, int top, int right, int bottom) {

	// This function takes in rectangle coordinates for our 2D view port.
	// What happens below is, we need to change our projection matrix.  To do this,
	// we call glMatrixMode(GL_PROJECTION) to tell OpenGL the current mode we want.
	// Then we push on a new matrix so we can just call glPopMatrix() to return us
	// to our previous projection matrix, but first we will render in ortho mode.
	//  Next, we load a new identity matrix so that everything is initialize before
	// we go into ortho mode.  One we switch to ortho by calling glOrth(), we then
	// want to set our matrix mode to GL_MODELVIEW, which puts us back and ready to
	// render our world using the model matrix.  We also initialize this new model view
	// matrix for our 2D rendering before drawing anything.

	// Switch to our projection matrix so that we can change it to ortho mode, not perspective.
	glMatrixMode(GL_PROJECTION);						

	// Push on a new matrix so that we can just pop it off to go back to perspective mode
	glPushMatrix();									
	
	// Reset the current matrix to our identify matrix
	glLoadIdentity();								

	//Pass in our 2D ortho screen coordinates.like so (left, right, bottom, top).  The last
	// 2 parameters are the near and far planes.
	glOrtho( left, right, bottom, top, 0, 1 );	
	
	// Switch to model view so that we can render the scope image
	glMatrixMode(GL_MODELVIEW);								

	// Initialize the current model view matrix with the identity matrix
	glLoadIdentity();
	
}


///////////////////////////////// PERSPECTIVE MODE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	This function changes our returns our projection mode from 2D to 3D
/////
///////////////////////////////// PERSPECTIVE MODE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void GraphicPipeline::PerspectiveMode()										// Set Up A Perspective View
{
	// This function doesn't really go to perspective mode, since we aren't
	// using gluPerspective(), but if you look in OrthoMode(), we push on
	// a matrix once we change to our projection matrix.  All we need to do is
	// pop that matrix off of our stack and it will return to our original perspective
	// matrix.  So, before we pop the current matrix, we need to tell OpenGL which
	// matrix we want to use by changing our current matrix mode to perspective.
	// Then we can go back to our model view matrix and render normally.

	// Enter into our projection matrix mode
	glMatrixMode( GL_PROJECTION );							

	// Pop off the last matrix pushed on when in projection mode (Get rid of ortho mode)
	glPopMatrix();											

	// Go back to our model view matrix like normal
	glMatrixMode( GL_MODELVIEW );							

	// We should be in the normal 3D perspective mode now
}

/** funzt noch nicht ganz kommentare mach ich noch, fosi*/
void GraphicPipeline::ImageMask(){
	
	//von 3D auf 2D umschalten
	OrthoMode(0,0,this->windowSize[0],this->windowSize[1]);

	glColor4f(1, 1, 1, 1);

	//Abschalten des Z-buffer Algorithmus
	glDisable(GL_DEPTH_TEST);

	//Einschalten des gewuenschten Tiefen-algorithmus und des Blending fuer alpha
	glBlendFunc(GL_DST_COLOR,GL_ZERO);
	glEnable(GL_BLEND);

	// Maske als Textur binden...
	glBindTexture(GL_TEXTURE_2D,g_Texture[0]);
	

	// Ein 2D Rechteck erzeugen und die gebundene Maske darüberlegen
	// Wir benutzen hier ein glVertex2f --> ScreenCoordinaten (keine Vertices)
	glBegin(GL_QUADS);
					
		// Oben links des 2D image
		glTexCoord2f(0.0f, 1.0f);	glVertex2f(0, 0);

		// Unten links des 2D image
		glTexCoord2f(0.0f, 0.0f);	glVertex2f(0, this->windowSize[1]);

		// Unten rechts des 2D image
		glTexCoord2f(1.0f, 0.0f);	glVertex2f(this->windowSize[0], this->windowSize[1]);

		// Oben rechts des 2D image
		glTexCoord2f(1.0f, 1.0f);	glVertex2f(this->windowSize[0], 0);

	glEnd();

	//hier maskieren der geladenenTextur--> 
	// Ein 2D Rechteck erzeugen und die gebundene Textur ueber die Maske legen
	
	// Once the mask is draw to the screen, we want to set a new blend mode.
	// This mode will perform the masking effect that we desire.  Then we just
	// render the desired transparent image over top of the mask and it will
	// only draw the desired effect.

	// Turn our blending mode to one to one
	glBlendFunc(GL_ONE,GL_ONE);
	
	// Bind the scope texture to this next 2D quad
    glBindTexture(GL_TEXTURE_2D, g_Texture[1]);

	// Display a 2D quad with our scope texture
	glBegin(GL_QUADS);

		// Oben links des 2D image
		glTexCoord2f(0.0f, 1.0f);	glVertex2f(0, 0);

		// Unten links des 2D image
		glTexCoord2f(0.0f, 0.0f);	glVertex2f(0, this->windowSize[1]);

		// Unten rechts des 2D image
		glTexCoord2f(1.0f, 0.0f);	glVertex2f(this->windowSize[0], this->windowSize[1]);

		// Oben rechts des 2D image
		glTexCoord2f(1.0f, 1.0f);	glVertex2f(this->windowSize[0], 0);

	// Stop drawing 
	glEnd();

	// Since we are done, we can now turn OFF blending and turn ON depth testing
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	// Let's set our mode back to perspective 3D mode.  None of this archaic 2D stuff :)
	PerspectiveMode();

	//glutSwapBuffers(); // Tauschen der Bildspeicher - for double buffer
}

/** kommt noch hier weg*/
void GraphicPipeline::CreateTexture(GLuint textureArray[], LPSTR strFileName, int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// Return from the function if no file name was passed in
		return;
	
	// We need to load the texture data, so we use a cool API that the glaux.lib offers.
	
	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data
	
	if(pBitmap == NULL)									// If we can't load the file, quit!
		exit(0);

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &textureArray[textureID]);

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though.  
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}
}
	
