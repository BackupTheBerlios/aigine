/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbMatrix.h
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Matrizenrechnung

	Autor:
	David Scherfgen

********************************************************************/

// Verwendete Symbole vordefinieren
class tbVector3;
TRIBASE_API tbMatrix tbMatrixInvert(const tbMatrix& m);

// ******************************************************************
// Die Matrixklasse
class TRIBASE_API tbMatrix
{
public:
	// Variablen
	union
	{
		struct
		{
			float m11, m12, m13, m14,	// Elemente der Matrix
				  m21, m22, m23, m24,
				  m31, m32, m33, m34,
				  m41, m42, m43, m44;
		};

		float		m[4][4];			// Zweidimensionales Array der Elemente
		float		n[16];				// Eindimensionales Array der Elemente
		D3DMATRIX	mD3DMatrix;			// Matrix als D3DMATRIX
	};

	// Konstruktoren
	inline tbMatrix() {};

	inline tbMatrix(float _m11, float _m12, float _m13, float _m14,
					float _m21, float _m22, float _m23, float _m24,
					float _m31, float _m32, float _m33, float _m34,
					float _m41, float _m42, float _m43, float _m44) : m11(_m11), m12(_m12), m13(_m13), m14(_m14),
					                                                  m21(_m21), m22(_m22), m23(_m23), m24(_m24),
																	  m31(_m31), m32(_m32), m33(_m33), m34(_m34),
																	  m41(_m41), m42(_m42), m43(_m43), m44(_m44)
	{
	}

	inline tbMatrix(const float* pfValue) : mD3DMatrix(*((D3DMATRIX*)(pfValue)))	{}
	inline tbMatrix(const D3DMATRIX& m) : mD3DMatrix(m)								{}

	// Casting-Opeatoren
	inline operator float* ()		{return (float*)(n);}
	inline operator D3DMATRIX& ()	{return mD3DMatrix;}

	// Arithmetische Operatoren
	inline tbMatrix operator + (const tbMatrix& m) const	{return tbMatrix(m11 + m.m11, m12 + m.m12, m13 + m.m13, m14 + m.m14, m21 + m.m21, m22 + m.m22, m23 + m.m23, m24 + m.m24, m31 + m.m31, m32 + m.m32, m33 + m.m33, m34 + m.m34, m41 + m.m41, m42 + m.m42, m43 + m.m43, m44 + m.m44);}
	inline tbMatrix operator - (const tbMatrix& m) const	{return tbMatrix(m11 - m.m11, m12 - m.m12, m13 - m.m13, m14 - m.m14, m21 - m.m21, m22 - m.m22, m23 - m.m23, m24 - m.m24, m31 - m.m31, m32 - m.m32, m33 - m.m33, m34 - m.m34, m41 - m.m41, m42 - m.m42, m43 - m.m43, m44 - m.m44);}
	inline tbMatrix operator - () const						{return tbMatrix(-m11, -m12, -m13, -m14, -m21, -m22, -m23, -m24, -m31, -m32, -m33, -m34, -m41, -m42, -m43, -m44);}

	inline tbMatrix operator * (const tbMatrix& m) const
	{
		return tbMatrix(m.m11 * m11 + m.m21 * m12 + m.m31 * m13 + m.m41 * m14,
						m.m12 * m11 + m.m22 * m12 + m.m32 * m13 + m.m42 * m14,
						m.m13 * m11 + m.m23 * m12 + m.m33 * m13 + m.m43 * m14,
						m.m14 * m11 + m.m24 * m12 + m.m34 * m13 + m.m44 * m14,
						m.m11 * m21 + m.m21 * m22 + m.m31 * m23 + m.m41 * m24,
						m.m12 * m21 + m.m22 * m22 + m.m32 * m23 + m.m42 * m24,
						m.m13 * m21 + m.m23 * m22 + m.m33 * m23 + m.m43 * m24,
						m.m14 * m21 + m.m24 * m22 + m.m34 * m23 + m.m44 * m24,
						m.m11 * m31 + m.m21 * m32 + m.m31 * m33 + m.m41 * m34,
						m.m12 * m31 + m.m22 * m32 + m.m32 * m33 + m.m42 * m34,
						m.m13 * m31 + m.m23 * m32 + m.m33 * m33 + m.m43 * m34,
						m.m14 * m31 + m.m24 * m32 + m.m34 * m33 + m.m44 * m34,
						m.m11 * m41 + m.m21 * m42 + m.m31 * m43 + m.m41 * m44,
						m.m12 * m41 + m.m22 * m42 + m.m32 * m43 + m.m42 * m44,
						m.m13 * m41 + m.m23 * m42 + m.m33 * m43 + m.m43 * m44,
						m.m14 * m41 + m.m24 * m42 + m.m34 * m43 + m.m44 * m44);
	}

