/* Game Engine Design */

#include "SceneGraph.h"
#include "../Util/loader3DS/Object3DS.h"
#include "AiGinEObject.h"
#include "ageObject3DS.h"

SceneGraph::SceneGraph() : Tree() {
}

void SceneGraph::draw()
{

   // TODO :: AiObjectTree abarbeiten und die einzelnen Objekte ausgeben.
	drawSceneGraph((AiGinEObject*)this->getRootKnot());
/*
	glPushMatrix();        
      glTranslatef(0.0, 0.0, 0.0);
      this->tmpTestObject->renderObject();
	  //glColor3f (1.0, 0.0, 0.0); // setzt die Farbe auf weiss
	  //glutSolidTeapot(1.0);
   glPopMatrix();

*/
   glutSwapBuffers(); // Tauschen der Bildspeicher - for double buffer

}
void SceneGraph::addObject(AiGinEObject* object){}

Object3DS* SceneGraph::getTmpTestObject(){ return tmpTestObject; }




/*
void SceneGraph::setTmpTestObject(Object3DS* tmpTestObject){ 
		// Chrome Shader : TODO : Wenn das AiGinEObject ferrtig ist, muss das in den Shader
						glBindTexture(GL_TEXTURE_2D, tmpTestObject->getTextureID(0));
						glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
						glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
						glEnable(GL_TEXTURE_GEN_S);     // Enable spherical
						glEnable(GL_TEXTURE_GEN_T);     // Environment Mapping

	this->tmpTestObject = tmpTestObject; }
*/

bool SceneGraph::removeObject(AiGinEObject* object){
	return false;
}

void SceneGraph::addRoot(AiGinEObject* rootObject) {
	this->setRoot(rootObject);
}

bool SceneGraph::addChild(AiGinEObject* parentObject, AiGinEObject* childObject){
	this->addKnotRight(parentObject, childObject);
	return true;
}

bool SceneGraph::addNext(AiGinEObject* parentObject, AiGinEObject* nextObject){
	this->addKnotLeft(parentObject, nextObject);
	return false;
}

void SceneGraph::drawSceneGraph(AiGinEObject* obj) {
	if(obj == NULL) return;
	glPushMatrix();
	//(AiGinEObject(k->obj))->getTranslation();
	//k->obj->getRotation()
	//k->obj->draw();
	
	Translation3D * trans = obj->getTranslation();
	glTranslatef(trans->x,trans->y,trans->z);

	/*Rotation3D* rotate= obj->getRotation();
	glScalef (rotate->x, rotate->y, rotate->z);/**/

	glPushMatrix();
	Scale3D* scale = obj->getScale();
	glScalef (scale->x, scale->y, scale->z);

	((ageObject3DS*)obj)->display();
	glPopMatrix();

	if(obj->right != NULL) {
		this->drawSceneGraph((AiGinEObject*)(obj->right));
	}
	glPopMatrix();
	if(obj->left != NULL) {
		this->drawSceneGraph((AiGinEObject*)(obj->left));
	}
}

Knot* SceneGraph::findObject(AiGinEObject* obj){
	return this->root; ;
}

