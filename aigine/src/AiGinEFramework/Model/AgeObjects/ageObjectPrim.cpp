/* Game Engine Design */

#include "../../AiGinE.h"

ageObjectPrim::ageObjectPrim(): AiGinEObject(), fileControl() {
	this->kind = 0;
	this->solid = true;
	this->colorR = 0;
	this->colorG = 0;
	this->colorB = 0;

	this->radiusSphere = 0.0;
	this->slicesSphere = 0;
	this->stacksSphere = 0;

	this->baseCone = 0.0;
	this->heightCone = 0.0;
	this->slicesCone = 0;
	this->stacksCone = 0;

	this->sizeCube = 0.0;

	this->innerRadiusTorus = 0.0;
	this->outerRadiusTorus = 0.0;
	this->sidesTorus = 0;
	this->ringsTorus = 0;
}
ageObjectPrim::ageObjectPrim(const char* fileName): AiGinEObject(), fileControl(fileName) {
	this->kind = 0;
	this->solid = true;
	this->colorR = 0;
	this->colorG = 0;
	this->colorB = 0;

	this->radiusSphere = 0.0;
	this->slicesSphere = 0;
	this->stacksSphere = 0;

	this->baseCone = 0.0;
	this->heightCone = 0.0;
	this->slicesCone = 0;
	this->stacksCone = 0;

	this->sizeCube = 0.0;

	this->innerRadiusTorus = 0.0;
	this->outerRadiusTorus = 0.0;
	this->sidesTorus = 0;
	this->ringsTorus = 0;
}

ageObjectPrim::~ageObjectPrim()
{

}

void ageObjectPrim::readData() {
	openFile('r');
	readFloat(&this->getScale()->x);
	readFloat(&this->getScale()->y);
	readFloat(&this->getScale()->z);
	readFloat(&this->getTranslation()->x);
	readFloat(&this->getTranslation()->y);
	readFloat(&this->getTranslation()->z);
	readFloat(&this->getRotation()->x);
	readFloat(&this->getRotation()->y);
	readFloat(&this->getRotation()->z);
	readFloat(&this->getRotation()->angle);
	readInt(&this->kind);
	readBool(&this->solid);
	readInt(&this->colorR);
	readInt(&this->colorG);
	readInt(&this->colorB);
	readFloat(&this->radiusSphere);
	readInt(&this->slicesSphere);
	readInt(&this->stacksSphere);
	readFloat(&this->baseCone);
	readFloat(&this->heightCone);
	readInt(&this->slicesCone);
	readInt(&this->stacksCone);
	readFloat(&this->sizeCube);
	readFloat(&this->innerRadiusTorus);
	readFloat(&this->outerRadiusTorus);
	readInt(&this->sidesTorus);
	readInt(&this->ringsTorus);
	closeFile();
/**/
}

void ageObjectPrim::writeData() {
	openFile('w');
	writeFloat(&this->getScale()->x);
	writeFloat(&this->getScale()->y);
	writeFloat(&this->getScale()->z);
	writeFloat(&this->getTranslation()->x);
	writeFloat(&this->getTranslation()->y);
	writeFloat(&this->getTranslation()->z);
	writeFloat(&this->getRotation()->x);
	writeFloat(&this->getRotation()->y);
	writeFloat(&this->getRotation()->z);
	writeFloat(&this->getRotation()->angle);
	writeInt(&this->kind);
	writeBool(&this->solid);
	writeInt(&this->colorR);
	writeInt(&this->colorG);
	writeInt(&this->colorB);
	writeFloat(&this->radiusSphere);
	writeInt(&this->slicesSphere);
	writeInt(&this->stacksSphere);
	writeFloat(&this->baseCone);
	writeFloat(&this->heightCone);
	writeInt(&this->slicesCone);
	writeInt(&this->stacksCone);
	writeFloat(&this->sizeCube);
	writeFloat(&this->innerRadiusTorus);
	writeFloat(&this->outerRadiusTorus);
	writeInt(&this->sidesTorus);
	writeInt(&this->ringsTorus);
	closeFile();
/**/
}

void ageObjectPrim::display() {
	glPushMatrix();
	glColor3ub(this->colorR, this->colorG, this->colorB);
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
			glutWireSphere(this->radiusSphere, this->slicesSphere, this->stacksSphere);
			break;
		case 2:
			glutWireCone(this->baseCone, this->heightCone, this->slicesCone, this->stacksCone);
			break;
		case 3:
			glutWireCube(this->sizeCube);
			break;
		case 4:
			glutWireTorus(this->innerRadiusTorus, this->outerRadiusTorus, this->sidesTorus, this->ringsTorus);
			break;
		default:
			cout << "Fehler in ageObjectPrim::display()" << endl;
			break;
		}
	}
	glPopMatrix();
}
void ageObjectPrim::setSphere(float radius, int slices, int stacks) {
	this->kind = 1;
	this->radiusSphere = radius;
	this->slicesSphere = slices;
	this->stacksSphere = stacks;
}
void ageObjectPrim::setCone(float base, float height, int slices, int stacks) {
	this->kind = 2;
	this->baseCone = base;
	this->heightCone = height;
	this->slicesCone = slices;
	this->stacksCone = stacks;
}
void ageObjectPrim::setCube(float size) {
	this->kind = 3;
	this->sizeCube = size;
}
void ageObjectPrim::setTorus(float innerRadius, float outerRadius, int sides, int rings) {
	this->kind = 4;
	this->innerRadiusTorus = innerRadius;
	this->outerRadiusTorus = outerRadius;
	this->sidesTorus = sides;
	this->ringsTorus = rings;
}
void ageObjectPrim::setColor(int r,int g,int b) {
	this->colorR = r;
	this->colorG = g;
	this->colorB = b;
}
