/* Game Engine Design */

#include "SceneGraph.h"

void SceneGraph::removeObject(AiGinEObject* object){}
void SceneGraph::draw()
{

   // TODO :: AiObjectTree abarbeiten und die einzelnen Objekte ausgeben.

   glPushMatrix();        
      glTranslatef(0.0, 0.0, 0.0);
	  glColor3f(0.0, 0.8, 0.0);
      glutSolidTeapot(1.0);
   glPopMatrix();

   glutSwapBuffers(); // Tauschen der Bildspeicher - for double buffer
}
void SceneGraph::addObject(AiGinEObject* object){}