	inline tbMatrix operator * (const float f) const
	{
		return tbMatrix(m11 * f, m12 * f, m13 * f, m14 * f,
			            m21 * f, m22 * f, m23 * f, m24 * f,
						m31 * f, m32 * f, m33 * f, m34 * f,
						m41 * f, m42 * f, m43 * f, m44 * f);
	}

	inline tbMatrix operator / (const tbMatrix& m) const {return *this * tbMatrixInvert(m);}

	inline tbMatrix operator / (const float f) const
	{
		return tbMatrix(m11 / f, m12 / f, m13 / f, m14 / f,
			            m21 / f, m22 / f, m23 / f, m24 / f,
						m31 / f, m32 / f, m33 / f, m34 / f,
						m41 / f, m42 / f, m43 / f, m44 / f);
	}
	
	inline friend tbMatrix operator * (const float f, const tbMatrix& m)
	{
		return tbMatrix(m.m11 * f, m.m12 * f, m.m13 * f, m.m14 * f,
			            m.m21 * f, m.m22 * f, m.m23 * f, m.m24 * f,
						m.m31 * f, m.m32 * f, m.m33 * f, m.m34 * f,
						m.m41 * f, m.m42 * f, m.m43 * f, m.m44 * f);
	}

	// Zuweisungsoperatoren
	inline tbMatrix operator = (const tbMatrix& m) {mD3DMatrix = m.mD3DMatrix; return *this;}
	
	inline tbMatrix operator += (const tbMatrix& m)
	{
		m11 += m.m11; m12 += m.m12; m13 += m.m13; m14 += m.m14;
		m21 += m.m21; m22 += m.m22; m23 += m.m23; m24 += m.m24;
		m31 += m.m31; m32 += m.m32; m33 += m.m33; m34 += m.m34;
		m41 += m.m41; m42 += m.m42; m43 += m.m43; m44 += m.m44;
		return *this;
	}

	inline tbMatrix operator -= (const tbMatrix& m)
	{
		m11 -= m.m11; m12 -= m.m12; m13 -= m.m13; m14 -= m.m14;
		m21 -= m.m21; m22 -= m.m22; m23 -= m.m23; m24 -= m.m24;
		m31 -= m.m31; m32 -= m.m32; m33 -= m.m33; m34 -= m.m34;
		m41 -= m.m41; m42 -= m.m42; m43 -= m.m43; m44 -= m.m44;
		return *this;
	}

	inline tbMatrix operator *= (const tbMatrix& m) {*this = *this * m; return *this;}

	inline tbMatrix operator *= (const float f)
	{
		m11 *= f; m12 *= f; m13 *= f; m14 *= f;
		m21 *= f; m22 *= f; m23 *= f; m24 *= f;
		m31 *= f; m32 *= f; m33 *= f; m34 *= f;
		m41 *= f; m42 *= f; m43 *= f; m44 *= f;
		return *this;
	}
	
	inline tbMatrix operator /= (const tbMatrix& m)	{*this = *this / m; return *this;}

