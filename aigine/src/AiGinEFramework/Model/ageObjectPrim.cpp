#include "ageObjectPrim.h"
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ageObjectPrim::ageObjectPrim(): AiGinEObject() {
//	glBindTexture(GL_TEXTURE_2D, this->my3DSObject->getTextureID(0));
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);     // Enable spherical
	glEnable(GL_TEXTURE_GEN_T);     // Environment Mapping

	this->solid = true;
}

ageObjectPrim::~ageObjectPrim()
{

}

void ageObjectPrim::display() {
	glPushMatrix();
	glColor3ub(255, 0, 0);
	if(this->solid) {
		switch(this->kind) {
		case 1:
			glutSolidSphere(this->radiusSphere, this->slicesSphere, this->stacksSphere);
			break;
		case 2:
			glutSolidCone(this->baseCone, this->heightCone, this->slicesCone, this->stacksCone);
			break;
		case 3:
			glutSolidCube(this->sizeCube);
			break;
		case 4:
			glutSolidTorus(this->innerRadiusTorus, this->outerRadiusTorus, this->sidesTorus, this->ringsTorus);
			break;
		default:
			break;
		}
	} else {
		switch(this->kind) {
		case 1:
			glutSolidSphere(this->radiusSphere, this->slicesSphere, this->stacksSphere);
			break;
		case 2:
			glutSolidCone(this->baseCone, this->heightCone, this->slicesCone, this->stacksCone);
			break;
		case 3:
			glutSolidCube(this->sizeCube);
			break;
		case 4:
			glutSolidTorus(this->innerRadiusTorus, this->outerRadiusTorus, this->sidesTorus, this->ringsTorus);
			break;
		default:
			break;
		}
	}
	glPopMatrix();
}

void ageObjectPrim::setSphere(double radius, int slices, int stacks) {
	this->kind = 1;
	this->radiusSphere = radius;
	this->slicesSphere = slices;
	this->stacksSphere = stacks;
}

void ageObjectPrim::setCone(double base, double height, int slices, int stacks) {
	this->kind = 2;
	this->baseCone = base;
	this->heightCone = height;
	this->slicesCone = slices;
	this->stacksCone = stacks;
}

void ageObjectPrim::setCube(double size) {
	this->kind = 3;
	this->sizeCube = size;
}

void ageObjectPrim::setTorus(double innerRadius, double outerRadius, int sides, int rings) {
	this->kind = 4;
	this->innerRadiusTorus = innerRadius;
	this->outerRadiusTorus = outerRadius;
	this->sidesTorus = sides;
	this->ringsTorus = rings;
}
