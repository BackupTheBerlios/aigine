/* Game Engine Design */



//////////////////////INCLUDE/////////////////////////////////////





#include "Object3D.h"

/**
*/
Object3D::~Object3D(){}
Object3D::Object3D(){
	this->initObject();
}
void Object3D::initObject(){

}
void Object3D::setRotation(Rotation3D rotation){ this->rotation = rotation; }
Rotation3D& Object3D::getRotation(){ return rotation; }
void Object3D::setTranslation(Translation3D translation){ this->translation = translation; }
Translation3D& Object3D::getTranslation(){ return translation; }
