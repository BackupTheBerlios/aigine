/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVector3.h
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Rechnen mit 3D-Vektoren

	Autor:
	David Scherfgen

********************************************************************/

// Verwendete Symbole vordefinieren
class tbMatrix;

// ******************************************************************
// Die 3D-Vektor-Klasse
class TRIBASE_API tbVector3
{
public:
	// Variablen
	union
	{
		struct
		{
			float x;	// Koordinaten
			float y;
			float z;
		};

		struct
		{
			float u;	// Texturkoordinaten
			float v;
			float w;
		};

		float		c[3];		// Array der Koordinaten
		D3DVECTOR	vD3DVector;	// Vektor in Form eines Direct3D-Vektors
	};

	// Konstruktoren
	inline tbVector3()																						{}
	inline tbVector3(const float f)	: x(f), y(f), z(f)														{}
	inline tbVector3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z)					{}
	inline tbVector3(const float* pfComponent) : x(pfComponent[0]), y(pfComponent[1]), z(pfComponent[2])	{}
	inline tbVector3(const D3DVECTOR& v) : vD3DVector(v)													{}

	// Casting-Operatoren
	inline operator float* ()		{return (float*)(c);}
	inline operator D3DVECTOR& ()	{return vD3DVector;}

	// Arithmetische Operatoren
	inline tbVector3 operator + (const tbVector3& v) const					{return tbVector3(x + v.x, y + v.y, z + v.z);}
	inline tbVector3 operator - (const tbVector3& v) const					{return tbVector3(x - v.x, y - v.y, z - v.z);}
	inline tbVector3 operator - () const									{return tbVector3(-x, -y, -z);}
	inline tbVector3 operator * (const tbVector3& v) const					{return tbVector3(x * v.x, y * v.y, z * v.z);}
	inline tbVector3 operator * (const float f) const						{return tbVector3(x * f, y * f, z * f);}
	inline tbVector3 operator / (const tbVector3& v) const					{return tbVector3(x / v.x, y / v.y, z / v.z);}
	inline tbVector3 operator / (const float f) const						{return tbVector3(x / f, y / f, z / f);}
	inline friend tbVector3 operator * (const float f, const tbVector3& v)	{return tbVector3(v.x * f, v.y * f, v.z * f);}

	// Zuweisungsoperatoren
	inline tbVector3 operator = (const tbVector3& v)	{x = v.x; y = v.y; z = v.z; return *this;}
	inline tbVector3 operator += (const tbVector3& v)	{x += v.x; y += v.y; z += v.z; return *this;}
	inline tbVector3 operator -= (const tbVector3& v)	{x -= v.x; y -= v.y; z -= v.z; return *this;}
	inline tbVector3 operator *= (const tbVector3& v)	{x *= v.x; y *= v.y; z *= v.z; return *this;}
	inline tbVector3 operator *= (const float f)		{x *= f; y *= f; z *= f; return *this;}
	inline tbVector3 operator /= (const tbVector3& v)	{x /= v.x; y /= v.y; z /= v.z; return *this;}
	inline tbVector3 operator /= (const float f)		{x /= f; y /= f; z /= f; return *this;}

	// Vergleichsoperatoren
	inline bool operator == (const tbVector3& v) const	{if(x != v.x) return false; if(y != v.y) return false; return z == v.z;}
	inline bool operator != (const tbVector3& v) const	{if(x != v.x) return true; if(y != v.y) return true; return z != v.z;}
};

// ******************************************************************
// Funktionen deklarieren
inline float		tbVector3Length(const tbVector3& v)													{return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);}
inline float		tbVector3LengthSq(const tbVector3& v)												{return v.x * v.x + v.y * v.y + v.z * v.z;}
inline tbVector3	tbVector3Normalize(const tbVector3& v)												{return v / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);}
inline tbVector3	tbVector3NormalizeEx(const tbVector3& v)											{return v / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);}
inline tbVector3	tbVector3Cross(const tbVector3& v1, const tbVector3& v2)							{return tbVector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);}
inline float		tbVector3Dot(const tbVector3& v1, const tbVector3& v2)								{return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;}
inline float		tbVector3Angle(const tbVector3& v1, const tbVector3& v2)							{return acosf(tbVector3Dot(tbVector3NormalizeEx(v1), tbVector3NormalizeEx(v2)));}
inline tbVector3	tbVector3InterpolateCoords(const tbVector3& v1, const tbVector3& v2, const float p)	{return v1 + p * (v2 - v1);}
inline tbVector3	tbVector3InterpolateNormal(const tbVector3& v1, const tbVector3& v2, const float p)	{return tbVector3Normalize(v1 + p * (v2 - v1));}
inline tbVector3	tbVector3Min(const tbVector3& v1, const tbVector3& v2)								{return tbVector3(TB_MIN(v1.x, v2.x), TB_MIN(v1.y, v2.y), TB_MIN(v1.z, v2.z));}
inline tbVector3	tbVector3Max(const tbVector3& v1, const tbVector3& v2)								{return tbVector3(TB_MAX(v1.x, v2.x), TB_MAX(v1.y, v2.y), TB_MAX(v1.z, v2.z));}
inline tbVector3	tbVector3Random()																	{return tbVector3NormalizeEx(tbVector3(tbFloatRandom(-1.0f, 1.0f), tbFloatRandom(-1.0f, 1.0f), tbFloatRandom(-1.0f, 1.0f)));}

// ******************************************************************