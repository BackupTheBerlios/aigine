#include "Vector2D.h"

Vector2D::Vector2D() {
}
Vector2D::Vector2D(const float f) : x(f), y(f) {
}
Vector2D::Vector2D(const float _x, const float _y) : x(_x), y(_y) {
}
Vector2D::Vector2D(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]) {
}

// Casting-Operatoren
Vector2D::operator float* () {
	return (float*)(c);
}

// Arithmetische Operatoren
Vector2D Vector2D::operator + (const Vector2D& v) const {
	return Vector2D(x + v.x, y + v.y);
}
Vector2D Vector2D::operator - (const Vector2D& v) const {
	return Vector2D(x - v.x, y - v.y);
}
Vector2D Vector2D::operator - () const {
	return Vector2D(-x, -y);
}
Vector2D Vector2D::operator * (const Vector2D& v) const {
	return Vector2D(x * v.x, y * v.y);
}
Vector2D Vector2D::operator * (const float f) const {
	return Vector2D(x * f, y * f);
}
Vector2D Vector2D::operator / (const Vector2D& v) const {
	return Vector2D(x / v.x, y / v.y);
}
Vector2D Vector2D::operator / (const float f) const {
	return Vector2D(x / f, y / f);
}

// Zuweisungsoperatoren
Vector2D Vector2D::operator = (const Vector2D& v) {
	x = v.x; y = v.y;
	return *this;
}
Vector2D Vector2D::operator += (const Vector2D& v) {
	x += v.x; y += v.y;
	return *this;
}
Vector2D Vector2D::operator -= (const Vector2D& v) {
	x -= v.x; y -= v.y;
	return *this;
}
Vector2D Vector2D::operator *= (const Vector2D& v) {
	x *= v.x; y *= v.y;
	return *this;
}
Vector2D Vector2D::operator *= (const float f) {
	x *= f; y *= f;
	return *this;
}
Vector2D Vector2D::operator /= (const Vector2D& v) {
	x /= v.x; y /= v.y;
	return *this;
}
Vector2D Vector2D::operator /= (const float f) {
	x /= f; y /= f;
	return *this;
}

// Vergleichsoperatoren
bool Vector2D::operator == (const Vector2D& v) const {
	if(x != v.x) {
		return false;
	} else {
		return y == v.y;
	}
}
bool Vector2D::operator != (const Vector2D& v) const {
	if(x != v.x) {
		return true;
	} else {
		return y != v.y;
	}
}

// ******************************************************************
float Vector2DLength(const Vector2D& v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}
float Vector2DLengthSq(const Vector2D& v) {
	return v.x * v.x + v.y * v.y;
}
float Vector2DDot(const Vector2D& v1, const Vector2D& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
float Vector2DAngle(const Vector2D& v1, const Vector2D& v2) {
	return acosf(Vector2DDot(Vector2DNormalizeEx(v1), Vector2DNormalizeEx(v2)));
}
Vector2D	Vector2DNormalize(const Vector2D& v) {
	return v / sqrtf(v.x * v.x + v.y * v.y);
}
Vector2D	Vector2DNormalizeEx(const Vector2D& v) {
	return v / (sqrtf(v.x * v.x + v.y * v.y) + 0.0001f);
}
Vector2D	Vector2DInterpolateCoords(const Vector2D& v1, const Vector2D& v2, const float p) {
	return v1 + p * (v2 - v1);
}
Vector2D	Vector2DInterpolateNormal(const Vector2D& v1, const Vector2D& v2, const float p) {
	return Vector2DNormalize(v1 + p * (v2 - v1));
}
Vector2D	Vector2DMin(const Vector2D& v1, const Vector2D& v2) {
	return Vector2D(MIN(v1.x, v2.x), MIN(v1.y, v2.y));
}
Vector2D	Vector2DMax(const Vector2D& v1, const Vector2D& v2) {
	return Vector2D(MAX(v1.x, v2.x), MAX(v1.y, v2.y));
}
Vector2D	Vector2DRandom() {
	return Vector2DNormalizeEx(Vector2D(FloatRandom(-1.0f, 1.0f), FloatRandom(-1.0f, 1.0f)));
}
/*
float FloatRandom(const float fMin, const float fMax) {
	return fMin + (fMax - fMin) * ((float)(rand() % 10001) / 10000.0f);
}
*/
