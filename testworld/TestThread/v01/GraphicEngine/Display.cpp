// Display.cpp: Implementierung der Klasse Display.
//
//////////////////////////////////////////////////////////////////////

#include "Display.h"




//////////////////////////////////////////////////////////////////////
// Konstruktion/Destruktion
//////////////////////////////////////////////////////////////////////

Display::Display()
{
	_x = 0.0;
	_y = 0.0;
	_z = 0.0;

}

Display::~Display()
{

}

//////////////////////////////////////////////////////////////////////
//					v o i d  I N I T ( v o i d )   
//////////////////////////////////////////////////////////////////////
static void Display::init()
{
	//glClearColor (0.0, 0.0, 0.0, 0.0);	//farbumgebung wird mit null initialisiert
   //glShadeModel (GL_FLAT);//shader mode wird spezifiziert#
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 0.0, 1.0, 1.0, 0.0 };
   GLfloat light_diffuse[]={1,0,0,0};

   //fuer das zweite licht
   GLfloat light_position1[] = { 1.0, 1.0, 1.0, 0.0 };
   GLfloat light_diffuse1[]={0,1,0,0};

   //fuer das dritte licht
   GLfloat light_position2[] = { 1.0, 1.0, 1.0, 0.0 };
   GLfloat light_diffuse2[]={0,0,1,0};

   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);  //lichtquelle deffinieren,positionieren
													//lichtquelle 0 scheint normalerweise weiss
													//			  1 scheint normalerweise

   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse );





	//zweite lichtquelle wird erstellt
   glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);

   //dritte lichtquelle wird erstellt
   glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);


   glEnable(GL_LIGHTING); //objecte sollen durch lichtqelle in ihere farbe bestimmt werden
   glEnable(GL_LIGHT0); //0 -> 1.Lichtquelle
   glEnable(GL_LIGHT1); //1 -> 2.Lichtquelle
   glEnable(GL_LIGHT2); //2 -> 3.Lichtquelle
   glEnable(GL_DEPTH_TEST); //verwendung des tiefenbuffers


}
//_______________________________________________//









///////////////////////////////////////////////////////////////////////
// DISPLAY	 (was passiert im fenster)			
///////////////////////////////////////////////////////////////////////

virtual void Display::display_cube() {


   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//bildspeicher loeschen
   glColor3f (0.5, 0.0, 0.5);		//farbe fuer nachfolgende objekte //
   glLoadIdentity ();             /* clear the matrix */ //lade transformationsmatrix (einheitsmatrix)
           /* viewing transformation  */
   gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //festlegung des Koordinatensystems
   glScalef (1.0, 1.0, 1.0);      /* modeling transformation */ //skallierung der nachstehenden objekte




   //das opbject wird gedreht
   glRotatef (_x, 3.0, 0.0, 0.0);

   glRotatef (_y, 0.0, 3.0, 0.0);

   glRotatef (_z, 0.0, 0.0, 3.0);


   glutSolidTeapot (1.0);		//teepot mit oberflaeche


   //
  /* glEnable(GL_LINE_STIPPLE);
   glLineStipple(1, 0x00FF);
   glBegin(GL_LINES);
      glVertex3fv(startpoint);
      glVertex3	f(5.0, 5.0);
   glEnd();
*/
   //glFlush ();						//alle befehle nun ausfueren!
   glutSwapBuffers();		//flush für den doppelten puffer

}
//_______________________________________________//


void Display::reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

float Display::get_x()
{
	return _x;

}

float Display::get_y()
{
	return _y;

}

float Display::get_z()
{
	return _z;

}


