/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbColor.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Deklaration einer Farbklasse und von Farbfunktionen

	Autor:
	David Scherfgen

********************************************************************/

// Makros und Definitionen
#define TB_COLOR_CONV (0.003921568627450980392156862745098f) // = 1/255

// ******************************************************************
// Die Farbklasse
class TRIBASE_API tbColor
{
public:
	// Variablen
	union
	{
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
		D3DCOLORVALUE	D3DColorValue;	// Farbe als D3DCOLORVALUE
	};

	// Konstruktoren
	inline tbColor()																																																	{}
	inline tbColor(const float f) : r(f), g(f), b(f), a(1.0f)																																							{}
	inline tbColor(const float _r, const float _g, const float _b) : r(_r), g(_g), b(_b), a(1.0f)																														{}
	inline tbColor(const float _r, const float _g, const float _b, const float _a) : r(_r), g(_g), b(_b), a(_a)																											{}
	inline tbColor(const float* pfComponent) : r(pfComponent[0]), g(pfComponent[1]), b(pfComponent[2]), a(pfComponent[3])																								{}
	inline tbColor(const BYTE _r, const BYTE _g, const BYTE _b) : r((float)(_r) * TB_COLOR_CONV), g((float)(_g) * TB_COLOR_CONV), b((float)(_b) * TB_COLOR_CONV), a(1.0f)												{}
	inline tbColor(const BYTE _r, const BYTE _g, const BYTE _b, const BYTE _a) : r((float)(_r) * TB_COLOR_CONV), g((float)(_g) * TB_COLOR_CONV), b((float)(_b) * TB_COLOR_CONV), a((float)(_a) * TB_COLOR_CONV)			{}
	inline tbColor(const BYTE* pComponent) : r((float)(pComponent[0]) * TB_COLOR_CONV), g((float)(pComponent[1]) * TB_COLOR_CONV), b((float)(pComponent[2]) * TB_COLOR_CONV), a((float)(pComponent[3]) * TB_COLOR_CONV)	{}
	inline tbColor(const DWORD c) : r(TB_COLOR_CONV * (float)(BYTE)(c >> 16)), g(TB_COLOR_CONV * (float)(BYTE)(c >> 8)), b(TB_COLOR_CONV * (float)(BYTE)(c)), a(TB_COLOR_CONV * (float)(BYTE)(c >> 24))					{}
	inline tbColor(const D3DCOLORVALUE& c) : D3DColorValue(c)																																							{}

	// Casting-Operatoren
	inline operator DWORD () const
	{
		return ((a >= 1.0f ? 255 : a <= 0.0f ? 0 : (DWORD)(a * 255.0f)) << 24) |
			   ((r >= 1.0f ? 255 : r <= 0.0f ? 0 : (DWORD)(r * 255.0f)) << 16) |
			   ((g >= 1.0f ? 255 : g <= 0.0f ? 0 : (DWORD)(g * 255.0f)) << 8)  |
			   (b >= 1.0f ? 255 : b <= 0.0f ? 0 : (DWORD)(b * 255.0f));
	}

	inline operator D3DCOLORVALUE& ()	{return D3DColorValue;}
	inline operator float* ()			{return (float*)(c);}

	// Arithmetische Operatoren
	inline tbColor operator + (const tbColor& c) const					{return tbColor(r + c.r, g + c.g, b + c.b, a + c.a);}
	inline tbColor operator - (const tbColor& c) const					{return tbColor(r - c.r, g - c.g, b - c.b, a - c.a);}
	inline tbColor operator - () const									{return tbColor(-r, -g, -b, -a);}
	inline tbColor operator * (const tbColor& c) const					{return tbColor(r * c.r, g * c.g, b * c.b, a * c.a);}
	inline tbColor operator * (const float f) const						{return tbColor(r * f, g * f, b * f, a * f);}
	inline tbColor operator / (const tbColor &c) const					{return tbColor(r / c.r, g / c.g, b / c.b, a / c.a);}
	inline tbColor operator / (const float f) const						{return tbColor(r / f, g / f, b / f, a / f);}
	inline friend tbColor operator * (const float f, const tbColor& c)	{return tbColor(c.r * f, c.g * f, c.b * f, c.a * f);}

	// Zuweisungsoperatoren
	inline tbColor operator = (const tbColor& c)	{D3DColorValue = c.D3DColorValue; return *this;}
	inline tbColor operator += (const tbColor& c)	{r += c.r; g += c.g; b += c.b; a += c.a; return *this;}
	inline tbColor operator -= (const tbColor& c)	{r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this;}
	inline tbColor operator *= (const tbColor& c)	{r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this;}
	inline tbColor operator *= (const float f)		{r *= f; g *= f; b *= f; a *= f; return *this;}
	inline tbColor operator /= (const tbColor& c)	{r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this;}
	inline tbColor operator /= (const float f)		{r /= f; g /= f; b /= f; a /= f; return *this;}

	// Vergleichsoperatoren
	inline bool operator == (const tbColor& c) const {if(r != c.r) return false; if(g != c.g) return false; if(b != c.b) return false; return a == c.a;}
	inline bool operator != (const tbColor& c) const {if(r != c.r) return true; if(g != c.g) return true; if(b != c.b) return true; return a != c.a;}
};

// ******************************************************************
// Funktionen deklarieren
inline tbColor	tbColorNegate(const tbColor& c)											{return tbColor(1.0f - c.r, 1.0f - c.g, 1.0f - c.b, 1.0f - c.a);}
inline float	tbColorBrightness(const tbColor& c)										{return c.r * 0.3f + c.g * 0.6f + c.b * 0.1f;}
inline tbColor	tbColorInterpolate(const tbColor& c1, const tbColor& c2, const float p)	{return c1 + p * (c2 - c1);}
inline tbColor	tbColorMin(const tbColor& c1, const tbColor& c2)						{return tbColor(TB_MIN(c1.r, c2.r), TB_MIN(c1.g, c2.g), TB_MIN(c1.b, c2.b), TB_MIN(c1.a, c2.a));}
inline tbColor	tbColorMax(const tbColor& c1, const tbColor& c2)						{return tbColor(TB_MAX(c1.r, c2.r), TB_MAX(c1.g, c2.g), TB_MAX(c1.b, c2.b), TB_MAX(c1.a, c2.a));}
inline tbColor	tbColorRandom(float fAlpha)												{return tbColor(tbFloatRandom(0.0f, 1.0f), tbFloatRandom(0.0f, 1.0f), tbFloatRandom(0.0f, 1.0f), fAlpha < 0.0f ? tbFloatRandom(0.0f, 1.0f) : fAlpha);}

// ******************************************************************