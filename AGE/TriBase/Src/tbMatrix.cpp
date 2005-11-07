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

#include <TriBase.h>

// ******************************************************************
// Translationsmatrix berechnen
TRIBASE_API tbMatrix tbMatrixTranslation(const tbVector3& v)
{
	// Translationsmatrix berechnen
	return tbMatrix(1.0f, 0.0f, 0.0f, 0.0f,
		            0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					v.x,  v.y,  v.z,  1.0f);
}

// ******************************************************************
// Rotationsmatrix für Rotation um die x-Achse berechnen
TRIBASE_API tbMatrix tbMatrixRotationX(const float f)
{
	tbMatrix mResult;

	// Rotationsmatrix berechnen
	mResult.m11 = 1.0f; mResult.m12 = 0.0f; mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	mResult.m21 = 0.0f;                                         mResult.m24 = 0.0f;
	mResult.m31 = 0.0f;                                         mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m22 = mResult.m33 = cosf(f);
	mResult.m23 = sinf(f);
	mResult.m32 = -mResult.m23;

	return mResult;
}

// ******************************************************************
// Rotationsmatrix für Rotation um die y-Achse berechnen
TRIBASE_API tbMatrix tbMatrixRotationY(const float f)
{
	tbMatrix mResult;

	// Rotationsmatrix berechnen
	                    mResult.m12 = 0.0f;                     mResult.m14 = 0.0f;
	mResult.m21 = 0.0f; mResult.m22 = 1.0f; mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	                    mResult.m32 = 0.0f;                     mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m33 = cosf(f);
	mResult.m31 = sinf(f);
	mResult.m13 = -mResult.m31;

	return mResult;
}

// ******************************************************************
// Rotationsmatrix für Rotation um die z-Achse berechnen
TRIBASE_API tbMatrix tbMatrixRotationZ(const float f)
{
	tbMatrix mResult;

	// Rotationsmatrix berechnen
	                                        mResult.m13 = 0.0f; mResult.m14 = 0.0f;
	                                        mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	mResult.m31 = 0.0f; mResult.m32 = 0.0f; mResult.m33 = 1.0f; mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;

	mResult.m11 = mResult.m22 = cosf(f);
	mResult.m12 = sinf(f);
	mResult.m21 = -mResult.m12;

	return mResult;
}

// ******************************************************************
// Rotiert um alle drei Achsen
TRIBASE_API tbMatrix tbMatrixRotation(const float x,
									  const float y,
									  const float z)
{
	return tbMatrixRotationZ(z) *
		   tbMatrixRotationX(x) *
		   tbMatrixRotationY(y);
}

// ******************************************************************
// Rotationsmatrix für Rotation um eine beliebige Achse berechnen
TRIBASE_API tbMatrix tbMatrixRotationAxis(const tbVector3& v,
										  const float f)
{
	// Sinus und Kosinus berechnen
	float fSin(sinf(-f));
	float fCos(cosf(-f));

	// Achsenvektor normalisieren
	tbVector3 vAxis(tbVector3Normalize(v));

	// Matrix erstellen
	return tbMatrix((vAxis.x * vAxis.x) * (1.0f - fCos) + fCos,
		            (vAxis.x * vAxis.y) * (1.0f - fCos) - (vAxis.z * fSin),
				    (vAxis.x * vAxis.z) * (1.0f - fCos) + (vAxis.y * fSin),
					0.0f,
					(vAxis.y * vAxis.x) * (1.0f - fCos) + (vAxis.z * fSin),
					(vAxis.y * vAxis.y) * (1.0f - fCos) + fCos,
					(vAxis.y * vAxis.z) * (1.0f - fCos) - (vAxis.x * fSin),
					0.0f,
					(vAxis.z * vAxis.x) * (1.0f - fCos) - (vAxis.y * fSin),
					(vAxis.z * vAxis.y) * (1.0f - fCos) + (vAxis.x * fSin),
					(vAxis.z * vAxis.z) * (1.0f - fCos) + fCos,
					0.0f,
					0.0f,
					0.0f,
					0.0f,
					1.0f);
}

