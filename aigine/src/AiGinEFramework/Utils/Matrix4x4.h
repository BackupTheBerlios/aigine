// Matrix4x4.h: interface for the Matrix4x4 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#define TB_DEG_TO_RAD(x)		((x) * 0.0174532925199432957692369076848f)		// Grad -> Bogenma�
#define TB_RAD_TO_DEG(x)		((x) * 57.295779513082320876798154814105f)		// Bogenma� -> Grad

#include <Math.h>
#include <iostream>
using namespace std;

class Matrix4x4  
{
public:
	union {
		struct {
			float	m11, m12, m13, m14,	// Elemente der Matrix
					m21, m22, m23, m24,
					m31, m32, m33, m34,
					m41, m42, m43, m44;
		};		
		float		m[4][4];			// Zweidimensionales Array der Elemente
		float		n[16];				// Eindimensionales Array der Elemente
	};	

	Matrix4x4();
	virtual ~Matrix4x4();
	Matrix4x4(	float _m11, float _m12, float _m13, float _m14,
				float _m21, float _m22, float _m23, float _m24,
				float _m31, float _m32, float _m33, float _m34,
				float _m41, float _m42, float _m43, float _m44
			);
//	Matrix4x4(const Matrix4x4& m);
	// Casting-Opeatoren
	operator float* ();

	// Arithmetische Operatoren
	Matrix4x4 operator + (const Matrix4x4& m) const;
	Matrix4x4 operator - (const Matrix4x4& m) const;
	Matrix4x4 operator - () const;
	Matrix4x4 operator * (const Matrix4x4& m) const;
	Matrix4x4 operator * (const float f) const;
	Matrix4x4 operator / (const Matrix4x4& m) const;
	Matrix4x4 operator / (const float f) const;
//	Matrix4x4 operator * (const float f, const Matrix4x4& m);

	// Zuweisungsoperatoren
	Matrix4x4 operator =  (const Matrix4x4& m);
	Matrix4x4 operator += (const Matrix4x4& m);
	Matrix4x4 operator -= (const Matrix4x4& m);
	Matrix4x4 operator *= (const Matrix4x4& m);
	Matrix4x4 operator *= (const float f);
	Matrix4x4 operator /= (const Matrix4x4& m);
	Matrix4x4 operator /= (const float f);

	// Vergleichsoperatoren
	bool operator == (const Matrix4x4& m) const;
	bool operator != (const Matrix4x4& m) const;


};

	Matrix4x4	Matrix4x4Identity();
	//{return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);}	// Identit�tsmatrix liefern
//	Matrix4x4	Matrix4x4Translation(const Vector3D& v);																								// Translationsmatrix (Verschiebungsmatrix) berechnen
	Matrix4x4	Matrix4x4RotationX(const float f);																										// Rotationsmatrix um die X-Achse berechnen
	Matrix4x4	Matrix4x4RotationY(const float f);																										// Rotationsmatrix um die Y-Achse berechnen
	Matrix4x4	Matrix4x4RotationZ(const float f);																										// Rotationsmatrix um die Z-Achse berechnen
	Matrix4x4	Matrix4x4Rotation(const float x, const float y, const float z);																			// Rotiert um alle drei Achsen
//	Matrix4x4	Matrix4x4RotationAxis(const Vector3D& v, const float f);																				// Rotationsmatrix um eine beliebige Achse berechnen
//	Matrix4x4	Matrix4x4Scaling(const Vector3D& v);																									// Skalierungsmatrix berechnen
//	Matrix4x4	Matrix4x4Axes(const Vector3D& vXAxis, const Vector3D& vYAxis, const Vector3D& vZAxis);												// Liefert eine Achsenmatrix
	float		Matrix4x4Det(const Matrix4x4& m);																							// Determinante berechnen
	Matrix4x4	Matrix4x4Invert(const Matrix4x4& m);																										// Invertierte (umgekehrte) Matrix berechnen
	Matrix4x4	Matrix4x4Transpose(const Matrix4x4& m);																									// Transponierte Matrix berechnen
//	Matrix4x4	Matrix4x4Projection(const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane);								// Projektionsmatrix berechnen
//	Matrix4x4	Matrix4x4Camera(const Vector3D& vPos, const Vector3D& vLookAt, const Vector3D& vUp = Vector3D(0.0f, 1.0f, 0.0f));					// Kameramatrix erzeugen
	Matrix4x4	Matrix4x4ToTex2DMatrix(const Matrix4x4& m);																								// In Texturmatrix umwandeln


	float Matrix2x2Det(Matrix4x4 m2x2);
	float Matrix3x3Det(Matrix4x4 m3x3);
//	float Matrix4x4Det(Matrix4x4 m4x4);
	Matrix4x4 buildSubMatrix2x2(Matrix4x4 m3x3, int i, int k);
	Matrix4x4 buildSubMatrix3x3(Matrix4x4 m4x4, int i, int k);

#endif //MATRIX4X4_H