#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <gl\glut.h>
#include <gl\glaux.h>


#include "3dsObject.h"


static float angle = 0.0, ratio;
static float x=0.0f,y=1.75f,z=5.0f;
static float lx=0.0f,ly=0.0f,lz=-1.0f;

float g_RotateX		  = 0.0f;							// This is the current value at which the model is rotated
float g_RotationSpeed = 0.2f;							// This is the speed that our model rotates.  (-speed rotates left)
int g_bLighting = 0;
int g_sphereMap = 0;
float g_Scale = 1.0;

Object3DS my3DSObject;

void changeSize(int w, int h) {

	if(h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);

}


void initScene() {	
	glEnable(GL_LIGHT0);								// Turn on a light with defaults set
	glEnable(GL_LIGHTING);								// Turn on lighting
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);

	//my3DSObject.loadObject("maskmetal.3ds");
	my3DSObject.loadObject("teekanne.3DS");
	//my3DSObject.loadObject("maskMarmor1.3ds");
	//my3DSObject.loadObject("maskMarmor2.3ds");
	//my3DSObject.loadObject("maskMetal1.3ds");
	//my3DSObject.loadObject("maskMetal2.3ds");
	//my3DSObject.loadObject("maskMetal3.3ds");
	//my3DSObject.loadObject("maskMetal4.3ds");			
	//my3DSObject.loadObject("victory1.3ds");
	//my3DSObject.loadObject("victory2.3ds");
    my3DSObject.toString();
}



void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
    //for (int i=0; i<10; i+=5) {
	//	for (int j=0; j<10; j+=5) {
			glPushMatrix();
				//glTranslatef(0.0, 0.0, (float)j);
				glPushMatrix();
				//	glTranslatef((float)i, 0.0, 0.0);
	                glPushMatrix();
						glRotatef(g_RotateX, 0, 1.0f, 0);						// Rotate the object around the Y-Axis
						glScalef(g_Scale, g_Scale, g_Scale);
						g_RotateX += g_RotationSpeed;							// Increase the speed of rotation
						my3DSObject.renderObject();
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();
	//	}
    //}

	glutSwapBuffers();
}

void orientMe(float ang) {	
	lx = sin(ang*3.1415/180.0);
	lz = -cos(ang*3.1415/180.0);	
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}


void moveMeFlat(int i) {
	x = x + i*(lx)*0.1;
	z = z + i*(lz)*0.1;
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}


void processNormalKeys(unsigned char key, int x, int y) {
    static int viewMode = TEXTURE;

	switch (key) {
		case 'w': if(viewMode == TEXTURE) {				// We our drawing mode is at triangles
					viewMode = WIREFRAME;					// Go to line stips
				  } else {
					viewMode = TEXTURE;					// Go to triangles
				  }
				  my3DSObject.setViewMode(viewMode);
				  break;

		case 'l': 	g_bLighting = !g_bLighting;						// Turn lighting ON/OFF
					if(g_bLighting) {								// If lighting is ON
						glEnable(GL_LIGHTING);						// Enable OpenGL lighting
					} else {
						glDisable(GL_LIGHTING);						// Disable OpenGL lighting
					}
					break;

		case 's': 	g_sphereMap = !g_sphereMap;
					if (g_sphereMap) { 
						glBindTexture(GL_TEXTURE_2D, my3DSObject.getTextureID(0));
						glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
						glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
						glEnable(GL_TEXTURE_GEN_S);     // Enable spherical
						glEnable(GL_TEXTURE_GEN_T);     // Environment Mapping
					}
					else {
						glBindTexture(GL_TEXTURE_2D, my3DSObject.getTextureID(0));
						glDisable(GL_TEXTURE_GEN_S);     // Enable spherical
						glDisable(GL_TEXTURE_GEN_T);     // Environment Mapping
					}
					break;

		case 'r': g_RotateX	+= 5;
				  break;

		case 'R': g_RotateX	-= 5;
				  break;

		case '+': g_Scale *= 2.0;
				  break;

		case '-': g_Scale /= 2.0;
				  break;
			
	}
	
	if (key == 27) 
		exit(0);
}


void inputKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : angle -= 5;/*-= 0.01f; */orientMe(angle);break;
		case GLUT_KEY_RIGHT : angle += 5; /* +=0.01f; */orientMe(angle);break;
		case GLUT_KEY_UP : moveMeFlat(2);break;
		case GLUT_KEY_DOWN : moveMeFlat(-2);break;
	}
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,360);

	glutCreateWindow("3DS-Loader");

	initScene();

	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(inputKey);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutReshapeFunc(changeSize);

	glutMainLoop();

	return(0);
}