// ******************************************************************
// Skalierungsmatrix berechnen
TRIBASE_API tbMatrix tbMatrixScaling(const tbVector3& v)
{
	// Skalierungsmatrix berechnen
	return tbMatrix(v.x,  0.0f, 0.0f, 0.0f,
		            0.0f, v.y,  0.0f, 0.0f,
					0.0f, 0.0f, v.z,  0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
}

// ******************************************************************
// Liefert eine Achsenmatrix
TRIBASE_API tbMatrix tbMatrixAxes(const tbVector3& vXAxis,
								  const tbVector3& vYAxis,
								  const tbVector3& vZAxis)
{
	return tbMatrix(vXAxis.x, vXAxis.y, vXAxis.z, 0.0f,
		            vYAxis.x, vYAxis.y, vYAxis.z, 0.0f,
					vZAxis.x, vZAxis.y, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************
// Determinante einer Matrix berechnen
TRIBASE_API float tbMatrixDet(const tbMatrix& m)
{
	// Determinante berechnen
	return m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) -
           m.m12 * (m.m21 * m.m33 - m.m23 * m.m31) +
           m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
}

// ******************************************************************
// Invertierte Matrix berechnen
TRIBASE_API tbMatrix tbMatrixInvert(const tbMatrix& m)
{
	// Determinante berechnen
	float fInvDet(tbMatrixDet(m));
	if(fInvDet == 0.0f) return tbMatrixIdentity();
	fInvDet = 1.0f / fInvDet;

	// Invertierte Matrix berechnen
	tbMatrix mResult;
	mResult.m11 =  fInvDet * (m.m22 * m.m33 - m.m23 * m.m32);
	mResult.m12 = -fInvDet * (m.m12 * m.m33 - m.m13 * m.m32);
	mResult.m13 =  fInvDet * (m.m12 * m.m23 - m.m13 * m.m22);
	mResult.m14 =  0.0f;
	mResult.m21 = -fInvDet * (m.m21 * m.m33 - m.m23 * m.m31);
	mResult.m22 =  fInvDet * (m.m11 * m.m33 - m.m13 * m.m31);
	mResult.m23 = -fInvDet * (m.m11 * m.m23 - m.m13 * m.m21);
	mResult.m24 =  0.0f;
	mResult.m31 =  fInvDet * (m.m21 * m.m32 - m.m22 * m.m31);
	mResult.m32 = -fInvDet * (m.m11 * m.m32 - m.m12 * m.m31);
	mResult.m33 =  fInvDet * (m.m11 * m.m22 - m.m12 * m.m21);
	mResult.m34 =  0.0f;
	mResult.m41 = -(m.m41 * mResult.m11 + m.m42 * mResult.m21 + m.m43 * mResult.m31);
	mResult.m42 = -(m.m41 * mResult.m12 + m.m42 * mResult.m22 + m.m43 * mResult.m32);
	mResult.m43 = -(m.m41 * mResult.m13 + m.m42 * mResult.m23 + m.m43 * mResult.m33);
	mResult.m44 =  1.0f;

	return mResult;
}

// ******************************************************************
// Transponierte Matrix berechnen
TRIBASE_API tbMatrix tbMatrixTranspose(const tbMatrix& m)
{
	// Matrix transponieren
	return tbMatrix(m.m11, m.m21, m.m31, m.m41,
		            m.m12, m.m22, m.m32, m.m42,
					m.m13, m.m23, m.m33, m.m43,
					m.m14, m.m24, m.m34, m.m44);
}

// ******************************************************************
// Projektionsmatrix berechnen
TRIBASE_API tbMatrix tbMatrixProjection(const float fFOV,
										const float fAspect,
										const float fNearPlane,
										const float fFarPlane)
{
	tbMatrix mResult;

	// D3DX-Funktion benutzen
	D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)(&mResult),
		                       fFOV,
							   fAspect,
							   fNearPlane,
							   fFarPlane);

	return mResult;
}

// ******************************************************************
// Kameramatrix berechnen
TRIBASE_API tbMatrix tbMatrixCamera(const tbVector3& vPos,
									const tbVector3& vLookAt,
									const tbVector3& vUp)	// = tbVector3(0.0f, 1.0f, 0.0f)
{
	// Die z-Achse des Kamerakoordinatensystems berechnen
	tbVector3 vZAxis(tbVector3Normalize(vLookAt - vPos));

	// Die x-Achse ist das Kreuzprodukt aus y- und z-Achse
	tbVector3 vXAxis(tbVector3Normalize(tbVector3Cross(vUp, vZAxis)));

	// y-Achse berechnen
	tbVector3 vYAxis(tbVector3Normalize(tbVector3Cross(vZAxis, vXAxis)));

	// Rotationsmatrix erzeugen und die Translationsmatrix mir ihr multiplizieren
	return tbMatrixTranslation(-vPos) *
	       tbMatrix(vXAxis.x, vYAxis.x, vZAxis.x, 0.0f,
		            vXAxis.y, vYAxis.y, vZAxis.y, 0.0f,
				    vXAxis.z, vYAxis.z, vZAxis.z, 0.0f,
					0.0f,     0.0f,     0.0f,     1.0f);
}

// ******************************************************************
// Texturmatrix berechnen
TRIBASE_API tbMatrix tbMatrixToTex2DMatrix(const tbMatrix& m)
{
	// Texturmatrix berechnen
	return tbMatrix(m.m11, m.m12, m.m14, 0.0f,
		            m.m21, m.m22, m.m24, 0.0f,
					m.m41, m.m42, m.m44, 0.0f,
					0.0f,  0.0f,  0.0f,  1.0f);
}

// ******************************************************************