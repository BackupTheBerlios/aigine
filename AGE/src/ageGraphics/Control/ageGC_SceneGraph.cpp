/* Game Engine Design */


#include <iostream>
using namespace std;


#include "ageGC_SceneGraph.h"
#include "../Model/AiGinEObject.h"
#include "ageGraphics/Model/ageGM_Scale3D.h"
class ageGM_Knot;
class ageGC_Object;


ageGC_SceneGraph::ageGC_SceneGraph() : Tree() {
}

void ageGC_SceneGraph::draw() {

   // TODO :: AiObjectTree abarbeiten und die einzelnen Objekte ausgeben.
	drawSceneGraph((AiGinEObject*)this->getRootKnot());
}

void ageGC_SceneGraph::addObject(ageGC_Object* object) {
}

bool ageGC_SceneGraph::removeObject(ageGC_Object* object) {
	return false;
}

void ageGC_SceneGraph::addRoot(ageGC_Object* rootObject) {
	this->setRoot(rootObject);
}

bool ageGC_SceneGraph::addChild(ageGC_Object* parentObject, ageGC_Object* childObject) {
	this->addKnotRight(parentObject, childObject);
	return true;
}

bool ageGC_SceneGraph::addNext(ageGC_Object* parentObject, ageGC_Object* nextObject) {
	this->addKnotLeft(parentObject, nextObject);
	return false;
}

void ageGC_SceneGraph::drawSceneGraph(ageGC_Object * obj) {
	if(obj == NULL) return;
	glPushMatrix();
	
	Translation3D * trans = obj->getTranslation();
	glTranslatef(trans->x,trans->y,trans->z);

	Rotation3D* rotate= obj->getRotation();
	glRotatef (rotate->angle, rotate->x, rotate->y, rotate->z);

	glPushMatrix();
	Scale3D* scale = obj->getScale();
	glScalef (scale->x, scale->y, scale->z);

	obj->display();

	glPopMatrix();

	if(obj->right != NULL) {
		this->drawSceneGraph((AiGinEObject*)(obj->right));
	}
	glPopMatrix();
	if(obj->left != NULL) {
		this->drawSceneGraph((AiGinEObject*)(obj->left));
	}
}

void ageGC_SceneGraph::printGraph() {
	printSceneGraph((AiGinEObject*)this->getRootKnot(), "");
}

void ageGC_SceneGraph::printSceneGraph(ageGC_Object* obj, string tabs) {
	if(obj == NULL) return;

	cout << tabs << obj << endl;

	if(obj->right != NULL) {
		this->printSceneGraph((AiGinEObject*)(obj->right), tabs += "\t");
	}
	if(obj->left != NULL) {
		this->printSceneGraph((AiGinEObject*)(obj->left), tabs);
	}
}

ageGM_Knot* ageGC_SceneGraph::findObject(ageGC_Object* obj) {
	return this->root; ;
}