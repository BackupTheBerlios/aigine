// Plane.cpp: implementation of the Plane class.
//
//////////////////////////////////////////////////////////////////////

#include "Plane.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Konstruktoren
Plane::Plane(){};
Plane::Plane(const float _a, const float _b, const float _c, const float _d) : a(_a), b(_b), c(_c), d(_d) {};
Plane::Plane(const float* pfValue) : a(pfValue[0]), b(pfValue[1]), c(pfValue[2]), d(pfValue[3]) {};
Plane::Plane(const Vector3D& _n, float _d) : n(_n), d(_d) {};

// Casting-Operatoren
Plane::operator float* () {
	return (float*)(v);
}

// Zuweisungsoperatoren
Plane Plane::operator = (const Plane& p) {
	a = p.a;
	b = p.b;
	c = p.c;
	d = p.d;
	return *this;
}

// Vergleichsoperatoren
bool Plane::operator == (const Plane& p) const {
	if(a != p.a) return false;
	if(b != p.b) return false;
	if(c != p.c) return false;
	return d == p.d;
}

bool Plane::operator != (const Plane& p) const {
	if(a != p.a) return true;
	if(b != p.b) return true;
	if(c != p.c) return true;
	return d != p.d;
}

Plane	Plane::PlaneNormalize(const Plane& p) {
	float fLength(Vector3D::Vector3DLength(p.n));
	return Plane(p.n / fLength, p.d / fLength);
}

float	Plane::PlaneDotNormal(const Plane& p, const Vector3D& v) {
	return p.a * v.x + p.b * v.y + p.c * v.z;
}

float	Plane::PlaneDotCoords(const Plane& p, const Vector3D& v) {
	return p.a * v.x + p.b * v.y + p.c * v.z + p.d;
}

Plane	Plane::PlaneFromPointNormal(const Vector3D& p, const Vector3D& n) {
	return Plane(n, -n.x * p.x - n.y * p.y - n.z * p.z);
}

Plane	Plane::PlaneFromPoints(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3) {
	return PlaneFromPointNormal(v1, Vector3D::Vector3DCross(v3 - v2, v1 - v2));
}
