// Matrix4x4.h: interface for the Matrix4x4 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MATRIX4X4_H
#define MATRIX4X4_H

//#define TB_DEG_TO_RAD(x)		((x) * 0.0174532925199432957692369076848f)		// Grad -> Bogenmaß
//#define TB_RAD_TO_DEG(x)		((x) * 57.295779513082320876798154814105f)		// Bogenmaß -> Grad

#include "../AiGinE.h"

class Matrix4x4  
{
public:

    /**
     * @alias data 
     */
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


//};

	static Matrix4x4	Identity();
	static Matrix4x4	Translation(const Vector3D& v);																								// Translationsmatrix (Verschiebungsmatrix) berechnen
	static Matrix4x4	RotationX(const float f);																										// Rotationsmatrix um die X-Achse berechnen
	static Matrix4x4	RotationY(const float f);																										// Rotationsmatrix um die Y-Achse berechnen
	static Matrix4x4	RotationZ(const float f);																										// Rotationsmatrix um die Z-Achse berechnen
	static Matrix4x4	Rotation(const float x, const float y, const float z);																			// Rotiert um alle drei Achsen
	static Matrix4x4	RotationAxis(const Vector3D& v, const float f);																				// Rotationsmatrix um eine beliebige Achse berechnen
	static Matrix4x4	Scaling(const Vector3D& v);																									// Skalierungsmatrix berechnen
	static Matrix4x4	Axes(const Vector3D& vXAxis, const Vector3D& vYAxis, const Vector3D& vZAxis);												// Liefert eine Achsenmatrix
	static float		Det(const Matrix4x4& m);																							// Determinante berechnen
	static Matrix4x4	Invert(const Matrix4x4& m);																										// Invertierte (umgekehrte) Matrix berechnen
	static Matrix4x4	Transpose(const Matrix4x4& m);																									// Transponierte Matrix berechnen
	static Matrix4x4	Projection(const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane);								// Projektionsmatrix berechnen
	static Matrix4x4	Camera(const Vector3D& vPos, const Vector3D& vLookAt, const Vector3D& vUp = Vector3D(0.0f, 1.0f, 0.0f));					// Kameramatrix erzeugen
	static Matrix4x4	ToTex2DMatrix(const Matrix4x4& m);																								// In Texturmatrix umwandeln


	static float Matrix2x2Det(Matrix4x4 m2x2);
	static float Matrix3x3Det(Matrix4x4 m3x3);
//	static float Matrix4x4Det(Matrix4x4 m4x4);
	static Matrix4x4 buildSubMatrix2x2(Matrix4x4 m3x3, int i, int k);
	static Matrix4x4 buildSubMatrix3x3(Matrix4x4 m4x4, int i, int k);
};

#endif //MATRIX4X4_H
