/* Generated by Together */

#ifndef MATRIX_H
#define MATRIX_H

/**
 * KönigsklassenMatrix
 * 
 * eine Matrix, die die Grundfunktionalitäten wie Inverse und Determinante bietet. 
 */

// Verwendete Symbole vordefinieren
class ageG_Vector3;
ageG_Matrix ageG_MatrixInvert(const ageG_Matrix& m);


// ******************************************************************
// Funktionen deklarieren
inline		ageG_Matrix	ageG_MatrixIdentity() {return ageG_Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);}	// Identitätsmatrix liefern
ageG_Matrix	ageG_MatrixTranslation(const tbVector3& v);																								// Translationsmatrix (Verschiebungsmatrix) berechnen
ageG_Matrix	ageG_MatrixRotationX(const float f);																										// Rotationsmatrix um die X-Achse berechnen
ageG_Matrix	ageG_MatrixRotationY(const float f);																										// Rotationsmatrix um die Y-Achse berechnen
ageG_Matrix	ageG_MatrixRotationZ(const float f);																										// Rotationsmatrix um die Z-Achse berechnen
ageG_Matrix	ageG_MatrixRotation(const float x, const float y, const float z);																			// Rotiert um alle drei Achsen
ageG_Matrix	ageG_MatrixRotationAxis(const tbVector3& v, const float f);																				// Rotationsmatrix um eine beliebige Achse berechnen
ageG_Matrix	ageG_MatrixScaling(const tbVector3& v);																									// Skalierungsmatrix berechnen
ageG_Matrix	ageG_MatrixAxes(const tbVector3& vXAxis, const tbVector3& vYAxis, const tbVector3& vZAxis);												// Liefert eine Achsenmatrix
float		ageG_MatrixDet(const ageG_Matrix& m, float* pfOut);																							// Determinante berechnen
ageG_Matrix	ageG_MatrixInvert(const ageG_Matrix& m);																										// Invertierte (umgekehrte) Matrix berechnen
ageG_Matrix	ageG_MatrixTranspose(const ageG_Matrix& m);																									// Transponierte Matrix berechnen
ageG_Matrix	ageG_MatrixProjection(const float fFOV, const float fAspect, const float fNearPlane, const float fFarPlane);								// Projektionsmatrix berechnen
ageG_Matrix	ageG_MatrixCamera(const tbVector3& vPos, const tbVector3& vLookAt, const tbVector3& vUp = tbVector3(0.0f, 1.0f, 0.0f));					// Kameramatrix erzeugen
ageG_Matrix	ageG_MatrixToTex2DMatrix(const ageG_Matrix& m);																								// In Texturmatrix umwandeln

#endif //MATRIX_H