#include "Vector3D.h"

// Konstruktoren
Vector3D::Vector3D() {}
Vector3D::Vector3D(const float f) : x(f), y(f), z(f) {}
Vector3D::Vector3D(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}
Vector3D::Vector3D(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2]) {}

// Casting-Operatoren
Vector3D::operator float* () {
	return (float*)(c);
}

// Arithmetische Operatoren
Vector3D Vector3D::operator + (const Vector3D& v) const {
	return Vector3D(x + v.x, y + v.y, z + v.z);
}
Vector3D Vector3D::operator - (const Vector3D& v) const {
	return Vector3D(x - v.x, y - v.y, z - v.z);
}
Vector3D Vector3D::operator - () const {
	return Vector3D(-x, -y, -z);
}
Vector3D Vector3D::operator * (const Vector3D& v) const {
	return Vector3D(x * v.x, y * v.y, z * v.z);
}
Vector3D Vector3D::operator * (const float f) const {
	return Vector3D(x * f, y * f, z * f);
}
Vector3D Vector3D::operator / (const Vector3D& v) const {
	return Vector3D(x / v.x, y / v.y, z / v.z);
}
Vector3D Vector3D::operator / (const float f) const {
	return Vector3D(x / f, y / f, z / f);
}

// Zuweisungsoperatoren
Vector3D Vector3D::operator = (const Vector3D& v) {
	x = v.x; y = v.y; z = v.z; return *this;
}
Vector3D Vector3D::operator += (const Vector3D& v) {
	x += v.x; y += v.y; z += v.z; return *this;
}
Vector3D Vector3D::operator -= (const Vector3D& v) {
	x -= v.x; y -= v.y; z -= v.z; return *this;
}
Vector3D Vector3D::operator *= (const Vector3D& v) {
	x *= v.x; y *= v.y; z *= v.z; return *this;
}
Vector3D Vector3D::operator *= (const float f) {
	x *= f; y *= f; z *= f; return *this;
}
Vector3D Vector3D::operator /= (const Vector3D& v) {
	x /= v.x; y /= v.y; z /= v.z; return *this;
}
Vector3D Vector3D::operator /= (const float f) {
	x /= f; y /= f; z /= f; return *this;
}
void Vector3D::print() {
	cout << "x: " << this->x << "   y: " << this->y << "   z: " << this->z  << endl;
}
void Vector3D::print(char* description) {
	cout << description;
	print();
}

// Vergleichsoperatoren
bool Vector3D::operator == (const Vector3D& v) const {
	if(x != v.x) return false; if(y != v.y) return false; return z == v.z;
}
bool Vector3D::operator != (const Vector3D& v) const {
	if(x != v.x) return true; if(y != v.y) return true; return z != v.z;
}

float Vector3D::Vector3DLength(const Vector3D& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
float Vector3D::Vector3DLengthSq(const Vector3D& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
float Vector3D::Vector3DDot(const Vector3D& v1, const Vector3D& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
float Vector3D::Vector3DAngle(const Vector3D& v1, const Vector3D& v2) {
	return acosf(Vector3DDot(Vector3DNormalizeEx(v1), Vector3DNormalizeEx(v2)));
}
Vector3D	Vector3D::Vector3DNormalize(const Vector3D& v) {
	return v / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
Vector3D	Vector3D::Vector3DNormalizeEx(const Vector3D& v) {
	return v / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);
}
Vector3D	Vector3D::Vector3DCross(const Vector3D& v1, const Vector3D& v2) {
	return Vector3D(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}
Vector3D	Vector3D::Vector3DInterpolateCoords(const Vector3D& v1, const Vector3D& v2, const float p) {
	return v1 + p * (v2 - v1);
}
Vector3D	Vector3D::Vector3DInterpolateNormal(const Vector3D& v1, const Vector3D& v2, const float p) {
	return Vector3DNormalize(v1 + p * (v2 - v1));
}
Vector3D	Vector3D::Vector3DMin(const Vector3D& v1, const Vector3D& v2) {
	return Vector3D(MIN(v1.x, v2.x), MIN(v1.y, v2.y), MIN(v1.z, v2.z));
}
Vector3D	Vector3D::Vector3DMax(const Vector3D& v1, const Vector3D& v2) {
	return Vector3D(MAX(v1.x, v2.x), MAX(v1.y, v2.y), MAX(v1.z, v2.z));
}
Vector3D	Vector3D::Vector3DRandom() {
	return Vector3DNormalizeEx(Vector3D(FloatRandom(-1.0f, 1.0f), FloatRandom(-1.0f, 1.0f), FloatRandom(-1.0f, 1.0f)));
}
/*
float FloatRandom(const float fMin, const float fMax) {
	return fMin + (fMax - fMin) * ((float)(rand() % 10001) / 10000.0f);
}
*/
