// ageObjectPrim.h: .
//
//////////////////////////////////////////////////////////////////////

#ifndef AGEOBJECTPRIM_H

#define AGEOBJECTPRIM_H

#include <GL/glut.h>
#include "../AiGinEObject.h"
#include "../../Utils/fileControl.h"

class ageObjectPrim : public AiGinEObject, public fileControl  
{
public:
	ageObjectPrim();
	ageObjectPrim(const char* fileName);
	virtual ~ageObjectPrim();
	void display();
	void setSphere(float radius, int slices, int stacks);
	void setCone(float base, float height, int slices, int stacks);
	void setCube(float size);
	void setTorus(float innerRadius, float outerRadius, int sides, int rings);
	void setColor(int r,int g,int b);
	void readData();
	void writeData();
private:
	int kind;	//0->not Set, 1->Sphere, 2->Cone, 3->Cube, 4->Torus
	bool solid;
	int colorR;
	int colorG;
	int colorB;

	float radiusSphere;
	int slicesSphere;
	int stacksSphere;

	float baseCone;
	float heightCone;
	int slicesCone;
	int stacksCone;

	float sizeCube;

	float innerRadiusTorus;
	float outerRadiusTorus;
	int sidesTorus;
	int ringsTorus;
};

#endif // AGEOBJECTPRIM
