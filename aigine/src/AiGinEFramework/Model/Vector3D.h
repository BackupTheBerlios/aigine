/* Game Engine Design */

#ifndef Vector3DD_H
#define Vector3DD_H

#include "Makros.h"
#include <Math.h>
#include <iostream>
using namespace std;

class Vector3D
{
public:
	// Variablen
	union
	{
		struct
		{
			float x;	// Koordinaten
			float y;
			float z;
		};

		struct
		{
			float u;	// Texturkoordinaten
			float v;
			float w;
		};

		float c[3];		// Array der Koordinaten
	};

	// Konstruktoren
	Vector3D();
	Vector3D(const float f);
	Vector3D(const float _x, const float _y, const float _z);
	Vector3D(const float* pfComponent);

	// Casting-Operatoren
	operator float* ();

	// Arithmetische Operatoren
	Vector3D operator + (const Vector3D& v) const;
	Vector3D operator - (const Vector3D& v) const;
	Vector3D operator - () const;
	Vector3D operator * (const Vector3D& v) const;
	Vector3D operator * (const float f) const;
	Vector3D operator / (const Vector3D& v) const;
	Vector3D operator / (const float f) const;
	inline friend Vector3D operator * (const float f, const Vector3D& v)	{return Vector3D(v.x * f, v.y * f, v.z * f);}

	// Zuweisungsoperatoren
	Vector3D operator = (const Vector3D& v);
	Vector3D operator += (const Vector3D& v);
	Vector3D operator -= (const Vector3D& v);
	Vector3D operator *= (const Vector3D& v);
	Vector3D operator *= (const float f);
	Vector3D operator /= (const Vector3D& v);
	Vector3D operator /= (const float f);

	// Vergleichsoperatoren
	bool operator == (const Vector3D& v) const;
	bool operator != (const Vector3D& v) const;

	void print();
	void print(char* description);
};

// ******************************************************************
// Funktionen deklarieren
float Vector3DLength(const Vector3D& v);
float Vector3DLengthSq(const Vector3D& v);
float Vector3DDot(const Vector3D& v1, const Vector3D& v2);
float Vector3DAngle(const Vector3D& v1, const Vector3D& v2);
Vector3D	Vector3DNormalize(const Vector3D& v);
Vector3D	Vector3DNormalizeEx(const Vector3D& v);
Vector3D	Vector3DCross(const Vector3D& v1, const Vector3D& v2);
Vector3D	Vector3DInterpolateCoords(const Vector3D& v1, const Vector3D& v2, const float p);
Vector3D	Vector3DInterpolateNormal(const Vector3D& v1, const Vector3D& v2, const float p);
Vector3D	Vector3DMin(const Vector3D& v1, const Vector3D& v2);
Vector3D	Vector3DMax(const Vector3D& v1, const Vector3D& v2);
Vector3D	Vector3DRandom();
//float FloatRandom(const float fMin, const float fMax);

#endif //Vector3D_H