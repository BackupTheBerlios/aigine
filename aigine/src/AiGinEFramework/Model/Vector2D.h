/* Game Engine Design */

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "Makros.h"
#include <Math.h>
#include <iostream>
using namespace std;

// ******************************************************************
// Die 2D-Vektor-Klasse
class Vector2D
{
public:
	// Variablen


    /**
     * @alias data 
     */
	union {
		struct
		{
			float x;	// Koordinaten
			float y;
		};

		struct
		{
			float u;	// Texturkoordinaten
			float v;
		};

		float c[2];		// Array der Koordinaten
	};


	// Konstruktoren
	Vector2D();
	Vector2D(const float f);
	Vector2D(const float _x, const float _y);
	Vector2D(const float* pfComponent);

	// Casting-Operatoren
	operator float* ();

	// Arithmetische Operatoren
	Vector2D operator + (const Vector2D& v) const;
	Vector2D operator - (const Vector2D& v) const;
	Vector2D operator - () const;
	Vector2D operator * (const Vector2D& v) const;
	Vector2D operator * (const float f) const;
	Vector2D operator / (const Vector2D& v) const;
	Vector2D operator / (const float f) const;
	inline friend Vector2D operator * (const float f, const Vector2D& v)	{ return Vector2D(v.x * f, v.y * f); }

	// Zuweisungsoperatoren
	Vector2D operator = (const Vector2D& v);
	Vector2D operator += (const Vector2D& v);
	Vector2D operator -= (const Vector2D& v);
	Vector2D operator *= (const Vector2D& v);
	Vector2D operator *= (const float f);
	Vector2D operator /= (const Vector2D& v);
	Vector2D operator /= (const float f);

	// Vergleichsoperatoren
	bool operator == (const Vector2D& v) const;
	bool operator != (const Vector2D& v) const;
//};

// ******************************************************************
// Funktionen deklarieren
static float Vector2DLength(const Vector2D& v);
static float Vector2DLengthSq(const Vector2D& v);
static float Vector2DDot(const Vector2D& v1, const Vector2D& v2);
static float Vector2DAngle(const Vector2D& v1, const Vector2D& v2);
static Vector2D	Vector2DNormalize(const Vector2D& v);
static Vector2D	Vector2DNormalizeEx(const Vector2D& v);
static Vector2D	Vector2DInterpolateCoords(const Vector2D& v1, const Vector2D& v2, const float p);
static Vector2D	Vector2DInterpolateNormal(const Vector2D& v1, const Vector2D& v2, const float p);
static Vector2D	Vector2DMin(const Vector2D& v1, const Vector2D& v2);
static Vector2D	Vector2DMax(const Vector2D& v1, const Vector2D& v2);
static Vector2D	Vector2DRandom();
};

#endif //Vector2D_H