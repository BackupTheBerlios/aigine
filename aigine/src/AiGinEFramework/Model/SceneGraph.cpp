/* Game Engine Design */

#include "SceneGraph.h"
#include "../Util/loader3DS/Object3DS.h"

void SceneGraph::draw()
{

   // TODO :: AiObjectTree abarbeiten und die einzelnen Objekte ausgeben.

   glPushMatrix();        
      glTranslatef(0.0, 0.0, 0.0);
      this->tmpTestObject->renderObject();
	  //glColor3f (1.0, 0.0, 0.0); // setzt die Farbe auf weiss
	  //glutSolidTeapot(1.0);
   glPopMatrix();


   glutSwapBuffers(); // Tauschen der Bildspeicher - for double buffer
}
void SceneGraph::addObject(AiGinEObject* object){}

Object3DS* SceneGraph::getTmpTestObject(){ return tmpTestObject; }




void SceneGraph::setTmpTestObject(Object3DS* tmpTestObject){ 
		// Chrome Shader : TODO : Wenn das AiGinEObject ferrtig ist, muss das in den Shader
						glBindTexture(GL_TEXTURE_2D, tmpTestObject->getTextureID(0));
						glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
						glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
						glEnable(GL_TEXTURE_GEN_S);     // Enable spherical
						glEnable(GL_TEXTURE_GEN_T);     // Environment Mapping

	this->tmpTestObject = tmpTestObject; }

bool SceneGraph::removeObject(AiGinEObject* object){
	return false;
}

bool addChild(AiGinEObject* parentObject, AiGinEObject* childObject){
	return false;
}

bool addNext(AiGinEObject* parentObject, AiGinEObject* nextObject){
	return false;
}

void SceneGraph::drawSceneGraph(knot* k) {
	if(k == NULL) return;
	glPushMatrix();
	//(AiGinEObject(k->obj))->getTranslation();
	//k->obj->getRotation()
	//k->obj->draw();
	if(k->right != NULL) {
		this->drawSceneGraph(k->right);
	}
	glPopMatrix();
	if(k->left != NULL) {
		this->drawSceneGraph(k->left);
	}
}

knot* SceneGraph::findObject(AiGinEObject* obj){
	return this->root; ;
}

