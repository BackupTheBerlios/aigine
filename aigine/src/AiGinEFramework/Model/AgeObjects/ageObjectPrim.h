// ageObjectPrim.h: .
//
//////////////////////////////////////////////////////////////////////

#ifndef AGEOBJECTPRIM_H

#define AGEOBJECTPRIM_H

#include <GL/glut.h>
#include "../AiGinEObject.h"

class ageObjectPrim : public AiGinEObject  
{
public:
	ageObjectPrim();
	virtual ~ageObjectPrim();
	void display();
	void setSphere(double radius, int slices, int stacks);
	void setCone(double base, double height, int slices, int stacks);
	void setCube(double size);
	void setTorus(double innerRadius, double outerRadius, int sides, int rings);
	void setColor(int r,int g,int b);
private:
	int kind;	//0->not Set, 1->Sphere, 2->Cone, 3->Cube, 4->Torus
	bool solid;
	int colorR;
	int colorG;
	int colorB;

	double radiusSphere;
	int slicesSphere;
	int stacksSphere;

	double baseCone;
	double heightCone;
	int slicesCone;
	int stacksCone;

	double sizeCube;

	double innerRadiusTorus;
	double outerRadiusTorus;
	int sidesTorus;
	int ringsTorus;
};

#endif // AGEOBJECTPRIM
