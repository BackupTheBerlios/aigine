// ageObject3DS.h: interface for the ageObject3DS class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AGEOBJECT3DS_H

#define AGEOBJECT3DS_H

#include "AiGinEObject.h"
#include "../Util/loader3DS/Object3DS.h"
#include <GL/glut.h>

class ageObject3DS : public AiGinEObject  
{
public:
	ageObject3DS(string);
	virtual ~ageObject3DS();
	void display();
private:
	void load(string fileName);
	Object3DS* my3DSObject;
};

#endif // AGEOBJECT3DS
