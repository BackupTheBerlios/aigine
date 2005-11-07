/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbUtils.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Verschiedene Hilfsfunktionen

	Autor:
	David Scherfgen

********************************************************************/

// Verwendete Symbole vordeklarieren
class tbVector2;
class tbVector3;
class tbVector4;
class tbMatrix;
class tbPlane;
class tbModel;
class tbOctree;
struct tbOctreeNode;

// ******************************************************************
// Funktionen deklarieren
TRIBASE_API char*		tbRemoveDir(char* pcFilename);						// Pfadangabe aus einem Dateinamen entfernen
TRIBASE_API char*		tbGetFilenameExtension(char* pcFilename);			// Liefert die Erweiterung einer Datei
TRIBASE_API	BOOL		tbFileExists(char* pcFilename);						// Fragt ab, ob die Datei existiert
inline		int			tbIntRandom(const int iMin, const int iMax)			{return iMin + (rand() % (iMax - iMin + 1));}
inline		float		tbFloatRandom(const float fMin, const float fMax)	{return fMin + (fMax - fMin) * ((float)(rand() % 10001) / 10000.0f);}

TRIBASE_API tbVector2	tbVector2TransformCoords(const tbVector2& v, const tbMatrix& m, float* pfOutW = NULL);																																																							// 2D-Vektor mit Matrix multiplizieren
TRIBASE_API tbVector2	tbVector2TransformNormal(const tbVector2& v, const tbMatrix& m);																																																												// 2D-Normalvektor mit Matrix multiplizieren
TRIBASE_API tbVector2	tbVector2TransformNormal_TranspInv(const tbVector2& v, const tbMatrix& m);																																																										// 2D-Normalvektor mit Matrix multiplizieren
TRIBASE_API tbVector3	tbVector3TransformCoords(const tbVector3& v, const tbMatrix& m, float* pfOutW = NULL);																																																							// 3D-Vektor mit Matrix multiplizieren
TRIBASE_API tbVector3	tbVector3TransformNormal(const tbVector3& v, const tbMatrix& m);																																																												// 3D-Normalvektor mit Matrix multiplizieren
TRIBASE_API tbVector3	tbVector3TransformNormal_TranspInv(const tbVector3& v, const tbMatrix& m);																																																										// 3D-Normalvektor mit Matrix multiplizieren
TRIBASE_API tbVector4	tbVector4TransformCoords(const tbVector4& v, const tbMatrix& m, float* pfOutW = NULL);																																																							// 4D-Vektor mit Matrix multiplizieren
TRIBASE_API tbVector3	tbVector3Project(const tbVector3& v, const tbMatrix& mWorldViewProj);																																																											// Projiziert einen 3D-Positionsvektor
TRIBASE_API tbPlane		tbPlaneTransform(const tbPlane& p, const tbMatrix& m);																																																															// Ebene mit Matrix transformieren
TRIBASE_API tbVector3	tbComputeTriangleNormal(const tbVector3& v1, const tbVector3& v2, const tbVector3& v3);																																																							// Normalvektor eines Dreiecks berechnen
TRIBASE_API tbResult	tbComputeBoxPlanes(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation, tbPlane* pOut);																																														// Die sechs Ebenen eines frei drehbaren Quaders berechnen
TRIBASE_API tbResult	tbComputeClipPlanes(const tbMatrix& mView, const tbMatrix& mProjection, tbPlane* pOut);																																																							// View-Frustum anhand zweier Matrizen berechnen
TRIBASE_API float		tbPointPlaneDistance(const tbVector3& vPoint, const tbPlane& Plane);																																																											// Minimale Entfernung zwischen Punkt und Ebene berechnen
TRIBASE_API float		tbPointPlaneDistanceFast(const tbVector3& vPoint, const tbPlane& Plane);																																																										// Minimale Entfernung zwischen Punkt und Ebene berechnen (schnell)
TRIBASE_API float		tbPointLineDistance(const tbVector3& vPoint, const tbVector3& vLineA, const tbVector3& vLineB);																																																					// Minimale Entfernung zwischen Punkt und Linie berechnen
TRIBASE_API float		tbLineLineDistance(const tbVector3& vLine1A, const tbVector3& vLine1B, const tbVector3& vLine2A, const tbVector3& vLine2B);																																														// Minimale Entfernung zwischen zwei Linien berechnen
TRIBASE_API BOOL		tbPointHitsSphere(const tbVector3& vPoint, const tbVector3& vSphere, const float fRadius);																																																						// Liegt ein Punkt in einer Kugel?
TRIBASE_API BOOL		tbPointHitsAABox(const tbVector3& vPoint, const tbVector3& vBoxMin, const tbVector3& vBoxMax);																																																					// Liegt ein Punkt in einem achsenausgerichteten Quader?
TRIBASE_API BOOL		tbPointHitsBox(const tbVector3& vPoint, const tbPlane* pBoxPlanes);																																																												// Liegt ein Punkt in einem frei drehbaren Quader?
TRIBASE_API BOOL		tbPointHitsBox(const tbVector3& vPoint, const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation);																																												// Liegt ein Punkt in einem frei drehbaren Quader?
TRIBASE_API BOOL		tbPointHitsBox2(const tbVector3& vPoint, const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation);																																												// Liegt ein Punkt in einem frei drehbaren Quader? Diese Funktion rechnet mit der invertierten Matrix.
TRIBASE_API BOOL		tbPointHitsBox2_Inv(const tbVector3& vPoint, const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mInvBoxTransformation);																																										// Liegt ein Punkt in einem frei drehbaren Quader? Diese Funktion rechnet mit der invertierten Matrix.
TRIBASE_API BOOL		tbLineHitsPlane(const tbVector3& vLineA, const tbVector3& vLineB, const tbPlane& Plane, tbVector3* pvOut = NULL);																																																// Schnittpunkt zwischen einer Linie und einer Ebene berechnen
TRIBASE_API float		tbLineHitsPlaneS(const tbVector3& vLineA, const tbVector3& vLineB, const tbPlane& Plane);																																																						// Schnittpunkt zwischen einer Linie und einer Ebene berechnen (Linienabschnitt)
TRIBASE_API BOOL		tbLineHitsPlaneFast(const tbVector3& vLineA, const tbVector3& vLineB, const tbPlane& Plane);																																																					// Kollision zwischen einer Linie und einer Ebene berechnen (schnell)
TRIBASE_API tbVector3	tbLineHitsPlaneFastEx(const tbVector3& vLineA, const tbVector3& vLineB, const tbPlane& Plane);																																																					// Kollision zwischen einer Linie und einer Ebene berechnen (schnell)
TRIBASE_API BOOL		tbLineHitsTriangle(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vTriangleA, const tbVector3& vTriangleB, const tbVector3& vTriangleC, tbVector3* pvOut = NULL);																															// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen
TRIBASE_API float		tbLineHitsTriangleS(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vTriangleA, const tbVector3& vTriangleB, const tbVector3& vTriangleC);																																					// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen (Linienabschnitt)
TRIBASE_API BOOL		tbLineHitsSphere(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vSphere, const float fRadius, tbVector3* pvOut = NULL);																																										// Schnittpunkt zwischen einer Linie und einer Kugel berechnen
TRIBASE_API BOOL		tbLineHitsSphereFast(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vSphere, const float fRadius);																																															// Schneidet eine Linie eine Kugel?
TRIBASE_API BOOL		tbLineHitsBox(const tbVector3& vLineA, const tbVector3& vLineB, const tbPlane* pBoxPlanes, tbVector3* pvOut = NULL);																																															// Schnittpunkt zwischen einer Linie und einem frei drehbaren Quader berechnen
TRIBASE_API BOOL		tbLineHitsBox(const tbVector3& vLineA, const tbVector3& vLineB, const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation, tbVector3* pvOut = NULL);																																// Schnittpunkt zwischen einer Linie und einem frei drehbaren Quader berechnen
TRIBASE_API BOOL		tbLineHitsModel(const tbVector3& vLineA, const tbVector3& vLineB, const tbModel* pModel, const tbMatrix& mMatrix, const tbMatrix& mInvMatrix, const float fTolerance = 0.0f, tbVector3* pvOutPos = NULL, tbVector3* pvOutNormal = NULL, int* piOutTriangle = NULL);												// Kollision zwischen Linie und Modell berechnen
TRIBASE_API BOOL		tbLineHitsOctree(const tbVector3& vLineA, const tbVector3& vLineB, const tbOctree* pOctree, const float fTolerance = 0.0f, tbVector3* pvOutPos = NULL, tbVector3* pvOutNormal = NULL, int* piOutTriangle = NULL, tbOctreeNode** ppOutNode = NULL);																// Kollision zwischen Linie und Octree berechnen
TRIBASE_API BOOL		tbTriangleHitsPlane(const tbVector3& vTriangleA, const tbVector3& vTriangleB, const tbVector3& vTriangleC, tbPlane& Plane, tbVector3* pvHitStart, tbVector3* pvHitEnd);																																			// Schnittlinie zwischen einem Dreieck und einer Ebene berechnen
TRIBASE_API BOOL		tbTriangleHitsTriangle(const tbVector3& v1A, const tbVector3& v1B, const tbVector3& v1C, const tbVector3& v2A, const tbVector3& v2B, const tbVector3& v2C, tbVector3* pvHitStart, tbVector3* pvHitEnd);																											// Schnittlinie zweier Dreiecke berechnen
TRIBASE_API BOOL		tbSphereHitsPlane(const tbVector3& vSphere, const float fRadius, const tbPlane& Plane);																																																							// Kollision zwischen einer Kugel und einer Ebene berechnen
TRIBASE_API BOOL		tbSphereHitsSphere(const tbVector3& vSphereA, const float fRadiusA, const tbVector3& vSphereB, const float fRadiusB);																																															// Kollision zweier Kugeln berechnen
TRIBASE_API BOOL		tbSphereHitsAABox(const tbVector3& vSphere, const float fRadius, const tbVector3& vBoxMin, const tbVector3& vBoxMax);																																															// Schneiden sich Kugel und achsenausgerichteter Quader?
TRIBASE_API BOOL		tbSphereHitsBox(const tbVector3& vSphere, const float fRadius, const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation);																																							// Schneiden sich Kugel und frei drehbarer Quader?
TRIBASE_API BOOL		tbBoxHitsBox(const tbVector3& vBox1Min, const tbVector3& vBox1Max, const tbMatrix& mBox1Transformation, const tbVector3& vBox2Min, const tbVector3& vBox2Max, const tbMatrix& mBox2Transformation, const int iNumSamples);																						// Kollision zwischen zwei Quadern berechnen
TRIBASE_API BOOL		tbModelHitsModel(const tbModel* pModelA, const tbMatrix& mMatrixA, const tbMatrix& mInvMatrixA, const tbModel* pModelB, const tbMatrix& mMatrixB, const tbMatrix& mInvMatrixB, tbVector3* pvOutPos=NULL, tbVector3* pvOutNormalA=NULL, tbVector3* pvOutNormalB=NULL, int* piOutTriA=NULL, int* piOutTriB=NULL);	// Kollision zwischen zwei Modellen berechnen
TRIBASE_API BOOL		tbModelHitsOctree(const tbModel* pModel, const tbMatrix& mMatrix, const tbMatrix& mInvMatrix, const tbOctree* pOctree, tbVector3* pvOutPos = NULL, tbVector3* pvOutNormalM = NULL, tbVector3* pvOutNormalO = NULL, int* piOutTriM = NULL, int* piOutTriO = NULL, tbOctreeNode** ppOutNodeO = NULL);				// Kollision zwischen Modell und Octree berechnen
TRIBASE_API int			tbClassifySphere(const tbVector3& vSphere, const float fRadius, const tbPlane& Plane);																																																							// Kugel klassifizieren (vor der Ebene, hinter der Ebene oder schneidend)
TRIBASE_API int			tbClassifyBox(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation, const tbPlane& Plane);																																													// Quader klassifizieren (vor der Ebene, hinter der Ebene oder schneidend)
TRIBASE_API int			tbClassifyBox_Inv(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mInvBoxTransformation, const tbPlane& Plane);																																												// Quader klassifizieren (vor der Ebene, hinter der Ebene oder schneidend)
TRIBASE_API int			tbClassifyAABox(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbPlane& Plane);																																																						// Achsenausgerichteten Quader klassifizieren
TRIBASE_API BOOL		tbPointVisible(const tbVector3& vPoint, const tbPlane* pClipPlanes);																																																											// Sichtbarkeit eines Punkts prüfen
TRIBASE_API BOOL		tbSphereVisible(const tbVector3& vSphere, const float fRadius, const tbPlane* pClipPlanes);																																																						// Sichtbarkeit einer Kugel prüfen
TRIBASE_API BOOL		tbAABoxVisible(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbPlane* pClipPlanes);																																																					// Sichtbarkeit eines achsenausgerichteten Quaders prüfen
TRIBASE_API BOOL		tbBoxVisible(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mBoxTransformation, const tbPlane* pClipPlanes);																																												// Sichtbarkeit eines Quaders prüfen
TRIBASE_API BOOL		tbBoxVisible_Inv(const tbVector3& vBoxMin, const tbVector3& vBoxMax, const tbMatrix& mInvBoxTransformation, const tbPlane* pClipPlanes);																																										// Sichtbarkeit eines Quaders prüfen

TRIBASE_API tbVector3	tbComputeAimingVector(const tbVector3& vOrigin, const tbVector3& vTargetPos, const tbVector3& vTargetVel, const float fProjectileSpeed);	// Berechnet den Zielvektor, um ein Objekt zu treffen

TRIBASE_API tbResult	tbGetD3DFormatString(D3DFORMAT Format, char* pcOut);	// Umwandeln eines Direct3D-Formats in einen String
TRIBASE_API DWORD		tbGetD3DFormatBPP(D3DFORMAT Format);					// Liefert die Bits pro Pixel eines Formats

// ******************************************************************