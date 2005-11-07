/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbFile.h
	========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit verschiedenen Dateien

	Autor:
	David Scherfgen

********************************************************************/

#pragma pack(1)

// Funktionen deklarieren
TRIBASE_API tbResult tbReadFile(char* pcFilename, void** ppOut);													// Datei laden, mit Specials
TRIBASE_API tbResult tbReadZIPFileObject(char* pcZIPFilename, char* pcObjectName, char* pcPassword, void** ppOut);	// Objekt aus .ZIP-Datei laden
TRIBASE_API tbResult tbReadRawFile(char* pcFilename, void** ppOut);													// "Normale" Datei laden
TRIBASE_API tbResult tbWriteFile(char* pcFilename, void* pData, int iNumBytes);										// Datei erstellen

// ******************************************************************