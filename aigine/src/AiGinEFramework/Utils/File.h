// File.h: interface for the File class.
//
//////////////////////////////////////////////////////////////////////

#ifndef FILE_H
#define FILE_H

#include "../AiGinE.h"

#pragma pack(1)

// Funktionen deklarieren
Result ReadFile(char* pcFilename, void** ppOut);													// Datei laden, mit Specials
Result ReadZIPFileObject(char* pcZIPFilename, char* pcObjectName, char* pcPassword, void** ppOut);	// Objekt aus .ZIP-Datei laden
Result ReadRawFile(char* pcFilename, void** ppOut);													// "Normale" Datei laden
Result WriteFile(char* pcFilename, void* pData, int iNumBytes);										// Datei erstellen

#endif //FILE_H
