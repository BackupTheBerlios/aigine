/* Game Engine Design */

#include "../AiGinE.h"

SceneGraph::SceneGraph() : Tree() {
}

void SceneGraph::draw() {

   // TODO :: AiObjectTree abarbeiten und die einzelnen Objekte ausgeben.
	drawSceneGraph((AiGinEObject*)this->getRootKnot());
}

void SceneGraph::addObject(AiGinEObject* object) {
}

bool SceneGraph::removeObject(AiGinEObject* object) {
	return false;
}

void SceneGraph::addRoot(AiGinEObject* rootObject) {
	this->setRoot(rootObject);
}

bool SceneGraph::addChild(AiGinEObject* parentObject, AiGinEObject* childObject) {
	this->addKnotRight(parentObject, childObject);
	return true;
}

bool SceneGraph::addNext(AiGinEObject* parentObject, AiGinEObject* nextObject) {
	this->addKnotLeft(parentObject, nextObject);
	return false;
}

void SceneGraph::drawSceneGraph(AiGinEObject * obj) {
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

void SceneGraph::printGraph() {
	printSceneGraph((AiGinEObject*)this->getRootKnot(), "");
}

void SceneGraph::printSceneGraph(AiGinEObject* obj, string tabs) {
	if(obj == NULL) return;

	cout << tabs << obj << endl;

	if(obj->right != NULL) {
		this->printSceneGraph((AiGinEObject*)(obj->right), tabs += "\t");
	}
	if(obj->left != NULL) {
		this->printSceneGraph((AiGinEObject*)(obj->left), tabs);
	}
}

Knot* SceneGraph::findObject(AiGinEObject* obj) {
	return this->root; ;
}