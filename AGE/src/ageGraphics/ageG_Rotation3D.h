// Rotation3D.h: interface for the Rotation3D class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AGEG_ROTATION3D_H
#define AGEG_ROTATION3D_H

#include "ageG_Vector3.h"

class ageG_Rotation3D : public ageG_Vector3 {

public:
	ageG_Rotation3D(float angle, float x, float y, float z);
	virtual ~ageG_Rotation3D();
	float angle;

};

#endif //AGEG_ROTATION3D_H
