// ageObjectPrim.h: .
//
//////////////////////////////////////////////////////////////////////

#ifndef AGEGC_OBJECTPRIM_H

#define AGEGC_OBJECTPRIM_H

#include <GL/glut.h>
#include "ageGC_Object.h"
#include "../../Control/fileControl.h"
#include "ageGC_Object.h"
#include "ageGraphics/Control/ageGC_Object.h"

/**
 * deprecated 
 */
class ageGC_ObjectPrim : public ageGC_Object{
public:
	ageGC_ObjectPrim();
	ageGC_ObjectPrim(const char* fileName);
	virtual ~ageGC_ObjectPrim();
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
