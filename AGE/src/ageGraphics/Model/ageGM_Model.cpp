/* Game Engine Design */



//////////////////////INCLUDE/////////////////////////////////////





#include "ageGM_Model.h"
class ageGM_Translation3D;
class ageGM_Scale3D;
class ageGM_Rotation3D;
class ageGM_Vector3;

void ageGM_Model::setDirectionVector3D(ageGM_Vector3 * directionVector3D){ 
  
  this->directionVector3D = directionVector3D; 

}

ageGM_Vector3* ageGM_Model::getDirectionVector3D(){ return directionVector3D; }
/**
*/
ageGM_Model::~ageGM_Model(){}

ageGM_Model::ageGM_Model(){
	this->translation = new Translation3D(0,0,0);
	this->rotation = new Rotation3D(0,0,0,0);
	this->scale = new Scale3D(1,1,1);	
}


//Rotation
void ageGM_Model::setRotation(ageGM_Rotation3D * rotation){ 
	this->rotation = rotation;
 }

ageGM_Rotation3D * ageGM_Model::getRotation() { return rotation; }

//____________________________________________________________________________
void ageGM_Model::rotate(int angle, char axis)
{

  
 
}

//Translation
void ageGM_Model::setTranslation(ageGM_Translation3D * translation){ 	
	this->translation = translation;	
}

//____________________________________________________________________________
void ageGM_Model::setTranslation(float x, float y, float z){ 	
	this->translation->x = x;	
  this->translation->y = y;	
  this->translation->z = z;	
}

ageGM_Translation3D* ageGM_Model::getTranslation(){ return this->translation; }


//Scale
void ageGM_Model::setScale(ageGM_Scale3D* scale){ 	
	this->scale = scale;
}

ageGM_Scale3D* ageGM_Model::getScale(){ return this->scale; }
