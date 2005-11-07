/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVector2.h
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Rechnen mit 2D-Vektoren

	Autor:
	David Scherfgen

********************************************************************/

// Verwendete Symbole vordefinieren
class tbMatrix;

// ******************************************************************
// Die 2D-Vektor-Klasse
class TRIBASE_API tbVector2
{
public:
	// Variablen
	union
	{
		struct
		{
			float x;	// Koordinaten
			float y;
		};

		struct
		{
			float u;	// Texturkoordinaten
			float v;
		};

		float c[2];		// Array der Koordinaten
	};

	// Konstruktoren
	inline tbVector2()																	{}
	inline tbVector2(const float f) : x(f), y(f)										{}
	inline tbVector2(const float _x, const float _y) : x(_x), y(_y)						{}
	inline tbVector2(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1])	{}

	// Casting-Operatoren
	inline operator float* () {return (float*)(c);}

	// Arithmetische Operatoren
	inline tbVector2 operator + (const tbVector2& v) const					{return tbVector2(x + v.x, y + v.y);}
	inline tbVector2 operator - (const tbVector2& v) const					{return tbVector2(x - v.x, y - v.y);}
	inline tbVector2 operator - () const									{return tbVector2(-x, -y);}
	inline tbVector2 operator * (const tbVector2& v) const					{return tbVector2(x * v.x, y * v.y);}
	inline tbVector2 operator * (const float f) const						{return tbVector2(x * f, y * f);}
	inline tbVector2 operator / (const tbVector2& v) const					{return tbVector2(x / v.x, y / v.y);}
	inline tbVector2 operator / (const float f) const						{return tbVector2(x / f, y / f);}
	inline friend tbVector2 operator * (const float f, const tbVector2& v)	{return tbVector2(v.x * f, v.y * f);}
	inline tbVector2 operator % (const tbVector2& v) const					{return tbVector2(float((DWORD)x % (DWORD)v.x), float((DWORD)y % (DWORD)v.x));}
	inline tbVector2 operator % (const float f) const						{return tbVector2(float((DWORD)x % (DWORD)f), float((DWORD)y % (DWORD)f));}

	// Zuweisungsoperatoren
	inline tbVector2 operator = (const tbVector2& v)	{x = v.x; y = v.y; return *this;}
	inline tbVector2 operator += (const tbVector2& v)	{x += v.x; y += v.y; return *this;}
	inline tbVector2 operator -= (const tbVector2& v)	{x -= v.x; y -= v.y; return *this;}
	inline tbVector2 operator *= (const tbVector2& v)	{x *= v.x; y *= v.y; return *this;}
	inline tbVector2 operator *= (const float f)		{x *= f; y *= f; return *this;}
	inline tbVector2 operator /= (const tbVector2& v)	{x /= v.x; y /= v.y; return *this;}
	inline tbVector2 operator /= (const float f)		{x /= f; y /= f; return *this;}
	inline tbVector2 operator %= (const tbVector2& v)	{x = float((DWORD)x % (DWORD)v.x); y = float((DWORD)y % (DWORD)v.x); return *this;}
	inline tbVector2 operator %= (const float f)		{x = float((DWORD)x % (DWORD)f); y = float((DWORD)y % (DWORD)f); return *this;}

	// Vergleichsoperatoren
	inline bool operator == (const tbVector2& v) const	{if(x != v.x) return false; return y == v.y;}
	inline bool operator != (const tbVector2& v) const	{if(x != v.x) return true; return y != v.y;}
};

// ******************************************************************
// Funktionen deklarieren
inline float		tbVector2Length(const tbVector2& v)													{return sqrtf(v.x * v.x + v.y * v.y);}
inline float		tbVector2LengthSq(const tbVector2& v)												{return v.x * v.x + v.y * v.y;}
inline tbVector2	tbVector2Normalize(const tbVector2& v)												{return v / sqrtf(v.x * v.x + v.y * v.y);}
inline tbVector2	tbVector2NormalizeEx(const tbVector2& v)											{return v / (sqrtf(v.x * v.x + v.y * v.y) + 0.0001f);}
inline float		tbVector2Dot(const tbVector2& v1, const tbVector2& v2)								{return v1.x * v2.x + v1.y * v2.y;}
inline float		tbVector2Angle(const tbVector2& v1, const tbVector2& v2)							{return acosf(tbVector2Dot(tbVector2NormalizeEx(v1), tbVector2NormalizeEx(v2)));}
inline tbVector2	tbVector2InterpolateCoords(const tbVector2& v1, const tbVector2& v2, const float p)	{return v1 + p * (v2 - v1);}
inline tbVector2	tbVector2InterpolateNormal(const tbVector2& v1, const tbVector2& v2, const float p)	{return tbVector2Normalize(v1 + p * (v2 - v1));}
inline tbVector2	tbVector2Min(const tbVector2& v1, const tbVector2& v2)								{return tbVector2(TB_MIN(v1.x, v2.x), TB_MIN(v1.y, v2.y));}
inline tbVector2	tbVector2Max(const tbVector2& v1, const tbVector2& v2)								{return tbVector2(TB_MAX(v1.x, v2.x), TB_MAX(v1.y, v2.y));}
inline tbVector2	tbVector2Random()																	{return tbVector2NormalizeEx(tbVector2(tbFloatRandom(-1.0f, 1.0f), tbFloatRandom(-1.0f, 1.0f)));}

// ******************************************************************