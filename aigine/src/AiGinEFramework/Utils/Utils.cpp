
#include "../AiGinE.h"

// ******************************************************************
char* RemoveDir(char* pcFilename)
{
	int iLasackSlash;
	int iChar;


	// Letzten Back-Slash ("\") suchen
	iLasackSlash = -1;
	iChar = 0;
	while(pcFilename[iChar] != 0)
	{
		if(pcFilename[iChar] == '\\') iLasackSlash = iChar;
		iChar++;
	}

	// String ab dem letzten Back-Slash plus 1 zurückliefern
	return pcFilename + iLasackSlash + 1;
}

// ******************************************************************
// Liefert die Erweiterung einer Datei (z.B. "cpp")
char* GetFilenameExtension(char* pcFilename)
{
	int iLastDot;
	int iChar;


	// Letzten Punkt (".") im Dateinamen suchen
	iLastDot = -1;
	iChar = 0;
	while(pcFilename[iChar] != 0)
	{
		if(pcFilename[iChar] == '.') iLastDot = iChar;
		iChar++;
	}

	// String ab dem letzten Punkt plus 1 zurückliefern
	return pcFilename + iLastDot + 1;
}

// ******************************************************************
// Prüft, ob eine Datei existiert
BOOL FileExists(char* pcFilename)
{
	FILE* pFile;

	// Parameter prüfen
	if(pcFilename == NULL) AGE_ERROR_NULL_POINTER("pcFilename", FALSE);


	// Versuchen, die Datei zu öffnen
	pFile = fopen(pcFilename, "r");
	if(pFile != NULL) fclose(pFile);

	// pFile = NULL: Datei existiert nicht (oder Fehler), ansonsten existiert sie.
	return pFile != NULL;
}

// ******************************************************************
// Zufällige Ganzzahl
#if 0
int IntRandom(const int iMin,
							const int iMax)
{
	// Zufallszahl generieren
	return iMin + (rand() % (iMax - iMin + 1));
}

// ******************************************************************
// Zufällige Fließkommazahl
float FloatRandom(const float fMin,
								const float fMax)
{
	// Zufallszahl generieren
	return fMin + (fMax - fMin) * ((float)(rand() % 10001) / 10000.0f);
}
#endif

// ******************************************************************
// 2D-Positionsvektor transformieren
Vector2D Vector2DTransformCoords(const Vector2D& v, const Matrix4x4& m, float* pfOutW) {
	// Vektor mit Matrix transformieren
	Vector2D vResult(v.x * m.m11 + v.y * m.m21 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + m.m42);

	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	float w = v.x * m.m14 + v.y * m.m24 + m.m44;
	if(w != 1.0f) vResult /= w;

	// Wenn erwünscht, w kopieren
	if(pfOutW != NULL) *pfOutW = w;

	return Vector2D::Normalize(vResult) * Vector2D::Length(v);
}

// ******************************************************************
// 2D-Richtungsvektor transformieren
Vector2D Vector2DTransformNormal(const Vector2D& v,
											   const Matrix4x4& m)
{
	// Vektorlänge berechnen
	float fLength = Vector2D::Length(v);
	if(fLength == 0.0f) return v;

	// Transponierte inverse Matrix berechnen
	Matrix4x4 mTransform(Matrix4x4::Transpose(Matrix4x4::Invert(m)));

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return Vector2D::Normalize(Vector2D(v.x * mTransform.m11 + v.y * mTransform.m21,
		                                v.x * mTransform.m12 + v.y * mTransform.m22))
	       * fLength;
}

// ******************************************************************
// 2D-Richtungsvektor transformieren
Vector2D Vector2DTransformNormal_TranspInv(const Vector2D& v,
														 const Matrix4x4& m)
{
	// Vektorlänge berechnen
	float fLength = Vector2D::Length(v);
	if(fLength == 0.0f) return v;

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return Vector2D::Normalize(Vector2D(v.x * m.m11 + v.y * m.m21,
		                                v.x * m.m12 + v.y * m.m22))
	       * fLength;
}

// ******************************************************************
// 3D-Positionsvektor transformieren
Vector3D Vector3DTransformCoords(const Vector3D& v,
											   const Matrix4x4& m,
											   float* pfOutW) // = NULL
{
	// Vektor mit Matrix multiplizieren
	Vector3D vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + m.m42,
					  v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + m.m43);

	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	float w = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + m.m44;
	if(w != 1.0f) vResult /= w;

	// Wenn erwünscht, w kopieren
	if(pfOutW != NULL) *pfOutW = w;

	return vResult;
}

// ******************************************************************
// 3D-Richtungsvektor transformieren
Vector3D Vector3DTransformNormal(const Vector3D& v,
											   const Matrix4x4& m)
{
	// Vektorlänge berechnen
	float fLength = Vector3D::Length(v);
	if(fLength == 0.0f) return v;

	// Transponierte invertierte Matrix berechnen
	Matrix4x4 mTransform(Matrix4x4::Transpose(Matrix4x4::Invert(m)));

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return Vector3D::Normalize(Vector3D(v.x * mTransform.m11 + v.y * mTransform.m21 + v.z * mTransform.m31,
		                                v.x * mTransform.m12 + v.y * mTransform.m22 + v.z * mTransform.m32,
										v.x * mTransform.m13 + v.y * mTransform.m23 + v.z * mTransform.m33))
		   * fLength;
}

// ******************************************************************
// 3D-Richtungsvektor transformieren
Vector3D Vector3DTransformNormal_TranspInv(const Vector3D& v,
														 const Matrix4x4& m)
{
	// Vektorlänge berechnen
	float fLength = Vector3D::Length(v);
	if(fLength == 0.0f) return v;

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return Vector3D::Normalize(Vector3D(v.x * m.m11 + v.y * m.m21 + v.z * m.m31,
		                                v.x * m.m12 + v.y * m.m22 + v.z * m.m32,
										v.x * m.m13 + v.y * m.m23 + v.z * m.m33))
		   * fLength;
}

// ******************************************************************
// Projektion eines 3D-Positionsvektors
Vector3D Vector3DProject(const Vector3D& v,
									   const Matrix4x4& mWorldViewProj)
{
	Vector3D vTemp(Vector3DTransformCoords(v, mWorldViewProj));

	return Vector3D(Direct3D::GetScreenSize().x * 0.5f + (vTemp.x * Direct3D::GetScreenSize().x * 0.5f),
					 Direct3D::GetScreenSize().y * 0.5f - (vTemp.y * Direct3D::GetScreenSize().y * 0.5f),
					 vTemp.z);
}

// ******************************************************************
// Ebene mit einer Matrix transformieren
Plane PlaneTransform(const Plane& p,
									 const Matrix4x4& m)
{
	float a = p.a * m.m11 + p.b * m.m21 + p.c * m.m31;
	float b = p.a * m.m12 + p.b * m.m22 + p.c * m.m32;
	float c = p.a * m.m13 + p.b * m.m23 + p.c * m.m33;

	return Plane(a, b, c,
				   p.d - (a * m.m41 + b * m.m42 + c * m.m43));
}

// ******************************************************************
// Normalvektor eines Dreiecks berechnen
Vector3D ComputeTriangleNormal(const Vector3D& v1,
											  const Vector3D& v2,
											  const Vector3D& v3)
{
	// Normalisiertes Kreuzprodukt der Verbindungsvektoren berechnen
	return Vector3D::Normalize(Vector3D::Cross(v2 - v1, v3 - v1));
}

// ******************************************************************
// Berechnen der sechs Ebenen eines frei drehbaren Quaders
Result ComputeBoxPlanes(const Vector3D& vBoxMin,
										const Vector3D& vBoxMax,
										const Matrix4x4& mBoxTransformation,
										Plane* pOut)
{
	// Die sechs Ebenen berechnen, ohne die Drehung zu berücksichtigen
	pOut[0] = Plane::FromPointNormal(vBoxMin, Vector3D(-1.0f,  0.0f,  0.0f));	// Linke Ebene
	pOut[1] = Plane::FromPointNormal(vBoxMax, Vector3D( 1.0f,  0.0f,  0.0f));	// Rechte Ebene
	pOut[2] = Plane::FromPointNormal(vBoxMax, Vector3D( 0.0f,  1.0f,  0.0f));	// Obere Ebene
	pOut[3] = Plane::FromPointNormal(vBoxMin, Vector3D( 0.0f, -1.0f,  0.0f));	// Untere Ebene
	pOut[4] = Plane::FromPointNormal(vBoxMin, Vector3D( 0.0f,  0.0f, -1.0f));	// Vordere Ebene
	pOut[5] = Plane::FromPointNormal(vBoxMax, Vector3D( 0.0f,  0.0f,  1.0f));	// Hintere Ebene

	// Alle Ebenen transformieren
	for(int iPlane = 0; iPlane < 6; iPlane++)
	{
		// Transformieren
		pOut[iPlane] = PlaneTransform(pOut[iPlane], mBoxTransformation);
	}

	return AGE_OK;
}

// ******************************************************************
// Berechnen des View-Frustums anhand der Projektionsmatrix und der Sichtmatrix
Result ComputeClipPlanes(const Matrix4x4& mView,
										 const Matrix4x4& mProjection,
										 Plane* pOut)
{
	// Sichtmatrix mit der Projektionsmatrix multiplizieren
	Matrix4x4 mTemp(mView * mProjection);

	// Linke Clippingebene
	pOut[0].a = -(mTemp.m14 + mTemp.m11);
	pOut[0].b = -(mTemp.m24 + mTemp.m21);
	pOut[0].c = -(mTemp.m34 + mTemp.m31);
	pOut[0].d = -(mTemp.m44 + mTemp.m41);
	pOut[0] = Plane::Normalize(pOut[0]);

	// Rechte Clippingebene
	pOut[1].a = -(mTemp.m14 - mTemp.m11);
	pOut[1].b = -(mTemp.m24 - mTemp.m21);
	pOut[1].c = -(mTemp.m34 - mTemp.m31);
	pOut[1].d = -(mTemp.m44 - mTemp.m41);
	pOut[1] = Plane::Normalize(pOut[1]);

	// Obere Clippingebene
	pOut[2].a = -(mTemp.m14 - mTemp.m12);
	pOut[2].b = -(mTemp.m24 - mTemp.m22);
	pOut[2].c = -(mTemp.m34 - mTemp.m32);
	pOut[2].d = -(mTemp.m44 - mTemp.m42);
	pOut[2] = Plane::Normalize(pOut[2]);

	// Untere Clippingebene
	pOut[3].a = -(mTemp.m14 + mTemp.m12);
	pOut[3].b = -(mTemp.m24 + mTemp.m22);
	pOut[3].c = -(mTemp.m34 + mTemp.m32);
	pOut[3].d = -(mTemp.m44 + mTemp.m42);
	pOut[3] = Plane::Normalize(pOut[3]);

	// Nahe Clippingebene
	pOut[4].a = -(mTemp.m14 + mTemp.m13);
	pOut[4].b = -(mTemp.m24 + mTemp.m23);
	pOut[4].c = -(mTemp.m34 + mTemp.m33);
	pOut[4].d = -(mTemp.m44 + mTemp.m43);
	pOut[4] = Plane::Normalize(pOut[4]);

	// Ferne Clippingebene
	pOut[5].a = -(mTemp.m14 - mTemp.m13);
	pOut[5].b = -(mTemp.m24 - mTemp.m23);
	pOut[5].c = -(mTemp.m34 - mTemp.m33);
	pOut[5].d = -(mTemp.m44 - mTemp.m43);
	pOut[5] = Plane::Normalize(pOut[5]);

	return AGE_OK;
}

// ******************************************************************
// Minimale Entfernung zwischen Punkt und Ebene berechnen
float PointPlaneDistance(const Vector3D& vPoint,
									   const Plane& Plane)
{
	// Die Ebene normalisieren und dann den Punkt einsetzen
	return Plane::DotCoords(Plane::Normalize(Plane), vPoint);
}

// ******************************************************************
// Minimale Entfernung zwischen Punkt und Ebene berechnen (schnell)
float PointPlaneDistanceFast(const Vector3D& vPoint,
										   const Plane& Plane)
{
	// Den Punkt in die Gleichung einsetzen
	return Plane.a * vPoint.x + Plane.b * vPoint.y + Plane.c * vPoint.z + Plane.d;
}

