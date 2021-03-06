// VFile.cpp: implementation of the VFile class.
//
//////////////////////////////////////////////////////////////////////

//#include "../AiGinE.h"
#include "VFile.h"

// ******************************************************************
// Konstruktor der VFile-Klasse
VFile::VFile()
{
	// Alles zur�cksetzen
	ZeroMemory(this, sizeof(VFile));
}

// ******************************************************************
// Destruktor der VFile-Klasse
VFile::~VFile()
{
	// Puffer l�schen
	AGE_SAFE_MEMFREE(m_pBuffer);
}

// ******************************************************************
// Erstellen einer virtuellen Datei aus einer echten Datei
Result VFile::Init(char* pcFilename)
{
	// Parameter pr�fen
	if(pcFilename == NULL) AGE_ERROR_NULL_POINTER("pcFilename", AGE_ERROR);


	// Datei laden
	if(ReadFile(pcFilename, &m_pBuffer)) AGE_ERROR_FILE(pcFilename, AGE_ERROR);

	// Gr��e eintragen
	m_iSize = MemGetSize(m_pBuffer);

	return AGE_OK;
}

// ******************************************************************
// Virtuelle Datei aus Speicherbereich erstellen
Result VFile::Init(void* pMemory,
					   int iSize)
{
	// Parameter pr�fen
	if(pMemory == NULL)	AGE_ERROR_NULL_POINTER("pMemory", AGE_ERROR);
	if(iSize <= 0)		AGE_ERROR_INVALID_VALUE("iSize", AGE_ERROR);


	// Kopie des Speicherbereichs anlegen und Daten ausf�llen
	m_pBuffer = MemAlloc(iSize);
	if(m_pBuffer == NULL) AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);

	// Daten kopieren und Gr��e eintragen
	memcpy(m_pBuffer, pMemory, iSize);
	m_iSize = iSize;

	return AGE_OK;
}

// ******************************************************************
// Virtuelle Datei aus Ressource erstellen
Result VFile::Init(HMODULE hModule,
					   char* pcResourceName,
					   char* pcResourceType)
{
	// Parameter pr�fen
	if(hModule == NULL)			AGE_ERROR_NULL_POINTER("hModule", AGE_ERROR);
	if(pcResourceName == NULL)	AGE_ERROR_NULL_POINTER("pcResourceName", AGE_ERROR);
	if(pcResourceType == NULL)	AGE_ERROR_NULL_POINTER("pcResourceType", AGE_ERROR);


	// Ressource suchen
	HRSRC hResourceInfo = FindResource(hModule, pcResourceName, pcResourceType);
	if(hResourceInfo == NULL) AGE_ERROR_RESOURCE(pcResourceName, pcResourceType, AGE_ERROR);

	// Gefundene Ressource laden
	HGLOBAL hResource = LoadResource(hModule, hResourceInfo);
	if(hResource == NULL) AGE_ERROR_RESOURCE(pcResourceName, pcResourceType, AGE_ERROR);

	// Gr��e eintragen und Kopie der Ressource anlegen
	m_iSize = SizeofResource(hModule, hResourceInfo);
	m_pBuffer = MemAlloc(m_iSize);
	if(m_pBuffer == NULL) AGE_ERROR_RESOURCE(pcResourceName, pcResourceType, AGE_ERROR);

	// Daten kopieren
	memcpy(m_pBuffer, LockResource(hResource), m_iSize);

	return AGE_OK;
}

// ******************************************************************
// Lesen aus einer virtuellen Datei
Result VFile::Read(int iNumBytes,
					   void* pOut)
{
	// Parameter pr�fen
	if(iNumBytes == 0)	return AGE_OK;
	if(iNumBytes < 0)	AGE_ERROR_INVALID_VALUE("iNumBytes", AGE_ERROR);
	if(pOut == NULL)	AGE_ERROR_NULL_POINTER("pOut", AGE_ERROR);


	BOOL bError = FALSE;

	// �berschreitet der Lesevorgang das Dateiende?
	if(m_iCursor + iNumBytes > m_iSize)
	{
		// Das Ende der Datei ist �berschritten - das wird vermerkt!
		// Die Anzahl der zu lesenden Bytes muss angepasst werden.
		iNumBytes = m_iSize - m_iCursor;
		bError = TRUE;
	}

	// Daten kopieren und den Lesezeiger weiterbewegen
	memcpy(pOut, (BYTE*)(m_pBuffer) + m_iCursor, iNumBytes);
	m_iCursor += iNumBytes;
	if(m_iCursor >= m_iSize) m_bEOF = TRUE;

	// Wurde das Ende der Datei weit �berschritten, so wird ein Fehlercode
	// zur�ckgeliefert.
	return bError ? AGE_ERROR : AGE_OK;
}

// ******************************************************************
// Speichern einer virtuellen Datei
Result VFile::SaveToFile(char* pcFilename)
{
	// Daten speichern
	return WriteFile(pcFilename, m_pBuffer, m_iSize);
}

// ******************************************************************
// Setzen des Lesezeigers
Result VFile::Seek(VFileSeekOrigin Origin,
					   int iOffset)
{
	// Parameter pr�fen
	if(Origin < 0 || Origin >= AGE_VFSO_MAX) AGE_ERROR_INVALID_VALUE("Origin", AGE_ERROR);


	int iNewCursor;

	// Neue Cursorposition berechnen
	switch(Origin)
	{
	case AGE_VFSO_START:		iNewCursor = iOffset; break;
	case AGE_VFSO_END:		iNewCursor = m_iSize - 1 + iOffset; break;
	case AGE_VFSO_CURSOR:	iNewCursor = m_iCursor + iOffset; break;
	}

	// Ist die neue Position in Ordnung? Falls ja - einsetzen!
	if(iNewCursor >= 0 && iNewCursor <= m_iSize)
	{
		m_iCursor = iNewCursor;
		m_bEOF = iNewCursor == m_iSize;
		return AGE_OK;
	}
	else
	{
		AGE_ERROR("Der Lesezeiger w�re au�erhalb der virtuellen Datei!", AGE_ERROR);
	}
}
