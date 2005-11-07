/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVFile.cpp
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit virtuellen Dateien

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbVFile-Klasse
tbVFile::tbVFile()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbVFile));
}

// ******************************************************************
// Destruktor der tbVFile-Klasse
tbVFile::~tbVFile()
{
	// Puffer löschen
	TB_SAFE_MEMFREE(m_pBuffer);
}

// ******************************************************************
// Erstellen einer virtuellen Datei aus einer echten Datei
tbResult tbVFile::Init(char* pcFilename)
{
	// Parameter prüfen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", TB_ERROR);


	// Datei laden
	if(tbReadFile(pcFilename, &m_pBuffer)) TB_ERROR_FILE(pcFilename, TB_ERROR);

	// Größe eintragen
	m_iSize = tbMemGetSize(m_pBuffer);

	return TB_OK;
}

// ******************************************************************
// Virtuelle Datei aus Speicherbereich erstellen
tbResult tbVFile::Init(void* pMemory,
					   int iSize)
{
	// Parameter prüfen
	if(pMemory == NULL)	TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);
	if(iSize <= 0)		TB_ERROR_INVALID_VALUE("iSize", TB_ERROR);


	// Kopie des Speicherbereichs anlegen und Daten ausfüllen
	m_pBuffer = tbMemAlloc(iSize);
	if(m_pBuffer == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Daten kopieren und Größe eintragen
	memcpy(m_pBuffer, pMemory, iSize);
	m_iSize = iSize;

	return TB_OK;
}

// ******************************************************************
// Virtuelle Datei aus Ressource erstellen
tbResult tbVFile::Init(HMODULE hModule,
					   char* pcResourceName,
					   char* pcResourceType)
{
	// Parameter prüfen
	if(hModule == NULL)			TB_ERROR_NULL_POINTER("hModule", TB_ERROR);
	if(pcResourceName == NULL)	TB_ERROR_NULL_POINTER("pcResourceName", TB_ERROR);
	if(pcResourceType == NULL)	TB_ERROR_NULL_POINTER("pcResourceType", TB_ERROR);


	// Ressource suchen
	HRSRC hResourceInfo = FindResource(hModule, pcResourceName, pcResourceType);
	if(hResourceInfo == NULL) TB_ERROR_RESOURCE(pcResourceName, pcResourceType, TB_ERROR);

	// Gefundene Ressource laden
	HGLOBAL hResource = LoadResource(hModule, hResourceInfo);
	if(hResource == NULL) TB_ERROR_RESOURCE(pcResourceName, pcResourceType, TB_ERROR);

	// Größe eintragen und Kopie der Ressource anlegen
	m_iSize = SizeofResource(hModule, hResourceInfo);
	m_pBuffer = tbMemAlloc(m_iSize);
	if(m_pBuffer == NULL) TB_ERROR_RESOURCE(pcResourceName, pcResourceType, TB_ERROR);

	// Daten kopieren
	memcpy(m_pBuffer, LockResource(hResource), m_iSize);

	return TB_OK;
}

// ******************************************************************
// Lesen aus einer virtuellen Datei
tbResult tbVFile::Read(int iNumBytes,
					   void* pOut)
{
	// Parameter prüfen
	if(iNumBytes == 0)	return TB_OK;
	if(iNumBytes < 0)	TB_ERROR_INVALID_VALUE("iNumBytes", TB_ERROR);
	if(pOut == NULL)	TB_ERROR_NULL_POINTER("pOut", TB_ERROR);


	BOOL bError = FALSE;

	// Überschreitet der Lesevorgang das Dateiende?
	if(m_iCursor + iNumBytes > m_iSize)
	{
		// Das Ende der Datei ist überschritten - das wird vermerkt!
		// Die Anzahl der zu lesenden Bytes muss angepasst werden.
		iNumBytes = m_iSize - m_iCursor;
		bError = TRUE;
	}

	// Daten kopieren und den Lesezeiger weiterbewegen
	memcpy(pOut, (BYTE*)(m_pBuffer) + m_iCursor, iNumBytes);
	m_iCursor += iNumBytes;
	if(m_iCursor >= m_iSize) m_bEOF = TRUE;

	// Wurde das Ende der Datei weit überschritten, so wird ein Fehlercode
	// zurückgeliefert.
	return bError ? TB_ERROR : TB_OK;
}

// ******************************************************************
// Speichern einer virtuellen Datei
tbResult tbVFile::SaveToFile(char* pcFilename)
{
	// Daten speichern
	return tbWriteFile(pcFilename, m_pBuffer, m_iSize);
}

// ******************************************************************
// Setzen des Lesezeigers
tbResult tbVFile::Seek(tbVFileSeekOrigin Origin,
					   int iOffset)
{
	// Parameter prüfen
	if(Origin < 0 || Origin >= TB_VFSO_MAX) TB_ERROR_INVALID_VALUE("Origin", TB_ERROR);


	int iNewCursor;

	// Neue Cursorposition berechnen
	switch(Origin)
	{
	case TB_VFSO_START:		iNewCursor = iOffset; break;
	case TB_VFSO_END:		iNewCursor = m_iSize - 1 + iOffset; break;
	case TB_VFSO_CURSOR:	iNewCursor = m_iCursor + iOffset; break;
	}

	// Ist die neue Position in Ordnung? Falls ja - einsetzen!
	if(iNewCursor >= 0 && iNewCursor <= m_iSize)
	{
		m_iCursor = iNewCursor;
		m_bEOF = iNewCursor == m_iSize;
		return TB_OK;
	}
	else
	{
		TB_ERROR("Der Lesezeiger wäre außerhalb der virtuellen Datei!", TB_ERROR);
	}
}

// ******************************************************************