// ******************************************************************
// Minimale Distanz zwischen Punkt und Linie berechnen
float PointLineDistance(const Vector3D& vPoint,
									  const Vector3D& vLineA,
									  const Vector3D& vLineB)
{
	Vector3D	v(vLineB - vLineA);
	Vector3D	w(vPoint - vLineA);


	return Vector3D::Length(vLineA + v * (Vector3D::Dot(w, v) / Vector3D::LengthSq(v)) - vPoint);
}

// ******************************************************************
// Minimale Distanz zwischen zwei Linien berechnen
float LineLineDistance(const Vector3D& vLine1A,
									 const Vector3D& vLine1B,
									 const Vector3D& vLine2A,
									 const Vector3D& vLine2B)
{
	Vector3D	u(vLine1B - vLine1A);
    Vector3D	v(vLine2B - vLine1A);
	Vector3D	w(vLine1A - vLine2A);
	float		a = Vector3D::LengthSq(u);
	float		c = Vector3D::LengthSq(v);
	float		b = Vector3D::Dot(u, v);
	float		d = Vector3D::Dot(u, w);
	float		e = Vector3D::Dot(v, w);
	float		D = a * c - b * b;


	if(D < 0.0001f)
	{
		float sc = 0.0f;
		float tc = b > c ? d / b : e / c;

		// Verbindungsvektor zwischen den zwei am nächsten liegenden
		// Punkten auf den Linien berechnen (L1(sc) - L2(tc))
		// Die Länge dieses Vektors ist die minimale Distanz.
		return Vector3D::Length(w + (u * sc) - (v * tc));
	}
	else
	{
		float sc = (b * e - c * d) / D;
		float tc = (a * e - b * d) / D;

		// Verbindungsvektor zwischen den zwei am nächsten liegenden
		// Punkten auf den Linien berechnen (L1(sc) - L2(tc))
		// Die Länge dieses Vektors ist die minimale Distanz.
		return Vector3D::Length(w + (u * sc) - (v * tc));
	}
}

// ******************************************************************
// Liegt ein Punkt innerhalb einer Kugel?
BOOL PointHitsSphere(const Vector3D& vPoint,
								   const Vector3D& vSphere,
								   const float fRadius)
{
	// Wenn die Distanz zwischen Punkt und dem Mittelpunkt der
	// Kugel kleiner gleich dem Radius ist, liegt der Punkt in
	// der Kugel.
	return Vector3D::LengthSq(vPoint - vSphere) <= fRadius * fRadius;
}

