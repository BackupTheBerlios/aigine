#include "../AiGinE.h"

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
float Vector2D::Length(const Vector2D& v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}
float Vector2D::LengthSq(const Vector2D& v) {
	return v.x * v.x + v.y * v.y;
}
float Vector2D::Dot(const Vector2D& v1, const Vector2D& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}
float Vector2D::Angle(const Vector2D& v1, const Vector2D& v2) {
	return acosf(Dot(NormalizeEx(v1), NormalizeEx(v2)));
}
Vector2D Vector2D::Normalize(const Vector2D& v) {
	return v / sqrtf(v.x * v.x + v.y * v.y);
}
Vector2D Vector2D::NormalizeEx(const Vector2D& v) {
	return v / (sqrtf(v.x * v.x + v.y * v.y) + 0.0001f);
}
Vector2D Vector2D::InterpolateCoords(const Vector2D& v1, const Vector2D& v2, const float p) {
	return v1 + p * (v2 - v1);
}
Vector2D Vector2D::InterpolateNormal(const Vector2D& v1, const Vector2D& v2, const float p) {
	return Normalize(v1 + p * (v2 - v1));
}
Vector2D Vector2D::Min(const Vector2D& v1, const Vector2D& v2) {
	return Vector2D(AGE_MIN(v1.x, v2.x), AGE_MIN(v1.y, v2.y));
}
Vector2D Vector2D::Max(const Vector2D& v1, const Vector2D& v2) {
	return Vector2D(AGE_MAX(v1.x, v2.x), AGE_MAX(v1.y, v2.y));
}
Vector2D Vector2D::Random() {
	return NormalizeEx(Vector2D(FloatRandom(-1.0f, 1.0f), FloatRandom(-1.0f, 1.0f)));
}
