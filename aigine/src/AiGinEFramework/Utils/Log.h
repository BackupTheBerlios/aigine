// Log.h: interface for the Log class.
//
//////////////////////////////////////////////////////////////////////

#ifndef LOG_H
#define LOG_H

#include "../AiGinE.h"

// Globale Variablen
extern FILE* _g_pLogFile;	// Logbuchdateihandle

// ******************************************************************
// Funktionen deklarieren
Result InitLog();								// Logbuchdatei initialisieren
Result ExitLog();								// Logbuchdatei herunterfahren
Result WriteToLogDirect(char* pcFormat, ...);	// String unverändert ins Logbuch schreiben
Result WriteToLog(char* pcFormat, ...);			// String an HTML anpassen und ins Logbuch schreiben
Result WriteVector2ToLog(Vector2D& v);			// 2D-Vektor ins Logbuch schreiben
Result WriteVector3ToLog(Vector3D& v);			// 3D-Vektor ins Logbuch schreiben
Result WriteMatrixToLog(Matrix4x4& m);			// Matrix ins Logbuch schreiben
Result WritePlaneToLog(Plane& p);				// Ebene ins Logbuch schreiben
Result WriteColorToLog(Color& c);				// Farbe ins Logbuch schreiben

// ******************************************************************

#endif //LOG_H