// ******************************************************************
// Liegt ein Punkt in einem achsenausgerichteten Quader?
BOOL PointHitsAABox(const Vector3D& vPoint,
								  const Vector3D& vBoxMin,
								  const Vector3D& vBoxMax)
{
	return vPoint.x >= vBoxMin.x && vPoint.x <= vBoxMax.x &&
		   vPoint.y >= vBoxMin.y && vPoint.y <= vBoxMax.y &&
		   vPoint.z >= vBoxMin.z && vPoint.z <= vBoxMax.z;
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
BOOL PointHitsBox(const Vector3D& vPoint,
								const Plane* pBoxPlanes)
{
	// Sobald der Punkt auf der Vorderseite auch nur einer einzigen der sechs
	// Ebenen des Quaders liegt, liegt er nicht mehr innerhalb des Quaders.
	// Es gibt eine kleine Toleranzgrenze.
	if(Plane::DotCoords(pBoxPlanes[0], vPoint) > 0.0001f) return FALSE;
	if(Plane::DotCoords(pBoxPlanes[1], vPoint) > 0.0001f) return FALSE;
	if(Plane::DotCoords(pBoxPlanes[2], vPoint) > 0.0001f) return FALSE;
	if(Plane::DotCoords(pBoxPlanes[3], vPoint) > 0.0001f) return FALSE;
	if(Plane::DotCoords(pBoxPlanes[4], vPoint) > 0.0001f) return FALSE;
	return Plane::DotCoords(pBoxPlanes[5], vPoint) <= 0.0001f;
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
BOOL PointHitsBox(const Vector3D& vPoint,
								const Vector3D& vBoxMin,
								const Vector3D& vBoxMax,
								const Matrix4x4& mBoxTransformation)
{
	Plane aBoxPlane[6];

	// Die sechs Ebenen berechnen
	ComputeBoxPlanes(vBoxMin, vBoxMax, mBoxTransformation, aBoxPlane);

	// Die andere Funktion aufrufen
	return PointHitsBox(vPoint, aBoxPlane);
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
// Diese Funktion verwendet die invertierte Matrix, um das herauszufinden.
BOOL PointHitsBox2(const Vector3D& vPoint,
								 const Vector3D& vBoxMin,
								 const Vector3D& vBoxMax,
								 const Matrix4x4& mBoxTransformation)
{
	// Punkt mit der invertierten Matrix transformieren
	Vector3D vNewPoint(Vector3DTransformCoords(vPoint, Matrix4x4::Invert(mBoxTransformation)));

	// Liegt der Punkt im Quader?
	if(vNewPoint.x < vBoxMin.x) return FALSE;
	if(vNewPoint.y < vBoxMin.y) return FALSE;
	if(vNewPoint.z < vBoxMin.z) return FALSE;
	if(vNewPoint.x > vBoxMax.x) return FALSE;
	if(vNewPoint.y > vBoxMax.y) return FALSE;
	if(vNewPoint.z > vBoxMax.z) return FALSE;

	// Der Punkt liegt im Quader!
	return TRUE;
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
// Diese Funktion verwendet die invertierte Matrix, um das herauszufinden.
BOOL PointHitsBox2_Inv(const Vector3D& vPoint,
									 const Vector3D& vBoxMin,
									 const Vector3D& vBoxMax,
									 const Matrix4x4& mInvBoxTransformation)
{
	// Punkt mit der invertierten Matrix transformieren
	Vector3D vNewPoint(Vector3DTransformCoords(vPoint, mInvBoxTransformation));

	// Liegt der Punkt im Quader?
	if(vNewPoint.x < vBoxMin.x) return FALSE;
	if(vNewPoint.y < vBoxMin.y) return FALSE;
	if(vNewPoint.z < vBoxMin.z) return FALSE;
	if(vNewPoint.x > vBoxMax.x) return FALSE;
	if(vNewPoint.y > vBoxMax.y) return FALSE;
	if(vNewPoint.z > vBoxMax.z) return FALSE;

	// Der Punkt liegt im Quader!
	return TRUE;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einer Ebene berechnen
BOOL LineHitsPlane(const Vector3D& vLineA,
								 const Vector3D& vLineB,
								 const Plane& Plane,
								 Vector3D* pvOut) // = NULL
{
	// Richtungsvektor der Linie berechnen
	Vector3D vLineDir(vLineB - vLineA);

	// Nenner des Bruchs für s berechnen. Falls dieser gleich null ist, sind Linie
	// und Ebene parallel und es gibt entweder keinen oder unendlich viele Schnittpunkte.
	float fDenominator = Plane.a * vLineDir.x + Plane.b * vLineDir.y + Plane.c * vLineDir.z;

	if(fabsf(fDenominator) < 0.0001f)
	{
		// Liegt der Startpunkt der Strecke auf der Ebene? Wenn ja, dann liegt die
		// gesamte Strecke auf ihr. Als Kollisionsort nehmen wir dann den Startpunkt.
		if(fabsf(vLineA.x * Plane.a + vLineA.y * Plane.b + vLineA.z * Plane.c + Plane.d) < 0.0001f)
		{
			// Strecke liegt komplett auf der Ebene!
			if(pvOut != NULL) *pvOut = vLineA;
			return TRUE;
		}
		else
		{
			// Keine Kollision!
			return FALSE;
		}
	}

	// Streckenabschnitt des Schnittpunkts des Strahls berechnen
	float s = (Plane.a * vLineA.x + Plane.b * vLineA.y + Plane.c * vLineA.z + Plane.d) / -fDenominator;

	// Liegt der Streckenabschnitt außerhalb von [0; 1], dann gibt es keinen Schnittpunkt,
	// da der Punkt außerhalb der Linie liegt.
	if(s < 0.0f) return FALSE;
	if(s > 1.0f) return FALSE;

	// Es gibt einen Schnittpunkt!
	if(pvOut != NULL) *pvOut = vLineA + (s * vLineDir);
	
	return TRUE;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einer Ebene berechnen (Linienabschnitt)
float LineHitsPlaneS(const Vector3D& vLineA,
								   const Vector3D& vLineB,
								   const Plane& Plane)
{
	// Richtungsvektor der Linie berechnen
	Vector3D vLineDir(vLineB - vLineA);

	// Nenner des Bruchs für s berechnen. Falls dieser gleich null ist, sind Linie
	// und Ebene parallel und es gibt entweder keinen oder unendlich viele Schnittpunkte.
	float fDenominator = Plane.a * vLineDir.x + Plane.b * vLineDir.y + Plane.c * vLineDir.z;
	if(fabsf(fDenominator) < 0.0001f) return -1.0f;

	// Streckenabschnitt des Schnittpunkts des Strahls berechnen
	float s = (-Plane.a * vLineA.x - Plane.b * vLineA.y - Plane.c * vLineA.z - Plane.d) / fDenominator;

	// Liegt der Streckenabschnitt außerhalb von [0; 1], dann gibt es keinen Schnittpunkt,
	// da der Punkt außerhalb der Linie liegt.
	if(s < 0.0f) return -1.0f;
	if(s > 1.0f) return -1.0f;

	// Es gibt einen Schnittpunkt!
	return s;
}

// ******************************************************************
// Kollision zwischen einer Linie und einer Ebene berechnen (schnell)
BOOL LineHitsPlaneFast(const Vector3D& vLineA,
									 const Vector3D& vLineB,
									 const Plane& Plane)
{
	// Es wird kein exakter Schnittpunkt verlangt - daher kann eine schnellere
	// Methode angewandt werden.

	// Punktprodukte zwischen den Linienpunkten und der Ebene bilden
	float d1 = Plane::DotCoords(Plane, vLineA);
	float d2 = Plane::DotCoords(Plane, vLineB);

	// Liegen die beiden Linienpunkte auf verschiedenen Seiten oder beide
	// direkt auf der Ebene? Falls ja, gibt es einen Schnittpunkt.
	if(d1 <= 0.0f) if(d2 >= 0.0f) return TRUE;
	if(d1 >= 0.0f) if(d2 <= 0.0f) return TRUE;

	// Kein Schnittpunkt!
	return FALSE;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einer Ebene berechnen (schnell)
Vector3D LineHitsPlaneFastEx(const Vector3D& vLineA,
											const Vector3D& vLineB,
											const Plane& Plane)
{
	// Richtungsvektor der Linie berechnen
	Vector3D vLineDir(vLineB - vLineA);

	// Schnittpunkt liefern
	return   vLineA
	       + (  (-Plane.a * vLineA.x - Plane.b * vLineA.y - Plane.c * vLineA.z - Plane.d)
		      / (Plane.a * vLineDir.x + Plane.b * vLineDir.y + Plane.c * vLineDir.z))
		   * vLineDir;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen
BOOL LineHitsTriangle(const Vector3D& vLineA,
									const Vector3D& vLineB,
									const Vector3D& vTriangleA,
									const Vector3D& vTriangleB,
									const Vector3D& vTriangleC,
									Vector3D* pvOut) // = NULL
{
	// Die Ebene dieses Dreiecks berechnen
	Plane Plane(Plane::FromPoints(vTriangleA, vTriangleB, vTriangleC));

	// Schnittpunkt der Linie mit der Ebene bestimmen
	Vector3D vIntersection;
	if(!LineHitsPlane(vLineA, vLineB, Plane, &vIntersection))
	{
		// Sie schneidet die Ebene nicht - dann kann sie das Dreieck
		// erst recht nicht schneiden.
		return FALSE;
	}

	// Erstellen der ersten Ebene entlang den Punkten A und B und
	// Einsetzen des Punkts in die Gleichung. Falls das Ergebnis kleiner
	// als null ist, liegt der Punkt hinter der Ebene und damit nicht im
	// Dreieck.
	Vector3D vTemp(Vector3D::Cross(vTriangleA - vTriangleB, Plane.n));
	Plane = Plane::FromPointNormal(vTriangleA, vTemp);
	if(Plane::DotCoords(Plane, vIntersection) < 0.0f) return FALSE;

	// Test mit der zweiten Ebene entlang den Punkten B und C
	vTemp = Vector3D::Cross(vTriangleB - vTriangleC, Plane.n);
	Plane = Plane::FromPointNormal(vTriangleB, vTemp);
	if(Plane::DotCoords(Plane, vIntersection) < 0.0f) return FALSE;

	// Test mit der dritten Ebene entlang den Punkten C und A
	vTemp = Vector3D::Cross(vTriangleC - vTriangleA, Plane.n);
	Plane = Plane::FromPointNormal(vTriangleC, vTemp);
	if(Plane::DotCoords(Plane, vIntersection) < 0.0f) return FALSE;

	// Der Punkt liegt im Dreieck, da er vor allen drei Ebenen liegt!
	// Schnittpunkt einsetzen.
	if(pvOut != NULL) *pvOut = vIntersection;

	return TRUE;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen (Linienabschnitt)
float LineHitsTriangleS(const Vector3D& vLineA,
									  const Vector3D& vLineB,
									  const Vector3D& vTriangleA,
									  const Vector3D& vTriangleB,
									  const Vector3D& vTriangleC)
{
	// Die Ebene dieses Dreiecks berechnen
	Plane Plane(Plane::FromPoints(vTriangleA, vTriangleB, vTriangleC));

	// Schneidet die Linie die Ebene nicht?
	float fIntersection = LineHitsPlaneS(vLineA, vLineB, Plane);
	if(fIntersection < 0.0f)
	{
		// Sie schneidet die Ebene nicht - dann kann sie das Dreieck
		// erst recht nicht schneiden.
		return -1.0f;
	}

	// Schnittpunkt berechnen
	Vector3D vIntersection(vLineA + (vLineB - vLineA) * fIntersection);

	// Erstellen der ersten Ebene entlang den Punkten A und B und
	// Einsetzen des Punkts in die Gleichung. Falls das Ergebnis kleiner
	// als null ist, liegt der Punkt hinter der Ebene und damit nicht im
	// Dreieck.
	Vector3D vTemp(Vector3D::Cross(vTriangleA - vTriangleB, Plane.n));
	Plane = Plane::FromPointNormal(vTriangleA, vTemp);
	if(Plane::DotCoords(Plane, vIntersection) < 0.0f) return -1.0f;

	// Test mit der zweiten Ebene entlang den Punkten B und C
	vTemp = Vector3D::Cross(vTriangleB - vTriangleC, Plane.n);
	Plane = Plane::FromPointNormal(vTriangleA, vTemp);
	if(Plane::DotCoords(Plane, vIntersection) < 0.0f) return -1.0f;

	// Test mit der dritten Ebene entlang den Punkten C und A
	vTemp = Vector3D::Cross(vTriangleC - vTriangleA, Plane.n);
	Plane = Plane::FromPointNormal(vTriangleA, vTemp);
	if(Plane::DotCoords(Plane, vIntersection) < 0.0f) return -1.0f;

	// Der Punkt liegt im Dreieck, da er vor allen drei Ebenen liegt!
	// Schnittstelle liefern.
	return fIntersection;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einer Kugel berechnen
BOOL LineHitsSphere(const Vector3D& vLineA,
								  const Vector3D& vLineB,
								  const Vector3D& vSphere,
								  const float fRadius,
								  Vector3D* pvOut) // = NULL
{
	// Test: liegt Punkt A in der Kugel?
	if(PointHitsSphere(vLineA, vSphere, fRadius)) {if(pvOut != NULL) *pvOut = vLineA; return TRUE;}

	// Länge und normalisierte Richtung der Linie berechnen
	float fLineLength = Vector3D::Length(vLineB - vLineA);
	Vector3D vLineDir((vLineB - vLineA) / fLineLength);

	// Die zwei Hilfsvariablen a und b berechnen
	Vector3D vDiff(vLineA - vSphere);
	float a = 2.0f * (vLineDir.x * vDiff.x + vLineDir.y * vDiff.y + vLineDir.z * vDiff.z);
	float b = Vector3D::LengthSq(vDiff) - fRadius * fRadius;

	// Die Diskriminante berechnen (a²/4 - b)
	float fDiscriminant = ((a * a) * 0.25f) - b;

	// Wenn die Diskriminante kleiner als null ist, gibt es keine Schnittpunkte.
	if(fDiscriminant < 0.0f) return FALSE;

	// Die Wurzel und danach die zweite Lösung der Gleichung berechnen
	float fRoot = sqrtf(fDiscriminant);
	float s = a * -0.5f - fRoot;

	// Ist dieser Schnittpunkt in Ordnung?
	if(s >= 0.0f)
	{
		if(s <= fLineLength)
		{
			// Den Schnittpunkt berechnen und die Funktion verlassen
			if(pvOut != NULL) *pvOut = vLineA + (s * vLineDir);
			return TRUE;
		}
	}

	// Es kann keinen Schnittpunkt mehr geben!
	return FALSE;
}

// ******************************************************************
// Schneidet eine Linie eine Kugel? (schnell)
BOOL LineHitsSphereFast(const Vector3D& vLineA,
									  const Vector3D& vLineB,
									  const Vector3D& vSphere,
									  const float fRadius)
{
	// Minimale Distanz zwischen der Linie und dem Mittelpunkt
	// der Kugel berechnen (den Abschnitt auf der Linie)
	Vector3D v(vLineB - vLineA);
	Vector3D w(vSphere - vLineA);
	float s = Vector3D::Dot(w, v) / Vector3D::LengthSq(v);

	// Wenn der Punkt s der Linie innerhalb der Kugel liegt,
	// schneidet die Linie die Kugel.
	return Vector3D::LengthSq(vLineA + v * s - vSphere) < fRadius * fRadius;
}

// ******************************************************************
// Schnittpunkt zwischen Linie und frei drehbarem Quader berechnen
BOOL LineHitsBox(const Vector3D& vLineA,
							   const Vector3D& vLineB,
							   const Plane* pBoxPlanes,
							   Vector3D* pvOut) // = NULL
{
	// Wenn der Linienstartpunkt im Quader liegt, setzen wir
	// den Schnittpunkt einfach auf den Linienstartpunkt.
	if(PointHitsBox(vLineA, pBoxPlanes))
	{
		if(pvOut != NULL) *pvOut = vLineA;
		return TRUE;
	}

	// Linienrichtung berechnen
	Vector3D vLineDir(vLineB - vLineA);

	float		fNearestIntersection = 100.0f;
	float		fIntersection;
	Vector3D	vIntersection;			// Schnittpunktkoordinaten
	Vector3D	vNearestIntersection;	// Koordinaten des nächsten Schnittpunkts

	// Jede Ebene durchgehen und prüfen, ob sie von der Linie geschnitten wird.
	// Es wird der nächste Schnittpunkt (niedrigster Linienabschnitt) gesucht.
	for(int iPlane = 0; iPlane < 6; iPlane++)
	{
		fIntersection = LineHitsPlaneS(vLineA, vLineB, pBoxPlanes[iPlane]);
		if(fIntersection >= 0.0f)
		{
			// Die Ebene wurde von der Linie geschnitten!
			// Wenn der Linienabschnitt des Schnittpunkts kleiner ist als
			// der aktuelle Minimumwert, dann rechnen wir weiter. Ansonsten
			// lassen wir es sein, da dieser Schnittpunkt so wie so nicht
			// der jenige mit der kleinsten Entfernung zum Linienstartpunkt
			// sein kann.
			if(fIntersection < fNearestIntersection)
			{
				// Dieser Schnittpunkt ist ein guter Kandidat!
				// Nun prüfen wir, ob der Schnittpunkt im Quader liegt.
				vIntersection = vLineA + vLineDir * fIntersection;
				if(PointHitsBox(vIntersection, pBoxPlanes))
				{
					// Aha! Der Schnittpunkt liegt im Quader.
					// Damit haben wir einen neuen nächsten Schnittpunkt gefunden.
					// Wir speichern seine Koordinaten und seinen Linienabschnitt.
					vNearestIntersection = vIntersection;
					fNearestIntersection = fIntersection;
				}
			}
		}
	}

	// Wenn der nächste Schnittpunkt immer noch 100 ist (so wurde er am Anfang
	// gesetzt), dann gab es keine Kollision.
	if(fNearestIntersection == 100.0f) return FALSE;

	// Ansonsten liefern wir nun die Koordinaten des am nächsten liegenden
	// Schnittpunkts zurück.
	if(pvOut != NULL) *pvOut = vNearestIntersection;

	return TRUE;
}

// ******************************************************************
// Schnittpunkt zwischen Linie und frei drehbarem Quader berechnen
BOOL LineHitsBox(const Vector3D& vLineA,
							   const Vector3D& vLineB,
							   const Vector3D& vBoxMin,
							   const Vector3D& vBoxMax,
							   const Matrix4x4& mBoxTransformation,
							   Vector3D* pvOut) // = NULL
{
	Plane aBoxPlane[6];

	// Die sechs Ebenen des Quaders berechnen
	ComputeBoxPlanes(vBoxMin, vBoxMax, mBoxTransformation, aBoxPlane);

	// Die andere Funktion aufrufen
	return LineHitsBox(vLineA, vLineB, aBoxPlane, pvOut);
}

// ******************************************************************
// Berechnung der Kollision zwischen Linie und Modell (rekursiv)
BOOL LineHitsModelRec(const Vector3D& vLineA,
									const Vector3D& vLineB,
									const Model* pModel,
									const ModelOctreeNode* pNode,
									int* piClosestTriangle,
									float* pfClosestCollision,
									BOOL bSimpleMode,
									BOOL* pbStop)
{
	// Prüfen, ob die Linie den Umgebungsquader des Knotens trifft.
	// Falls nicht, können wir sofort abbrechen
	if(!LineHitsBox(vLineA, vLineB,
		              pNode->vBoundingBoxMin,
					  pNode->vBoundingBoxMax,
					  Matrix4x4::Identity()))
	{
		// Abbruch!
		return FALSE;
	}

	// Ist dieser Knoten ein Endknoten? Falls ja, dann testen wir die
	// Kollision jetzt auf Dreiecksebene.
	if(pNode->bIsLeaf)
	{
		float		fCollision;	// Linienabschnitt des Schnittpunkts
		Vector3D	vCollision;	// Position des Schnittpunkts

		// Nun jedes Dreieck dieses Knotens durchgehen und
		// nach der nächsten Kollisionen suchen
		for(DWORD t = 0; t < pNode->dwNumTriangles; t++)
		{
			// Schneidet die Linie das Dreieck?
			// Dazu berechnen wir zuerst den Schnittpunkt zwischen der Linie und der
			// ersten Ebene des Dreiecks.
			fCollision = LineHitsPlaneS(vLineA, vLineB, pModel->m_pTrianglePlanes[pNode->pdwTriangles[t] * 4]);
			if(fCollision >= 0.0f)
			{
				// Die erste Ebene wird geschnitten.
				// Der Schnittpunkt muss nun auf der Vorderseite der drei anderen
				// Ebenen des Dreiecks liegen, damit eine Kollision vorliegt.
				vCollision = vLineA + (vLineB - vLineA) * fCollision;
				if(Plane::DotCoords(pModel->m_pTrianglePlanes[pNode->pdwTriangles[t] * 4 + 1], vCollision) < 0.0f) continue;
				if(Plane::DotCoords(pModel->m_pTrianglePlanes[pNode->pdwTriangles[t] * 4 + 2], vCollision) < 0.0f) continue;
				if(Plane::DotCoords(pModel->m_pTrianglePlanes[pNode->pdwTriangles[t] * 4 + 3], vCollision) < 0.0f) continue;

				// Kollision! Bricht sie den Rekord?
				if(fCollision < *pfClosestCollision)
				{
					*pfClosestCollision = fCollision;
					*piClosestTriangle = (int)(pNode->pdwTriangles[t]);
				}

				// Wenn der Benutzer den Ort der Kollision nicht kennen möchte
				// (bSimpleMode = TRUE), dann können wir jetzt schon abbrechen.
				if(bSimpleMode)
				{
					*pbStop = TRUE;
					return TRUE;
				}
			}
		}
	}
	else
	{
		// Dieser Knoten ist kein Endknoten.
		// Wir gehen seine Unterknoten durch und testen diese.
		for(DWORD i = 0; i < 8; i++)
		{
			LineHitsModelRec(vLineA, vLineB, pModel, pNode->apChild[i],
					           piClosestTriangle, pfClosestCollision, bSimpleMode,
							   pbStop);

			// Wurde die pbStop-Variable auf TRUE gesetzt?
			// Falls ja, dann brechen wir ab und liefern TRUE zurück.
			if(*pbStop) return TRUE;
		}

		// Wenn es eine Kollision gab, wird TRUE geliefert.
		if(*piClosestTriangle != -1) return TRUE;
	}

	// Keine Kollision!
	return FALSE;
}

// ******************************************************************
// Berechnung der Kollision zwischen Linie und Modell
BOOL LineHitsModel(const Vector3D& vLineA,
								 const Vector3D& vLineB,
								 const Model* pModel,
								 const Matrix4x4& mMatrix,
								 const Matrix4x4& mInvMatrix,
								 const float fTolerance,	// = 0.0f
								 Vector3D* pvOutPos,		// = NULL
								 Vector3D* pvOutNormal,	// = NULL
								 int* piOutTriangle)		// = NULL
{
	// Parameter prüfen
	if(pModel == NULL)			AGE_ERROR_NULL_POINTER("pModel", ERROR);
	if(!pModel->m_bExtraData)	ERROR("Das Modell hat keine Extradaten!", ERROR);


	// Startwerte setzen
	int iClosestTriangle = -1;
	float fClosestCollision = 100000.0f;
	BOOL bStop = FALSE;

	// Erst einmal transformieren wir die Linie mit der inversen Transformationsmatrix,
	// um sie in "Model-Space" umzurechnen. Dann müssen wir später die Modellvertizes
	// nicht transformieren, sparen also eine Menge Rechenzeit.
	Vector3D vNewLineA(Vector3DTransformCoords(vLineA, mInvMatrix));
	Vector3D vNewLineB(Vector3DTransformCoords(vLineB, mInvMatrix));

	// Start und Ende der Linie ein wenig auseinander schieben - je nach Toleranz
	vNewLineA -= Vector3D::NormalizeEx(vNewLineB - vNewLineA) * fTolerance;
	vNewLineB += Vector3D::NormalizeEx(vNewLineB - vNewLineA) * fTolerance;

	// Rekursive Funktion aufrufen
	LineHitsModelRec(vNewLineA, vNewLineB, pModel, pModel->m_pRootNode,
	                   &iClosestTriangle, &fClosestCollision,
					   pvOutPos == NULL, &bStop);

	// Gab es eine Kollision?
	if(iClosestTriangle != -1)
	{
		// Falls erwünscht: Ort der Kollision berechnen
		if(pvOutPos != NULL) *pvOutPos = vLineA + fClosestCollision * (vLineB - vLineA);

		// Normalvektor des getroffenen Dreiecks eintragen
		if(pvOutNormal != NULL)
		{
			// Wir erhalten den Normalvektor aus der Ebenengleichung des getroffenen
			// Dreiecks. Dieser Vektor muss noch transformiert werden.
			*pvOutNormal = Vector3DTransformNormal(pModel->m_pTrianglePlanes[iClosestTriangle * 4].n,
				                                    mMatrix);
		}

		// Das getroffene Dreieck selbst eintragen
		if(piOutTriangle != NULL) *piOutTriangle = iClosestTriangle;

		return TRUE;
	}

	// Keine Kollision!
	return FALSE;
}

// ******************************************************************
// Kollision zwischen Linie und Octree berechnen (rekursiv)
BOOL LineHitsOctreeRec(const Vector3D& vLineA,
									 const Vector3D& vLineB,
									 const Octree* pOctree,
									 const OctreeNode* pNode,
									 int* piClosestTriangle,
									 float* pfClosestCollision,
									 OctreeNode** ppClosestNode,
									 BOOL bSimpleMode,
									 BOOL* pbStop)
{
	// Prüfen, ob die Linie den Umgebungsquader des Knotens trifft.
	// Falls nicht, können wir sofort abbrechen
	if(!LineHitsBox(vLineA, vLineB,
		              pNode->vBoundingBoxMin,
					  pNode->vBoundingBoxMax,
					  Matrix4x4::Identity()))
	{
		// Abbruch!
		return FALSE;
	}

	// Ist dieser Knoten ein Endknoten? Falls ja, dann testen wir die
	// Kollision jetzt auf Dreiecksebene.
	if(pNode->bIsLeaf)
	{
		float		fCollision;	// Linienabschnitt des Schnittpunkts
		Vector3D	vCollision;	// Position des Schnittpunkts

		// Nun jedes Dreieck dieses Knotens durchgehen und
		// nach der nächsten Kollisionen suchen
		for(DWORD t = 0; t < pNode->dwNumIndices / 3; t++)
		{
			// Schneidet die Linie das Dreieck?
			// Dazu berechnen wir zuerst den Schnittpunkt zwischen der Linie und der
			// ersten Ebene des Dreiecks.
			fCollision = LineHitsPlaneS(vLineA, vLineB, pNode->pTrianglePlanes[t * 4]);
			if(fCollision >= 0.0f)
			{
				// Die erste Ebene wird geschnitten.
				// Der Schnittpunkt muss nun auf der Vorderseite der drei anderen
				// Ebenen des Dreiecks liegen, damit eine Kollision vorliegt.
				vCollision = vLineA + (vLineB - vLineA) * fCollision;
				if(PlaneDotCoords(pNode->pTrianglePlanes[t * 4 + 1], vCollision) < 0.0f) continue;
				if(PlaneDotCoords(pNode->pTrianglePlanes[t * 4 + 2], vCollision) < 0.0f) continue;
				if(PlaneDotCoords(pNode->pTrianglePlanes[t * 4 + 3], vCollision) < 0.0f) continue;

				// Kollision! Bricht sie den Rekord?
				if(fCollision < *pfClosestCollision)
				{
					*pfClosestCollision = fCollision;
					*piClosestTriangle = t;
					*ppClosestNode = (OctreeNode*)(pNode);
				}

				// Wenn der Benutzer den Ort der Kollision nicht kennen möchte
				// (bSimpleMode = TRUE), dann können wir jetzt schon abbrechen.
				if(bSimpleMode)
				{
					*pbStop = TRUE;
					return TRUE;
				}
			}
		}
	}
	else
	{
		// Dieser Knoten ist kein Endknoten.
		// Wir gehen seine Unterknoten durch und testen diese.
		for(DWORD i = 0; i < 8; i++)
		{
			LineHitsOctreeRec(vLineA, vLineB, pOctree, pNode->apChild[i],
								piClosestTriangle, pfClosestCollision, ppClosestNode,
								bSimpleMode, pbStop);

			// Wurde die pbStop-Variable auf TRUE gesetzt?
			// Falls ja, dann brechen wir ab und liefern TRUE zurück.
			if(*pbStop) return TRUE;
		}

		// Wenn es eine Kollision gab, wird TRUE geliefert.
		if(*piClosestTriangle != -1) return TRUE;
	}

	// Keine Kollision!
	return FALSE;
}

// ******************************************************************
// Kollision zwischen Linie und Octree berechnen
BOOL LineHitsOctree(const Vector3D& vLineA,
								  const Vector3D& vLineB,
								  const Octree* pOctree,
								  const float fTolerance,	// = 0.0f
								  Vector3D* pvOutPos,		// = NULL
								  Vector3D* pvOutNormal,	// = NULL
								  int* piOutTriangle,		// = NULL
								  OctreeNode** ppOutNode)	// = NULL
{
	// Parameter prüfen
	if(pOctree == NULL)			ERROR_NULL_POINTER("pOctree", ERROR);
	if(!pOctree->m_bExtraData)	ERROR("Der Octree hat keine Extradaten!", ERROR);


	// Startwerte setzen
	int iClosestTriangle = -1;
	float fClosestCollision = 100000.0f;
	BOOL bStop = FALSE;
	OctreeNode* pNode;

	// Start und Ende der Linie ein wenig auseinander schieben - je nach Toleranz
	Vector3D vNewLineA(vLineA - Vector3DNormalizeEx(vLineB - vLineA) * fTolerance);
	Vector3D vNewLineB(vLineB + Vector3DNormalizeEx(vLineB - vLineA) * fTolerance);

	// Rekursive Funktion aufrufen
	LineHitsOctreeRec(vNewLineA, vNewLineB, pOctree, pOctree->m_pRootNode,
						&iClosestTriangle, &fClosestCollision, &pNode,
						pvOutPos == NULL, &bStop);

	// Gab es eine Kollision?
	if(iClosestTriangle != -1)
	{
		// Falls erwünscht: Ort der Kollision berechnen
		if(pvOutPos != NULL) *pvOutPos = vLineA + fClosestCollision * (vLineB - vLineA);

		// Normalvektor des getroffenen Dreiecks eintragen
		if(pvOutNormal != NULL)
		{
			// Wir erhalten den Normalvektor aus der Ebenengleichung des getroffenen Dreiecks.
			*pvOutNormal = pNode->pTrianglePlanes[iClosestTriangle * 4].n;
		}

		// Das getroffene Dreieck und den Knoten selbst eintragen
		if(piOutTriangle != NULL) *piOutTriangle = iClosestTriangle;
		if(ppOutNode != NULL) *ppOutNode = pNode;

		return TRUE;
	}

	// Keine Kollision!
	return FALSE;
}

// ******************************************************************
// Kollision zwischen zwei Modellen berechnen (rekursiv)
BOOL ModelHitsModelRec(const Model* pModelA,
									 const Matrix4x4& mMatrixA,
									 const Matrix4x4& mInvMatrixA,
									 const ModelOctreeNode* pNodeA,
									 const Model* pModelB,
									 const Matrix4x4& mMatrixB,
									 const Matrix4x4& mInvMatrixB,
									 const ModelOctreeNode* pNodeB,
									 Vector3D* pvOutPos,
									 Vector3D* pvOutNormalA,
									 Vector3D* pvOutNormalB,
									 int* piOutTriA,
									 int* piOutTriB,
									 BOOL bSkipBBTest)
{
	// Schneiden sich die Umgebungsquader der beiden Knoten?
	// Falls sie es nicht tun, kann es hier keine Kollision geben.
	if(!bSkipBBTest)
	{
		if(!BoxHitsBox(pNodeA->vBoundingBoxMin, pNodeA->vBoundingBoxMax, mMatrixA,
						 pNodeB->vBoundingBoxMin, pNodeB->vBoundingBoxMax, mMatrixB,
						 3))
		{
			// Zurück!
			return FALSE;
		}
	}

	// Die Umgebungsquader schneiden sich.
	// Wenn beide Knoten Endknoten sind, können wir die Kollision auf Dreiecksebene prüfen.
	if(pNodeA->bIsLeaf && pNodeB->bIsLeaf)
	{
		// Einer der beiden Knoten muss in jedem Fall transformiert werden.
		// Wir nehmen dafür Knoten B. Hat Knoten B mehr Dreiecke als Knoten A,
		// wird getauscht.
		if(pNodeB->dwNumTriangles > pNodeA->dwNumTriangles)
		{
			// Tauschen!
			return ModelHitsModelRec(pModelB, mMatrixB, mInvMatrixB, pNodeB,
									   pModelA, mMatrixA, mInvMatrixA, pNodeA,
									   pvOutPos,
									   pvOutNormalB, pvOutNormalA,
									   piOutTriB, piOutTriA,
									   TRUE);
		}

		// Die Vektoren von Knoten B werden später beim Test transformiert.
		// Sie sollen zuerst absolut und dann relativ zu Modell A umgerechnet werden.
		// Es ist also die Transformation mit der Matrix von Modell B und anschließend
		// mit der inversen Matrix von Modell A nötig.
		Matrix4x4 mTransformation(mMatrixB * mInvMatrixA);

		Vector3D*	pvVector;
		Vector3D	vTriA;
		Vector3D	vTriB;
		Vector3D	vTriC;
		Vector3D	vHitStart;
		Vector3D	vHitEnd;

		// Alle Dreiecke von Knoten B durchgehen
		for(DWORD  = 0;  < pNodeB->dwNumTriangles; ++)
		{
			// Ersten Vektor dieses Dreiecks von Knoten B transformieren
			pvVector = &pModelB->m_pvVectors[pModelB->m_pdwIndices[pNodeB->pdwTriangles[] * 3]];
			vTriA.x = pvVector->x * mTransformation.m11 + pvVector->y * mTransformation.m21 + pvVector->z * mTransformation.m31 + mTransformation.m41;
			vTriA.y = pvVector->x * mTransformation.m12 + pvVector->y * mTransformation.m22 + pvVector->z * mTransformation.m32 + mTransformation.m42;
			vTriA.z = pvVector->x * mTransformation.m13 + pvVector->y * mTransformation.m23 + pvVector->z * mTransformation.m33 + mTransformation.m43;

			// Zweiten Vektor dieses Dreiecks von Knoten B transformieren
			pvVector = &pModelB->m_pvVectors[pModelB->m_pdwIndices[pNodeB->pdwTriangles[] * 3 + 1]];
			vTriB.x = pvVector->x * mTransformation.m11 + pvVector->y * mTransformation.m21 + pvVector->z * mTransformation.m31 + mTransformation.m41;
			vTriB.y = pvVector->x * mTransformation.m12 + pvVector->y * mTransformation.m22 + pvVector->z * mTransformation.m32 + mTransformation.m42;
			vTriB.z = pvVector->x * mTransformation.m13 + pvVector->y * mTransformation.m23 + pvVector->z * mTransformation.m33 + mTransformation.m43;

			// Dritten Vektor dieses Dreiecks von Knoten B transformieren
			pvVector = &pModelB->m_pvVectors[pModelB->m_pdwIndices[pNodeB->pdwTriangles[] * 3 + 2]];
			vTriC.x = pvVector->x * mTransformation.m11 + pvVector->y * mTransformation.m21 + pvVector->z * mTransformation.m31 + mTransformation.m41;
			vTriC.y = pvVector->x * mTransformation.m12 + pvVector->y * mTransformation.m22 + pvVector->z * mTransformation.m32 + mTransformation.m42;
			vTriC.z = pvVector->x * mTransformation.m13 + pvVector->y * mTransformation.m23 + pvVector->z * mTransformation.m33 + mTransformation.m43;

			// Alle Dreiecke von Knoten A durchgehen
			for(DWORD ta = 0; ta < pNodeA->dwNumTriangles; ta++)
			{
				// Kollidieren diese beiden Dreiecke?
				if(TriangleHitsTriangle(pModelA->m_pvVectors[pModelA->m_pdwIndices[pNodeA->pdwTriangles[ta] * 3]],
										  pModelA->m_pvVectors[pModelA->m_pdwIndices[pNodeA->pdwTriangles[ta] * 3 + 1]],
										  pModelA->m_pvVectors[pModelA->m_pdwIndices[pNodeA->pdwTriangles[ta] * 3 + 2]],
										  vTriA, vTriB, vTriC,
										  &vHitStart,
										  &vHitEnd))
				{
					// Es gibt eine Kollision!
					// Wenn der Ort der Kollision gefragt ist: ausfüllen!
					// Der Vektor muss aber noch mit der Matrix von Modell A transformiert werden.
					if(pvOutPos != NULL) *pvOutPos = Vector3DTransformCoords(0.5f * (vHitStart + vHitEnd), mMatrixA);

					// Sind die beiden Dreiecke gefragt?
					if(piOutTriA != NULL) *piOutTriA = (int)(pNodeA->pdwTriangles[ta]);
					if(piOutTriB != NULL) *piOutTriB = (int)(pNodeB->pdwTriangles[]);

					// Sind die Normalvektoren gefragt?
					// Transformation mit der Matrix des jeweiligen Objekts ist notwendig.
					// Wir erhalten die Normalvektoren aus den Ebenengleichungen der Dreiecke.
					if(pvOutNormalA != NULL) *pvOutNormalA = Vector3DTransformNormal(pModelA->m_pTrianglePlanes[pNodeA->pdwTriangles[ta] * 4].n, mMatrixA);
					if(pvOutNormalB != NULL) *pvOutNormalB = Vector3DTransformNormal(pModelB->m_pTrianglePlanes[pNodeB->pdwTriangles[] * 4].n, mMatrixB);

					return TRUE;
				}
			}
		}
	}
	else if(pNodeA->bIsLeaf && !pNodeB->bIsLeaf)
	{
		// Knoten A ist ein Endknoten, Knoten B aber nicht.
		// Wir rufen die Funktion erneut auf, für jeden untergeordneten Knoten von B.
		for(int i = 0; i < 8; i++)
		{
			if(ModelHitsModelRec(pModelA, mMatrixA, mInvMatrixA, pNodeA,
				                   pModelB, mMatrixB, mInvMatrixB, pNodeB->apChild[i],
								   pvOutPos,
								   pvOutNormalA, pvOutNormalB,
								   piOutTriA, piOutTriB,
								   FALSE))
			{
				// Es gab eine Kollision! Sofort abbrechen.
				return TRUE;
			}
		}
	}
	else if(!pNodeA->bIsLeaf && pNodeB->bIsLeaf)
	{
		// Knoten B ist ein Endknoten, Knoten A aber nicht.
		// Wir rufen die Funktion erneut auf, für jeden untergeordneten Knoten von A.
		for(int i = 0; i < 8; i++)
		{
			if(ModelHitsModelRec(pModelA, mMatrixA, mInvMatrixA, pNodeA->apChild[i],
				                   pModelB, mMatrixB, mInvMatrixB, pNodeB,
								   pvOutPos,
								   pvOutNormalA, pvOutNormalB,
								   piOutTriA, piOutTriB,
								   FALSE))
			{
				// Es gab eine Kollision! Sofort abbrechen.
				return TRUE;
			}
		}
	}
	else
	{
		// Beide Knoten sind keine Endknoten!
		// Wir testen nun Knoten A mit allen Unterknoten von Knoten B.
		for(int i = 0; i < 8; i++)
		{
			if(ModelHitsModelRec(pModelA, mMatrixA, mInvMatrixA, pNodeA,
				                   pModelB, mMatrixB, mInvMatrixB, pNodeB->apChild[i],
								   pvOutPos,
								   pvOutNormalA, pvOutNormalB,
								   piOutTriA, piOutTriB,
								   FALSE))
			{
				// Es gab eine Kollision! Sofort abbrechen.
				return TRUE;
			}
		}
	}

	// Keine Kollision!
	return FALSE;
}

// ******************************************************************
// Kollision zwischen zwei Modellen berechnen
BOOL ModelHitsModel(const Model* pModelA,
								  const Matrix4x4& mMatrixA,
								  const Matrix4x4& mInvMatrixA,
								  const Model* pModelB,
								  const Matrix4x4& mMatrixB,
								  const Matrix4x4& mInvMatrixB,
								  Vector3D* pvOutPos,		// = NULL
								  Vector3D* pvOutNormalA,	// = NULL
								  Vector3D* pvOutNormalB,	// = NULL
								  int* piOutTriA,			// = NULL
								  int* piOutTriB)			// = NULL
{
	// Parameter prüfen
	if(pModelA == NULL)			ERROR_NULL_POINTER("pModelA", ERROR);
	if(pModelB == NULL)			ERROR_NULL_POINTER("pModelB", ERROR);
	if(!pModelA->m_bExtraData)	ERROR("Das Modell A hat keine Extradaten!", ERROR);
	if(!pModelB->m_bExtraData)	ERROR("Das Modell B hat keine Extradaten!", ERROR);


	// Die Positionen der beiden Modelle aus den Matrizen ablesen
	Vector3D vModelA(Vector3D(mMatrixA.m41, mMatrixA.m42, mMatrixA.m43));
	Vector3D vModelB(Vector3D(mMatrixB.m41, mMatrixB.m42, mMatrixB.m43));

	// Prüfen, ob sich die Umgebungskugeln der beiden Modelle schneiden.
	// Falls sie es nicht tun, brauchen wir gar nicht weiter zu testen.
	if(!SphereHitsSphere(vModelA, pModelA->m_fBoundingSphereRadius,
		                   vModelB, pModelB->m_fBoundingSphereRadius)) return FALSE;

	// Die rekursive Funktion aufrufen, mit den Wurzelknoten beider Modelle
	return ModelHitsModelRec(pModelA, mMatrixA, mInvMatrixA, pModelA->m_pRootNode,
		                       pModelB, mMatrixB, mInvMatrixB, pModelB->m_pRootNode,
							   pvOutPos,
							   pvOutNormalA, pvOutNormalB,
							   piOutTriA, piOutTriB,
							   FALSE);
}

// ******************************************************************
// Kollision zwischen Modell und Octree berechnen (rekursiv)
BOOL ModelHitsOctreeRec(const Model* pModel,
									  const Matrix4x4& mMatrix,
									  const Matrix4x4& mInvMatrix,
									  const ModelOctreeNode* pModelNode,
									  const Octree* pOctree,
									  const OctreeNode* pOctreeNode,
									  Vector3D* pvOutPos,
									  Vector3D* pvOutNormalM,
									  Vector3D* pvOutNormalO,
									  int* piOutTriM,
									  int* piOutTriO,
									  OctreeNode** ppOutNodeO)
{
	// Schneiden sich die Umgebungsquader der beiden Knoten?
	// Falls sie es nicht tun, kann es hier keine Kollision geben.
	if(!BoxHitsBox(pModelNode->vBoundingBoxMin, pModelNode->vBoundingBoxMax, mMatrix,
					 pOctreeNode->vBoundingBoxMin, pOctreeNode->vBoundingBoxMax, Matrix4x4Identity(),
					 3))
	{
		// Zurück!
		return FALSE;
	}

	// Die Umgebungsquader schneiden sich.
	// Wenn beide Knoten Endknoten sind, können wir die Kollision auf Dreiecksebene prüfen.
	if(pModelNode->bIsLeaf && pOctreeNode->bIsLeaf)
	{
		// Den Knoten des Octrees in Model-Space umrechnen
		Vector3D*	pvVector;
		Vector3D	vTriA;
		Vector3D	vTriB;
		Vector3D	vTriC;
		Vector3D	vHitStart;
		Vector3D	vHitEnd;

		// Alle Dreiecke vom Octree durchgehen
		for(DWORD  = 0;  < pOctreeNode->dwNumIndices / 3; ++)
		{
			// Ersten Vektor dieses Dreiecks vom Octree transformieren
			pvVector = &pOctree->m_pvVectors[pOctreeNode->pdwIndices[ * 3]];
			vTriA.x = pvVector->x * mInvMatrix.m11 + pvVector->y * mInvMatrix.m21 + pvVector->z * mInvMatrix.m31 + mInvMatrix.m41;
			vTriA.y = pvVector->x * mInvMatrix.m12 + pvVector->y * mInvMatrix.m22 + pvVector->z * mInvMatrix.m32 + mInvMatrix.m42;
			vTriA.z = pvVector->x * mInvMatrix.m13 + pvVector->y * mInvMatrix.m23 + pvVector->z * mInvMatrix.m33 + mInvMatrix.m43;

			// Zweiten Vektor dieses Dreiecks vom Octree transformieren
			pvVector = &pOctree->m_pvVectors[pOctreeNode->pdwIndices[ * 3 + 1]];
			vTriB.x = pvVector->x * mInvMatrix.m11 + pvVector->y * mInvMatrix.m21 + pvVector->z * mInvMatrix.m31 + mInvMatrix.m41;
			vTriB.y = pvVector->x * mInvMatrix.m12 + pvVector->y * mInvMatrix.m22 + pvVector->z * mInvMatrix.m32 + mInvMatrix.m42;
			vTriB.z = pvVector->x * mInvMatrix.m13 + pvVector->y * mInvMatrix.m23 + pvVector->z * mInvMatrix.m33 + mInvMatrix.m43;

			// Dritten Vektor dieses Dreiecks vom Octree transformieren
			pvVector = &pOctree->m_pvVectors[pOctreeNode->pdwIndices[ * 3 + 2]];
			vTriC.x = pvVector->x * mInvMatrix.m11 + pvVector->y * mInvMatrix.m21 + pvVector->z * mInvMatrix.m31 + mInvMatrix.m41;
			vTriC.y = pvVector->x * mInvMatrix.m12 + pvVector->y * mInvMatrix.m22 + pvVector->z * mInvMatrix.m32 + mInvMatrix.m42;
			vTriC.z = pvVector->x * mInvMatrix.m13 + pvVector->y * mInvMatrix.m23 + pvVector->z * mInvMatrix.m33 + mInvMatrix.m43;

			// Alle Dreiecke vom Modell durchgehen
			for(DWORD ta = 0; ta < pModelNode->dwNumTriangles; ta++)
			{
				// Kollidieren diese beiden Dreiecke?
				if(TriangleHitsTriangle(pModel->m_pvVectors[pModel->m_pdwIndices[pModelNode->pdwTriangles[ta] * 3]],
										  pModel->m_pvVectors[pModel->m_pdwIndices[pModelNode->pdwTriangles[ta] * 3 + 1]],
										  pModel->m_pvVectors[pModel->m_pdwIndices[pModelNode->pdwTriangles[ta] * 3 + 2]],
										  vTriA, vTriB, vTriC,
										  &vHitStart,
										  &vHitEnd))
				{
					// Es gibt eine Kollision!
					// Wenn der Ort der Kollision gefragt ist: ausfüllen!
					// Der Vektor muss aber noch mit der Matrix vom Modell transformiert werden.
					if(pvOutPos != NULL) *pvOutPos = Vector3DTransformCoords(0.5f * (vHitStart + vHitEnd), mMatrix);

					// Sind die beiden Dreiecke gefragt?
					if(piOutTriM != NULL) *piOutTriM = (int)(pModelNode->pdwTriangles[ta]);
					if(piOutTriO != NULL) *piOutTriO = (int)();

					// Ist der Octree-Knoten gefragt?
					if(ppOutNodeO != NULL) *ppOutNodeO = (OctreeNode*)(pOctreeNode);

					// Sind die Normalvektoren gefragt?
					if(pvOutNormalM != NULL) *pvOutNormalM = Vector3DTransformNormal(pModel->m_pTrianglePlanes[pModelNode->pdwTriangles[ta] * 4].n, mMatrix);
					if(pvOutNormalO != NULL) *pvOutNormalO = pOctreeNode->pTrianglePlanes[ * 4].n;

					return TRUE;
				}
			}
		}
	}
	else if(pModelNode->bIsLeaf && !pOctreeNode->bIsLeaf)
	{
		for(int i = 0; i < 8; i++)
		{
			if(ModelHitsOctreeRec(pModel, mMatrix, mInvMatrix, pModelNode,
									pOctree, pOctreeNode->apChild[i],
									pvOutPos,
									pvOutNormalM, pvOutNormalO,
									piOutTriM, piOutTriO,
									ppOutNodeO))
			{
				// Es gab eine Kollision! Sofort abbrechen.
				return TRUE;
			}
		}
	}
	else if(!pModelNode->bIsLeaf && pOctreeNode->bIsLeaf)
	{
		for(int i = 0; i < 8; i++)
		{
			if(ModelHitsOctreeRec(pModel, mMatrix, mInvMatrix, pModelNode->apChild[i],
									pOctree, pOctreeNode,
									pvOutPos,
									pvOutNormalM, pvOutNormalO,
									piOutTriM, piOutTriO,
									ppOutNodeO))
			{
				// Es gab eine Kollision! Sofort abbrechen.
				return TRUE;
			}
		}
	}
	else
	{
		for(int i = 0; i < 8; i++)
		{
			if(ModelHitsOctreeRec(pModel, mMatrix, mInvMatrix, pModelNode,
									pOctree, pOctreeNode->apChild[i],
									pvOutPos,
									pvOutNormalM, pvOutNormalO,
									piOutTriM, piOutTriO,
									ppOutNodeO))
			{
				// Es gab eine Kollision! Sofort abbrechen.
				return TRUE;
			}
		}
	}

	// Keine Kollision!
	return FALSE;
}

// ******************************************************************
// Kollision zwischen Modell und Octree berechnen
BOOL ModelHitsOctree(const Model* pModel,
								   const Matrix4x4& mMatrix,
								   const Matrix4x4& mInvMatrix,
								   const Octree* pOctree,
								   Vector3D* pvOutPos,			// = NULL
								   Vector3D* pvOutNormalM,		// = NULL
								   Vector3D* pvOutNormalO,		// = NULL
								   int* piOutTriM,				// = NULL
								   int* piOutTriO,				// = NULL
								   OctreeNode** ppOutNodeO)	// = NULL
{
	// Parameter prüfen
	if(pModel == NULL)			ERROR_NULL_POINTER("pModel", ERROR);
	if(pOctree == NULL)			ERROR_NULL_POINTER("pOctree", ERROR);
	if(!pModel->m_bExtraData)	ERROR("Das Modell hat keine Extradaten!", ERROR);
	if(!pOctree->m_bExtraData)	ERROR("Der Octree hat keine Extradaten!", ERROR);


	// Die rekursive Funktion aufrufen, mit den Wurzelknoten beider Objekte
	return ModelHitsOctreeRec(pModel, mMatrix, mInvMatrix, pModel->m_pRootNode,
								pOctree, pOctree->m_pRootNode,
								pvOutPos,
								pvOutNormalM, pvOutNormalO,
								piOutTriM, piOutTriO,
								ppOutNodeO);
}

// ******************************************************************
// Schnittlinie zwischen einem Dreieck und einer Ebene berechnen
BOOL TriangleHitsPlane(const Vector3D& vTriangleA,
									 const Vector3D& vTriangleB,
									 const Vector3D& vTriangleC,
									 Plane& Plane,
									 Vector3D* pvHitStart,
									 Vector3D* pvHitEnd)
{
	// Punkte des Dreiecks in die Ebenengleichung einsetzen
	float fDotA = PlaneDotCoords(Plane, vTriangleA);
	float fDo = PlaneDotCoords(Plane, vTriangleB);
	float fDotC = PlaneDotCoords(Plane, vTriangleC);

	// Wenn alle Punkte auf der gleichen Seite liegen, gibt es keine Schnittlinie.
	if(fDotA > 0.0f) if(fDo > 0.0f) if(fDotC > 0.0f) return FALSE;
	if(fDotA < 0.0f) if(fDo < 0.0f) if(fDotC < 0.0f) return FALSE;

	// Den Punkt finden, der auf seiner Ebenenseite alleine ist
	int iTop = 0;
	if(fDo <= 0.0f && fDotC <= 0.0f) iTop = 1;
	else if(fDotA >= 0.0f && fDo >= 0.0f) iTop = 3;
	else if(fDotA >= 0.0f && fDotC >= 0.0f) iTop = 2;
	else if(fDo >= 0.0f && fDotC >= 0.0f) iTop = 1;
	else if(fDotA <= 0.0f && fDo <= 0.0f) iTop = 3;
	else iTop = 2;

	// Schnittpunkte zwischen der Linie vom einzelnen Punkt zu den beiden anderen
	// und der Ebene berechnen
	switch(iTop)
	{
	case 1: *pvHitStart = LineHitsPlaneFastEx(vTriangleA, vTriangleB, Plane);
			*pvHitEnd = LineHitsPlaneFastEx(vTriangleA, vTriangleC, Plane);
			return TRUE;

	case 2: *pvHitStart = LineHitsPlaneFastEx(vTriangleB, vTriangleA, Plane);
			*pvHitEnd = LineHitsPlaneFastEx(vTriangleB, vTriangleC, Plane);
			return TRUE;
	
	case 3: *pvHitStart = LineHitsPlaneFastEx(vTriangleC, vTriangleA, Plane);
			*pvHitEnd = LineHitsPlaneFastEx(vTriangleC, vTriangleB, Plane);
			return TRUE;
	}

	// Das Dreieck schneidet die Ebene nicht!
	return FALSE;
}

// ******************************************************************
// Schnittlinie zweier Dreiecke berechnen
BOOL TriangleHitsTriangle(const Vector3D& v1A,
										const Vector3D& v1B,
										const Vector3D& v1C,
										const Vector3D& v2A,
										const Vector3D& v2B,
										const Vector3D& v2C,
										Vector3D* pvHitStart,
										Vector3D* pvHitEnd)
{
	// Ebene des zweiten Dreiecks berechnen
	Plane Plane2(PlaneFromPoints(v2A, v2B, v2C));

	// Punkte des ersten Dreiecks in die Ebenengleichung einsetzen
	float fDot2A = PlaneDotCoords(Plane2, v1A);
	float fDot2B = PlaneDotCoords(Plane2, v1B);
	float fDot2C = PlaneDotCoords(Plane2, v1C);

	// Liegen alle Punkte auf der gleichen Seite?
	if(fDot2A < -0.0001f && fDot2B < -0.0001f && fDot2C < -0.0001f) return FALSE;
	if(fDot2A > 0.0001f && fDot2B > 0.0001f && fDot2C > 0.0001f) return FALSE;

	// Nun auch die Ebene von Dreieck 1 berechnen und Tests durchführen
	Plane Plane1(PlaneFromPoints(v1A, v1B, v1C));
	float fDot1A = PlaneDotCoords(Plane1, v2A);
	float fDot1B = PlaneDotCoords(Plane1, v2B);
	float fDot1C = PlaneDotCoords(Plane1, v2C);
	if(fDot1A < -0.0001f && fDot1B < -0.0001f && fDot1C < -0.0001f) return FALSE;
	if(fDot1A > 0.0001f && fDot1B > 0.0001f && fDot1C > 0.0001f) return FALSE;

	// Die "höchsten" Punkte beider Dreiecke suchen
	int iTop1 = 0;
	int iTop2 = 0;
	if(fDot2B <= -0.0001f && fDot2C <= -0.0001f) iTop1 = 1;
	else if(fDot2A >= 0.0001f && fDot2B >= 0.0001f) iTop1 = 3;
	else if(fDot2A >= 0.0001f && fDot2C >= 0.0001f) iTop1 = 2;
	else if(fDot2B >= 0.0001f && fDot2C >= 0.0001f) iTop1 = 1;
	else if(fDot2A <= -0.0001f && fDot2B <= -0.0001f) iTop1 = 3;
	else iTop1 = 2;
	if(fDot1B <= -0.0001f && fDot1C <= -0.0001f) iTop2 = 1;
	else if(fDot1A >= 0.0001f && fDot1B >= 0.0001f) iTop2 = 3;
	else if(fDot1A >= 0.0001f && fDot1C >= 0.0001f) iTop2 = 2;
	else if(fDot1B >= 0.0001f && fDot1C >= 0.0001f) iTop2 = 1;
	else if(fDot1A <= -0.0001f && fDot1B <= -0.0001f) iTop2 = 3;
	else iTop2 = 2;

	// L1 berechnen
	Vector3D vL1A;	// Startpunkt von L1
	Vector3D vL1B;	// Endpunkt von L1
	switch(iTop1)
	{
	case 1: vL1A = LineHitsPlaneFastEx(v1A, v1B, Plane2);
			vL1B = LineHitsPlaneFastEx(v1A, v1C, Plane2);
			break;

	case 2: vL1A = LineHitsPlaneFastEx(v1B, v1A, Plane2);
			vL1B = LineHitsPlaneFastEx(v1B, v1C, Plane2);
			break;

	default: vL1A = LineHitsPlaneFastEx(v1C, v1A, Plane2);
			 vL1B = LineHitsPlaneFastEx(v1C, v1B, Plane2);
			 break;
	}

	// L2 berechnen
	Vector3D vL2A;	// Startpunkt von L2
	Vector3D vL2B;	// Endpunkt von L2
	switch(iTop2)
	{
	case 1: vL2A = LineHitsPlaneFastEx(v2A, v2B, Plane1);
			vL2B = LineHitsPlaneFastEx(v2A, v2C, Plane1);
			break;

	case 2: vL2A = LineHitsPlaneFastEx(v2B, v2A, Plane1);
			vL2B = LineHitsPlaneFastEx(v2B, v2C, Plane1);
			break;

	default: vL2A = LineHitsPlaneFastEx(v2C, v2A, Plane1);
			 vL2B = LineHitsPlaneFastEx(v2C, v2B, Plane1);
			 break;
	}

	// Die eindimensionale Achse berechnen (Kreuzprodukt aus den
	// Normalvektoren der beiden Dreiecksebenen bilden)
	Vector3D vAxis(Vector3DCross(Plane1.n, Plane2.n));

	// Die dominante Koordinatenkomponente finden und dann die
	// Schnittlinie auf die 1D-Achse projizieren
	float fMax = MAX(MAX(vAxis.x, vAxis.y), vAxis.z);
	if(fabsf(fMax) < 0.0001f) fMax = MIN(MIN(vAxis.x, vAxis.y), vAxis.z);
	float fInvMax = 1.0f / fMax;
	float fMin1 = 0.0f;
	float fMin2;
	float fMax1;
	float fMax2;

	if(fMax == vAxis.x)
	{
		// Die x-Achse ist die dominante Achse.
		// Anstatt durch fMax zu teilen, multiplizieren wir mit dem Kehrwert,
		// was schneller ist.
		fMax1 = (vL1B.x - vL1A.x) * fInvMax;
		fMin2 = (vL2A.x - vL1A.x) * fInvMax;
		fMax2 = (vL2B.x - vL1A.x) * fInvMax;
	}
	else if(fMax == vAxis.y)
	{
		// Die y-Achse ist die dominante Achse.
		fMax1 = (vL1B.y - vL1A.y) * fInvMax;
		fMin2 = (vL2A.y - vL1A.y) * fInvMax;
		fMax2 = (vL2B.y - vL1A.y) * fInvMax;
	}
	else
	{
		// Die z-Achse ist die dominante Achse.
		fMax1 = (vL1B.z - vL1A.z) * fInvMax;
		fMin2 = (vL2A.z - vL1A.z) * fInvMax;
		fMax2 = (vL2B.z - vL1A.z) * fInvMax;
	}

	// Die Werte in die richtige Reihenfolge bringen
	if(fMin1 > fMax1) {float fSwap = fMin1; fMin1 = fMax1; fMax1 = fSwap;}
	if(fMin2 > fMax2) {float fSwap = fMin2; fMin2 = fMax2; fMax2 = fSwap;}

	// Falls sich die beiden Bereiche überschneiden, liegt eine Kollision vor.
	if(fMin1 <= fMin2)
	{
		if(fMax1 >= fMin2)
		{
			// Kollision! Schnittlinie eintragen.
			*pvHitStart = vL1A + (MAX(fMin1, fMin2) * vAxis);
			*pvHitEnd = vL1A + (MIN(fMax1, fMax2) * vAxis);
			return TRUE;
		}
		
		// Keine Schnittlinie!
		return FALSE;
	}
	// fMin1 >= fMin2
	else if(fMin1 <= fMax2)
	{
		// Kollision! Schnittlinie eintragen.
		*pvHitStart = vL1A + (MAX(fMin1, fMin2) * vAxis);
		*pvHitEnd = vL1A + (MIN(fMax1, fMax2) * vAxis);
		return TRUE;
	}

	// Keine Schnittlinie!
	return FALSE;
}

// ******************************************************************
// Kollision zwischen einer Kugel und einer Ebene berechnen
BOOL SphereHitsPlane(const Vector3D& vSphere,
								   const float fRadius,
								   const Plane& Plane)
{
	// Wenn der Abstand des Kugelmittelpunkts zur Ebene kleiner oder gleich
	// dem Radius der Kugel ist, dann gibt es eine Kollision.
	return fabsf(PointPlaneDistance(vSphere, Plane)) <= fRadius;
}

// ******************************************************************
// Kollision zweier Kugeln berechnen
BOOL SphereHitsSphere(const Vector3D& vSphereA,
									const float fRadiusA,
									const Vector3D& vSphereB,
									const float fRadiusB)
{
	// Quadrat der Entfernung berechnen und entscheiden, ob eine Kollision vorliegt
	return Vector3DLengthSq(vSphereA - vSphereB) <= (fRadiusA + fRadiusB) * (fRadiusA + fRadiusB);
}

// ******************************************************************
// Schneiden sich Kugel und achsenausgerichteter Quader?
BOOL SphereHitsAABox(const Vector3D& vSphere,
								   const float fRadius,
								   const Vector3D& vBoxMin,
								   const Vector3D& vBoxMax)
{
	return PointHitsAABox(vSphere,
							vBoxMin - Vector3D(fRadius),
							vBoxMax + Vector3D(fRadius));
}

// ******************************************************************
// Schneiden sich Kugel und frei drehbarer Quader?
BOOL SphereHitsBox(const Vector3D& vSphere,
								 const float fRadius,
								 const Vector3D& vBoxMin,
								 const Vector3D& vBoxMax,
								 const Matrix4x4& mBoxTransformation)
{
	return PointHitsBox2(vSphere,
		                   vBoxMin - Vector3D(fRadius),
						   vBoxMax + Vector3D(fRadius),
						   mBoxTransformation);
}

// ******************************************************************
// Kollision zwischen zwei Quadern berechnen
BOOL BoxHitsBox(const Vector3D& vBox1Min,
							  const Vector3D& vBox1Max,
							  const Matrix4x4& mBox1Transformation,
							  const Vector3D& vBox2Min,
							  const Vector3D& vBox2Max,
							  const Matrix4x4& mBox2Transformation,
							  const int iNumSamples)
{
	Plane aBoxPlanes[6];	// Die sechs Ebenen

	// Die Ebenen des zweiten Quaders berechnen
	ComputeBoxPlanes(vBox2Min, vBox2Max, mBox2Transformation, aBoxPlanes);

	// Schrittweiten berechnen
	float fStepX = (vBox1Max.x - vBox1Min.x) / (float)(iNumSamples);
	float fStepY = (vBox1Max.y - vBox1Min.y) / (float)(iNumSamples);
	float fStepZ = (vBox1Max.z - vBox1Min.z) / (float)(iNumSamples);

	// Am Anfang ist der zu testende Punkt beim Minimum des ersten Quaders.
	Vector3D vPoint(vBox1Min);

	// Den ersten Quader durchlaufen
	for(int x = 0; x < iNumSamples; x++)
	{
		// y- und z-Koordinate zurücksetzen
		vPoint.y = vBox1Min.y;
		vPoint.z = vBox1Min.z;
		for(int y = 0; y < iNumSamples; y++)
		{
			// z-Koordinate zurücksetzen
			vPoint.z = vBox1Min.z;
			for(int z = 0; z < iNumSamples; z++)
			{
				// Wenn sich der aktuelle Punkt transformiert innerhalb von
				// Quader 2 befindet, dann liegt eine Kollision vor.
				if(PointHitsBox(Vector3DTransformCoords(vPoint,
					                                       mBox1Transformation),
					              aBoxPlanes))
				{
					// Kollision!
					return TRUE;
				}

				// Nächster Punkt
				vPoint.z += fStepZ;
			}

			// Nächster Punkt
			vPoint.y += fStepY;
		}

		// Nächster Punkt
		vPoint.x += fStepX;
	}


	// Nun machen wir das ganze noch einmal, aber diesmal durchwandern wir
	// den zweiten Quader und nicht den ersten.

	ComputeBoxPlanes(vBox1Min, vBox1Max, mBox1Transformation, aBoxPlanes);
	fStepX = (vBox2Max.x - vBox2Min.x) / (float)(iNumSamples);
	fStepY = (vBox2Max.y - vBox2Min.y) / (float)(iNumSamples);
	fStepZ = (vBox2Max.z - vBox2Min.z) / (float)(iNumSamples);
	vPoint = vBox2Min;

	for(x = 0; x < iNumSamples; x++)
	{
		vPoint.y = vBox2Min.y;
		vPoint.z = vBox2Min.z;
		for(int y = 0; y < iNumSamples; y++)
		{
			vPoint.z = vBox2Min.z;
			for(int z = 0; z < iNumSamples; z++)
			{
				if(PointHitsBox(Vector3DTransformCoords(vPoint,
					                                       mBox2Transformation),
					              aBoxPlanes))
				{
					return TRUE;
				}

				vPoint.z += fStepZ;
			}

			vPoint.y += fStepY;
		}

		vPoint.x += fStepX;
	}

	// Keine Kollision!
	return FALSE;
}

// ******************************************************************
// Diese Funktion liefert:
//  1, wenn die Kugel vollständig vor der Ebene liegt,
// -1, wenn sie vollständig hinter der Ebene liegt und
//  0, wenn die Kugel die Ebene schneidet.
int ClassifySphere(const Vector3D& vSphere,
								 const float fRadius,
								 const Plane& Plane)
{
	// Die Entfernung des Kugelmittelpunkts zur Ebene ist entscheidend.
	float fDistance = PointPlaneDistance(vSphere, Plane);
	if(fabsf(fDistance) < fRadius) return 0;

	return fDistance > 0.0f ? 1 : -1;
}

// ******************************************************************
// Diese Funktion liefert:
//  1, wenn der Quader vollständig vor der Ebene liegt,
// -1, wenn er vollständig hinter der Ebene liegt und
//  0, wenn der Quader die Ebene schneidet.
int ClassifyBox(const Vector3D& vBoxMin,
							  const Vector3D& vBoxMax,
							  const Matrix4x4& mBoxTransformation,
							  const Plane& Plane)
{
	// Es gibt ein recht schnelles Verfahren, dies herauszufinden, aber
	// es funktioniert nur mit achsenausgerichteten Quadern.
	// Darum wenden wir einen Trick an und tun so, als sei der Quader
	// achsenausgerichtet und transformieren statt dessen einfach die Ebene
	// mit der invertierten Matrix des Quaders.

	// Die Ebene transformieren
	Plane NewPlane(PlaneTransform(Plane, Matrix4x4Invert(mBoxTransformation)));

	// Nächsten und fernsten Punkt berechnen
	Vector3D vNearPoint(vBoxMax);
	Vector3D vFarPoint(vBoxMax);
	if(NewPlane.a > 0.0f) {vNearPoint.x = vBoxMin.x; vFarPoint.x = vBoxMax.x;}
	if(NewPlane.b > 0.0f) {vNearPoint.y = vBoxMin.y; vFarPoint.y = vBoxMax.y;}
	if(NewPlane.c > 0.0f) {vNearPoint.z = vBoxMin.z; vFarPoint.z = vBoxMax.z;}

	// Prüfen, ob der nächste Punkt vor der Ebene liegt. Falls ja, liegt der ganze Quader vor ihr.
	if(PlaneDotCoords(NewPlane, vNearPoint) > 0.0f) return 1;

	// Prüfen, ob der fernste Punkt vor der Ebene liegt. Falls ja, gibt es einen Schnitt.
	if(PlaneDotCoords(NewPlane, vFarPoint) > 0.0f) return 0;

	// Ansonsten liegt der Quader auf der Rückseite.
	return -1;
}

// ******************************************************************
// Diese Funktion liefert:
//  1, wenn der Quader vollständig vor der Ebene liegt,
// -1, wenn er vollständig hinter der Ebene liegt und
//  0, wenn der Quader die Ebene schneidet.
int ClassifyBox_Inv(const Vector3D& vBoxMin,
								  const Vector3D& vBoxMax,
								  const Matrix4x4& mInvBoxTransformation,
								  const Plane& Plane)
{
	// Es gibt ein recht schnelles Verfahren, dies herauszufinden, aber
	// es funktioniert nur mit achsenausgerichteten Quadern.
	// Darum wenden wir einen Trick an und tun so, als sei der Quader
	// achsenausgerichtet und transformieren statt dessen einfach die Ebene
	// mit der invertierten Matrix des Quaders.

	// Die Ebene transformieren
	Plane NewPlane(PlaneTransform(Plane, mInvBoxTransformation));

	// Nächsten und fernsten Punkt berechnen
	Vector3D vNearPoint(vBoxMax);
	Vector3D vFarPoint(vBoxMax);
	if(NewPlane.a > 0.0f) {vNearPoint.x = vBoxMin.x; vFarPoint.x = vBoxMax.x;}
	if(NewPlane.b > 0.0f) {vNearPoint.y = vBoxMin.y; vFarPoint.y = vBoxMax.y;}
	if(NewPlane.c > 0.0f) {vNearPoint.z = vBoxMin.z; vFarPoint.z = vBoxMax.z;}

	// Prüfen, ob der nächste Punkt vor der Ebene liegt. Falls ja, liegt der ganze Quader vor ihr.
	if(PlaneDotCoords(NewPlane, vNearPoint) > 0.0f) return 1;

	// Prüfen, ob der fernste Punkt vor der Ebene liegt. Falls ja, gibt es einen Schnitt.
	if(PlaneDotCoords(NewPlane, vFarPoint) > 0.0f) return 0;

	// Ansonsten liegt der Quader auf der Rückseite.
	return -1;
}

// ******************************************************************
// Diese Funktion liefert:
//  1, wenn der Quader vollständig vor der Ebene liegt,
// -1, wenn er vollständig hinter der Ebene liegt und
//  0, wenn der Quader die Ebene schneidet.
//
// ACHTUNG: Diese Funktion arbeitet mit achsenausgerichteten Quadern!
//          Diese Quader haben keine Transformationsmatrix!
int ClassifyAABox(const Vector3D& vBoxMin,
								const Vector3D& vBoxMax,
								const Plane& Plane)
{
	// Nächsten und fernsten Punkt berechnen
	Vector3D vNearPoint(vBoxMax);
	Vector3D vFarPoint(vBoxMin);
	if(Plane.a > 0.0f) {vNearPoint.x = vBoxMin.x; vFarPoint.x = vBoxMax.x;}
	if(Plane.b > 0.0f) {vNearPoint.y = vBoxMin.y; vFarPoint.y = vBoxMax.y;}
	if(Plane.c > 0.0f) {vNearPoint.z = vBoxMin.z; vFarPoint.z = vBoxMax.z;}

	// Prüfen, ob der nächste Punkt vor der Ebene liegt. Falls ja, liegt der ganze Quader vor ihr.
	if(PlaneDotCoords(Plane, vNearPoint) > 0.0f) return 1;

	// Prüfen, ob der fernste Punkt vor der Ebene liegt. Falls ja, gibt es einen Schnitt.
	if(PlaneDotCoords(Plane, vFarPoint) > 0.0f) return 0;

	// Der Quader befindet sich hinter der Ebene.
	return -1;
}

// ******************************************************************
// Sicharkeit eines Punkts prüfen
BOOL PointVisible(const Vector3D& vPoint,
								const Plane* pClipPlanes)
{
	// Den Punkt in jede Ebenengleichung einsetzen
	if(PointPlaneDistanceFast(vPoint, pClipPlanes[0]) > 0.0f) return FALSE;
	if(PointPlaneDistanceFast(vPoint, pClipPlanes[1]) > 0.0f) return FALSE;
	if(PointPlaneDistanceFast(vPoint, pClipPlanes[2]) > 0.0f) return FALSE;
	if(PointPlaneDistanceFast(vPoint, pClipPlanes[3]) > 0.0f) return FALSE;
	if(PointPlaneDistanceFast(vPoint, pClipPlanes[4]) > 0.0f) return FALSE;
	return PointPlaneDistanceFast(vPoint, pClipPlanes[5]) <= 0.0f;
}

// ******************************************************************
// Sicharkeit einer Kugel prüfen
BOOL SphereVisible(const Vector3D& vSphere,
								 const float fRadius,
								 const Plane* pClipPlanes)
{
	// Den Kugelmittelpunkt in jede Ebenengleichung einsetzen
	if(PointPlaneDistanceFast(vSphere, pClipPlanes[0]) > fRadius) return FALSE;
	if(PointPlaneDistanceFast(vSphere, pClipPlanes[1]) > fRadius) return FALSE;
	if(PointPlaneDistanceFast(vSphere, pClipPlanes[2]) > fRadius) return FALSE;
	if(PointPlaneDistanceFast(vSphere, pClipPlanes[3]) > fRadius) return FALSE;
	if(PointPlaneDistanceFast(vSphere, pClipPlanes[4]) > fRadius) return FALSE;
	return PointPlaneDistanceFast(vSphere, pClipPlanes[5]) <= fRadius;
}

// ******************************************************************
// Sicharkeit eines achsenausgerichteten Quaders prüfen
BOOL AABoxVisible(const Vector3D& vBoxMin,
								const Vector3D& vBoxMax,
								const Plane* pClipPlanes)
{
	// Wenn der Quader auf der Vorderseite einer Ebene liegt, ist er unsichar.
	if(ClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[0]) == 1) return FALSE;
	if(ClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[1]) == 1) return FALSE;
	if(ClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[2]) == 1) return FALSE;
	if(ClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[3]) == 1) return FALSE;
	if(ClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[4]) == 1) return FALSE;
	return ClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[5]) != 1;
}

// ******************************************************************
// Sicharkeit eines Quaders prüfen
BOOL BoxVisible(const Vector3D& vBoxMin,
							  const Vector3D& vBoxMax,
							  const Matrix4x4& mBoxTransformation,
							  const Plane* pClipPlanes)
{
	// Matrix invertieren
	Matrix4x4 mInvBoxTransformation(Matrix4x4Invert(mBoxTransformation));

	// Wenn der Quader auf der Vorderseite einer Ebene liegt, ist er unsichar.
	if(ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[0]) == 1) return FALSE;
	if(ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[1]) == 1) return FALSE;
	if(ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[2]) == 1) return FALSE;
	if(ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[3]) == 1) return FALSE;
	if(ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[4]) == 1) return FALSE;
	return ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[5]) != 1;
}

