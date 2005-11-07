/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbPlane.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Deklaration einer Ebenenklasse und von Ebenenfunktionen

	Autor:
	David Scherfgen

********************************************************************/

// Die Ebenenklasse
class TRIBASE_API tbPlane
{
public:
	// Variablen
	union
	{
		struct
		{
			float a;		// Variablen der Ebenengleichung
			float b;
			float c;
			float d;
		};

		struct
		{
			tbVector3	n;	// Normalvektor
			float		d;	// Distanz
		};

		float v[4];			// Array der Variablen
	};

	// Konstruktoren
	inline tbPlane()																							{};
	inline tbPlane(const float _a, const float _b, const float _c, const float _d) : a(_a), b(_b), c(_c), d(_d)	{};
	inline tbPlane(const float* pfValue) : a(pfValue[0]), b(pfValue[1]), c(pfValue[2]), d(pfValue[3])			{};
	inline tbPlane(const tbVector3& _n, float _d) : n(_n), d(_d)												{};

	// Casting-Operatoren
	inline operator float* () {return (float*)(v);}

	// Zuweisungsoperatoren
	inline tbPlane operator = (const tbPlane& p) {a = p.a; b = p.b; c = p.c; d = p.d; return *this;}

	// Vergleichsoperatoren
	inline bool operator == (const tbPlane& p) const {if(a != p.a) return false; if(b != p.b) return false; if(c != p.c) return false; return d == p.d;}
	inline bool operator != (const tbPlane& p) const {if(a != p.a) return true; if(b != p.b) return true; if(c != p.c) return true; return d != p.d;}
};

// ******************************************************************
// Funktionen deklarieren
inline tbPlane	tbPlaneNormalize(const tbPlane& p)													{float fLength(tbVector3Length(p.n)); return tbPlane(p.n / fLength, p.d / fLength);}
inline float	tbPlaneDotNormal(const tbPlane& p, const tbVector3& v)								{return p.a * v.x + p.b * v.y + p.c * v.z;}
inline float	tbPlaneDotCoords(const tbPlane& p, const tbVector3& v)								{return p.a * v.x + p.b * v.y + p.c * v.z + p.d;}
inline tbPlane	tbPlaneFromPointNormal(const tbVector3& p, const tbVector3& n)						{return tbPlane(n, -n.x * p.x - n.y * p.y - n.z * p.z);}
inline tbPlane	tbPlaneFromPoints(const tbVector3& v1, const tbVector3& v2, const tbVector3& v3)	{return tbPlaneFromPointNormal(v1, tbVector3Cross(v3 - v2, v1 - v2));}

// ******************************************************************