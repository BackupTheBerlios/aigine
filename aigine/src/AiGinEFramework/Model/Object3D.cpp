/* Game Engine Design */



//////////////////////INCLUDE/////////////////////////////////////





#include "Object3D.h"

/**
*/
Object3D::~Object3D(){}
Object3D::Object3D(){

	this->translation = new Translation3D(0,0,0);
	//this->rotation = new Translation3D(0,0,0);
	//this->scale = new Translation3D(0,0,0);

	
}

void Object3D::setRotation(Rotation3D * rotation){ 
	this->rotation = rotation;
 }

Rotation3D * Object3D::getRotation(){ return rotation; }


void Object3D::setTranslation(Translation3D * translation){ 

	
	this->translation = translation;
	
}


Translation3D * Object3D::getTranslation(){ return this->translation; }
