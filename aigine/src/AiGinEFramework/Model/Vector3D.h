/* Game Engine Design */

#ifndef Vector3DD_H
#define Vector3DD_H

#include "../AiGinE.h"

class Vector3D
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
//};

// ******************************************************************
// Funktionen deklarieren
static float Length(const Vector3D& v);
static float LengthSq(const Vector3D& v);
static float Dot(const Vector3D& v1, const Vector3D& v2);
static float Angle(const Vector3D& v1, const Vector3D& v2);
static Vector3D	Normalize(const Vector3D& v);
static Vector3D	NormalizeEx(const Vector3D& v);
static Vector3D	Cross(const Vector3D& v1, const Vector3D& v2);
static Vector3D	InterpolateCoords(const Vector3D& v1, const Vector3D& v2, const float p);
static Vector3D	InterpolateNormal(const Vector3D& v1, const Vector3D& v2, const float p);
static Vector3D	Min(const Vector3D& v1, const Vector3D& v2);
static Vector3D	Max(const Vector3D& v1, const Vector3D& v2);
static Vector3D	Random();
//float FloatRandom(const float fMin, const float fMax);
};

#endif //Vector3D_H