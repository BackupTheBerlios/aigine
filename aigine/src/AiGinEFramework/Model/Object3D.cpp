/* Game Engine Design */



//////////////////////INCLUDE/////////////////////////////////////





#include "Object3D.h"

/**
*/
Object3D::~Object3D(){}

Object3D::Object3D(){
	this->translation = new Translation3D(0,0,0);
	this->rotation = new Rotation3D(0,0,0,0);
	this->scale = new Scale3D(1,1,1);	
}


//Rotation
void Object3D::setRotation(Rotation3D * rotation){ 
	this->rotation = rotation;
 }

Rotation3D * Object3D::getRotation() { return rotation; }

//____________________________________________________________________________
void Object3D::rotate(int angle, char axis)
{

}

//Translation
void Object3D::setTranslation(Translation3D * translation){ 	
	this->translation = translation;	
}

//____________________________________________________________________________
void Object3D::setTranslation(float x, float y, float z){ 	
	this->translation->x = x;	
  this->translation->y = y;	
  this->translation->z = z;	
}

Translation3D* Object3D::getTranslation(){ return this->translation; }


//Scale
void Object3D::setScale(Scale3D* scale){ 	
	this->scale = scale;
}

Scale3D* Object3D::getScale(){ return this->scale; }
