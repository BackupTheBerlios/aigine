/* Game Engine Design */

#include "SceneGraph.h"
class AiGinEObject;
void SceneGraph::draw()
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Bildspeicher mit Tiefeneffekt loeschen
   glColor3f (1.0, 1.0, 1.0); // setzt die Farbe auf weiss
   glLoadIdentity (); /* clear the matrix */ // Transformationsmatrix wird auf
     // .. die Einheitsmatrix gesetzt
           /* viewing transformation  */
   gluLookAt (0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // Betrachterposition
   glScalef (1.0, 1.0, 1.0); /* modeling transformation */ // Skalierungsmatrix
     // ..  setzten

   glPushMatrix();        
      glTranslatef(0.0, 0.0, 5.0);
      glutSolidTeapot(1.0);
   glPopMatrix();

   glutSwapBuffers(); // Tauschen der Bildspeicher - for double buffer
}
void SceneGraph::addObject(AiGinEObject* object){}
