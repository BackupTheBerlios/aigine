/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbMemory.h
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Logbuchdateifunktionen

	Autor:
	David Scherfgen

********************************************************************/

// Globale Variablen
extern FILE* tb_g_pLogFile;	// Logbuchdateihandle

// ******************************************************************
// Funktionen deklarieren
TRIBASE_API tbResult tbInitLog();								// Logbuchdatei initialisieren
TRIBASE_API tbResult tbExitLog();								// Logbuchdatei herunterfahren
TRIBASE_API tbResult tbWriteToLogDirect(char* pcFormat, ...);	// String unverändert ins Logbuch schreiben
TRIBASE_API tbResult tbWriteToLog(char* pcFormat, ...);			// String an HTML anpassen und ins Logbuch schreiben
TRIBASE_API tbResult tbWriteVector2ToLog(tbVector2& v);			// 2D-Vektor ins Logbuch schreiben
TRIBASE_API tbResult tbWriteVector3ToLog(tbVector3& v);			// 3D-Vektor ins Logbuch schreiben
TRIBASE_API tbResult tbWriteMatrixToLog(tbMatrix& m);			// Matrix ins Logbuch schreiben
TRIBASE_API tbResult tbWritePlaneToLog(tbPlane& p);				// Ebene ins Logbuch schreiben
TRIBASE_API tbResult tbWriteColorToLog(tbColor& c);				// Farbe ins Logbuch schreiben

// ******************************************************************