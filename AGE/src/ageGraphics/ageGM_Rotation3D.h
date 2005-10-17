// Rotation3D.h: interface for the Rotation3D class.
//
//////////////////////////////////////////////////////////////////////

#ifndef AGEGM_ROTATION3D_H
#define AGEGM_ROTATION3D_H

#include "ageGM_Vector3.h"
#include "ageGraphics/Model/ageGM_Vector3.h"

class ageGM_Rotation3D : public ageGM_Vector3 {

public:
	ageGM_Rotation3D(float angle, float x, float y, float z);
	virtual ~ageGM_Rotation3D();
	float angle;

};

#endif //AGEGM_ROTATION3D_H
