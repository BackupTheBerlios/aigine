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

#include <AGE.h>

// ******************************************************************
// Konstruktor der tbVFile-Klasse
ageD_VFile::ageD_VFile()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(ageD_VFile));
}

// ******************************************************************
// Destruktor der tbVFile-Klasse
ageD_VFile::~ageD_VFile()
{
	// Puffer löschen
	AGE_SAFE_MEMFREE(m_pBuffer);
}

// ******************************************************************
// Erstellen einer virtuellen Datei aus einer echten Datei
age_Result ageD_VFile::Init(char* pcFilename)
{
	// Parameter prüfen
	if(pcFilename == NULL) AGE_ERROR_NULL_POINTER("pcFilename", AGE_ERROR);


	// Datei laden
	if(ageD_ReadFile(pcFilename, &m_pBuffer)) AGE_ERROR_FILE(pcFilename, AGE_ERROR);

	// Größe eintragen
	m_iSize = ageD_MemGetSize(m_pBuffer);

	return AGE_OK;
}

// ******************************************************************
// Virtuelle Datei aus Speicherbereich erstellen
age_Result ageD_VFile::Init(void* pMemory,
					   int iSize)
{
	// Parameter prüfen
	if(pMemory == NULL)	AGE_ERROR_NULL_POINTER("pMemory", AGE_ERROR);
	if(iSize <= 0)		AGE_ERROR_INVALID_VALUE("iSize", AGE_ERROR);


	// Kopie des Speicherbereichs anlegen und Daten ausfüllen
	m_pBuffer = ageD_MemAlloc(iSize);
	if(m_pBuffer == NULL) AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);

	// Daten kopieren und Größe eintragen
	memcpy(m_pBuffer, pMemory, iSize);
	m_iSize = iSize;

	return AGE_OK;
}

// ******************************************************************
// Virtuelle Datei aus Ressource erstellen
age_Result ageD_VFile::Init(HMODULE hModule,
					   char* pcResourceName,
					   char* pcResourceType)
{
	// Parameter prüfen
	if(hModule == NULL)			AGE_ERROR_NULL_POINTER("hModule", AGE_ERROR);
	if(pcResourceName == NULL)	AGE_ERROR_NULL_POINTER("pcResourceName", AGE_ERROR);
	if(pcResourceType == NULL)	AGE_ERROR_NULL_POINTER("pcResourceType", AGE_ERROR);


	// Ressource suchen
	HRSRC hResourceInfo = FindResource(hModule, pcResourceName, pcResourceType);
	if(hResourceInfo == NULL) AGE_ERROR_RESOURCE(pcResourceName, pcResourceType,AGE_ERROR);

	// Gefundene Ressource laden
	HGLOBAL hResource = LoadResource(hModule, hResourceInfo);
	if(hResource == NULL) AGE_ERROR_RESOURCE(pcResourceName, pcResourceType, AGE_ERROR);

	// Größe eintragen und Kopie der Ressource anlegen
	m_iSize = SizeofResource(hModule, hResourceInfo);
	m_pBuffer = ageD_MemAlloc(m_iSize);
	if(m_pBuffer == NULL) AGE_ERROR_RESOURCE(pcResourceName, pcResourceType, AGE_ERROR);

	// Daten kopieren
	memcpy(m_pBuffer, LockResource(hResource), m_iSize);

	return AGE_OK;
}

// ******************************************************************
// Lesen aus einer virtuellen Datei
age_Result ageD_VFile::Read(int iNumBytes,
					   void* pOut)
{
	// Parameter prüfen
	if(iNumBytes == 0)	return AGE_OK;
	if(iNumBytes < 0)	AGE_ERROR_INVALID_VALUE("iNumBytes", AGE_ERROR);
	if(pOut == NULL)	AGE_ERROR_NULL_POINTER("pOut", AGE_ERROR);


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
	return bError ? AGE_ERROR : AGE_OK;
}

// ******************************************************************
// Speichern einer virtuellen Datei
age_Result ageD_VFile::SaveToFile(char* pcFilename)
{
	// Daten speichern
	return ageWriteFile(pcFilename, m_pBuffer, m_iSize);
}

// ******************************************************************
// Setzen des Lesezeigers
age_Result ageD_VFile::Seek(tbVFileSeekOrigin Origin,
					   int iOffset)
{
	// Parameter prüfen
	if(Origin < 0 || Origin >= AGE_VFSO_MAX) AGE_ERROR_INVALID_VALUE("Origin", AGE_ERROR);


	int iNewCursor;

	// Neue Cursorposition berechnen
	switch(Origin)
	{
	case AGE_VFSO_START:		iNewCursor = iOffset; break;
	case AGE_VFSO_END:		    iNewCursor = m_iSize - 1 + iOffset; break;
	case AGE_VFSO_CURSOR:	    iNewCursor = m_iCursor + iOffset; break;
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
		AGE_ERROR("Der Lesezeiger wäre außerhalb der virtuellen Datei!", AGE_ERROR);
	}
}

// ******************************************************************