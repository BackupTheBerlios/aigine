// Plane.h: interface for the Plane class.
//
//////////////////////////////////////////////////////////////////////

#ifndef PLANE_H
#define PLANE_H

#include "Vector3D.h"

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
	//{};
	Plane(const float _a, const float _b, const float _c, const float _d);
	//: a(_a), b(_b), c(_c), d(_d)	{};
	Plane(const float* pfValue);
	//: a(pfValue[0]), b(pfValue[1]), c(pfValue[2]), d(pfValue[3])			{};
	Plane(const Vector3D& _n, float _d);
	//: n(_n), d(_d)												{};

	// Casting-Operatoren
	operator float* ();
	//{return (float*)(v);}

	// Zuweisungsoperatoren
	Plane operator = (const Plane& p);
	//{a = p.a; b = p.b; c = p.c; d = p.d; return *this;}

	// Vergleichsoperatoren
	bool operator == (const Plane& p) const;
	//{if(a != p.a) return false; if(b != p.b) return false; if(c != p.c) return false; return d == p.d;}
	bool operator != (const Plane& p) const;
	//{if(a != p.a) return true; if(b != p.b) return true; if(c != p.c) return true; return d != p.d;}
//};

// ******************************************************************
// Funktionen deklarieren
static Plane	PlaneNormalize(const Plane& p);
//{float fLength(tbVector3Length(p.n)); return tbPlane(p.n / fLength, p.d / fLength);}
static float	PlaneDotNormal(const Plane& p, const Vector3D& v);
//{return p.a * v.x + p.b * v.y + p.c * v.z;}
static float	PlaneDotCoords(const Plane& p, const Vector3D& v);
//{return p.a * v.x + p.b * v.y + p.c * v.z + p.d;}
static Plane	PlaneFromPointNormal(const Vector3D& p, const Vector3D& n);
//{return tbPlane(n, -n.x * p.x - n.y * p.y - n.z * p.z);}
static Plane	PlaneFromPoints(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3);
//{return tbPlaneFromPointNormal(v1, tbVector3Cross(v3 - v2, v1 - v2));}

};

#endif //PLANE_H
