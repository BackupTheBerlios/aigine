#include "../AiGinE.h"

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

float Vector3D::Length(const Vector3D& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
float Vector3D::LengthSq(const Vector3D& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}
float Vector3D::Dot(const Vector3D& v1, const Vector3D& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
float Vector3D::Angle(const Vector3D& v1, const Vector3D& v2) {
	return acosf(Dot(NormalizeEx(v1), NormalizeEx(v2)));
}
Vector3D	Vector3D::Normalize(const Vector3D& v) {
	return v / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}
Vector3D	Vector3D::NormalizeEx(const Vector3D& v) {
	return v / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);
}
Vector3D	Vector3D::Cross(const Vector3D& v1, const Vector3D& v2) {
	return Vector3D(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}
Vector3D	Vector3D::InterpolateCoords(const Vector3D& v1, const Vector3D& v2, const float p) {
	return v1 + p * (v2 - v1);
}
Vector3D	Vector3D::InterpolateNormal(const Vector3D& v1, const Vector3D& v2, const float p) {
	return Normalize(v1 + p * (v2 - v1));
}
Vector3D	Vector3D::Min(const Vector3D& v1, const Vector3D& v2) {
	return Vector3D(AGE_MIN(v1.x, v2.x), AGE_MIN(v1.y, v2.y), AGE_MIN(v1.z, v2.z));
}
Vector3D	Vector3D::Max(const Vector3D& v1, const Vector3D& v2) {
	return Vector3D(AGE_MAX(v1.x, v2.x), AGE_MAX(v1.y, v2.y), AGE_MAX(v1.z, v2.z));
}
Vector3D	Vector3D::Random() {
	return NormalizeEx(Vector3D(FloatRandom(-1.0f, 1.0f), FloatRandom(-1.0f, 1.0f), FloatRandom(-1.0f, 1.0f)));
}
