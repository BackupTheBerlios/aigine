// Color.h: interface for the Color class.
//
//////////////////////////////////////////////////////////////////////

#ifndef COLOR_H
#define COLOR_H

#include "../AiGinE.h"

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
	Color(const float f);
	Color(const float _r, const float _g, const float _b);
	Color(const float _r, const float _g, const float _b, const float _a);
	Color(const float* pfComponent);
	Color(const BYTE _r, const BYTE _g, const BYTE _b);
	Color(const BYTE _r, const BYTE _g, const BYTE _b, const BYTE _a);
	Color(const BYTE* pComponent);
	Color(const DWORD c);
	
	// Casting-Operatoren
	operator DWORD () const;
	//{return ((a >= 1.0f ? 255 : a <= 0.0f ? 0 : (DWORD)(a * 255.0f)) << 24) | ((r >= 1.0f ? 255 : r <= 0.0f ? 0 : (DWORD)(r * 255.0f)) << 16) | ((g >= 1.0f ? 255 : g <= 0.0f ? 0 : (DWORD)(g * 255.0f)) << 8)  | (b >= 1.0f ? 255 : b <= 0.0f ? 0 : (DWORD)(b * 255.0f));}
	
	//	operator D3DCOLORVALUE& ()	{return D3DColorValue;}
	operator float* ();
	
	// Arithmetische Operatoren
	Color operator + (const Color& c) const;
	Color operator - (const Color& c) const;
	Color operator - () const;
	Color operator * (const Color& c) const;
	Color operator * (const float f) const;
	Color operator / (const Color &c) const;
	Color operator / (const float f) const;
	friend Color operator * (const float f, const Color& c)	{return Color(c.r * f, c.g * f, c.b * f, c.a * f);}
	
	// Zuweisungsoperatoren
	Color operator = (const Color& c);
	Color operator += (const Color& c);
	Color operator -= (const Color& c);
	Color operator *= (const Color& c);
	Color operator *= (const float f);
	Color operator /= (const Color& c);
	Color operator /= (const float f);
	
	// Vergleichsoperatoren
	bool operator == (const Color& c) const;
	bool operator != (const Color& c) const;
	
	// Funktionen deklarieren
	static Color ColorNegate(const Color& c);
	static float ColorBrightness(const Color& c);
	static Color ColorInterpolate(const Color& c1, const Color& c2, const float p);
	static Color ColorMin(const Color& c1, const Color& c2);
	static Color ColorMax(const Color& c1, const Color& c2);
	static Color ColorRandom(float fAlpha);
};

#endif //COLOR_H
