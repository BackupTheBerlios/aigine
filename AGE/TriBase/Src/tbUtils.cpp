/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbUtils.cpp
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Verschiedene Hilfsfunktionen

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
TRIBASE_API char* tbRemoveDir(char* pcFilename)
{
	int iLastBackSlash;
	int iChar;


	// Letzten Back-Slash ("\") suchen
	iLastBackSlash = -1;
	iChar = 0;
	while(pcFilename[iChar] != 0)
	{
		if(pcFilename[iChar] == '\\') iLastBackSlash = iChar;
		iChar++;
	}

	// String ab dem letzten Back-Slash plus 1 zurückliefern
	return pcFilename + iLastBackSlash + 1;
}

// ******************************************************************
// Liefert die Erweiterung einer Datei (z.B. "cpp")
TRIBASE_API char* tbGetFilenameExtension(char* pcFilename)
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
TRIBASE_API BOOL tbFileExists(char* pcFilename)
{
	FILE* pFile;

	// Parameter prüfen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", FALSE);


	// Versuchen, die Datei zu öffnen
	pFile = fopen(pcFilename, "r");
	if(pFile != NULL) fclose(pFile);

	// pFile = NULL: Datei existiert nicht (oder Fehler), ansonsten existiert sie.
	return pFile != NULL;
}

// ******************************************************************
// Zufällige Ganzzahl
#if 0
TRIBASE_API int tbIntRandom(const int iMin,
							const int iMax)
{
	// Zufallszahl generieren
	return iMin + (rand() % (iMax - iMin + 1));
}

// ******************************************************************
// Zufällige Fließkommazahl
TRIBASE_API float tbFloatRandom(const float fMin,
								const float fMax)
{
	// Zufallszahl generieren
	return fMin + (fMax - fMin) * ((float)(rand() % 10001) / 10000.0f);
}
#endif

// ******************************************************************
// 2D-Positionsvektor transformieren
TRIBASE_API tbVector2 tbVector2TransformCoords(const tbVector2& v,
											   const tbMatrix& m,
											   float* pfOutW) // = NULL
{
	// Vektor mit Matrix transformieren
	tbVector2 vResult(v.x * m.m11 + v.y * m.m21 + m.m41,
		              v.x * m.m12 + v.y * m.m22 + m.m42);

	// Vierte Koordinate (w) berechnen. Wenn diese ungleich eins
	// ist, müssen die anderen Vektorelemente durch sie geteilt
	// werden.
	float w = v.x * m.m14 + v.y * m.m24 + m.m44;
	if(w != 1.0f) vResult /= w;

	// Wenn erwünscht, w kopieren
	if(pfOutW != NULL) *pfOutW = w;

	return tbVector2Normalize(vResult) * tbVector2Length(v);
}

// ******************************************************************
// 2D-Richtungsvektor transformieren
TRIBASE_API tbVector2 tbVector2TransformNormal(const tbVector2& v,
											   const tbMatrix& m)
{
	// Vektorlänge berechnen
	float fLength = tbVector2Length(v);
	if(fLength == 0.0f) return v;

	// Transponierte inverse Matrix berechnen
	tbMatrix mTransform(tbMatrixTranspose(tbMatrixInvert(m)));

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return tbVector2Normalize(tbVector2(v.x * mTransform.m11 + v.y * mTransform.m21,
		                                v.x * mTransform.m12 + v.y * mTransform.m22))
	       * fLength;
}

// ******************************************************************
// 2D-Richtungsvektor transformieren
TRIBASE_API tbVector2 tbVector2TransformNormal_TranspInv(const tbVector2& v,
														 const tbMatrix& m)
{
	// Vektorlänge berechnen
	float fLength = tbVector2Length(v);
	if(fLength == 0.0f) return v;

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return tbVector2Normalize(tbVector2(v.x * m.m11 + v.y * m.m21,
		                                v.x * m.m12 + v.y * m.m22))
	       * fLength;
}