	inline tbMatrix operator /= (const float f)
	{
		m11 /= f; m12 /= f; m13 /= f; m14 /= f;
		m21 /= f; m22 /= f; m23 /= f; m24 /= f;
		m31 /= f; m32 /= f; m33 /= f; m34 /= f;
		m41 /= f; m42 /= f; m43 /= f; m44 /= f;
		return *this;
	}

	// Vergleichsoperatoren
	inline bool operator == (const tbMatrix& m) const
	{
		if(m11 != m.m11) return false;
		if(m12 != m.m12) return false;
		if(m13 != m.m13) return false;
		if(m14 != m.m14) return false;
		if(m21 != m.m21) return false;
		if(m22 != m.m22) return false;
		if(m23 != m.m23) return false;
		if(m24 != m.m24) return false;
		if(m31 != m.m31) return false;
		if(m32 != m.m32) return false;
		if(m33 != m.m33) return false;
		if(m34 != m.m34) return false;
		if(m41 != m.m41) return false;
		if(m42 != m.m42) return false;
		if(m43 != m.m43) return false;
		return m44 == m.m44;
	}

	inline bool operator != (const tbMatrix& m) const
	{
		if(m11 != m.m11) return true;
		if(m12 != m.m12) return true;
		if(m13 != m.m13) return true;
		if(m14 != m.m14) return true;
		if(m21 != m.m21) return true;
		if(m22 != m.m22) return true;
		if(m23 != m.m23) return true;
		if(m24 != m.m24) return true;
		if(m31 != m.m31) return true;
		if(m32 != m.m32) return true;
		if(m33 != m.m33) return true;
		if(m34 != m.m34) return true;
		if(m41 != m.m41) return true;
		if(m42 != m.m42) return true;
		if(m43 != m.m43) return true;
		return m44 != m.m44;
	}
};

// ******************************************************************
// Funktionen deklarieren
inline		tbMatrix	tbMatrixIdentity() {return tbMatrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);}	// Identitätsmatrix liefern
TRIBASE_API tbMatrix	tbMatrixTranslation(const tbVector3& v);																								// Translationsmatrix (Verschiebungsmatrix) berechnen
TRIBASE_API tbMatrix	tbMatrixRotationX(const float f);																										// Rotationsmatrix um die X-Achse berechnen
TRIBASE_API tbMatrix	tbMatrixRotationY(const float f);																										// Rotationsmatrix um die Y-Achse berechnen
TRIBASE_API tbMatrix	tbMatrixRotationZ(const float f);																										// Rotationsmatrix um die Z-Achse berechnen
TRIBASE_API tbMatrix	tbMatrixRotation(const float x, const float y, const float z);																			// Rotiert um alle drei Achsen
TRIBASE_API tbMatrix	tbMatrixRotationAxis(const tbVector3& v, const float f);																				// Rotationsmatrix um eine beliebige Achse berechnen
TRIBASE_API tbMatrix	tbMatrixScaling(const tbVector3& v);																									// Skalierungsmatrix berechnen
TRIBASE_API tbMatrix	tbMatrixAxes(const tbVector3& vXAxis, const tbVector3& vYAxis, const tbVector3& vZAxis);												// Liefert eine Achsenmatrix
TRIBASE_API float		tbMatrixDet(const tbMatrix& m, float* pfOut);																							// Determinante berechnen
TRIBASE_API tbMatrix	tbMatrixInvert(const tbMatrix& m);																										// Invertierte (umgekehrte) Matrix berechnen
TRIBASE_API tbMatrix	tbMatrixTranspose(const tbMatrix& m);																									// Transponierte Matrix berechnen
TRIBASE_API tbMatrix	tbMatrixProjection(const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane);								// Projektionsmatrix berechnen
TRIBASE_API tbMatrix	tbMatrixCamera(const tbVector3& vPos, const tbVector3& vLookAt, const tbVector3& vUp = tbVector3(0.0f, 1.0f, 0.0f));					// Kameramatrix erzeugen
TRIBASE_API tbMatrix	tbMatrixToTex2DMatrix(const tbMatrix& m);																								// In Texturmatrix umwandeln

// ******************************************************************