// ******************************************************************
// Sicharkeit eines Quaders prüfen
BOOL BoxVisible_Inv(const Vector3D& vBoxMin,
								  const Vector3D& vBoxMax,
								  const Matrix4x4& mInvBoxTransformation,
								  const Plane* pClipPlanes)
{
	// Wenn der Quader auf der Vorderseite einer Ebene liegt, ist er unsichar.
	if(ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[0]) == 1) return FALSE;
	if(ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[1]) == 1) return FALSE;
	if(ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[2]) == 1) return FALSE;
	if(ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[3]) == 1) return FALSE;
	if(ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[4]) == 1) return FALSE;
	return ClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[5]) != 1;
}

// ******************************************************************
// Berechnet den Zielvektor, um ein Objekt zu treffen
Vector3D ComputeAimingVector(const Vector3D& vOrigin,
											const Vector3D& vTargetPos,
											const Vector3D& vTargetVel,
											const float fProjectileSpeed)
{
	// Richtung vom Abschusspunkt zum Ziel bestimmen
	Vector3D vDirection(vTargetPos - vOrigin);

	// Diskriminante des Terms für die Zeit berechnen.
	// Wenn sie kleiner als null ist, dann ist es nicht möglich, das Ziel zu treffen.
	// Das kann zum Beispiel passieren, wenn das Ziel schneller als das Geschoss ist.
	// Wir liefern dann einfach die Position des Ziels als Zielvektor zurück.
	float fDiscriminant = fProjectileSpeed * fProjectileSpeed * Vector3DLengthSq(vDirection) - Vector3DLengthSq(Vector3DCross(vDirection, vTargetVel));
	if(fDiscriminant < 0.0f) return vTargetPos;

	// Zeit des Treffers berechnen
	float fTime = (sqrtf(fDiscriminant) + Vector3DDot(vDirection, vTargetVel))
				/ (fProjectileSpeed * fProjectileSpeed - Vector3DLengthSq(vTargetVel));

	// Zielvektor liefern
	return vTargetPos + fTime * vTargetVel;
}

