// Color.cpp: implementation of the Color class.
//
//////////////////////////////////////////////////////////////////////

#include "Color.h"


// Konstruktoren
Color::Color() {}
Color::Color(const float f) : r(f), g(f), b(f), a(1.0f) {}
Color::Color(const float _r, const float _g, const float _b) : r(_r), g(_g), b(_b), a(1.0f) {}
Color::Color(const float _r, const float _g, const float _b, const float _a) : r(_r), g(_g), b(_b), a(_a) {}
Color::Color(const float* pfComponent) : r(pfComponent[0]), g(pfComponent[1]), b(pfComponent[2]), a(pfComponent[3]) {}
Color::Color(const BYTE _r, const BYTE _g, const BYTE _b) : r((float)(_r) * COLOR_CONV), g((float)(_g) * COLOR_CONV), b((float)(_b) * COLOR_CONV), a(1.0f) {}
Color::Color(const BYTE _r, const BYTE _g, const BYTE _b, const BYTE _a) : r((float)(_r) * COLOR_CONV), g((float)(_g) * COLOR_CONV), b((float)(_b) * COLOR_CONV), a((float)(_a) * COLOR_CONV) {}
Color::Color(const BYTE* pComponent) : r((float)(pComponent[0]) * COLOR_CONV), g((float)(pComponent[1]) * COLOR_CONV), b((float)(pComponent[2]) * COLOR_CONV), a((float)(pComponent[3]) * COLOR_CONV) {}
Color::Color(const DWORD c) : r(COLOR_CONV * (float)(BYTE)(c >> 16)), g(COLOR_CONV * (float)(BYTE)(c >> 8)), b(COLOR_CONV * (float)(BYTE)(c)), a(COLOR_CONV * (float)(BYTE)(c >> 24)) {}
//Color(const D3DCOLORVALUE& c) : D3DColorValue(c) {}

// Casting-Operatoren
Color::operator DWORD () const {
	return ((a >= 1.0f ? 255 : a <= 0.0f ? 0 : (DWORD)(a * 255.0f)) << 24) | ((r >= 1.0f ? 255 : r <= 0.0f ? 0 : (DWORD)(r * 255.0f)) << 16) | ((g >= 1.0f ? 255 : g <= 0.0f ? 0 : (DWORD)(g * 255.0f)) << 8)  | (b >= 1.0f ? 255 : b <= 0.0f ? 0 : (DWORD)(b * 255.0f));
}

//	operator D3DCOLORVALUE& ()	{return D3DColorValue;}
Color::operator float* () {
	return (float*)(c);
}

// Arithmetische Operatoren
Color Color::operator + (const Color& c) const {
	return Color(r + c.r, g + c.g, b + c.b, a + c.a);
}
Color Color::operator - (const Color& c) const {
	return Color(r - c.r, g - c.g, b - c.b, a - c.a);
}
Color Color::operator - () const {
	return Color(-r, -g, -b, -a);
}
Color Color::operator * (const Color& c) const {
	return Color(r * c.r, g * c.g, b * c.b, a * c.a);
}
Color Color::operator * (const float f) const {
	return Color(r * f, g * f, b * f, a * f);
}
Color Color::operator / (const Color &c) const {
	return Color(r / c.r, g / c.g, b / c.b, a / c.a);
}
Color Color::operator / (const float f) const {
	return Color(r / f, g / f, b / f, a / f);
}

// Zuweisungsoperatoren
Color Color::operator = (const Color& c) {
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
	return *this;
}
Color Color::operator += (const Color& c) {
	r += c.r;
	g += c.g;
	b += c.b;
	a += c.a;
	return *this;
}
Color Color::operator -= (const Color& c) {
	r -= c.r;
	g -= c.g;
	b -= c.b;
	a -= c.a;
	return *this;
}
Color Color::operator *= (const Color& c) {
	r *= c.r;
	g *= c.g;
	b *= c.b;
	a *= c.a;
	return *this;
}
Color Color::operator *= (const float f) {
	r *= f;
	g *= f;
	b *= f;
	a *= f;
	return *this;
}
Color Color::operator /= (const Color& c) {
	r /= c.r;
	g /= c.g;
	b /= c.b;
	a /= c.a;
	return *this;
}
Color Color::operator /= (const float f) {
	r /= f;
	g /= f;
	b /= f;
	a /= f;
	return *this;
}

// Vergleichsoperatoren
bool Color::operator == (const Color& c) const {
	if(r != c.r) return false;
	if(g != c.g) return false;
	if(b != c.b) return false;
	return a == c.a;
}
bool Color::operator != (const Color& c) const {
	if(r != c.r) return true;
	if(g != c.g) return true;
	if(b != c.b) return true;
	return a != c.a;
}


// ******************************************************************

Color Color::ColorNegate(const Color& c) {
	return Color(1.0f - c.r, 1.0f - c.g, 1.0f - c.b, 1.0f - c.a);
}
float Color::ColorBrightness(const Color& c) {
	return c.r * 0.3f + c.g * 0.6f + c.b * 0.1f;
}
Color Color::ColorInterpolate(const Color& c1, const Color& c2, const float p) {
	return c1 + p * (c2 - c1);
}
Color Color::ColorMin(const Color& c1, const Color& c2) {
	return Color(MIN(c1.r, c2.r), MIN(c1.g, c2.g), MIN(c1.b, c2.b), MIN(c1.a, c2.a));
}
Color Color::ColorMax(const Color& c1, const Color& c2) {
	return Color(MAX(c1.r, c2.r), MAX(c1.g, c2.g), MAX(c1.b, c2.b), MAX(c1.a, c2.a));
}
Color Color::ColorRandom(float fAlpha) {
	return Color(FloatRandom(0.0f, 1.0f), FloatRandom(0.0f, 1.0f), FloatRandom(0.0f, 1.0f), fAlpha < 0.0f ? FloatRandom(0.0f, 1.0f) : fAlpha);
}
