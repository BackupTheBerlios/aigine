// ageObject3DS.cpp: implementation of the ageObject3DS class.
//
//////////////////////////////////////////////////////////////////////

#include "../../AiGinE.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ageObject3DS::ageObject3DS(string fileName): AiGinEObject() {
	this->my3DSObject = new Object3DS();
	this->my3DSObject->loadObject((char*)fileName.c_str());
//	this->my3DSObject->model.pObject.
	glBindTexture(GL_TEXTURE_2D, this->my3DSObject->getTextureID(0));
//	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
//	glEnable(GL_TEXTURE_GEN_S);     // Enable spherical
//	glEnable(GL_TEXTURE_GEN_T);     // Environment Mapping
}

ageObject3DS::~ageObject3DS()
{

}

void ageObject3DS::display() {
	this->my3DSObject->renderObject();
}