// ******************************************************************
// Diese Funktion wandelt ein Direct3D-Format in einen Text um.
Result GetD3DFormatString(D3DFORMAT Format,
										  char* pcOut)
{
	// Parameter prüfen
	if(pcOut == NULL) ERROR_NULL_POINTER("pcOut", ERROR);


	// Format suchen und Beschreibung kopieren
	switch(Format)
	{
	case D3DFMT_R8G8B8:			strcpy(pcOut, "R8G8B8"); break;
	case D3DFMT_A8R8G8B8:		strcpy(pcOut, "A8R8G8B8"); break;
	case D3DFMT_X8R8G8B8:		strcpy(pcOut, "X8R8G8B8"); break;
	case D3DFMT_R5G6B5:			strcpy(pcOut, "R5G6B5"); break;
	case D3DFMT_X1R5G5B5:		strcpy(pcOut, "X1R5G5B5"); break;
	case D3DFMT_A1R5G5B5:		strcpy(pcOut, "A1R5G5B5"); break;
	case D3DFMT_A4R4G4B4:		strcpy(pcOut, "A4R4G4B4"); break;
	case D3DFMT_A2B10G10R10:	strcpy(pcOut, "A2B10G10R10"); break;
	case D3DFMT_A16B16G16R16:	strcpy(pcOut, "A16B16G16R16"); break;
	case D3DFMT_G16R16:			strcpy(pcOut, "G16R16"); break;
	case D3DFMT_A8P8:			strcpy(pcOut, "A8P8"); break;
	case D3DFMT_P8:				strcpy(pcOut, "P8"); break;
	case D3DFMT_L8:				strcpy(pcOut, "L8"); break;
	case D3DFMT_A8L8:			strcpy(pcOut, "A8L8"); break;
	case D3DFMT_A4L4:			strcpy(pcOut, "A4L4"); break;
	
	case D3DFMT_V8U8:			strcpy(pcOut, "V8U8"); break;
	case D3DFMT_Q8W8V8U8:		strcpy(pcOut, "Q8W8V8U8"); break;
	case D3DFMT_V16U16:			strcpy(pcOut, "V16U16"); break;
	case D3DFMT_Q16W16V16U16:	strcpy(pcOut, "Q16W16V16U16"); break;
	
	case D3DFMT_L6V5U5:			strcpy(pcOut, "L6V5U5"); break;
	case D3DFMT_X8L8V8U8:		strcpy(pcOut, "X8L8V8U8"); break;
	case D3DFMT_A2W10V10U10:	strcpy(pcOut, "A2W10V10U10"); break;
	case D3DFMT_L16:			strcpy(pcOut, "L16"); break;

	case D3DFMT_UYVY:			strcpy(pcOut, "UYVY"); break;
	case D3DFMT_YUY2:			strcpy(pcOut, "YUY2"); break;
	case D3DFMT_DXT1:			strcpy(pcOut, "DXT1"); break;
	case D3DFMT_DXT2:			strcpy(pcOut, "DXT2"); break;
	case D3DFMT_DXT3:			strcpy(pcOut, "DXT3"); break;
	case D3DFMT_DXT4:			strcpy(pcOut, "DXT4"); break;
	case D3DFMT_DXT5:			strcpy(pcOut, "DXT5"); break;
	case D3DFMT_MULTI2_ARGB8:	strcpy(pcOut, "MULTI2_ARGB8"); break;
	
	case D3DFMT_D16_LOCKABLE:	strcpy(pcOut, "D16_LOCKABLE"); break;
	case D3DFMT_D32:			strcpy(pcOut, "D32"); break;
	case D3DFMT_D32F_LOCKABLE:	strcpy(pcOut, "D32F_LOCKABLE"); break;
	case D3DFMT_D24FS8:			strcpy(pcOut, "D24FS8"); break;
	case D3DFMT_D15S1:			strcpy(pcOut, "D15S1"); break;
	case D3DFMT_D24S8:			strcpy(pcOut, "D24S8"); break;
	case D3DFMT_D16:			strcpy(pcOut, "D16"); break;
	case D3DFMT_D24X8:			strcpy(pcOut, "D24X8"); break;
	case D3DFMT_D24X4S4:		strcpy(pcOut, "D24X4S4"); break;
	case D3DFMT_VERTEXDATA:		strcpy(pcOut, "VERTEXDATA"); break;
	case D3DFMT_INDEX16:		strcpy(pcOut, "INDEX16"); break;
	case D3DFMT_INDEX32:		strcpy(pcOut, "INDEX32"); break;

	case D3DFMT_UNKNOWN:		strcpy(pcOut, "UNKNOWN"); break;

	default: strcpy(pcOut, "Unbekanntes Format"); break;
	}

	return OK;
}

