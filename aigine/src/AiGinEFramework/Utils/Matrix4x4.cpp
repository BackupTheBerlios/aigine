// Matrix4x4.cpp: implementation of the Matrix4x4 class.
//
//////////////////////////////////////////////////////////////////////

#include "Matrix4x4.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Matrix4x4::Matrix4x4() {
}

Matrix4x4::~Matrix4x4() {

}
Matrix4x4::Matrix4x4(float _m11, float _m12, float _m13, float _m14,
					 float _m21, float _m22, float _m23, float _m24,
					 float _m31, float _m32, float _m33, float _m34,
					 float _m41, float _m42, float _m43, float _m44) {
	this->m11 = _m11; this->m12 = _m12; this->m13 = _m13; this->m14 = _m14; 
	this->m21 = _m21; this->m22 = _m22; this->m23 = _m23; this->m24 = _m24; 
	this->m31 = _m31; this->m32 = _m32; this->m33 = _m33; this->m34 = _m34; 
	this->m41 = _m41; this->m42 = _m42; this->m43 = _m43; this->m44 = _m44; 
}
/*
Matrix4x4::Matrix4x4(const Matrix4x4& m) {
	this->m = m;
}
/**/
// Casting-Opeatoren
Matrix4x4::operator float* () {}

// Arithmetische Operatoren
Matrix4x4 Matrix4x4::operator + (const Matrix4x4& m) const {
	return Matrix4x4(m11 + m.m11, m12 + m.m12, m13 + m.m13, m14 + m.m14, m21 + m.m21, m22 + m.m22, m23 + m.m23, m24 + m.m24, m31 + m.m31, m32 + m.m32, m33 + m.m33, m34 + m.m34, m41 + m.m41, m42 + m.m42, m43 + m.m43, m44 + m.m44);
}
Matrix4x4 Matrix4x4::operator - (const Matrix4x4& m) const {
	return Matrix4x4(m11 - m.m11, m12 - m.m12, m13 - m.m13, m14 - m.m14, m21 - m.m21, m22 - m.m22, m23 - m.m23, m24 - m.m24, m31 - m.m31, m32 - m.m32, m33 - m.m33, m34 - m.m34, m41 - m.m41, m42 - m.m42, m43 - m.m43, m44 - m.m44);
}
Matrix4x4 Matrix4x4::operator - () const {
	return Matrix4x4(-m11, -m12, -m13, -m14, -m21, -m22, -m23, -m24, -m31, -m32, -m33, -m34, -m41, -m42, -m43, -m44);
}
Matrix4x4 Matrix4x4::operator * (const Matrix4x4& m) const {
	return Matrix4x4(m.m11 * m11 + m.m21 * m12 + m.m31 * m13 + m.m41 * m14,
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
Matrix4x4 Matrix4x4::operator * (const float f) const {
	return Matrix4x4(m11 * f, m12 * f, m13 * f, m14 * f,
		m21 * f, m22 * f, m23 * f, m24 * f,
		m31 * f, m32 * f, m33 * f, m34 * f,
		m41 * f, m42 * f, m43 * f, m44 * f);
}
/*
Matrix4x4 Matrix4x4::operator / (const Matrix4x4& m) const {
//	return Matrix4x4Invert(m);
//return *this * Matrix4x4Invert(m);
}
/**/
Matrix4x4 Matrix4x4::operator / (const float f) const {
	return Matrix4x4(m11 / f, m12 / f, m13 / f, m14 / f,
		m21 / f, m22 / f, m23 / f, m24 / f,
		m31 / f, m32 / f, m33 / f, m34 / f,
		m41 / f, m42 / f, m43 / f, m44 / f);
}
//	Matrix4x4 Matrix4x4::operator * (const float f, const Matrix4x4& m) {}

// Zuweisungsoperatoren
Matrix4x4 Matrix4x4::operator = (const Matrix4x4& m) {
	m11 = m.m11; m12 = m.m12; m13 = m.m13; m14 = m.m14;
	m21 = m.m21; m22 = m.m22; m23 = m.m23; m14 = m.m24;
	m31 = m.m31; m32 = m.m32; m33 = m.m33; m34 = m.m34;
	m41 = m.m41; m42 = m.m42; m43 = m.m43; m44 = m.m44;
	return *this;
}
Matrix4x4 Matrix4x4::operator += (const Matrix4x4& m) {
	m11 += m.m11; m12 += m.m12; m13 += m.m13; m14 += m.m14;
	m21 += m.m21; m22 += m.m22; m23 += m.m23; m24 += m.m24;
	m31 += m.m31; m32 += m.m32; m33 += m.m33; m34 += m.m34;
	m41 += m.m41; m42 += m.m42; m43 += m.m43; m44 += m.m44;
	return *this;
}
Matrix4x4 Matrix4x4::operator -= (const Matrix4x4& m) {
	m11 -= m.m11; m12 -= m.m12; m13 -= m.m13; m14 -= m.m14;
	m21 -= m.m21; m22 -= m.m22; m23 -= m.m23; m24 -= m.m24;
	m31 -= m.m31; m32 -= m.m32; m33 -= m.m33; m34 -= m.m34;
	m41 -= m.m41; m42 -= m.m42; m43 -= m.m43; m44 -= m.m44;
	return *this;
}
Matrix4x4 Matrix4x4::operator *= (const Matrix4x4& m) {
	*this = *this * m;
	return *this;
}
Matrix4x4 Matrix4x4::operator *= (const float f) {
	m11 *= f; m12 *= f; m13 *= f; m14 *= f;
	m21 *= f; m22 *= f; m23 *= f; m24 *= f;
	m31 *= f; m32 *= f; m33 *= f; m34 *= f;
	m41 *= f; m42 *= f; m43 *= f; m44 *= f;
	return *this;
}
Matrix4x4 Matrix4x4::operator /= (const Matrix4x4& m) {
	this = this / m;
	return *this;
}
Matrix4x4 Matrix4x4::operator /= (const float f) {
	m11 /= f; m12 /= f; m13 /= f; m14 /= f;
	m21 /= f; m22 /= f; m23 /= f; m24 /= f;
	m31 /= f; m32 /= f; m33 /= f; m34 /= f;
	m41 /= f; m42 /= f; m43 /= f; m44 /= f;
	return *this;
	
}

// Vergleichsoperatoren
bool Matrix4x4::operator == (const Matrix4x4& m) const {
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
bool Matrix4x4::operator != (const Matrix4x4& m) const {
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

Matrix4x4 Matrix4x4::Matrix4x4Identity() {
	return Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}
//	Matrix4x4 Matrix4x4::Matrix4x4Translation(const Vector3D& v) {}

Matrix4x4	Matrix4x4::Matrix4x4RotationX(const float f) {
	Matrix4x4 mResult;
	
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
Matrix4x4	Matrix4x4::Matrix4x4RotationY(const float f) {
	Matrix4x4 mResult;
	
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
Matrix4x4	Matrix4x4::Matrix4x4RotationZ(const float f) {
	Matrix4x4 mResult;
	
	// Rotationsmatrix berechnen
	mResult.m13 = 0.0f; mResult.m14 = 0.0f; mResult.m23 = 0.0f; mResult.m24 = 0.0f;
	mResult.m31 = 0.0f; mResult.m32 = 0.0f; mResult.m33 = 1.0f; mResult.m34 = 0.0f;
	mResult.m41 = 0.0f; mResult.m42 = 0.0f; mResult.m43 = 0.0f; mResult.m44 = 1.0f;
	
	mResult.m11 = mResult.m22 = cosf(f);
	mResult.m12 = sinf(f);
	mResult.m21 = -mResult.m12;
	
	return mResult;
}
Matrix4x4 Matrix4x4::Matrix4x4Rotation(const float x, const float y, const float z) {
	return Matrix4x4RotationZ(z) * Matrix4x4RotationX(x) * Matrix4x4RotationY(y);
}
//	Matrix4x4	Matrix4x4::Matrix4x4RotationAxis(const Vector3D& v, const float f) {}
//	Matrix4x4	Matrix4x4::Matrix4x4Scaling(const Vector3D& v);																									// Skalierungsmatrix berechnen
//	Matrix4x4	Matrix4x4::Matrix4x4Axes(const Vector3D& vXAxis, const Vector3D& vYAxis, const Vector3D& vZAxis) {}
float Matrix4x4::Matrix4x4Det(const Matrix4x4& m) {
	return	m.m11 * (m.m22 * m.m33 - m.m23 * m.m32) -
		m.m12 * (m.m21 * m.m33 - m.m23 * m.m31) +
		m.m13 * (m.m21 * m.m32 - m.m22 * m.m31);
}
Matrix4x4 Matrix4x4::Matrix4x4Invert(const Matrix4x4& m) {
	// Determinante berechnen
	float fInvDet(Matrix4x4Det(m));
	if(fInvDet == 0.0f) return Matrix4x4Identity();
	fInvDet = 1.0f / fInvDet;
	
	// Invertierte Matrix berechnen
	Matrix4x4 mResult;
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
Matrix4x4	Matrix4x4::Matrix4x4Transpose(const Matrix4x4& m) {
	// Matrix transponieren
	return Matrix4x4(m.m11, m.m21, m.m31, m.m41,
		m.m12, m.m22, m.m32, m.m42,
		m.m13, m.m23, m.m33, m.m43,
		m.m14, m.m24, m.m34, m.m44);
}
/*
Matrix4x4	Matrix4x4::Matrix4x4Projection(const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane) {
	Matrix4x4 mResult;
	
	// D3DX-Funktion benutzen
	D3DXMatrixPerspectiveFovLH((D3DXMATRIX*)(&mResult),
		fFOV,
		fAspect,
		fNearPlane,
		fFarPlane);
	
	return mResult;
}
/**/
//	Matrix4x4	Matrix4x4::Matrix4x4Camera(const Vector3D& vPos, const Vector3D& vLookAt, const Vector3D& vUp = Vector3D(0.0f, 1.0f, 0.0f)) {}
Matrix4x4	Matrix4x4::Matrix4x4ToTex2DMatrix(const Matrix4x4& m) {
	return Matrix4x4(m.m11, m.m12, m.m14, 0.0f,
		            m.m21, m.m22, m.m24, 0.0f,
					m.m41, m.m42, m.m44, 0.0f,
					0.0f,  0.0f,  0.0f,  1.0f);}
