// Plane.h: interface for the Plane class.
//
//////////////////////////////////////////////////////////////////////

//minor change fpr cvs-test

#ifndef PLANE_H
#define PLANE_H

#include "../AiGinE.h"

class Plane {
public:
	// Variablen


    /**
     * @alias data 
     */
	union {
		struct
		{
			float a;		// Variablen der Ebenengleichung
			float b;
			float c;
			float d;
		};

		struct
		{
			Vector3D	n;	// Normalvektor
			float		d;	// Distanz
		};

		float v[4];			// Array der Variablen
	};

	// Konstruktoren
	Plane();
	Plane(const float _a, const float _b, const float _c, const float _d);
	Plane(const float* pfValue);
	Plane(const Vector3D& _n, float _d);

	// Casting-Operatoren
	operator float* ();

	// Zuweisungsoperatoren
	Plane operator = (const Plane& p);

	// Vergleichsoperatoren
	bool operator == (const Plane& p) const;
	bool operator != (const Plane& p) const;

// ******************************************************************
// Funktionen deklarieren
static Plane	Normalize(const Plane& p);
static float	DotNormal(const Plane& p, const Vector3D& v);
static float	DotCoords(const Plane& p, const Vector3D& v);
static Plane	FromPointNormal(const Vector3D& p, const Vector3D& n);
static Plane	FromPoints(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3);

};

#endif //PLANE_H