// ******************************************************************
// Diese Funktion liefert die Bits pro Pixel eines Formats.
DWORD GetD3DFormaPP(D3DFORMAT Format)
{
	// Format suchen
	switch(Format)
	{
	case D3DFMT_R8G8B8:			return 24;
	case D3DFMT_A8R8G8B8:		return 32;
	case D3DFMT_X8R8G8B8:		return 32;
	case D3DFMT_R5G6B5:			return 16;
	case D3DFMT_X1R5G5B5:		return 16;
	case D3DFMT_A1R5G5B5:		return 16;
	case D3DFMT_A4R4G4B4:		return 16;
	case D3DFMT_A2B10G10R10:	return 32;
	case D3DFMT_A16B16G16R16:	return 64;
	case D3DFMT_G16R16:			return 32;
	case D3DFMT_A8P8:			return 16;
	case D3DFMT_P8:				return 8;
	case D3DFMT_L8:				return 8;
	case D3DFMT_A8L8:			return 16;
	case D3DFMT_A4L4:			return 8;
	
	case D3DFMT_V8U8:			return 16;
	case D3DFMT_Q8W8V8U8:		return 32;
	case D3DFMT_V16U16:			return 32;
	case D3DFMT_Q16W16V16U16:	return 64;
	
	case D3DFMT_L6V5U5:			return 16;
	case D3DFMT_X8L8V8U8:		return 32;
	case D3DFMT_A2W10V10U10:	return 32;
	case D3DFMT_L16:			return 16;

	case D3DFMT_D16_LOCKABLE:	return 16;
	case D3DFMT_D32:			return 32;
	case D3DFMT_D32F_LOCKABLE:	return 32;
	case D3DFMT_D24FS8:			return 32;
	case D3DFMT_D15S1:			return 16;
	case D3DFMT_D24S8:			return 32;
	case D3DFMT_D16:			return 16;
	case D3DFMT_D24X8:			return 32;
	case D3DFMT_D24X4S4:		return 32;
	}

	return 0;
}

// ******************************************************************