// ******************************************************************
// 3D-Positionsvektor transformieren
TRIBASE_API tbVector3 tbVector3TransformCoords(const tbVector3& v,
											   const tbMatrix& m,
											   float* pfOutW) // = NULL
{
	// Vektor mit Matrix multiplizieren
	tbVector3 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + m.m41,
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
TRIBASE_API tbVector3 tbVector3TransformNormal(const tbVector3& v,
											   const tbMatrix& m)
{
	// Vektorlänge berechnen
	float fLength = tbVector3Length(v);
	if(fLength == 0.0f) return v;

	// Transponierte invertierte Matrix berechnen
	tbMatrix mTransform(tbMatrixTranspose(tbMatrixInvert(m)));

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return tbVector3Normalize(tbVector3(v.x * mTransform.m11 + v.y * mTransform.m21 + v.z * mTransform.m31,
		                                v.x * mTransform.m12 + v.y * mTransform.m22 + v.z * mTransform.m32,
										v.x * mTransform.m13 + v.y * mTransform.m23 + v.z * mTransform.m33))
		   * fLength;
}

// ******************************************************************
// 3D-Richtungsvektor transformieren
TRIBASE_API tbVector3 tbVector3TransformNormal_TranspInv(const tbVector3& v,
														 const tbMatrix& m)
{
	// Vektorlänge berechnen
	float fLength = tbVector3Length(v);
	if(fLength == 0.0f) return v;

	// Vektor mit Matrix transformieren und ursprüngliche Länge wiederherstellen
	return tbVector3Normalize(tbVector3(v.x * m.m11 + v.y * m.m21 + v.z * m.m31,
		                                v.x * m.m12 + v.y * m.m22 + v.z * m.m32,
										v.x * m.m13 + v.y * m.m23 + v.z * m.m33))
		   * fLength;
}

// ******************************************************************
// 4D-Positionsvektor transformieren
TRIBASE_API tbVector4 tbVector4TransformCoords(const tbVector4& v,
											   const tbMatrix& m,
											   float* pfOutW) // = NULL
{
	// Vektor mit Matrix multiplizieren
	tbVector4 vResult(v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + v.w * m.m41,
		              v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + v.w * m.m42,
					  v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + v.w * m.m43,
					  v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + v.w * m.m44);

	// Wenn erwünscht, w berechnen und kopieren
	if(pfOutW != NULL)
	{
		float w = v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + v.w * m.m44;
		*pfOutW = w;
	}

	return vResult;
}

// ******************************************************************
// Projektion eines 3D-Positionsvektors
TRIBASE_API tbVector3 tbVector3Project(const tbVector3& v,
									   const tbMatrix& mWorldViewProj)
{
	tbVector3 vTemp(tbVector3TransformCoords(v, mWorldViewProj));

	return tbVector3(tbDirect3D::GetScreenSize().x * 0.5f + (vTemp.x * tbDirect3D::GetScreenSize().x * 0.5f),
					 tbDirect3D::GetScreenSize().y * 0.5f - (vTemp.y * tbDirect3D::GetScreenSize().y * 0.5f),
					 vTemp.z);
}

// ******************************************************************
// Ebene mit einer Matrix transformieren
TRIBASE_API tbPlane tbPlaneTransform(const tbPlane& p,
									 const tbMatrix& m)
{
	float a = p.a * m.m11 + p.b * m.m21 + p.c * m.m31;
	float b = p.a * m.m12 + p.b * m.m22 + p.c * m.m32;
	float c = p.a * m.m13 + p.b * m.m23 + p.c * m.m33;

	return tbPlane(a, b, c,
				   p.d - (a * m.m41 + b * m.m42 + c * m.m43));
}

// ******************************************************************
// Normalvektor eines Dreiecks berechnen
TRIBASE_API tbVector3 tbComputeTriangleNormal(const tbVector3& v1,
											  const tbVector3& v2,
											  const tbVector3& v3)
{
	// Normalisiertes Kreuzprodukt der Verbindungsvektoren berechnen
	return tbVector3Normalize(tbVector3Cross(v2 - v1, v3 - v1));
}

// ******************************************************************
// Berechnen der sechs Ebenen eines frei drehbaren Quaders
TRIBASE_API tbResult tbComputeBoxPlanes(const tbVector3& vBoxMin,
										const tbVector3& vBoxMax,
										const tbMatrix& mBoxTransformation,
										tbPlane* pOut)
{
	// Die sechs Ebenen berechnen, ohne die Drehung zu berücksichtigen
	pOut[0] = tbPlaneFromPointNormal(vBoxMin, tbVector3(-1.0f,  0.0f,  0.0f));	// Linke Ebene
	pOut[1] = tbPlaneFromPointNormal(vBoxMax, tbVector3( 1.0f,  0.0f,  0.0f));	// Rechte Ebene
	pOut[2] = tbPlaneFromPointNormal(vBoxMax, tbVector3( 0.0f,  1.0f,  0.0f));	// Obere Ebene
	pOut[3] = tbPlaneFromPointNormal(vBoxMin, tbVector3( 0.0f, -1.0f,  0.0f));	// Untere Ebene
	pOut[4] = tbPlaneFromPointNormal(vBoxMin, tbVector3( 0.0f,  0.0f, -1.0f));	// Vordere Ebene
	pOut[5] = tbPlaneFromPointNormal(vBoxMax, tbVector3( 0.0f,  0.0f,  1.0f));	// Hintere Ebene

	// Alle Ebenen transformieren
	for(int iPlane = 0; iPlane < 6; iPlane++)
	{
		// Transformieren
		pOut[iPlane] = tbPlaneTransform(pOut[iPlane], mBoxTransformation);
	}

	return TB_OK;
}

// ******************************************************************
// Berechnen des View-Frustums anhand der Projektionsmatrix und der Sichtmatrix
TRIBASE_API tbResult tbComputeClipPlanes(const tbMatrix& mView,
										 const tbMatrix& mProjection,
										 tbPlane* pOut)
{
	// Sichtmatrix mit der Projektionsmatrix multiplizieren
	tbMatrix mTemp(mView * mProjection);

	// Linke Clippingebene
	pOut[0].a = -(mTemp.m14 + mTemp.m11);
	pOut[0].b = -(mTemp.m24 + mTemp.m21);
	pOut[0].c = -(mTemp.m34 + mTemp.m31);
	pOut[0].d = -(mTemp.m44 + mTemp.m41);
	pOut[0] = tbPlaneNormalize(pOut[0]);

	// Rechte Clippingebene
	pOut[1].a = -(mTemp.m14 - mTemp.m11);
	pOut[1].b = -(mTemp.m24 - mTemp.m21);
	pOut[1].c = -(mTemp.m34 - mTemp.m31);
	pOut[1].d = -(mTemp.m44 - mTemp.m41);
	pOut[1] = tbPlaneNormalize(pOut[1]);

	// Obere Clippingebene
	pOut[2].a = -(mTemp.m14 - mTemp.m12);
	pOut[2].b = -(mTemp.m24 - mTemp.m22);
	pOut[2].c = -(mTemp.m34 - mTemp.m32);
	pOut[2].d = -(mTemp.m44 - mTemp.m42);
	pOut[2] = tbPlaneNormalize(pOut[2]);

	// Untere Clippingebene
	pOut[3].a = -(mTemp.m14 + mTemp.m12);
	pOut[3].b = -(mTemp.m24 + mTemp.m22);
	pOut[3].c = -(mTemp.m34 + mTemp.m32);
	pOut[3].d = -(mTemp.m44 + mTemp.m42);
	pOut[3] = tbPlaneNormalize(pOut[3]);

	// Nahe Clippingebene
	pOut[4].a = -(mTemp.m14 + mTemp.m13);
	pOut[4].b = -(mTemp.m24 + mTemp.m23);
	pOut[4].c = -(mTemp.m34 + mTemp.m33);
	pOut[4].d = -(mTemp.m44 + mTemp.m43);
	pOut[4] = tbPlaneNormalize(pOut[4]);

	// Ferne Clippingebene
	pOut[5].a = -(mTemp.m14 - mTemp.m13);
	pOut[5].b = -(mTemp.m24 - mTemp.m23);
	pOut[5].c = -(mTemp.m34 - mTemp.m33);
	pOut[5].d = -(mTemp.m44 - mTemp.m43);
	pOut[5] = tbPlaneNormalize(pOut[5]);

	return TB_OK;
}

// ******************************************************************
// Minimale Entfernung zwischen Punkt und Ebene berechnen
TRIBASE_API float tbPointPlaneDistance(const tbVector3& vPoint,
									   const tbPlane& Plane)
{
	// Die Ebene normalisieren und dann den Punkt einsetzen
	return tbPlaneDotCoords(tbPlaneNormalize(Plane), vPoint);
}

// ******************************************************************
// Minimale Entfernung zwischen Punkt und Ebene berechnen (schnell)
TRIBASE_API float tbPointPlaneDistanceFast(const tbVector3& vPoint,
										   const tbPlane& Plane)
{
	// Den Punkt in die Gleichung einsetzen
	return Plane.a * vPoint.x + Plane.b * vPoint.y + Plane.c * vPoint.z + Plane.d;
}

// ******************************************************************
// Minimale Distanz zwischen Punkt und Linie berechnen
TRIBASE_API float tbPointLineDistance(const tbVector3& vPoint,
									  const tbVector3& vLineA,
									  const tbVector3& vLineB)
{
	tbVector3	v(vLineB - vLineA);
	tbVector3	w(vPoint - vLineA);


	return tbVector3Length(vLineA + v * (tbVector3Dot(w, v) / tbVector3LengthSq(v)) - vPoint);
}

// ******************************************************************
// Minimale Distanz zwischen zwei Linien berechnen
TRIBASE_API float tbLineLineDistance(const tbVector3& vLine1A,
									 const tbVector3& vLine1B,
									 const tbVector3& vLine2A,
									 const tbVector3& vLine2B)
{
	tbVector3	u(vLine1B - vLine1A);
    tbVector3	v(vLine2B - vLine1A);
	tbVector3	w(vLine1A - vLine2A);
	float		a = tbVector3LengthSq(u);
	float		c = tbVector3LengthSq(v);
	float		b = tbVector3Dot(u, v);
	float		d = tbVector3Dot(u, w);
	float		e = tbVector3Dot(v, w);
	float		D = a * c - b * b;


	if(D < 0.0001f)
	{
		float sc = 0.0f;
		float tc = b > c ? d / b : e / c;

		// Verbindungsvektor zwischen den zwei am nächsten liegenden
		// Punkten auf den Linien berechnen (L1(sc) - L2(tc))
		// Die Länge dieses Vektors ist die minimale Distanz.
		return tbVector3Length(w + (u * sc) - (v * tc));
	}
	else
	{
		float sc = (b * e - c * d) / D;
		float tc = (a * e - b * d) / D;

		// Verbindungsvektor zwischen den zwei am nächsten liegenden
		// Punkten auf den Linien berechnen (L1(sc) - L2(tc))
		// Die Länge dieses Vektors ist die minimale Distanz.
		return tbVector3Length(w + (u * sc) - (v * tc));
	}
}

// ******************************************************************
// Liegt ein Punkt innerhalb einer Kugel?
TRIBASE_API BOOL tbPointHitsSphere(const tbVector3& vPoint,
								   const tbVector3& vSphere,
								   const float fRadius)
{
	// Wenn die Distanz zwischen Punkt und dem Mittelpunkt der
	// Kugel kleiner gleich dem Radius ist, liegt der Punkt in
	// der Kugel.
	return tbVector3LengthSq(vPoint - vSphere) <= fRadius * fRadius;
}

// ******************************************************************
// Liegt ein Punkt in einem achsenausgerichteten Quader?
TRIBASE_API BOOL tbPointHitsAABox(const tbVector3& vPoint,
								  const tbVector3& vBoxMin,
								  const tbVector3& vBoxMax)
{
	return vPoint.x >= vBoxMin.x && vPoint.x <= vBoxMax.x &&
		   vPoint.y >= vBoxMin.y && vPoint.y <= vBoxMax.y &&
		   vPoint.z >= vBoxMin.z && vPoint.z <= vBoxMax.z;
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
TRIBASE_API BOOL tbPointHitsBox(const tbVector3& vPoint,
								const tbPlane* pBoxPlanes)
{
	// Sobald der Punkt auf der Vorderseite auch nur einer einzigen der sechs
	// Ebenen des Quaders liegt, liegt er nicht mehr innerhalb des Quaders.
	// Es gibt eine kleine Toleranzgrenze.
	if(tbPlaneDotCoords(pBoxPlanes[0], vPoint) > 0.0001f) return FALSE;
	if(tbPlaneDotCoords(pBoxPlanes[1], vPoint) > 0.0001f) return FALSE;
	if(tbPlaneDotCoords(pBoxPlanes[2], vPoint) > 0.0001f) return FALSE;
	if(tbPlaneDotCoords(pBoxPlanes[3], vPoint) > 0.0001f) return FALSE;
	if(tbPlaneDotCoords(pBoxPlanes[4], vPoint) > 0.0001f) return FALSE;
	return tbPlaneDotCoords(pBoxPlanes[5], vPoint) <= 0.0001f;
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
TRIBASE_API BOOL tbPointHitsBox(const tbVector3& vPoint,
								const tbVector3& vBoxMin,
								const tbVector3& vBoxMax,
								const tbMatrix& mBoxTransformation)
{
	tbPlane aBoxPlane[6];

	// Die sechs Ebenen berechnen
	tbComputeBoxPlanes(vBoxMin, vBoxMax, mBoxTransformation, aBoxPlane);

	// Die andere Funktion aufrufen
	return tbPointHitsBox(vPoint, aBoxPlane);
}

// ******************************************************************
// Liegt ein Punkt in einem frei drehbaren Quader?
// Diese Funktion verwendet die invertierte Matrix, um das herauszufinden.
TRIBASE_API BOOL tbPointHitsBox2(const tbVector3& vPoint,
								 const tbVector3& vBoxMin,
								 const tbVector3& vBoxMax,
								 const tbMatrix& mBoxTransformation)
{
	// Punkt mit der invertierten Matrix transformieren
	tbVector3 vNewPoint(tbVector3TransformCoords(vPoint, tbMatrixInvert(mBoxTransformation)));

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
TRIBASE_API BOOL tbPointHitsBox2_Inv(const tbVector3& vPoint,
									 const tbVector3& vBoxMin,
									 const tbVector3& vBoxMax,
									 const tbMatrix& mInvBoxTransformation)
{
	// Punkt mit der invertierten Matrix transformieren
	tbVector3 vNewPoint(tbVector3TransformCoords(vPoint, mInvBoxTransformation));

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
TRIBASE_API BOOL tbLineHitsPlane(const tbVector3& vLineA,
								 const tbVector3& vLineB,
								 const tbPlane& Plane,
								 tbVector3* pvOut) // = NULL
{
	// Richtungsvektor der Linie berechnen
	tbVector3 vLineDir(vLineB - vLineA);

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
TRIBASE_API float tbLineHitsPlaneS(const tbVector3& vLineA,
								   const tbVector3& vLineB,
								   const tbPlane& Plane)
{
	// Richtungsvektor der Linie berechnen
	tbVector3 vLineDir(vLineB - vLineA);

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
TRIBASE_API BOOL tbLineHitsPlaneFast(const tbVector3& vLineA,
									 const tbVector3& vLineB,
									 const tbPlane& Plane)
{
	// Es wird kein exakter Schnittpunkt verlangt - daher kann eine schnellere
	// Methode angewandt werden.

	// Punktprodukte zwischen den Linienpunkten und der Ebene bilden
	float d1 = tbPlaneDotCoords(Plane, vLineA);
	float d2 = tbPlaneDotCoords(Plane, vLineB);

	// Liegen die beiden Linienpunkte auf verschiedenen Seiten oder beide
	// direkt auf der Ebene? Falls ja, gibt es einen Schnittpunkt.
	if(d1 <= 0.0f) if(d2 >= 0.0f) return TRUE;
	if(d1 >= 0.0f) if(d2 <= 0.0f) return TRUE;

	// Kein Schnittpunkt!
	return FALSE;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einer Ebene berechnen (schnell)
TRIBASE_API tbVector3 tbLineHitsPlaneFastEx(const tbVector3& vLineA,
											const tbVector3& vLineB,
											const tbPlane& Plane)
{
	// Richtungsvektor der Linie berechnen
	tbVector3 vLineDir(vLineB - vLineA);

	// Schnittpunkt liefern
	return   vLineA
	       + (  (-Plane.a * vLineA.x - Plane.b * vLineA.y - Plane.c * vLineA.z - Plane.d)
		      / (Plane.a * vLineDir.x + Plane.b * vLineDir.y + Plane.c * vLineDir.z))
		   * vLineDir;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen
TRIBASE_API BOOL tbLineHitsTriangle(const tbVector3& vLineA,
									const tbVector3& vLineB,
									const tbVector3& vTriangleA,
									const tbVector3& vTriangleB,
									const tbVector3& vTriangleC,
									tbVector3* pvOut) // = NULL
{
	// Die Ebene dieses Dreiecks berechnen
	tbPlane Plane(tbPlaneFromPoints(vTriangleA, vTriangleB, vTriangleC));

	// Schnittpunkt der Linie mit der Ebene bestimmen
	tbVector3 vIntersection;
	if(!tbLineHitsPlane(vLineA, vLineB, Plane, &vIntersection))
	{
		// Sie schneidet die Ebene nicht - dann kann sie das Dreieck
		// erst recht nicht schneiden.
		return FALSE;
	}

	// Erstellen der ersten Ebene entlang den Punkten A und B und
	// Einsetzen des Punkts in die Gleichung. Falls das Ergebnis kleiner
	// als null ist, liegt der Punkt hinter der Ebene und damit nicht im
	// Dreieck.
	tbVector3 vTemp(tbVector3Cross(vTriangleA - vTriangleB, Plane.n));
	Plane = tbPlaneFromPointNormal(vTriangleA, vTemp);
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return FALSE;

	// Test mit der zweiten Ebene entlang den Punkten B und C
	vTemp = tbVector3Cross(vTriangleB - vTriangleC, Plane.n);
	Plane = tbPlaneFromPointNormal(vTriangleB, vTemp);
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return FALSE;

	// Test mit der dritten Ebene entlang den Punkten C und A
	vTemp = tbVector3Cross(vTriangleC - vTriangleA, Plane.n);
	Plane = tbPlaneFromPointNormal(vTriangleC, vTemp);
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return FALSE;

	// Der Punkt liegt im Dreieck, da er vor allen drei Ebenen liegt!
	// Schnittpunkt einsetzen.
	if(pvOut != NULL) *pvOut = vIntersection;

	return TRUE;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen (Linienabschnitt)
TRIBASE_API float tbLineHitsTriangleS(const tbVector3& vLineA,
									  const tbVector3& vLineB,
									  const tbVector3& vTriangleA,
									  const tbVector3& vTriangleB,
									  const tbVector3& vTriangleC)
{
	// Die Ebene dieses Dreiecks berechnen
	tbPlane Plane(tbPlaneFromPoints(vTriangleA, vTriangleB, vTriangleC));

	// Schneidet die Linie die Ebene nicht?
	float fIntersection = tbLineHitsPlaneS(vLineA, vLineB, Plane);
	if(fIntersection < 0.0f)
	{
		// Sie schneidet die Ebene nicht - dann kann sie das Dreieck
		// erst recht nicht schneiden.
		return -1.0f;
	}

	// Schnittpunkt berechnen
	tbVector3 vIntersection(vLineA + (vLineB - vLineA) * fIntersection);

	// Erstellen der ersten Ebene entlang den Punkten A und B und
	// Einsetzen des Punkts in die Gleichung. Falls das Ergebnis kleiner
	// als null ist, liegt der Punkt hinter der Ebene und damit nicht im
	// Dreieck.
	tbVector3 vTemp(tbVector3Cross(vTriangleA - vTriangleB, Plane.n));
	Plane = tbPlaneFromPointNormal(vTriangleA, vTemp);
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return -1.0f;

	// Test mit der zweiten Ebene entlang den Punkten B und C
	vTemp = tbVector3Cross(vTriangleB - vTriangleC, Plane.n);
	Plane = tbPlaneFromPointNormal(vTriangleA, vTemp);
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return -1.0f;

	// Test mit der dritten Ebene entlang den Punkten C und A
	vTemp = tbVector3Cross(vTriangleC - vTriangleA, Plane.n);
	Plane = tbPlaneFromPointNormal(vTriangleA, vTemp);
	if(tbPlaneDotCoords(Plane, vIntersection) < 0.0f) return -1.0f;

	// Der Punkt liegt im Dreieck, da er vor allen drei Ebenen liegt!
	// Schnittstelle liefern.
	return fIntersection;
}

// ******************************************************************
// Schnittpunkt zwischen einer Linie und einer Kugel berechnen
TRIBASE_API BOOL tbLineHitsSphere(const tbVector3& vLineA,
								  const tbVector3& vLineB,
								  const tbVector3& vSphere,
								  const float fRadius,
								  tbVector3* pvOut) // = NULL
{
	// Test: liegt Punkt A in der Kugel?
	if(tbPointHitsSphere(vLineA, vSphere, fRadius)) {if(pvOut != NULL) *pvOut = vLineA; return TRUE;}

	// Länge und normalisierte Richtung der Linie berechnen
	float fLineLength = tbVector3Length(vLineB - vLineA);
	tbVector3 vLineDir((vLineB - vLineA) / fLineLength);

	// Die zwei Hilfsvariablen a und b berechnen
	tbVector3 vDiff(vLineA - vSphere);
	float a = 2.0f * (vLineDir.x * vDiff.x + vLineDir.y * vDiff.y + vLineDir.z * vDiff.z);
	float b = tbVector3LengthSq(vDiff) - fRadius * fRadius;

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
TRIBASE_API BOOL tbLineHitsSphereFast(const tbVector3& vLineA,
									  const tbVector3& vLineB,
									  const tbVector3& vSphere,
									  const float fRadius)
{
	// Minimale Distanz zwischen der Linie und dem Mittelpunkt
	// der Kugel berechnen (den Abschnitt auf der Linie)
	tbVector3 v(vLineB - vLineA);
	tbVector3 w(vSphere - vLineA);
	float s = tbVector3Dot(w, v) / tbVector3LengthSq(v);

	// Wenn der Punkt s der Linie innerhalb der Kugel liegt,
	// schneidet die Linie die Kugel.
	return tbVector3LengthSq(vLineA + v * s - vSphere) < fRadius * fRadius;
}

// ******************************************************************
// Schnittpunkt zwischen Linie und frei drehbarem Quader berechnen
TRIBASE_API BOOL tbLineHitsBox(const tbVector3& vLineA,
							   const tbVector3& vLineB,
							   const tbPlane* pBoxPlanes,
							   tbVector3* pvOut) // = NULL
{
	// Wenn der Linienstartpunkt im Quader liegt, setzen wir
	// den Schnittpunkt einfach auf den Linienstartpunkt.
	if(tbPointHitsBox(vLineA, pBoxPlanes))
	{
		if(pvOut != NULL) *pvOut = vLineA;
		return TRUE;
	}

	// Linienrichtung berechnen
	tbVector3 vLineDir(vLineB - vLineA);

	float		fNearestIntersection = 100.0f;
	float		fIntersection;
	tbVector3	vIntersection;			// Schnittpunktkoordinaten
	tbVector3	vNearestIntersection;	// Koordinaten des nächsten Schnittpunkts

	// Jede Ebene durchgehen und prüfen, ob sie von der Linie geschnitten wird.
	// Es wird der nächste Schnittpunkt (niedrigster Linienabschnitt) gesucht.
	for(int iPlane = 0; iPlane < 6; iPlane++)
	{
		fIntersection = tbLineHitsPlaneS(vLineA, vLineB, pBoxPlanes[iPlane]);
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
				if(tbPointHitsBox(vIntersection, pBoxPlanes))
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
TRIBASE_API BOOL tbLineHitsBox(const tbVector3& vLineA,
							   const tbVector3& vLineB,
							   const tbVector3& vBoxMin,
							   const tbVector3& vBoxMax,
							   const tbMatrix& mBoxTransformation,
							   tbVector3* pvOut) // = NULL
{
	tbPlane aBoxPlane[6];

	// Die sechs Ebenen des Quaders berechnen
	tbComputeBoxPlanes(vBoxMin, vBoxMax, mBoxTransformation, aBoxPlane);

	// Die andere Funktion aufrufen
	return tbLineHitsBox(vLineA, vLineB, aBoxPlane, pvOut);
}

// ******************************************************************
// Berechnung der Kollision zwischen Linie und Modell (rekursiv)
TRIBASE_API BOOL tbLineHitsModelRec(const tbVector3& vLineA,
									const tbVector3& vLineB,
									const tbModel* pModel,
									const tbModelOctreeNode* pNode,
									int* piClosestTriangle,
									float* pfClosestCollision,
									BOOL bSimpleMode,
									BOOL* pbStop)
{
	// Prüfen, ob die Linie den Umgebungsquader des Knotens trifft.
	// Falls nicht, können wir sofort abbrechen
	if(!tbLineHitsBox(vLineA, vLineB,
		              pNode->vBoundingBoxMin,
					  pNode->vBoundingBoxMax,
					  tbMatrixIdentity()))
	{
		// Abbruch!
		return FALSE;
	}

	// Ist dieser Knoten ein Endknoten? Falls ja, dann testen wir die
	// Kollision jetzt auf Dreiecksebene.
	if(pNode->bIsLeaf)
	{
		float		fCollision;	// Linienabschnitt des Schnittpunkts
		tbVector3	vCollision;	// Position des Schnittpunkts

		// Nun jedes Dreieck dieses Knotens durchgehen und
		// nach der nächsten Kollisionen suchen
		for(DWORD t = 0; t < pNode->dwNumTriangles; t++)
		{
			// Schneidet die Linie das Dreieck?
			// Dazu berechnen wir zuerst den Schnittpunkt zwischen der Linie und der
			// ersten Ebene des Dreiecks.
			fCollision = tbLineHitsPlaneS(vLineA, vLineB, pModel->m_pTrianglePlanes[pNode->pdwTriangles[t] * 4]);
			if(fCollision >= 0.0f)
			{
				// Die erste Ebene wird geschnitten.
				// Der Schnittpunkt muss nun auf der Vorderseite der drei anderen
				// Ebenen des Dreiecks liegen, damit eine Kollision vorliegt.
				vCollision = vLineA + (vLineB - vLineA) * fCollision;
				if(tbPlaneDotCoords(pModel->m_pTrianglePlanes[pNode->pdwTriangles[t] * 4 + 1], vCollision) < 0.0f) continue;
				if(tbPlaneDotCoords(pModel->m_pTrianglePlanes[pNode->pdwTriangles[t] * 4 + 2], vCollision) < 0.0f) continue;
				if(tbPlaneDotCoords(pModel->m_pTrianglePlanes[pNode->pdwTriangles[t] * 4 + 3], vCollision) < 0.0f) continue;

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
			tbLineHitsModelRec(vLineA, vLineB, pModel, pNode->apChild[i],
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
TRIBASE_API BOOL tbLineHitsModel(const tbVector3& vLineA,
								 const tbVector3& vLineB,
								 const tbModel* pModel,
								 const tbMatrix& mMatrix,
								 const tbMatrix& mInvMatrix,
								 const float fTolerance,	// = 0.0f
								 tbVector3* pvOutPos,		// = NULL
								 tbVector3* pvOutNormal,	// = NULL
								 int* piOutTriangle)		// = NULL
{
	// Parameter prüfen
	if(pModel == NULL)			TB_ERROR_NULL_POINTER("pModel", TB_ERROR);
	if(!pModel->m_bExtraData)	TB_ERROR("Das Modell hat keine Extradaten!", TB_ERROR);


	// Startwerte setzen
	int iClosestTriangle = -1;
	float fClosestCollision = 100000.0f;
	BOOL bStop = FALSE;

	// Erst einmal transformieren wir die Linie mit der inversen Transformationsmatrix,
	// um sie in "Model-Space" umzurechnen. Dann müssen wir später die Modellvertizes
	// nicht transformieren, sparen also eine Menge Rechenzeit.
	tbVector3 vNewLineA(tbVector3TransformCoords(vLineA, mInvMatrix));
	tbVector3 vNewLineB(tbVector3TransformCoords(vLineB, mInvMatrix));

	// Start und Ende der Linie ein wenig auseinander schieben - je nach Toleranz
	vNewLineA -= tbVector3NormalizeEx(vNewLineB - vNewLineA) * fTolerance;
	vNewLineB += tbVector3NormalizeEx(vNewLineB - vNewLineA) * fTolerance;

	// Rekursive Funktion aufrufen
	tbLineHitsModelRec(vNewLineA, vNewLineB, pModel, pModel->m_pRootNode,
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
			*pvOutNormal = tbVector3TransformNormal(pModel->m_pTrianglePlanes[iClosestTriangle * 4].n,
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
TRIBASE_API BOOL tbLineHitsOctreeRec(const tbVector3& vLineA,
									 const tbVector3& vLineB,
									 const tbOctree* pOctree,
									 const tbOctreeNode* pNode,
									 int* piClosestTriangle,
									 float* pfClosestCollision,
									 tbOctreeNode** ppClosestNode,
									 BOOL bSimpleMode,
									 BOOL* pbStop)
{
	// Prüfen, ob die Linie den Umgebungsquader des Knotens trifft.
	// Falls nicht, können wir sofort abbrechen
	if(!tbLineHitsBox(vLineA, vLineB,
		              pNode->vBoundingBoxMin,
					  pNode->vBoundingBoxMax,
					  tbMatrixIdentity()))
	{
		// Abbruch!
		return FALSE;
	}

	// Ist dieser Knoten ein Endknoten? Falls ja, dann testen wir die
	// Kollision jetzt auf Dreiecksebene.
	if(pNode->bIsLeaf)
	{
		float		fCollision;	// Linienabschnitt des Schnittpunkts
		tbVector3	vCollision;	// Position des Schnittpunkts

		// Nun jedes Dreieck dieses Knotens durchgehen und
		// nach der nächsten Kollisionen suchen
		for(DWORD t = 0; t < pNode->dwNumIndices / 3; t++)
		{
			// Schneidet die Linie das Dreieck?
			// Dazu berechnen wir zuerst den Schnittpunkt zwischen der Linie und der
			// ersten Ebene des Dreiecks.
			fCollision = tbLineHitsPlaneS(vLineA, vLineB, pNode->pTrianglePlanes[t * 4]);
			if(fCollision >= 0.0f)
			{
				// Die erste Ebene wird geschnitten.
				// Der Schnittpunkt muss nun auf der Vorderseite der drei anderen
				// Ebenen des Dreiecks liegen, damit eine Kollision vorliegt.
				vCollision = vLineA + (vLineB - vLineA) * fCollision;
				if(tbPlaneDotCoords(pNode->pTrianglePlanes[t * 4 + 1], vCollision) < 0.0f) continue;
				if(tbPlaneDotCoords(pNode->pTrianglePlanes[t * 4 + 2], vCollision) < 0.0f) continue;
				if(tbPlaneDotCoords(pNode->pTrianglePlanes[t * 4 + 3], vCollision) < 0.0f) continue;

				// Kollision! Bricht sie den Rekord?
				if(fCollision < *pfClosestCollision)
				{
					*pfClosestCollision = fCollision;
					*piClosestTriangle = t;
					*ppClosestNode = (tbOctreeNode*)(pNode);
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
			tbLineHitsOctreeRec(vLineA, vLineB, pOctree, pNode->apChild[i],
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
TRIBASE_API BOOL tbLineHitsOctree(const tbVector3& vLineA,
								  const tbVector3& vLineB,
								  const tbOctree* pOctree,
								  const float fTolerance,	// = 0.0f
								  tbVector3* pvOutPos,		// = NULL
								  tbVector3* pvOutNormal,	// = NULL
								  int* piOutTriangle,		// = NULL
								  tbOctreeNode** ppOutNode)	// = NULL
{
	// Parameter prüfen
	if(pOctree == NULL)			TB_ERROR_NULL_POINTER("pOctree", TB_ERROR);
	if(!pOctree->m_bExtraData)	TB_ERROR("Der Octree hat keine Extradaten!", TB_ERROR);


	// Startwerte setzen
	int iClosestTriangle = -1;
	float fClosestCollision = 100000.0f;
	BOOL bStop = FALSE;
	tbOctreeNode* pNode;

	// Start und Ende der Linie ein wenig auseinander schieben - je nach Toleranz
	tbVector3 vNewLineA(vLineA - tbVector3NormalizeEx(vLineB - vLineA) * fTolerance);
	tbVector3 vNewLineB(vLineB + tbVector3NormalizeEx(vLineB - vLineA) * fTolerance);

	// Rekursive Funktion aufrufen
	tbLineHitsOctreeRec(vNewLineA, vNewLineB, pOctree, pOctree->m_pRootNode,
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
TRIBASE_API BOOL tbModelHitsModelRec(const tbModel* pModelA,
									 const tbMatrix& mMatrixA,
									 const tbMatrix& mInvMatrixA,
									 const tbModelOctreeNode* pNodeA,
									 const tbModel* pModelB,
									 const tbMatrix& mMatrixB,
									 const tbMatrix& mInvMatrixB,
									 const tbModelOctreeNode* pNodeB,
									 tbVector3* pvOutPos,
									 tbVector3* pvOutNormalA,
									 tbVector3* pvOutNormalB,
									 int* piOutTriA,
									 int* piOutTriB,
									 BOOL bSkipBBTest)
{
	// Schneiden sich die Umgebungsquader der beiden Knoten?
	// Falls sie es nicht tun, kann es hier keine Kollision geben.
	if(!bSkipBBTest)
	{
		if(!tbBoxHitsBox(pNodeA->vBoundingBoxMin, pNodeA->vBoundingBoxMax, mMatrixA,
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
			return tbModelHitsModelRec(pModelB, mMatrixB, mInvMatrixB, pNodeB,
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
		tbMatrix mTransformation(mMatrixB * mInvMatrixA);

		tbVector3*	pvVector;
		tbVector3	vTriA;
		tbVector3	vTriB;
		tbVector3	vTriC;
		tbVector3	vHitStart;
		tbVector3	vHitEnd;

		// Alle Dreiecke von Knoten B durchgehen
		for(DWORD tb = 0; tb < pNodeB->dwNumTriangles; tb++)
		{
			// Ersten Vektor dieses Dreiecks von Knoten B transformieren
			pvVector = &pModelB->m_pvVectors[pModelB->m_pdwIndices[pNodeB->pdwTriangles[tb] * 3]];
			vTriA.x = pvVector->x * mTransformation.m11 + pvVector->y * mTransformation.m21 + pvVector->z * mTransformation.m31 + mTransformation.m41;
			vTriA.y = pvVector->x * mTransformation.m12 + pvVector->y * mTransformation.m22 + pvVector->z * mTransformation.m32 + mTransformation.m42;
			vTriA.z = pvVector->x * mTransformation.m13 + pvVector->y * mTransformation.m23 + pvVector->z * mTransformation.m33 + mTransformation.m43;

			// Zweiten Vektor dieses Dreiecks von Knoten B transformieren
			pvVector = &pModelB->m_pvVectors[pModelB->m_pdwIndices[pNodeB->pdwTriangles[tb] * 3 + 1]];
			vTriB.x = pvVector->x * mTransformation.m11 + pvVector->y * mTransformation.m21 + pvVector->z * mTransformation.m31 + mTransformation.m41;
			vTriB.y = pvVector->x * mTransformation.m12 + pvVector->y * mTransformation.m22 + pvVector->z * mTransformation.m32 + mTransformation.m42;
			vTriB.z = pvVector->x * mTransformation.m13 + pvVector->y * mTransformation.m23 + pvVector->z * mTransformation.m33 + mTransformation.m43;

			// Dritten Vektor dieses Dreiecks von Knoten B transformieren
			pvVector = &pModelB->m_pvVectors[pModelB->m_pdwIndices[pNodeB->pdwTriangles[tb] * 3 + 2]];
			vTriC.x = pvVector->x * mTransformation.m11 + pvVector->y * mTransformation.m21 + pvVector->z * mTransformation.m31 + mTransformation.m41;
			vTriC.y = pvVector->x * mTransformation.m12 + pvVector->y * mTransformation.m22 + pvVector->z * mTransformation.m32 + mTransformation.m42;
			vTriC.z = pvVector->x * mTransformation.m13 + pvVector->y * mTransformation.m23 + pvVector->z * mTransformation.m33 + mTransformation.m43;

			// Alle Dreiecke von Knoten A durchgehen
			for(DWORD ta = 0; ta < pNodeA->dwNumTriangles; ta++)
			{
				// Kollidieren diese beiden Dreiecke?
				if(tbTriangleHitsTriangle(pModelA->m_pvVectors[pModelA->m_pdwIndices[pNodeA->pdwTriangles[ta] * 3]],
										  pModelA->m_pvVectors[pModelA->m_pdwIndices[pNodeA->pdwTriangles[ta] * 3 + 1]],
										  pModelA->m_pvVectors[pModelA->m_pdwIndices[pNodeA->pdwTriangles[ta] * 3 + 2]],
										  vTriA, vTriB, vTriC,
										  &vHitStart,
										  &vHitEnd))
				{
					// Es gibt eine Kollision!
					// Wenn der Ort der Kollision gefragt ist: ausfüllen!
					// Der Vektor muss aber noch mit der Matrix von Modell A transformiert werden.
					if(pvOutPos != NULL) *pvOutPos = tbVector3TransformCoords(0.5f * (vHitStart + vHitEnd), mMatrixA);

					// Sind die beiden Dreiecke gefragt?
					if(piOutTriA != NULL) *piOutTriA = (int)(pNodeA->pdwTriangles[ta]);
					if(piOutTriB != NULL) *piOutTriB = (int)(pNodeB->pdwTriangles[tb]);

					// Sind die Normalvektoren gefragt?
					// Transformation mit der Matrix des jeweiligen Objekts ist notwendig.
					// Wir erhalten die Normalvektoren aus den Ebenengleichungen der Dreiecke.
					if(pvOutNormalA != NULL) *pvOutNormalA = tbVector3TransformNormal(pModelA->m_pTrianglePlanes[pNodeA->pdwTriangles[ta] * 4].n, mMatrixA);
					if(pvOutNormalB != NULL) *pvOutNormalB = tbVector3TransformNormal(pModelB->m_pTrianglePlanes[pNodeB->pdwTriangles[tb] * 4].n, mMatrixB);

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
			if(tbModelHitsModelRec(pModelA, mMatrixA, mInvMatrixA, pNodeA,
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
			if(tbModelHitsModelRec(pModelA, mMatrixA, mInvMatrixA, pNodeA->apChild[i],
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
			if(tbModelHitsModelRec(pModelA, mMatrixA, mInvMatrixA, pNodeA,
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
TRIBASE_API BOOL tbModelHitsModel(const tbModel* pModelA,
								  const tbMatrix& mMatrixA,
								  const tbMatrix& mInvMatrixA,
								  const tbModel* pModelB,
								  const tbMatrix& mMatrixB,
								  const tbMatrix& mInvMatrixB,
								  tbVector3* pvOutPos,		// = NULL
								  tbVector3* pvOutNormalA,	// = NULL
								  tbVector3* pvOutNormalB,	// = NULL
								  int* piOutTriA,			// = NULL
								  int* piOutTriB)			// = NULL
{
	// Parameter prüfen
	if(pModelA == NULL)			TB_ERROR_NULL_POINTER("pModelA", TB_ERROR);
	if(pModelB == NULL)			TB_ERROR_NULL_POINTER("pModelB", TB_ERROR);
	if(!pModelA->m_bExtraData)	TB_ERROR("Das Modell A hat keine Extradaten!", TB_ERROR);
	if(!pModelB->m_bExtraData)	TB_ERROR("Das Modell B hat keine Extradaten!", TB_ERROR);


	// Die Positionen der beiden Modelle aus den Matrizen ablesen
	tbVector3 vModelA(tbVector3(mMatrixA.m41, mMatrixA.m42, mMatrixA.m43));
	tbVector3 vModelB(tbVector3(mMatrixB.m41, mMatrixB.m42, mMatrixB.m43));

	// Prüfen, ob sich die Umgebungskugeln der beiden Modelle schneiden.
	// Falls sie es nicht tun, brauchen wir gar nicht weiter zu testen.
	if(!tbSphereHitsSphere(vModelA, pModelA->m_fBoundingSphereRadius,
		                   vModelB, pModelB->m_fBoundingSphereRadius)) return FALSE;

	// Die rekursive Funktion aufrufen, mit den Wurzelknoten beider Modelle
	return tbModelHitsModelRec(pModelA, mMatrixA, mInvMatrixA, pModelA->m_pRootNode,
		                       pModelB, mMatrixB, mInvMatrixB, pModelB->m_pRootNode,
							   pvOutPos,
							   pvOutNormalA, pvOutNormalB,
							   piOutTriA, piOutTriB,
							   FALSE);
}

// ******************************************************************
// Kollision zwischen Modell und Octree berechnen (rekursiv)
TRIBASE_API BOOL tbModelHitsOctreeRec(const tbModel* pModel,
									  const tbMatrix& mMatrix,
									  const tbMatrix& mInvMatrix,
									  const tbModelOctreeNode* pModelNode,
									  const tbOctree* pOctree,
									  const tbOctreeNode* pOctreeNode,
									  tbVector3* pvOutPos,
									  tbVector3* pvOutNormalM,
									  tbVector3* pvOutNormalO,
									  int* piOutTriM,
									  int* piOutTriO,
									  tbOctreeNode** ppOutNodeO)
{
	// Schneiden sich die Umgebungsquader der beiden Knoten?
	// Falls sie es nicht tun, kann es hier keine Kollision geben.
	if(!tbBoxHitsBox(pModelNode->vBoundingBoxMin, pModelNode->vBoundingBoxMax, mMatrix,
					 pOctreeNode->vBoundingBoxMin, pOctreeNode->vBoundingBoxMax, tbMatrixIdentity(),
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
		tbVector3*	pvVector;
		tbVector3	vTriA;
		tbVector3	vTriB;
		tbVector3	vTriC;
		tbVector3	vHitStart;
		tbVector3	vHitEnd;

		// Alle Dreiecke vom Octree durchgehen
		for(DWORD tb = 0; tb < pOctreeNode->dwNumIndices / 3; tb++)
		{
			// Ersten Vektor dieses Dreiecks vom Octree transformieren
			pvVector = &pOctree->m_pvVectors[pOctreeNode->pdwIndices[tb * 3]];
			vTriA.x = pvVector->x * mInvMatrix.m11 + pvVector->y * mInvMatrix.m21 + pvVector->z * mInvMatrix.m31 + mInvMatrix.m41;
			vTriA.y = pvVector->x * mInvMatrix.m12 + pvVector->y * mInvMatrix.m22 + pvVector->z * mInvMatrix.m32 + mInvMatrix.m42;
			vTriA.z = pvVector->x * mInvMatrix.m13 + pvVector->y * mInvMatrix.m23 + pvVector->z * mInvMatrix.m33 + mInvMatrix.m43;

			// Zweiten Vektor dieses Dreiecks vom Octree transformieren
			pvVector = &pOctree->m_pvVectors[pOctreeNode->pdwIndices[tb * 3 + 1]];
			vTriB.x = pvVector->x * mInvMatrix.m11 + pvVector->y * mInvMatrix.m21 + pvVector->z * mInvMatrix.m31 + mInvMatrix.m41;
			vTriB.y = pvVector->x * mInvMatrix.m12 + pvVector->y * mInvMatrix.m22 + pvVector->z * mInvMatrix.m32 + mInvMatrix.m42;
			vTriB.z = pvVector->x * mInvMatrix.m13 + pvVector->y * mInvMatrix.m23 + pvVector->z * mInvMatrix.m33 + mInvMatrix.m43;

			// Dritten Vektor dieses Dreiecks vom Octree transformieren
			pvVector = &pOctree->m_pvVectors[pOctreeNode->pdwIndices[tb * 3 + 2]];
			vTriC.x = pvVector->x * mInvMatrix.m11 + pvVector->y * mInvMatrix.m21 + pvVector->z * mInvMatrix.m31 + mInvMatrix.m41;
			vTriC.y = pvVector->x * mInvMatrix.m12 + pvVector->y * mInvMatrix.m22 + pvVector->z * mInvMatrix.m32 + mInvMatrix.m42;
			vTriC.z = pvVector->x * mInvMatrix.m13 + pvVector->y * mInvMatrix.m23 + pvVector->z * mInvMatrix.m33 + mInvMatrix.m43;

			// Alle Dreiecke vom Modell durchgehen
			for(DWORD ta = 0; ta < pModelNode->dwNumTriangles; ta++)
			{
				// Kollidieren diese beiden Dreiecke?
				if(tbTriangleHitsTriangle(pModel->m_pvVectors[pModel->m_pdwIndices[pModelNode->pdwTriangles[ta] * 3]],
										  pModel->m_pvVectors[pModel->m_pdwIndices[pModelNode->pdwTriangles[ta] * 3 + 1]],
										  pModel->m_pvVectors[pModel->m_pdwIndices[pModelNode->pdwTriangles[ta] * 3 + 2]],
										  vTriA, vTriB, vTriC,
										  &vHitStart,
										  &vHitEnd))
				{
					// Es gibt eine Kollision!
					// Wenn der Ort der Kollision gefragt ist: ausfüllen!
					// Der Vektor muss aber noch mit der Matrix vom Modell transformiert werden.
					if(pvOutPos != NULL) *pvOutPos = tbVector3TransformCoords(0.5f * (vHitStart + vHitEnd), mMatrix);

					// Sind die beiden Dreiecke gefragt?
					if(piOutTriM != NULL) *piOutTriM = (int)(pModelNode->pdwTriangles[ta]);
					if(piOutTriO != NULL) *piOutTriO = (int)(tb);

					// Ist der Octree-Knoten gefragt?
					if(ppOutNodeO != NULL) *ppOutNodeO = (tbOctreeNode*)(pOctreeNode);

					// Sind die Normalvektoren gefragt?
					if(pvOutNormalM != NULL) *pvOutNormalM = tbVector3TransformNormal(pModel->m_pTrianglePlanes[pModelNode->pdwTriangles[ta] * 4].n, mMatrix);
					if(pvOutNormalO != NULL) *pvOutNormalO = pOctreeNode->pTrianglePlanes[tb * 4].n;

					return TRUE;
				}
			}
		}
	}
	else if(pModelNode->bIsLeaf && !pOctreeNode->bIsLeaf)
	{
		for(int i = 0; i < 8; i++)
		{
			if(tbModelHitsOctreeRec(pModel, mMatrix, mInvMatrix, pModelNode,
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
			if(tbModelHitsOctreeRec(pModel, mMatrix, mInvMatrix, pModelNode->apChild[i],
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
			if(tbModelHitsOctreeRec(pModel, mMatrix, mInvMatrix, pModelNode,
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
TRIBASE_API BOOL tbModelHitsOctree(const tbModel* pModel,
								   const tbMatrix& mMatrix,
								   const tbMatrix& mInvMatrix,
								   const tbOctree* pOctree,
								   tbVector3* pvOutPos,			// = NULL
								   tbVector3* pvOutNormalM,		// = NULL
								   tbVector3* pvOutNormalO,		// = NULL
								   int* piOutTriM,				// = NULL
								   int* piOutTriO,				// = NULL
								   tbOctreeNode** ppOutNodeO)	// = NULL
{
	// Parameter prüfen
	if(pModel == NULL)			TB_ERROR_NULL_POINTER("pModel", TB_ERROR);
	if(pOctree == NULL)			TB_ERROR_NULL_POINTER("pOctree", TB_ERROR);
	if(!pModel->m_bExtraData)	TB_ERROR("Das Modell hat keine Extradaten!", TB_ERROR);
	if(!pOctree->m_bExtraData)	TB_ERROR("Der Octree hat keine Extradaten!", TB_ERROR);


	// Die rekursive Funktion aufrufen, mit den Wurzelknoten beider Objekte
	return tbModelHitsOctreeRec(pModel, mMatrix, mInvMatrix, pModel->m_pRootNode,
								pOctree, pOctree->m_pRootNode,
								pvOutPos,
								pvOutNormalM, pvOutNormalO,
								piOutTriM, piOutTriO,
								ppOutNodeO);
}

// ******************************************************************
// Schnittlinie zwischen einem Dreieck und einer Ebene berechnen
TRIBASE_API BOOL tbTriangleHitsPlane(const tbVector3& vTriangleA,
									 const tbVector3& vTriangleB,
									 const tbVector3& vTriangleC,
									 tbPlane& Plane,
									 tbVector3* pvHitStart,
									 tbVector3* pvHitEnd)
{
	// Punkte des Dreiecks in die Ebenengleichung einsetzen
	float fDotA = tbPlaneDotCoords(Plane, vTriangleA);
	float fDotB = tbPlaneDotCoords(Plane, vTriangleB);
	float fDotC = tbPlaneDotCoords(Plane, vTriangleC);

	// Wenn alle Punkte auf der gleichen Seite liegen, gibt es keine Schnittlinie.
	if(fDotA > 0.0f) if(fDotB > 0.0f) if(fDotC > 0.0f) return FALSE;
	if(fDotA < 0.0f) if(fDotB < 0.0f) if(fDotC < 0.0f) return FALSE;

	// Den Punkt finden, der auf seiner Ebenenseite alleine ist
	int iTop = 0;
	if(fDotB <= 0.0f && fDotC <= 0.0f) iTop = 1;
	else if(fDotA >= 0.0f && fDotB >= 0.0f) iTop = 3;
	else if(fDotA >= 0.0f && fDotC >= 0.0f) iTop = 2;
	else if(fDotB >= 0.0f && fDotC >= 0.0f) iTop = 1;
	else if(fDotA <= 0.0f && fDotB <= 0.0f) iTop = 3;
	else iTop = 2;

	// Schnittpunkte zwischen der Linie vom einzelnen Punkt zu den beiden anderen
	// und der Ebene berechnen
	switch(iTop)
	{
	case 1: *pvHitStart = tbLineHitsPlaneFastEx(vTriangleA, vTriangleB, Plane);
			*pvHitEnd = tbLineHitsPlaneFastEx(vTriangleA, vTriangleC, Plane);
			return TRUE;

	case 2: *pvHitStart = tbLineHitsPlaneFastEx(vTriangleB, vTriangleA, Plane);
			*pvHitEnd = tbLineHitsPlaneFastEx(vTriangleB, vTriangleC, Plane);
			return TRUE;
	
	case 3: *pvHitStart = tbLineHitsPlaneFastEx(vTriangleC, vTriangleA, Plane);
			*pvHitEnd = tbLineHitsPlaneFastEx(vTriangleC, vTriangleB, Plane);
			return TRUE;
	}

	// Das Dreieck schneidet die Ebene nicht!
	return FALSE;
}

// ******************************************************************
// Schnittlinie zweier Dreiecke berechnen
TRIBASE_API BOOL tbTriangleHitsTriangle(const tbVector3& v1A,
										const tbVector3& v1B,
										const tbVector3& v1C,
										const tbVector3& v2A,
										const tbVector3& v2B,
										const tbVector3& v2C,
										tbVector3* pvHitStart,
										tbVector3* pvHitEnd)
{
	// Ebene des zweiten Dreiecks berechnen
	tbPlane Plane2(tbPlaneFromPoints(v2A, v2B, v2C));

	// Punkte des ersten Dreiecks in die Ebenengleichung einsetzen
	float fDot2A = tbPlaneDotCoords(Plane2, v1A);
	float fDot2B = tbPlaneDotCoords(Plane2, v1B);
	float fDot2C = tbPlaneDotCoords(Plane2, v1C);

	// Liegen alle Punkte auf der gleichen Seite?
	if(fDot2A < -0.0001f && fDot2B < -0.0001f && fDot2C < -0.0001f) return FALSE;
	if(fDot2A > 0.0001f && fDot2B > 0.0001f && fDot2C > 0.0001f) return FALSE;

	// Nun auch die Ebene von Dreieck 1 berechnen und Tests durchführen
	tbPlane Plane1(tbPlaneFromPoints(v1A, v1B, v1C));
	float fDot1A = tbPlaneDotCoords(Plane1, v2A);
	float fDot1B = tbPlaneDotCoords(Plane1, v2B);
	float fDot1C = tbPlaneDotCoords(Plane1, v2C);
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
	tbVector3 vL1A;	// Startpunkt von L1
	tbVector3 vL1B;	// Endpunkt von L1
	switch(iTop1)
	{
	case 1: vL1A = tbLineHitsPlaneFastEx(v1A, v1B, Plane2);
			vL1B = tbLineHitsPlaneFastEx(v1A, v1C, Plane2);
			break;

	case 2: vL1A = tbLineHitsPlaneFastEx(v1B, v1A, Plane2);
			vL1B = tbLineHitsPlaneFastEx(v1B, v1C, Plane2);
			break;

	default: vL1A = tbLineHitsPlaneFastEx(v1C, v1A, Plane2);
			 vL1B = tbLineHitsPlaneFastEx(v1C, v1B, Plane2);
			 break;
	}

	// L2 berechnen
	tbVector3 vL2A;	// Startpunkt von L2
	tbVector3 vL2B;	// Endpunkt von L2
	switch(iTop2)
	{
	case 1: vL2A = tbLineHitsPlaneFastEx(v2A, v2B, Plane1);
			vL2B = tbLineHitsPlaneFastEx(v2A, v2C, Plane1);
			break;

	case 2: vL2A = tbLineHitsPlaneFastEx(v2B, v2A, Plane1);
			vL2B = tbLineHitsPlaneFastEx(v2B, v2C, Plane1);
			break;

	default: vL2A = tbLineHitsPlaneFastEx(v2C, v2A, Plane1);
			 vL2B = tbLineHitsPlaneFastEx(v2C, v2B, Plane1);
			 break;
	}

	// Die eindimensionale Achse berechnen (Kreuzprodukt aus den
	// Normalvektoren der beiden Dreiecksebenen bilden)
	tbVector3 vAxis(tbVector3Cross(Plane1.n, Plane2.n));

	// Die dominante Koordinatenkomponente finden und dann die
	// Schnittlinie auf die 1D-Achse projizieren
	float fMax = TB_MAX(TB_MAX(vAxis.x, vAxis.y), vAxis.z);
	if(fabsf(fMax) < 0.0001f) fMax = TB_MIN(TB_MIN(vAxis.x, vAxis.y), vAxis.z);
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
			*pvHitStart = vL1A + (TB_MAX(fMin1, fMin2) * vAxis);
			*pvHitEnd = vL1A + (TB_MIN(fMax1, fMax2) * vAxis);
			return TRUE;
		}
		
		// Keine Schnittlinie!
		return FALSE;
	}
	// fMin1 >= fMin2
	else if(fMin1 <= fMax2)
	{
		// Kollision! Schnittlinie eintragen.
		*pvHitStart = vL1A + (TB_MAX(fMin1, fMin2) * vAxis);
		*pvHitEnd = vL1A + (TB_MIN(fMax1, fMax2) * vAxis);
		return TRUE;
	}

	// Keine Schnittlinie!
	return FALSE;
}

// ******************************************************************
// Kollision zwischen einer Kugel und einer Ebene berechnen
TRIBASE_API BOOL tbSphereHitsPlane(const tbVector3& vSphere,
								   const float fRadius,
								   const tbPlane& Plane)
{
	// Wenn der Abstand des Kugelmittelpunkts zur Ebene kleiner oder gleich
	// dem Radius der Kugel ist, dann gibt es eine Kollision.
	return fabsf(tbPointPlaneDistance(vSphere, Plane)) <= fRadius;
}

// ******************************************************************
// Kollision zweier Kugeln berechnen
TRIBASE_API BOOL tbSphereHitsSphere(const tbVector3& vSphereA,
									const float fRadiusA,
									const tbVector3& vSphereB,
									const float fRadiusB)
{
	// Quadrat der Entfernung berechnen und entscheiden, ob eine Kollision vorliegt
	return tbVector3LengthSq(vSphereA - vSphereB) <= (fRadiusA + fRadiusB) * (fRadiusA + fRadiusB);
}

// ******************************************************************
// Schneiden sich Kugel und achsenausgerichteter Quader?
TRIBASE_API BOOL tbSphereHitsAABox(const tbVector3& vSphere,
								   const float fRadius,
								   const tbVector3& vBoxMin,
								   const tbVector3& vBoxMax)
{
	return tbPointHitsAABox(vSphere,
							vBoxMin - tbVector3(fRadius),
							vBoxMax + tbVector3(fRadius));
}

// ******************************************************************
// Schneiden sich Kugel und frei drehbarer Quader?
TRIBASE_API BOOL tbSphereHitsBox(const tbVector3& vSphere,
								 const float fRadius,
								 const tbVector3& vBoxMin,
								 const tbVector3& vBoxMax,
								 const tbMatrix& mBoxTransformation)
{
	return tbPointHitsBox2(vSphere,
		                   vBoxMin - tbVector3(fRadius),
						   vBoxMax + tbVector3(fRadius),
						   mBoxTransformation);
}

// ******************************************************************
// Kollision zwischen zwei Quadern berechnen
TRIBASE_API BOOL tbBoxHitsBox(const tbVector3& vBox1Min,
							  const tbVector3& vBox1Max,
							  const tbMatrix& mBox1Transformation,
							  const tbVector3& vBox2Min,
							  const tbVector3& vBox2Max,
							  const tbMatrix& mBox2Transformation,
							  const int iNumSamples)
{
	tbPlane aBoxPlanes[6];	// Die sechs Ebenen

	// Die Ebenen des zweiten Quaders berechnen
	tbComputeBoxPlanes(vBox2Min, vBox2Max, mBox2Transformation, aBoxPlanes);

	// Schrittweiten berechnen
	float fStepX = (vBox1Max.x - vBox1Min.x) / (float)(iNumSamples);
	float fStepY = (vBox1Max.y - vBox1Min.y) / (float)(iNumSamples);
	float fStepZ = (vBox1Max.z - vBox1Min.z) / (float)(iNumSamples);

	// Am Anfang ist der zu testende Punkt beim Minimum des ersten Quaders.
	tbVector3 vPoint(vBox1Min);

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
				if(tbPointHitsBox(tbVector3TransformCoords(vPoint,
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

	tbComputeBoxPlanes(vBox1Min, vBox1Max, mBox1Transformation, aBoxPlanes);
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
				if(tbPointHitsBox(tbVector3TransformCoords(vPoint,
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
TRIBASE_API int tbClassifySphere(const tbVector3& vSphere,
								 const float fRadius,
								 const tbPlane& Plane)
{
	// Die Entfernung des Kugelmittelpunkts zur Ebene ist entscheidend.
	float fDistance = tbPointPlaneDistance(vSphere, Plane);
	if(fabsf(fDistance) < fRadius) return 0;

	return fDistance > 0.0f ? 1 : -1;
}

// ******************************************************************
// Diese Funktion liefert:
//  1, wenn der Quader vollständig vor der Ebene liegt,
// -1, wenn er vollständig hinter der Ebene liegt und
//  0, wenn der Quader die Ebene schneidet.
TRIBASE_API int tbClassifyBox(const tbVector3& vBoxMin,
							  const tbVector3& vBoxMax,
							  const tbMatrix& mBoxTransformation,
							  const tbPlane& Plane)
{
	// Es gibt ein recht schnelles Verfahren, dies herauszufinden, aber
	// es funktioniert nur mit achsenausgerichteten Quadern.
	// Darum wenden wir einen Trick an und tun so, als sei der Quader
	// achsenausgerichtet und transformieren statt dessen einfach die Ebene
	// mit der invertierten Matrix des Quaders.

	// Die Ebene transformieren
	tbPlane NewPlane(tbPlaneTransform(Plane, tbMatrixInvert(mBoxTransformation)));

	// Nächsten und fernsten Punkt berechnen
	tbVector3 vNearPoint(vBoxMax);
	tbVector3 vFarPoint(vBoxMax);
	if(NewPlane.a > 0.0f) {vNearPoint.x = vBoxMin.x; vFarPoint.x = vBoxMax.x;}
	if(NewPlane.b > 0.0f) {vNearPoint.y = vBoxMin.y; vFarPoint.y = vBoxMax.y;}
	if(NewPlane.c > 0.0f) {vNearPoint.z = vBoxMin.z; vFarPoint.z = vBoxMax.z;}

	// Prüfen, ob der nächste Punkt vor der Ebene liegt. Falls ja, liegt der ganze Quader vor ihr.
	if(tbPlaneDotCoords(NewPlane, vNearPoint) > 0.0f) return 1;

	// Prüfen, ob der fernste Punkt vor der Ebene liegt. Falls ja, gibt es einen Schnitt.
	if(tbPlaneDotCoords(NewPlane, vFarPoint) > 0.0f) return 0;

	// Ansonsten liegt der Quader auf der Rückseite.
	return -1;
}

// ******************************************************************
// Diese Funktion liefert:
//  1, wenn der Quader vollständig vor der Ebene liegt,
// -1, wenn er vollständig hinter der Ebene liegt und
//  0, wenn der Quader die Ebene schneidet.
TRIBASE_API int tbClassifyBox_Inv(const tbVector3& vBoxMin,
								  const tbVector3& vBoxMax,
								  const tbMatrix& mInvBoxTransformation,
								  const tbPlane& Plane)
{
	// Es gibt ein recht schnelles Verfahren, dies herauszufinden, aber
	// es funktioniert nur mit achsenausgerichteten Quadern.
	// Darum wenden wir einen Trick an und tun so, als sei der Quader
	// achsenausgerichtet und transformieren statt dessen einfach die Ebene
	// mit der invertierten Matrix des Quaders.

	// Die Ebene transformieren
	tbPlane NewPlane(tbPlaneTransform(Plane, mInvBoxTransformation));

	// Nächsten und fernsten Punkt berechnen
	tbVector3 vNearPoint(vBoxMax);
	tbVector3 vFarPoint(vBoxMax);
	if(NewPlane.a > 0.0f) {vNearPoint.x = vBoxMin.x; vFarPoint.x = vBoxMax.x;}
	if(NewPlane.b > 0.0f) {vNearPoint.y = vBoxMin.y; vFarPoint.y = vBoxMax.y;}
	if(NewPlane.c > 0.0f) {vNearPoint.z = vBoxMin.z; vFarPoint.z = vBoxMax.z;}

	// Prüfen, ob der nächste Punkt vor der Ebene liegt. Falls ja, liegt der ganze Quader vor ihr.
	if(tbPlaneDotCoords(NewPlane, vNearPoint) > 0.0f) return 1;

	// Prüfen, ob der fernste Punkt vor der Ebene liegt. Falls ja, gibt es einen Schnitt.
	if(tbPlaneDotCoords(NewPlane, vFarPoint) > 0.0f) return 0;

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
TRIBASE_API int tbClassifyAABox(const tbVector3& vBoxMin,
								const tbVector3& vBoxMax,
								const tbPlane& Plane)
{
	// Nächsten und fernsten Punkt berechnen
	tbVector3 vNearPoint(vBoxMax);
	tbVector3 vFarPoint(vBoxMin);
	if(Plane.a > 0.0f) {vNearPoint.x = vBoxMin.x; vFarPoint.x = vBoxMax.x;}
	if(Plane.b > 0.0f) {vNearPoint.y = vBoxMin.y; vFarPoint.y = vBoxMax.y;}
	if(Plane.c > 0.0f) {vNearPoint.z = vBoxMin.z; vFarPoint.z = vBoxMax.z;}

	// Prüfen, ob der nächste Punkt vor der Ebene liegt. Falls ja, liegt der ganze Quader vor ihr.
	if(tbPlaneDotCoords(Plane, vNearPoint) > 0.0f) return 1;

	// Prüfen, ob der fernste Punkt vor der Ebene liegt. Falls ja, gibt es einen Schnitt.
	if(tbPlaneDotCoords(Plane, vFarPoint) > 0.0f) return 0;

	// Der Quader befindet sich hinter der Ebene.
	return -1;
}

// ******************************************************************
// Sichtbarkeit eines Punkts prüfen
TRIBASE_API BOOL tbPointVisible(const tbVector3& vPoint,
								const tbPlane* pClipPlanes)
{
	// Den Punkt in jede Ebenengleichung einsetzen
	if(tbPointPlaneDistanceFast(vPoint, pClipPlanes[0]) > 0.0f) return FALSE;
	if(tbPointPlaneDistanceFast(vPoint, pClipPlanes[1]) > 0.0f) return FALSE;
	if(tbPointPlaneDistanceFast(vPoint, pClipPlanes[2]) > 0.0f) return FALSE;
	if(tbPointPlaneDistanceFast(vPoint, pClipPlanes[3]) > 0.0f) return FALSE;
	if(tbPointPlaneDistanceFast(vPoint, pClipPlanes[4]) > 0.0f) return FALSE;
	return tbPointPlaneDistanceFast(vPoint, pClipPlanes[5]) <= 0.0f;
}

// ******************************************************************
// Sichtbarkeit einer Kugel prüfen
TRIBASE_API BOOL tbSphereVisible(const tbVector3& vSphere,
								 const float fRadius,
								 const tbPlane* pClipPlanes)
{
	// Den Kugelmittelpunkt in jede Ebenengleichung einsetzen
	if(tbPointPlaneDistanceFast(vSphere, pClipPlanes[0]) > fRadius) return FALSE;
	if(tbPointPlaneDistanceFast(vSphere, pClipPlanes[1]) > fRadius) return FALSE;
	if(tbPointPlaneDistanceFast(vSphere, pClipPlanes[2]) > fRadius) return FALSE;
	if(tbPointPlaneDistanceFast(vSphere, pClipPlanes[3]) > fRadius) return FALSE;
	if(tbPointPlaneDistanceFast(vSphere, pClipPlanes[4]) > fRadius) return FALSE;
	return tbPointPlaneDistanceFast(vSphere, pClipPlanes[5]) <= fRadius;
}

// ******************************************************************
// Sichtbarkeit eines achsenausgerichteten Quaders prüfen
TRIBASE_API BOOL tbAABoxVisible(const tbVector3& vBoxMin,
								const tbVector3& vBoxMax,
								const tbPlane* pClipPlanes)
{
	// Wenn der Quader auf der Vorderseite einer Ebene liegt, ist er unsichtbar.
	if(tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[0]) == 1) return FALSE;
	if(tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[1]) == 1) return FALSE;
	if(tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[2]) == 1) return FALSE;
	if(tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[3]) == 1) return FALSE;
	if(tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[4]) == 1) return FALSE;
	return tbClassifyAABox(vBoxMin, vBoxMax, pClipPlanes[5]) != 1;
}

// ******************************************************************
// Sichtbarkeit eines Quaders prüfen
TRIBASE_API BOOL tbBoxVisible(const tbVector3& vBoxMin,
							  const tbVector3& vBoxMax,
							  const tbMatrix& mBoxTransformation,
							  const tbPlane* pClipPlanes)
{
	// Matrix invertieren
	tbMatrix mInvBoxTransformation(tbMatrixInvert(mBoxTransformation));

	// Wenn der Quader auf der Vorderseite einer Ebene liegt, ist er unsichtbar.
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[0]) == 1) return FALSE;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[1]) == 1) return FALSE;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[2]) == 1) return FALSE;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[3]) == 1) return FALSE;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[4]) == 1) return FALSE;
	return tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[5]) != 1;
}

// ******************************************************************
// Sichtbarkeit eines Quaders prüfen
TRIBASE_API BOOL tbBoxVisible_Inv(const tbVector3& vBoxMin,
								  const tbVector3& vBoxMax,
								  const tbMatrix& mInvBoxTransformation,
								  const tbPlane* pClipPlanes)
{
	// Wenn der Quader auf der Vorderseite einer Ebene liegt, ist er unsichtbar.
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[0]) == 1) return FALSE;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[1]) == 1) return FALSE;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[2]) == 1) return FALSE;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[3]) == 1) return FALSE;
	if(tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[4]) == 1) return FALSE;
	return tbClassifyBox_Inv(vBoxMin, vBoxMax, mInvBoxTransformation, pClipPlanes[5]) != 1;
}

// ******************************************************************
// Berechnet den Zielvektor, um ein Objekt zu treffen
TRIBASE_API tbVector3 tbComputeAimingVector(const tbVector3& vOrigin,
											const tbVector3& vTargetPos,
											const tbVector3& vTargetVel,
											const float fProjectileSpeed)
{
	// Richtung vom Abschusspunkt zum Ziel bestimmen
	tbVector3 vDirection(vTargetPos - vOrigin);

	// Diskriminante des Terms für die Zeit berechnen.
	// Wenn sie kleiner als null ist, dann ist es nicht möglich, das Ziel zu treffen.
	// Das kann zum Beispiel passieren, wenn das Ziel schneller als das Geschoss ist.
	// Wir liefern dann einfach die Position des Ziels als Zielvektor zurück.
	float fDiscriminant = fProjectileSpeed * fProjectileSpeed * tbVector3LengthSq(vDirection) - tbVector3LengthSq(tbVector3Cross(vDirection, vTargetVel));
	if(fDiscriminant < 0.0f) return vTargetPos;

	// Zeit des Treffers berechnen
	float fTime = (sqrtf(fDiscriminant) + tbVector3Dot(vDirection, vTargetVel))
				/ (fProjectileSpeed * fProjectileSpeed - tbVector3LengthSq(vTargetVel));

	// Zielvektor liefern
	return vTargetPos + fTime * vTargetVel;
}

// ******************************************************************
// Diese Funktion wandelt ein Direct3D-Format in einen Text um.
TRIBASE_API tbResult tbGetD3DFormatString(D3DFORMAT Format,
										  char* pcOut)
{
	// Parameter prüfen
	if(pcOut == NULL) TB_ERROR_NULL_POINTER("pcOut", TB_ERROR);


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

	return TB_OK;
}

// ******************************************************************
// Diese Funktion liefert die Bits pro Pixel eines Formats.
TRIBASE_API DWORD tbGetD3DFormatBPP(D3DFORMAT Format)
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