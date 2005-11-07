/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVector4.h
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Rechnen mit 4D-Vektoren

	Autor:
	Snorky
	[24.9.03]

********************************************************************/

#ifndef __TBVECTOR4__
#define __TBVECTOR4__


// ******************************************************************
// Die 4D-Vektor-Klasse
class TRIBASE_API tbVector4
{
public:
	// Variablen
	union
	{
		struct
		{
			float x1;	// Koordinaten
			float y1;
			float x2;
			float y2;
		};

		struct
		{
			float left;	// Texturkoordinaten
			float top;
			float right;
			float bottom;
		};

		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			tbVector2 vTopLeft;
			tbVector2 vBottomRight;
		};

		RECT		rect;		// RECT-Struktur
		float		c[4];		// Array der Koordinaten
		D3DCOLORVALUE	D3DColorValue;	// Farbe als D3DCOLORVALUE
	};

	// Konstruktoren
	inline tbVector4()																												{}
	inline tbVector4(const float f)	: x1(f), y1(f), x2(f), y2(f)																	{}
	inline tbVector4(const float _x1, const float _y1, const float _x2, const float _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2)		{}
	inline tbVector4(const float* pfComponent) : x1(pfComponent[0]), y1(pfComponent[1]), x2(pfComponent[2]), y2(pfComponent[3])		{}
	inline tbVector4(const tbVector2 _vTopLeft, const tbVector2 _vBottomRight) : vTopLeft(_vTopLeft), vBottomRight(_vBottomRight)	{}
	inline tbVector4(const D3DCOLORVALUE& c) : D3DColorValue(c)																		{}
	inline tbVector4(const tbColor _vColor) : x1(_vColor.r), y1(_vColor.g), x2(_vColor.b), y2(_vColor.a)							{}
	inline tbVector4(const RECT _rect) : rect(_rect)																				{}
	
	// Casting-Operatoren
	inline operator float* ()			{return (float*)(c);}
	inline operator D3DCOLORVALUE& ()	{return D3DColorValue;}
	inline operator tbColor ()			{return tbColor(x1, y1, x2, y2);}

	// Arithmetische Operatoren
	inline tbVector4 operator + (const tbVector4& v) const					{return tbVector4(x1 + v.x1, y1 + v.y1, x2 + v.x2, y2 + v.y2);}
	inline tbVector4 operator - (const tbVector4& v) const					{return tbVector4(x1 - v.x1, y1 - v.y1, x2 - v.x2, y2 - v.y2);}
	inline tbVector4 operator - () const									{return tbVector4(-x1, -y1, -x2, -y2);}
	inline tbVector4 operator * (const tbVector4& v) const					{return tbVector4(x1 * v.x1, y1 * v.y1, x2 * v.x2, y2 * v.y2);}
	inline tbVector4 operator * (const float f) const						{return tbVector4(x1 * f, y1 * f, x2 * f, y2 * f);}
	inline tbVector4 operator / (const tbVector4& v) const					{return tbVector4(x1 / v.x1, y1 / v.y1, x2 / v.x2, y2 / v.y2);}
	inline tbVector4 operator / (const float f) const						{return tbVector4(x1 / f, y1 / f, x2 / f, y2 / f);}
	inline friend tbVector4 operator * (const float f, const tbVector4& v)	{return tbVector4(v.x1 * f, v.y1 * f, v.x2 * f, v.y2 * f);}

	// Zuweisungsoperatoren
	inline tbVector4 operator = (const tbVector4& v)	{x1 = v.x1; y1 = v.y1; x2 = v.x2; y2 = v.y2; return *this;}
	inline tbVector4 operator += (const tbVector4& v)	{x1 += v.x1; y1 += v.y1; x2 += v.x2; y2 += v.y2; return *this;}
	inline tbVector4 operator -= (const tbVector4& v)	{x1 -= v.x1; y1 -= v.y1; x2 -= v.x2; y2 -= v.y2; return *this;}
	inline tbVector4 operator *= (const tbVector4& v)	{x1 *= v.x1; y1 *= v.y1; x2 *= v.x2; y2 *= v.y2; return *this;}
	inline tbVector4 operator *= (const float f)		{x1 *= f; y1 *= f; x2 *= f; y2 *= f; return *this;}
	inline tbVector4 operator /= (const tbVector4& v)	{x1 /= v.x1; y1 /= v.y1; x2 /= v.x2; y2 /= v.y2; return *this;}
	inline tbVector4 operator /= (const float f)		{x1 /= f; y1 /= f; x2 /= f; y2 /= f; return *this;}

	// Vergleichsoperatoren
	inline bool operator == (const tbVector4& v) const	{if(x1 != v.x1) return false; if(y1 != v.y1) return false; if(x2 != v.x2) return false; return y2 == v.y2;}
	inline bool operator != (const tbVector4& v) const	{if(x1 != v.x1) return true; if(y1 != v.y1) return true; if(x2 != v.x2) return true; return y2 != v.y2;}
};

// ******************************************************************
// Funktionen deklarieren
inline float		tbVector4Length(const tbVector4& v)													{return sqrtf(v.x1 * v.x1 + v.y1 * v.y1 + v.x2 * v.x2 + v.y2 * v.y2);}
inline float		tbVector4LengthSq(const tbVector4& v)												{return v.x1 * v.x1 + v.y1 * v.y1 + v.x2 * v.x2 + v.y2 * v.y2;}
inline tbVector4	tbVector4Min(const tbVector4& v1, const tbVector4& v2)								{return tbVector4(TB_MIN(v1.x1, v2.x1), TB_MIN(v1.y1, v2.y1), TB_MIN(v1.x2, v2.x2), TB_MIN(v1.y2, v2.y2));}
inline tbVector4	tbVector4Max(const tbVector4& v1, const tbVector4& v2)								{return tbVector4(TB_MAX(v1.x1, v2.x1), TB_MAX(v1.y1, v2.y1), TB_MAX(v1.x2, v2.x2), TB_MAX(v1.y2, v2.y2));}
inline tbVector4	tbVector4Random()																	{return tbVector4(tbFloatRandom(-1.0f, 1.0f), tbFloatRandom(-1.0f, 1.0f), tbFloatRandom(-1.0f, 1.0f), tbFloatRandom(-1.0f, 1.0f));}
inline bool			tbVector2In4(const tbVector2& v2, const tbVector4& v4)								{return ((v2.x >= v4.x1) && (v2.y >= v4.y1) && (v2.x <= v4.x2) && (v2.y <= v4.y2));}

// ******************************************************************

#endif __TBVECTOR4__
