// Rotation3D.h: interface for the Rotation3D class.
//
//////////////////////////////////////////////////////////////////////

#ifndef ROTATION3D_H
#define ROTATION3D_H

#include "../AiGinE.h"

class Rotation3D : public Vector3D {

public:
	Rotation3D(float angle, float x, float y, float z);
	virtual ~Rotation3D();
	float angle;

};

#endif //ROTATION3D_H
