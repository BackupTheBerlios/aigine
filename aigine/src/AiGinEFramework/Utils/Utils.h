#include "../AiGinE.h"

// Verwendete Symbole vordeklarieren
class Vector2D;
class Vector3D;
class Matrix4x4;
class Plane;
class Model;
class Octree;
struct OctreeNode;

// ******************************************************************
// Funktionen deklarieren
char*		RemoveDir(char* pcFilename);						// Pfadangabe aus einem Dateinamen entfernen
char*		GetFilenameExtension(char* pcFilename);			// Liefert die Erweiterung einer Datei
BOOL		FileExists(char* pcFilename);						// Fragt ab, ob die Datei existiert
inline		int			IntRandom(const int iMin, const int iMax)			{return iMin + (rand() % (iMax - iMin + 1));}
inline		float		FloatRandom(const float fMin, const float fMax)	{return fMin + (fMax - fMin) * ((float)(rand() % 10001) / 10000.0f);}

Vector2D	Vector2DTransformCoords(const Vector2D& v, const Matrix4x4& m, float* pfOutW = NULL);																																																							// 2D-Vektor mit Matrix multiplizieren
Vector2D	Vector2DTransformNormal(const Vector2D& v, const Matrix4x4& m);																																																												// 2D-Normalvektor mit Matrix multiplizieren
Vector2D	Vector2DTransformNormal_TranspInv(const Vector2D& v, const Matrix4x4& m);																																																										// 2D-Normalvektor mit Matrix multiplizieren
Vector3D	Vector3DTransformCoords(const Vector3D& v, const Matrix4x4& m, float* pfOutW = NULL);																																																							// 3D-Vektor mit Matrix multiplizieren
Vector3D	Vector3DTransformNormal(const Vector3D& v, const Matrix4x4& m);																																																												// 3D-Normalvektor mit Matrix multiplizieren
Vector3D	Vector3DTransformNormal_TranspInv(const Vector3D& v, const Matrix4x4& m);																																																										// 3D-Normalvektor mit Matrix multiplizieren
Vector3D	Vector3DProject(const Vector3D& v, const Matrix4x4& mWorldViewProj);																																																											// Projiziert einen 3D-Positionsvektor
Plane		PlaneTransform(const Plane& p, const Matrix4x4& m);																																																															// Ebene mit Matrix transformieren
Vector3D	ComputeTriangleNormal(const Vector3D& v1, const Vector3D& v2, const Vector3D& v3);																																																							// Normalvektor eines Dreiecks berechnen
Result		ComputeBoxPlanes(const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Matrix4x4& mBoxTransformation, Plane* pOut);																																														// Die sechs Ebenen eines frei drehbaren Quaders berechnen
Result		ComputeClipPlanes(const Matrix4x4& mView, const Matrix4x4& mProjection, Plane* pOut);																																																							// View-Frustum anhand zweier Matrizen berechnen
float		PointPlaneDistance(const Vector3D& vPoint, const Plane& Plane);																																																											// Minimale Entfernung zwischen Punkt und Ebene berechnen
float		PointPlaneDistanceFast(const Vector3D& vPoint, const Plane& Plane);																																																										// Minimale Entfernung zwischen Punkt und Ebene berechnen (schnell)
float		PointLineDistance(const Vector3D& vPoint, const Vector3D& vLineA, const Vector3D& vLineB);																																																					// Minimale Entfernung zwischen Punkt und Linie berechnen
float		LineLineDistance(const Vector3D& vLine1A, const Vector3D& vLine1B, const Vector3D& vLine2A, const Vector3D& vLine2B);																																														// Minimale Entfernung zwischen zwei Linien berechnen
BOOL		PointHitsSphere(const Vector3D& vPoint, const Vector3D& vSphere, const float fRadius);																																																						// Liegt ein Punkt in einer Kugel?
BOOL		PointHitsAABox(const Vector3D& vPoint, const Vector3D& vBoxMin, const Vector3D& vBoxMax);																																																					// Liegt ein Punkt in einem achsenausgerichteten Quader?
BOOL		PointHitsBox(const Vector3D& vPoint, const Plane* pBoxPlanes);																																																												// Liegt ein Punkt in einem frei drehbaren Quader?
BOOL		PointHitsBox(const Vector3D& vPoint, const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Matrix4x4& mBoxTransformation);																																												// Liegt ein Punkt in einem frei drehbaren Quader?
BOOL		PointHitsBox2(const Vector3D& vPoint, const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Matrix4x4& mBoxTransformation);																																												// Liegt ein Punkt in einem frei drehbaren Quader? Diese Funktion rechnet mit der invertierten Matrix.
BOOL		PointHitsBox2_Inv(const Vector3D& vPoint, const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Matrix4x4& mInvBoxTransformation);																																										// Liegt ein Punkt in einem frei drehbaren Quader? Diese Funktion rechnet mit der invertierten Matrix.
BOOL		LineHitsPlane(const Vector3D& vLineA, const Vector3D& vLineB, const Plane& Plane, Vector3D* pvOut = NULL);																																																// Schnittpunkt zwischen einer Linie und einer Ebene berechnen
float		LineHitsPlaneS(const Vector3D& vLineA, const Vector3D& vLineB, const Plane& Plane);																																																						// Schnittpunkt zwischen einer Linie und einer Ebene berechnen (Linienabschnitt)
BOOL		LineHitsPlaneFast(const Vector3D& vLineA, const Vector3D& vLineB, const Plane& Plane);																																																					// Kollision zwischen einer Linie und einer Ebene berechnen (schnell)
Vector3D	LineHitsPlaneFastEx(const Vector3D& vLineA, const Vector3D& vLineB, const Plane& Plane);																																																					// Kollision zwischen einer Linie und einer Ebene berechnen (schnell)
BOOL		LineHitsTriangle(const Vector3D& vLineA, const Vector3D& vLineB, const Vector3D& vTriangleA, const Vector3D& vTriangleB, const Vector3D& vTriangleC, Vector3D* pvOut = NULL);																															// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen
float		LineHitsTriangleS(const Vector3D& vLineA, const Vector3D& vLineB, const Vector3D& vTriangleA, const Vector3D& vTriangleB, const Vector3D& vTriangleC);																																					// Schnittpunkt zwischen einer Linie und einem Dreieck berechnen (Linienabschnitt)
BOOL		LineHitsSphere(const Vector3D& vLineA, const Vector3D& vLineB, const Vector3D& vSphere, const float fRadius, Vector3D* pvOut = NULL);																																										// Schnittpunkt zwischen einer Linie und einer Kugel berechnen
BOOL		LineHitsSphereFast(const Vector3D& vLineA, const Vector3D& vLineB, const Vector3D& vSphere, const float fRadius);																																															// Schneidet eine Linie eine Kugel?
BOOL		LineHitsBox(const Vector3D& vLineA, const Vector3D& vLineB, const Plane* pBoxPlanes, Vector3D* pvOut = NULL);																																															// Schnittpunkt zwischen einer Linie und einem frei drehbaren Quader berechnen
BOOL		LineHitsBox(const Vector3D& vLineA, const Vector3D& vLineB, const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Matrix4x4& mBoxTransformation, Vector3D* pvOut = NULL);																																// Schnittpunkt zwischen einer Linie und einem frei drehbaren Quader berechnen
BOOL		LineHitsModel(const Vector3D& vLineA, const Vector3D& vLineB, const Model* pModel, const Matrix4x4& mMatrix, const Matrix4x4& mInvMatrix, const float fTolerance = 0.0f, Vector3D* pvOutPos = NULL, Vector3D* pvOutNormal = NULL, int* piOutTriangle = NULL);												// Kollision zwischen Linie und Modell berechnen
BOOL		LineHitsOctree(const Vector3D& vLineA, const Vector3D& vLineB, const Octree* pOctree, const float fTolerance = 0.0f, Vector3D* pvOutPos = NULL, Vector3D* pvOutNormal = NULL, int* piOutTriangle = NULL, OctreeNode** ppOutNode = NULL);																// Kollision zwischen Linie und Octree berechnen
BOOL		TriangleHitsPlane(const Vector3D& vTriangleA, const Vector3D& vTriangleB, const Vector3D& vTriangleC, Plane& Plane, Vector3D* pvHitStart, Vector3D* pvHitEnd);																																			// Schnittlinie zwischen einem Dreieck und einer Ebene berechnen
BOOL		TriangleHitsTriangle(const Vector3D& v1A, const Vector3D& v1B, const Vector3D& v1C, const Vector3D& v2A, const Vector3D& v2B, const Vector3D& v2C, Vector3D* pvHitStart, Vector3D* pvHitEnd);																											// Schnittlinie zweier Dreiecke berechnen
BOOL		SphereHitsPlane(const Vector3D& vSphere, const float fRadius, const Plane& Plane);																																																							// Kollision zwischen einer Kugel und einer Ebene berechnen
BOOL		SphereHitsSphere(const Vector3D& vSphereA, const float fRadiusA, const Vector3D& vSphereB, const float fRadiusB);																																															// Kollision zweier Kugeln berechnen
BOOL		SphereHitsAABox(const Vector3D& vSphere, const float fRadius, const Vector3D& vBoxMin, const Vector3D& vBoxMax);																																															// Schneiden sich Kugel und achsenausgerichteter Quader?
BOOL		SphereHitsBox(const Vector3D& vSphere, const float fRadius, const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Matrix4x4& mBoxTransformation);																																							// Schneiden sich Kugel und frei drehbarer Quader?
BOOL		BoxHitsBox(const Vector3D& vBox1Min, const Vector3D& vBox1Max, const Matrix4x4& mBox1Transformation, const Vector3D& vBox2Min, const Vector3D& vBox2Max, const Matrix4x4& mBox2Transformation, const int iNumSamples);																						// Kollision zwischen zwei Quadern berechnen
BOOL		ModelHitsModel(const Model* pModelA, const Matrix4x4& mMatrixA, const Matrix4x4& mInvMatrixA, const Model* pModelB, const Matrix4x4& mMatrixB, const Matrix4x4& mInvMatrixB, Vector3D* pvOutPos=NULL, Vector3D* pvOutNormalA=NULL, Vector3D* pvOutNormalB=NULL, int* piOutTriA=NULL, int* piOutTriB=NULL);	// Kollision zwischen zwei Modellen berechnen
BOOL		ModelHitsOctree(const Model* pModel, const Matrix4x4& mMatrix, const Matrix4x4& mInvMatrix, const Octree* pOctree, Vector3D* pvOutPos = NULL, Vector3D* pvOutNormalM = NULL, Vector3D* pvOutNormalO = NULL, int* piOutTriM = NULL, int* piOutTriO = NULL, OctreeNode** ppOutNodeO = NULL);				// Kollision zwischen Modell und Octree berechnen
int			ClassifySphere(const Vector3D& vSphere, const float fRadius, const Plane& Plane);																																																							// Kugel klassifizieren (vor der Ebene, hinter der Ebene oder schneidend)
int			ClassifyBox(const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Matrix4x4& mBoxTransformation, const Plane& Plane);																																													// Quader klassifizieren (vor der Ebene, hinter der Ebene oder schneidend)
int			ClassifyBox_Inv(const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Matrix4x4& mInvBoxTransformation, const Plane& Plane);																																												// Quader klassifizieren (vor der Ebene, hinter der Ebene oder schneidend)
int			ClassifyAABox(const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Plane& Plane);																																																						// Achsenausgerichteten Quader klassifizieren
BOOL		PointVisible(const Vector3D& vPoint, const Plane* pClipPlanes);																																																											// Sicharkeit eines Punkts prüfen
BOOL		SphereVisible(const Vector3D& vSphere, const float fRadius, const Plane* pClipPlanes);																																																						// Sicharkeit einer Kugel prüfen
BOOL		AABoxVisible(const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Plane* pClipPlanes);																																																					// Sicharkeit eines achsenausgerichteten Quaders prüfen
BOOL		BoxVisible(const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Matrix4x4& mBoxTransformation, const Plane* pClipPlanes);																																												// Sicharkeit eines Quaders prüfen
BOOL		BoxVisible_Inv(const Vector3D& vBoxMin, const Vector3D& vBoxMax, const Matrix4x4& mInvBoxTransformation, const Plane* pClipPlanes);																																										// Sicharkeit eines Quaders prüfen

Vector3D	ComputeAimingVector(const Vector3D& vOrigin, const Vector3D& vTargetPos, const Vector3D& vTargetVel, const float fProjectileSpeed);	// Berechnet den Zielvektor, um ein Objekt zu treffen

//Result		GetD3DFormatString(D3DFORMAT Format, char* pcOut);	// Umwandeln eines Direct3D-Formats in einen String
//DWORD		GetD3DFormaPP(D3DFORMAT Format);					// Liefert die Bits pro Pixel eines Formats

// ******************************************************************