/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVFile.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit virtuellen Dateien

	Autor:
	David Scherfgen

********************************************************************/

// Aufzählung für die Seek-Methode der tbVFile-Klasse
enum tbVFileSeekOrigin
{
	TB_VFSO_START,	// Start der Datei
	TB_VFSO_END,	// Ende der Datei
	TB_VFSO_CURSOR,	// Zeigerposition
	TB_VFSO_MAX
};

// ******************************************************************
// Klasse für virtuelle Dateien
class TRIBASE_API tbVFile
{
private:
	// Variablen
	void*	m_pBuffer;	// Puffer (Kopie der Quelldaten)
	int		m_iSize;	// Größe der virtuellen Datei
	int		m_iCursor;	// Cursorposition
	BOOL	m_bEOF;		// Ende der Datei?

public:
	// Konstruktor und Destruktor
	tbVFile();
	~tbVFile();

	// Methoden
	tbResult Init(char* pcFilename);											// Aus Datei initialisieren
	tbResult Init(void* pMemory, int iSize);									// Aus Speicher initialisieren
	tbResult Init(HMODULE hModule, char* pcResourceName, char* pcResourceType);	// Aus Ressource initialisieren
	tbResult Read(int iNumBytes, void* pOut);									// Daten lesen
	tbResult SaveToFile(char* pcFilename);										// Inhalt in Datei speichern
	tbResult Seek(tbVFileSeekOrigin Origin, int iOffset);						// Zeiger setzen

	// Inline-Methoden
	inline void*	GetBuffer()	{return m_pBuffer;}
	inline int		GetSize()	{return m_iSize;}
	inline int		GetCursor()	{return m_iCursor;}
	inline BOOL		IsEOF()		{return m_bEOF;}
};

// ******************************************************************