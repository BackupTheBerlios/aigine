// Rotation3D.cpp: implementation of the Rotation3D class.
//
//////////////////////////////////////////////////////////////////////

#include "ageGM_Rotation3D.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ageGM_Rotation3D::ageGM_Rotation3D(float angle, float x, float y, float z) : Vector3D(x, y, z) {
	this->angle = angle;
}

ageGM_Rotation3D::~ageGM_Rotation3D()
{

}