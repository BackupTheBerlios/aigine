// Color.h: interface for the Color class.
//
//////////////////////////////////////////////////////////////////////

#ifndef COLOR_H
#define COLOR_H

#include "Makros.h"
#include <iostream>
using namespace std;

class Color  
{
public:
	// Variablen


    /**
     * @alias data 
     */
	union {
		struct
		{
			float r;	// Rot-, Gruen-, Blau- und Alphakomponente
			float g;
			float b;
			float a;
		};

		struct
		{
			float fRed;		// Rot-, Gruen-, Blau- und Alphakomponente
			float fGreen;
			float fBlue;
			float fAlpha;
		};

		float			c[4];			// Array der Komponenten
//		D3DCOLORVALUE	D3DColorValue;	// Farbe als D3DCOLORVALUE
	};

	// Konstruktoren
	Color();
	//{}
	Color(const float f);
	//: r(f), g(f), b(f), a(1.0f);
	//{}
	Color(const float _r, const float _g, const float _b);
	//: r(_r), g(_g), b(_b), a(1.0f)																														{}
	Color(const float _r, const float _g, const float _b, const float _a);
	//: r(_r), g(_g), b(_b), a(_a)																											{}
	Color(const float* pfComponent);
	//: r(pfComponent[0]), g(pfComponent[1]), b(pfComponent[2]), a(pfComponent[3])																								{}
	Color(const BYTE _r, const BYTE _g, const BYTE _b);
	// : r((float)(_r) * TB_COLOR_CONV), g((float)(_g) * TB_COLOR_CONV), b((float)(_b) * TB_COLOR_CONV), a(1.0f)												{}
	Color(const BYTE _r, const BYTE _g, const BYTE _b, const BYTE _a);
	//: r((float)(_r) * TB_COLOR_CONV), g((float)(_g) * TB_COLOR_CONV), b((float)(_b) * TB_COLOR_CONV), a((float)(_a) * TB_COLOR_CONV)			{}
	Color(const BYTE* pComponent);
	//: r((float)(pComponent[0]) * TB_COLOR_CONV), g((float)(pComponent[1]) * TB_COLOR_CONV), b((float)(pComponent[2]) * TB_COLOR_CONV), a((float)(pComponent[3]) * TB_COLOR_CONV)	{}
	Color(const DWORD c);
	//: r(TB_COLOR_CONV * (float)(BYTE)(c >> 16)), g(TB_COLOR_CONV * (float)(BYTE)(c >> 8)), b(TB_COLOR_CONV * (float)(BYTE)(c)), a(TB_COLOR_CONV * (float)(BYTE)(c >> 24))					{}
	//Color(const D3DCOLORVALUE& c) : D3DColorValue(c)																																							{}

	// Casting-Operatoren
	operator DWORD () const;
	//{return ((a >= 1.0f ? 255 : a <= 0.0f ? 0 : (DWORD)(a * 255.0f)) << 24) | ((r >= 1.0f ? 255 : r <= 0.0f ? 0 : (DWORD)(r * 255.0f)) << 16) | ((g >= 1.0f ? 255 : g <= 0.0f ? 0 : (DWORD)(g * 255.0f)) << 8)  | (b >= 1.0f ? 255 : b <= 0.0f ? 0 : (DWORD)(b * 255.0f));}

//	operator D3DCOLORVALUE& ()	{return D3DColorValue;}
	operator float* ();
	//{return (float*)(c);}

	// Arithmetische Operatoren
	Color operator + (const Color& c) const;
	//{return Color(r + c.r, g + c.g, b + c.b, a + c.a);}
	Color operator - (const Color& c) const;
	//{return Color(r - c.r, g - c.g, b - c.b, a - c.a);}
	Color operator - () const;
	//{return Color(-r, -g, -b, -a);}
	Color operator * (const Color& c) const;
	//{return Color(r * c.r, g * c.g, b * c.b, a * c.a);}
	Color operator * (const float f) const;
	//{return Color(r * f, g * f, b * f, a * f);}
	Color operator / (const Color &c) const;
	//{return Color(r / c.r, g / c.g, b / c.b, a / c.a);}
	Color operator / (const float f) const;
	//{return Color(r / f, g / f, b / f, a / f);}
	friend Color operator * (const float f, const Color& c)	{return Color(c.r * f, c.g * f, c.b * f, c.a * f);}

	// Zuweisungsoperatoren
	Color operator = (const Color& c);
	//{D3DColorValue = c.D3DColorValue; return *this;}
	Color operator += (const Color& c);
	//{r += c.r; g += c.g; b += c.b; a += c.a; return *this;}
	Color operator -= (const Color& c);
	//{r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this;}
	Color operator *= (const Color& c);
	//{r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this;}
	Color operator *= (const float f);
	//{r *= f; g *= f; b *= f; a *= f; return *this;}
	Color operator /= (const Color& c);
	//{r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this;}
	Color operator /= (const float f);
	//{r /= f; g /= f; b /= f; a /= f; return *this;}

	// Vergleichsoperatoren
	bool operator == (const Color& c) const;
	//{if(r != c.r) return false; if(g != c.g) return false; if(b != c.b) return false; return a == c.a;}
	bool operator != (const Color& c) const;
	//{if(r != c.r) return true; if(g != c.g) return true; if(b != c.b) return true; return a != c.a;}
//};

// ******************************************************************
// Funktionen deklarieren
static Color ColorNegate(const Color& c);
//{return Color(1.0f - c.r, 1.0f - c.g, 1.0f - c.b, 1.0f - c.a);}
static float ColorBrightness(const Color& c);
//{return c.r * 0.3f + c.g * 0.6f + c.b * 0.1f;}
static Color ColorInterpolate(const Color& c1, const Color& c2, const float p);
//{return c1 + p * (c2 - c1);}
static Color ColorMin(const Color& c1, const Color& c2);
//{return Color(TB_MIN(c1.r, c2.r), TB_MIN(c1.g, c2.g), TB_MIN(c1.b, c2.b), TB_MIN(c1.a, c2.a));}
static Color ColorMax(const Color& c1, const Color& c2);
//{return Color(TB_MAX(c1.r, c2.r), TB_MAX(c1.g, c2.g), TB_MAX(c1.b, c2.b), TB_MAX(c1.a, c2.a));}
static Color ColorRandom(float fAlpha);
//{return Color(tbFloatRandom(0.0f, 1.0f), tbFloatRandom(0.0f, 1.0f), tbFloatRandom(0.0f, 1.0f), fAlpha < 0.0f ? tbFloatRandom(0.0f, 1.0f) : fAlpha);}

};

#endif //COLOR_H
