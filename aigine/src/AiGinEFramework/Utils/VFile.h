// VFile.h: interface for the VFile class.
//
//////////////////////////////////////////////////////////////////////

#ifndef VFILE_H
#define VFILE_H

#include "../AiGinE.h"

// Aufzählung für die Seek-Methode der VFile-Klasse
enum VFileSeekOrigin
{
	AGE_VFSO_START,	// Start der Datei
	AGE_VFSO_END,	// Ende der Datei
	AGE_VFSO_CURSOR,	// Zeigerposition
	AGE_VFSO_MAX
};

// ******************************************************************
// Klasse für virtuelle Dateien
class VFile
{
private:
	// Variablen
	void*	m_pBuffer;	// Puffer (Kopie der Quelldaten)
	int		m_iSize;	// Größe der virtuellen Datei
	int		m_iCursor;	// Cursorposition
	BOOL	m_bEOF;		// Ende der Datei?

public:
	// Konstruktor und Destruktor
	VFile();
	~VFile();

	// Methoden
	Result Init(char* pcFilename);											// Aus Datei initialisieren
	Result Init(void* pMemory, int iSize);									// Aus Speicher initialisieren
	Result Init(HMODULE hModule, char* pcResourceName, char* pcResourceType);	// Aus Ressource initialisieren
	Result Read(int iNumBytes, void* pOut);									// Daten lesen
	Result SaveToFile(char* pcFilename);										// Inhalt in Datei speichern
	Result Seek(VFileSeekOrigin Origin, int iOffset);						// Zeiger setzen

	// Inline-Methoden
	inline void*	GetBuffer()	{return m_pBuffer;}
	inline int		GetSize()	{return m_iSize;}
	inline int		GetCursor()	{return m_iCursor;}
	inline BOOL		IsEOF()		{return m_bEOF;}
};

#endif //VFILE_H
