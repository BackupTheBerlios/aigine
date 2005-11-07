/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbTextureManager.cpp
	====================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Implementierung des Texturmanagers

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
BOOL				tbTextureManager::m_bInitialized = FALSE;
int					tbTextureManager::m_iNumTextures = 0;
int					tbTextureManager::m_iListSize = 0;
tbTextureListEntry*	tbTextureManager::m_pTextureList = NULL;

// ******************************************************************
// Die Initialisierungsmethode
tbResult tbTextureManager::Init(int iInitialListSize) // = 256
{
	// Den entsprechenden Platz für die Liste schaffen
	if(SetListSize(iInitialListSize))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der Texturliste!", TB_ERROR);
	}

	TB_INFO("Texturmanager wurde initialisiert!");

	m_bInitialized = TRUE;

	return TB_OK;
}

// ******************************************************************
// Herunterfahren
tbResult tbTextureManager::Exit()
{
	m_bInitialized = FALSE;

	// Alle Texturen löschen
	DeleteAllTextures();

	TB_INFO("Der Texturmanager wurde heruntergefahren.");

	return TB_OK;
}

// ******************************************************************
// Diese Methode passt die Größe der Texturliste an.
tbResult tbTextureManager::SetListSize(int iNewSize)
{
	// Wenn die Größe schon stimmt, müssen wir nichts tun.
	if(m_iListSize == iNewSize) return TB_OK;

	// Die neue Größe sollte nicht kleiner sein als die alte!
	if(iNewSize < m_iListSize)
	{
		// Warnmeldung ausgeben
		TB_WARNING("Die Texturliste wird verkleinert!");
	}

	// Wenn die Liste noch gar nicht existiert, muss erst einmal Speicher
	// reserviert werden.
	if(m_pTextureList == NULL)
	{
		m_pTextureList = (tbTextureListEntry*)(tbMemAlloc(iNewSize * sizeof(tbTextureListEntry)));
		if(m_pTextureList == NULL)
		{
			// Fehler!
			TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
		}
	}
	else
	{
		// Die Liste existiert schon, also müssen wir den Speicher nur
		// neu anpassen.
		m_pTextureList = (tbTextureListEntry*)(tbMemReAlloc(m_pTextureList,
			                                                iNewSize * sizeof(tbTextureListEntry)));
		if(m_pTextureList == NULL)
		{
			// Fehler!
			TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
		}
	}

	// Die neue Größe eintragen
	m_iListSize = iNewSize;

	return TB_OK;
}

// ******************************************************************
// Diese Methode findet den Index einer Textur heraus.
int tbTextureManager::GetTextureIndex(PDIRECT3DBASETEXTURE9 pTexture)
{
	// Parameter prüfen
	if(pTexture == NULL) TB_ERROR_NULL_POINTER("pTexture", -1);


	// Jetzt gehen wir die Liste durch, bis die entsprechende
	// Textur gefunden wurde.
	for(int iEntry = 0; iEntry < m_iListSize; iEntry++)
	{
		if(m_pTextureList[iEntry].bExists)
		{
			// Vergleichen
			if(m_pTextureList[iEntry].pTexture == pTexture)
			{
				// Gefunden! Index zurückliefern.
				return iEntry;
			}
		}
	}

	// Die Textur ist nicht in der Liste!
	return -1;
}

// ******************************************************************
// Erweiterte Suche nach einer Textur
int tbTextureManager::GetTextureIndex(char* pcSourceFilename,
									  BOOL bDontCareForOptions,
									  int iWidth,
									  int iHeight,
									  int iDepth,
									  int iNumMIPLevels,
									  D3DFORMAT Format,
									  DWORD dwUsage,
									  D3DPOOL Pool,
									  DWORD dwFilter,
									  DWORD dwMIPFilter,
									  D3DCOLOR ColorKey)
{
	// Parameter prüfen
	if(pcSourceFilename == NULL) TB_ERROR_NULL_POINTER(pcSourceFilename, -1);


	// Die Liste durchgehen und jede Textur mit der gesuchten vergleichen
	for(int iEntry = 0; iEntry < m_iListSize; iEntry++)
	{
		if(m_pTextureList[iEntry].bExists)
		{
			// Vergleichen. Zuerst nur den Dateinamen.
			if(!stricmp(pcSourceFilename, m_pTextureList[iEntry].acSourceFilename))
			{
				// Die Dateinamen stimmen überein!
				// Sind weitere Übereinstimmungskriterien erwünscht?
				if(bDontCareForOptions)
				{
					// Nein - den Index zurückliefern!
					return iEntry;
				}
				else
				{
					// Weitere Vergleiche durchführen
					if(iWidth != m_pTextureList[iEntry].iWidth) goto NextTexture;
					if(iHeight != m_pTextureList[iEntry].iHeight) goto NextTexture;
					if(iDepth != m_pTextureList[iEntry].iDepth) goto NextTexture;
					if(iNumMIPLevels != m_pTextureList[iEntry].iNumMIPLevels) goto NextTexture;
					if(Format != m_pTextureList[iEntry].Format) goto NextTexture;
					if(dwUsage != m_pTextureList[iEntry].dwUsage) goto NextTexture;
					if(Pool != m_pTextureList[iEntry].Pool) goto NextTexture;
					if(dwFilter != m_pTextureList[iEntry].dwFilter) goto NextTexture;
					if(dwMIPFilter != m_pTextureList[iEntry].dwMIPFilter) goto NextTexture;
					if(ColorKey != m_pTextureList[iEntry].ColorKey) goto NextTexture;

					// OK - es handelt sich um die gesuchte Textur!
					return iEntry;
				}
			}
		}

NextTexture:
		{}
	}

	// Die Textur ist nicht in der Liste!
	return -1;
}

// ******************************************************************
// Suchen nach einem freien Index
int tbTextureManager::GetNewIndex()
{
	// Die Liste durchgehen und nach dem ersten Eintrag suchen,
	// bei dem das bExists-Element FALSE ist
	for(int iEntry = 0; iEntry < m_iListSize; iEntry++)
	{
		if(!m_pTextureList[iEntry].bExists)
		{
			// Freier Index gefunden - zurückliefern!
			return iEntry;
		}
	}

	// Kein freier Index mehr!
	return -1;
}

// ******************************************************************
// Diese Methode gibt eine Textur frei.
tbResult tbTextureManager::ReleaseTexture(PDIRECT3DBASETEXTURE9 pTexture)
{
	int iIndex;

	// Parameter prüfen
	if(pTexture == NULL) TB_ERROR_NULL_POINTER("pTexture", TB_ERROR);


	// Den Index der Textur in der Liste herausfinden
	iIndex = GetTextureIndex(pTexture);
	if(iIndex == -1)
	{
		// Die Textur gibt es nicht!
		TB_ERROR("Die angegebene Textur wurde nicht gefunden!", TB_NOT_FOUND);
	}

	// Release aufrufen und prüfen, ob der Referenzzähler null erreicht hat
	if(m_pTextureList[iIndex].pTexture->Release() == 0)
	{
		// Referenzzähler ist bei null! Die Textur wird gelöscht, indem wir
		// den Speicherbereich des Listeneintrags mit Nullen überschreiben
		// also auch das bExists-Element.
		ZeroMemory(&m_pTextureList[iIndex], sizeof(tbTextureListEntry));
		m_iNumTextures--;
	}

	return TB_OK;
}

// ******************************************************************
// Löschen aller Texturen
tbResult tbTextureManager::DeleteAllTextures()
{
	// Die gesamte Liste durchgehen
	for(int iEntry = 0; iEntry < m_iListSize; iEntry++)
	{
		// Befindet sich dort eine Textur?
		if(m_pTextureList[iEntry].bExists &&
		   m_pTextureList[iEntry].pTexture != NULL)
		{
			// So lange Release aufrufen, bis der Referenzzähler null ist
			while(m_pTextureList[iEntry].pTexture->Release() > 0) {}

			// Texturzähler verringern
			m_iNumTextures--;
		}
	}

	// Prüfen, ob die Anzahl der Texturen null ist
	if(m_iNumTextures != 0)
	{
		// Das sollte eigentlich nie passieren...
		TB_WARNING("Es wurden nicht alle Texturen gelöscht!");
	}

	// Die ganze Liste löschen
	TB_SAFE_MEMFREE(m_pTextureList);
	m_iListSize = 0;
	m_iNumTextures = 0;

	return TB_OK;
}

// ******************************************************************
// Diese Methode fügt eine Textur zur Liste hinzu.
int tbTextureManager::AddTextureToList(PDIRECT3DBASETEXTURE9 pTexture,
									   char* pcSourceFilename,
									   int iWidth,
									   int iHeight,
									   int iDepth,
									   int iNumMIPLevels,
									   D3DFORMAT Format,
									   DWORD dwUsage,
									   D3DPOOL Pool,
									   DWORD dwFilter,
									   DWORD dwMIPFilter,
									   D3DCOLOR ColorKey)
{
	int iIndex;

	// Parameter prüfen
	if(pTexture == NULL) TB_ERROR_NULL_POINTER("pTexture", -1);


	// Als erstes suchen wir einen freien Platz in der Texturliste.
	iIndex = GetNewIndex();
	if(iIndex == -1)
	{
		// Kein Platz mehr! Wir erweitern die Texturliste um 32 Elemente.
		if(SetListSize(m_iListSize + 32))
		{
			// Fehler - nicht genug Speicher zum Erweitern der Liste!
			TB_ERROR_OUT_OF_MEMORY(-1);
		}

		// Noch einmal einen freien Platz suchen
		iIndex = GetNewIndex();
		if(iIndex == -1)
		{
			// Das sollte nicht passieren!
			TB_ERROR("Unerwarteter Fehler!", -1);
		}
	}

	// Die Daten eintragen
	m_pTextureList[iIndex].bExists			= TRUE;
	m_pTextureList[iIndex].pTexture			= pTexture;
	m_pTextureList[iIndex].iWidth			= iWidth;
	m_pTextureList[iIndex].iHeight			= iHeight;
	m_pTextureList[iIndex].iDepth			= iDepth;
	m_pTextureList[iIndex].iNumMIPLevels	= iNumMIPLevels;
	m_pTextureList[iIndex].Format			= Format;
	m_pTextureList[iIndex].dwUsage			= dwUsage;
	m_pTextureList[iIndex].Pool				= Pool;
	m_pTextureList[iIndex].dwFilter			= dwFilter;
	m_pTextureList[iIndex].dwMIPFilter		= dwMIPFilter;
	m_pTextureList[iIndex].ColorKey			= ColorKey;
	if(pcSourceFilename == NULL) strcpy(m_pTextureList[iIndex].acSourceFilename, "");
	else strcpy(m_pTextureList[iIndex].acSourceFilename, pcSourceFilename);

	// Anzahl der Texturen erhöhen
	m_iNumTextures++;

	// Den Index zurückliefern
	return iIndex;
}

// ******************************************************************
// Laden einer gewöhnlichen Textur (virtuelle Datei)
PDIRECT3DTEXTURE9 tbTextureManager::LoadTexture(tbVFile* pVFile,
												int iWidth,				// = D3DX_DEFAULT
												int iHeight,			// = D3DX_DEFAULT
												int iNumMIPLevels,		// = D3DX_DEFAULT
												D3DFORMAT Format,		// = D3DFMT_UNKNOWN
												DWORD dwUsage,			// = 0
												D3DPOOL Pool,			// = D3DPOOL_MANAGED
												DWORD dwFilter,			// = D3DX_DEFAULT
												DWORD dwMIPFilter,		// = D3DX_DEFAULT
												D3DCOLOR ColorKey,		// = 0x00000000
												char* pcSourceFilename)	// = NULL
{
	HRESULT				hResult;
	PDIRECT3DTEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(pVFile == NULL)					TB_ERROR_NULL_POINTER("pVFile", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);
	if(pcSourceFilename == NULL)		pcSourceFilename = "";


	// Die D3DX-Funktion aufrufen, die eine Textur aus dem Speicher lädt.
	// Als Speicherangabe verwenden wir die aktuelle Lesezeigerposition der virtuellen Datei.
	// Für die Größe geben wir die Anzahl der restlichen Bytes an (Größe - Lesezeiger).
TryAgain:
	if(FAILED(hResult = D3DXCreateTextureFromFileInMemoryEx(tbDirect3D::GetDevice(),
		                                                    &(((BYTE*)(pVFile->GetBuffer()))[pVFile->GetCursor()]),
															pVFile->GetSize() - pVFile->GetCursor(),
															iWidth,
															iHeight,
															iNumMIPLevels,
															dwUsage,
															Format,
															Pool,
															dwFilter,
															dwMIPFilter,
															ColorKey,
															NULL,
															NULL,
															&pTexture)))
	{
		// Fehler beim Laden!
		if(Format == D3DFMT_UNKNOWN) {Format = D3DFMT_X8R8G8B8; goto TryAgain;}
		else if(Format == D3DFMT_X8R8G8B8) {Format = D3DFMT_X1R5G5B5; goto TryAgain;}
		else if(Format == D3DFMT_X1R5G5B5) {Format = D3DFMT_R5G6B5; goto TryAgain;}
		else TB_ERROR_DIRECTX("D3DXCreateTextureFromFileInMemoryEx", hResult, NULL);
	}

	// Die Textur in die Liste eintragen
	if(AddTextureToList(pTexture, pcSourceFilename, iWidth, iHeight, 0, iNumMIPLevels, Format,
		                dwUsage, Pool, dwFilter, dwMIPFilter, ColorKey) == -1)
	{
		// Fehler!
		TB_ERROR("Fehler beim Hinzufügen der Textur zur Liste!", NULL);
	}

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer Würfeltextur (virtuelle Datei)
PDIRECT3DCUBETEXTURE9 tbTextureManager::LoadCubeTexture(tbVFile* pVFile,
														int iSize,				// = D3DX_DEFAULT
														int iNumMIPLevels,		// = D3DX_DEFAULT
														D3DFORMAT Format,		// = D3DFMT_UNKNOWN
														DWORD dwUsage,			// = 0
														D3DPOOL Pool,			// = D3DPOOL_MANAGED
														DWORD dwFilter,			// = D3DX_DEFAULT
														DWORD dwMIPFilter,		// = D3DX_DEFAULT
														D3DCOLOR ColorKey,		// = 0x00000000
														char* pcSourceFilename)	// = NULL
{
	HRESULT					hResult;
	PDIRECT3DCUBETEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(pVFile == NULL)					TB_ERROR_NULL_POINTER("pVFile", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);
	if(pcSourceFilename == NULL)		pcSourceFilename = "";


	// Die D3DX-Funktion aufrufen, die eine Textur aus dem Speicher lädt.
	// Als Speicherangabe verwenden wir die aktuelle Leseposition der virtuellen Datei.
	// Für die Größe geben wir die Anzahl der restlichen Bytes an (Größe - Cursor).
TryAgain:
	if(FAILED(hResult = D3DXCreateCubeTextureFromFileInMemoryEx(tbDirect3D::GetDevice(),
																&(((BYTE*)(pVFile->GetBuffer()))[pVFile->GetCursor()]),
																pVFile->GetSize() - pVFile->GetCursor(),
																iSize,
																iNumMIPLevels,
																dwUsage,
																Format,
																Pool,
																dwFilter,
																dwMIPFilter,
																ColorKey,
																NULL,
																NULL,
																&pTexture)))
	{
		// Fehler beim Laden!
		if(Format == D3DFMT_UNKNOWN) {Format = D3DFMT_X8R8G8B8; goto TryAgain;}
		else if(Format == D3DFMT_X8R8G8B8) {Format = D3DFMT_X1R5G5B5; goto TryAgain;}
		else if(Format == D3DFMT_X1R5G5B5) {Format = D3DFMT_R5G6B5; goto TryAgain;}
		else TB_ERROR_DIRECTX("D3DXCreateCubeTextureFromFileInMemoryEx", hResult, NULL);
	}

	// Die Textur in die Liste eintragen
	if(AddTextureToList(pTexture, pcSourceFilename, iSize, iSize, 0, iNumMIPLevels, Format,
		                dwUsage, Pool, dwFilter, dwMIPFilter, ColorKey) == -1)
	{
		// Fehler!
		TB_ERROR("Fehler beim Hinzufügen der Textur zur Liste!", NULL);
	}

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer Volumentextur (virtuelle Datei)
PDIRECT3DVOLUMETEXTURE9 tbTextureManager::LoadVolumeTexture(tbVFile* pVFile,
															int iWidth,				// = D3DX_DEFAULT
															int iHeight,			// = D3DX_DEFAULT
															int iDepth,				// = D3DX_DEFAULT
															int iNumMIPLevels,		// = D3DX_DEFAULT
															D3DFORMAT Format,		// = D3DFMT_UNKNOWN
															DWORD dwUsage,			// = 0
															D3DPOOL Pool,			// = D3DPOOL_MANAGED
															DWORD dwFilter,			// = D3DX_DEFAULT
															DWORD dwMIPFilter,		// = D3DX_DEFAULT
															D3DCOLOR ColorKey,		// = 0x00000000
															char* pcSourceFilename)	// = NULL
{
	HRESULT					hResult;
	PDIRECT3DVOLUMETEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(pVFile == NULL)					TB_ERROR_NULL_POINTER("pVFile", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);
	if(pcSourceFilename == NULL)		pcSourceFilename = "";


	// Die D3DX-Funktion aufrufen, die eine Textur aus dem Speicher lädt.
	// Als Speicherangabe verwenden wir die aktuelle Leseposition der virtuellen Datei.
	// Für die Größe geben wir die Anzahl der restlichen Bytes an (Größe - Cursor).
TryAgain:
	if(FAILED(hResult = D3DXCreateVolumeTextureFromFileInMemoryEx(tbDirect3D::GetDevice(),
																  &(((BYTE*)(pVFile->GetBuffer()))[pVFile->GetCursor()]),
																  pVFile->GetSize() - pVFile->GetCursor(),
																  iWidth,
																  iHeight,
																  iDepth,
																  iNumMIPLevels,
																  dwUsage,
																  Format,
																  Pool,
																  dwFilter,
																  dwMIPFilter,
																  ColorKey,
																  NULL,
																  NULL,
																  &pTexture)))
	{
		// Fehler beim Laden!
		if(Format == D3DFMT_UNKNOWN) {Format = D3DFMT_X8R8G8B8; goto TryAgain;}
		else if(Format == D3DFMT_X8R8G8B8) {Format = D3DFMT_X1R5G5B5; goto TryAgain;}
		else if(Format == D3DFMT_X1R5G5B5) {Format = D3DFMT_R5G6B5; goto TryAgain;}
		else TB_ERROR_DIRECTX("D3DXCreateVolumeTextureFromFileInMemoryEx", hResult, NULL);
	}

	// Die Textur in die Liste eintragen
	if(AddTextureToList(pTexture, pcSourceFilename, iWidth, iHeight, iDepth, iNumMIPLevels, Format,
		                dwUsage, Pool, dwFilter, dwMIPFilter, ColorKey) == -1)
	{
		// Fehler!
		TB_ERROR("Fehler beim Hinzufügen der Textur zur Liste!", NULL);
	}

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer gewöhnlichen Textur (Datei)
PDIRECT3DTEXTURE9 tbTextureManager::LoadTexture(char* pcFilename,
												int iWidth,			// = D3DX_DEFAULT
												int iHeight,		// = D3DX_DEFAULT
												int iNumMIPLevels,	// = D3DX_DEFAULT
												D3DFORMAT Format,	// = D3DFMT_UNKNOWN
												DWORD dwUsage,		// = 0
												D3DPOOL Pool,		// = D3DPOOL_MANAGED
												DWORD dwFilter,		// = D3DX_DEFAULT
												DWORD dwMIPFilter,	// = D3DX_DEFAULT
												D3DCOLOR ColorKey)	// = 0x00000000
{
	tbVFile*			pVFile;
	PDIRECT3DTEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(pcFilename == NULL)				TB_ERROR_NULL_POINTER("pcFilename", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);


	// Eine virtuelle Datei aus der echten Datei erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Laden
	if(pVFile->Init(pcFilename))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", NULL);
	}

	// Die Methode zum Laden der Textur aus der virtuellen Datei aufrufen
	pTexture = LoadTexture(pVFile, iWidth, iHeight, iNumMIPLevels, Format, dwUsage, Pool,
		                   dwFilter, dwMIPFilter, ColorKey, pcFilename);
	if(pTexture == NULL)
	{
		// Fehler beim Laden der Textur!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Die Textur konnte nicht geladen werden!", NULL);
	}

	// Die virtuelle Datei löschen
	TB_SAFE_DELETE(pVFile);

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer Würfeltextur (Datei)
PDIRECT3DCUBETEXTURE9 tbTextureManager::LoadCubeTexture(char* pcFilename,
														int iSize,			// = D3DX_DEFAULT
														int iNumMIPLevels,	// = D3DX_DEFAULT
														D3DFORMAT Format,	// = D3DFMT_UNKNOWN
														DWORD dwUsage,		// = 0
														D3DPOOL Pool,		// = D3DPOOL_MANAGED
														DWORD dwFilter,		// = D3DX_DEFAULT
														DWORD dwMIPFilter,	// = D3DX_DEFAULT
														D3DCOLOR ColorKey)	// = 0x00000000
{
	tbVFile*				pVFile;
	PDIRECT3DCUBETEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(pcFilename == NULL)				TB_ERROR_NULL_POINTER("pcFilename", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);


	// Eine virtuelle Datei aus der echten Datei erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Laden
	if(pVFile->Init(pcFilename))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", NULL);
	}

	// Die Methode zum Laden der Textur aus der virtuellen Datei aufrufen
	pTexture = LoadCubeTexture(pVFile, iSize, iNumMIPLevels, Format, dwUsage, Pool,
							   dwFilter, dwMIPFilter, ColorKey, pcFilename);
	if(pTexture == NULL)
	{
		// Fehler beim Laden der Textur!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Die Textur konnte nicht geladen werden!", NULL);
	}

	// Die virtuelle Datei löschen
	TB_SAFE_DELETE(pVFile);

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer Volumentextur (Datei)
PDIRECT3DVOLUMETEXTURE9 tbTextureManager::LoadVolumeTexture(char* pcFilename,
															int iWidth,			// = D3DX_DEFAULT
															int iHeight,		// = D3DX_DEFAULT
															int iDepth,			// = D3DX_DEFAULT
															int iNumMIPLevels,	// = D3DX_DEFAULT
															D3DFORMAT Format,	// = D3DFMT_UNKNOWN
															DWORD dwUsage,		// = 0
															D3DPOOL Pool,		// = D3DPOOL_MANAGED
															DWORD dwFilter,		// = D3DX_DEFAULT
															DWORD dwMIPFilter,	// = D3DX_DEFAULT
															D3DCOLOR ColorKey)	// = 0x00000000
{
	tbVFile*				pVFile;
	PDIRECT3DVOLUMETEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(pcFilename == NULL)				TB_ERROR_NULL_POINTER("pcFilename", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);


	// Eine virtuelle Datei aus der echten Datei erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Laden
	if(pVFile->Init(pcFilename))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", NULL);
	}

	// Die Methode zum Laden der Textur aus der virtuellen Datei aufrufen
	pTexture = LoadVolumeTexture(pVFile, iWidth, iHeight, iDepth, iNumMIPLevels, Format, dwUsage, Pool,
								 dwFilter, dwMIPFilter, ColorKey, pcFilename);
	if(pTexture == NULL)
	{
		// Fehler beim Laden der Textur!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Die Textur konnte nicht geladen werden!", NULL);
	}

	// Die virtuelle Datei löschen
	TB_SAFE_DELETE(pVFile);

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer gewöhnlichen Textur (Speicher)
PDIRECT3DTEXTURE9 tbTextureManager::LoadTexture(void* pMemory,
												int iMemorySize,
												int iWidth,			// = D3DX_DEFAULT
												int iHeight,		// = D3DX_DEFAULT
												int iNumMIPLevels,	// = D3DX_DEFAULT
												D3DFORMAT Format,	// = D3DFMT_UNKNOWN
												DWORD dwUsage,		// = 0
												D3DPOOL Pool,		// = D3DPOOL_MANAGED
												DWORD dwFilter,		// = D3DX_DEFAULT
												DWORD dwMIPFilter,	// = D3DX_DEFAULT
												D3DCOLOR ColorKey)	// = 0x00000000
{
	tbVFile*			pVFile;
	PDIRECT3DTEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(pMemory == NULL)					TB_ERROR_NULL_POINTER("pMemory", NULL);
	if(iMemorySize <= 0)				TB_ERROR_INVALID_VALUE("iMemorySize", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);


	// Eine virtuelle Datei aus dem Speicher erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Laden
	if(pVFile->Init(pMemory, iMemorySize))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", NULL);
	}

	// Die Methode zum Laden der Textur aus der virtuellen Datei aufrufen
	pTexture = LoadTexture(pVFile, iWidth, iHeight, iNumMIPLevels, Format, dwUsage, Pool,
		                   dwFilter, dwMIPFilter, ColorKey, "");
	if(pTexture == NULL)
	{
		// Fehler beim Laden der Textur!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Die Textur konnte nicht geladen werden!", NULL);
	}

	// Die virtuelle Datei löschen
	TB_SAFE_DELETE(pVFile);

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer Würfeltextur (Speicher)
PDIRECT3DCUBETEXTURE9 tbTextureManager::LoadCubeTexture(void* pMemory,
														int iMemorySize,
														int iSize,			// = D3DX_DEFAULT
														int iNumMIPLevels,	// = D3DX_DEFAULT
														D3DFORMAT Format,	// = D3DFMT_UNKNOWN
														DWORD dwUsage,		// = 0
														D3DPOOL Pool,		// = D3DPOOL_MANAGED
														DWORD dwFilter,		// = D3DX_DEFAULT
														DWORD dwMIPFilter,	// = D3DX_DEFAULT
														D3DCOLOR ColorKey)	// = 0x00000000
{
	tbVFile*				pVFile;
	PDIRECT3DCUBETEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(pMemory == NULL)					TB_ERROR_NULL_POINTER("pMemory", NULL);
	if(iMemorySize <= 0)				TB_ERROR_INVALID_VALUE("iMemorySize", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);


	// Eine virtuelle Datei aus dem Speicher erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Laden
	if(pVFile->Init(pMemory, iMemorySize))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", NULL);
	}

	// Die Methode zum Laden der Textur aus der virtuellen Datei aufrufen
	pTexture = LoadCubeTexture(pVFile, iSize, iNumMIPLevels, Format, dwUsage, Pool,
							   dwFilter, dwMIPFilter, ColorKey, "");
	if(pTexture == NULL)
	{
		// Fehler beim Laden der Textur!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Die Textur konnte nicht geladen werden!", NULL);
	}

	// Die virtuelle Datei löschen
	TB_SAFE_DELETE(pVFile);

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer Volumentextur (Speicher)
PDIRECT3DVOLUMETEXTURE9 tbTextureManager::LoadVolumeTexture(void* pMemory,
															int iMemorySize,
															int iWidth,			// = D3DX_DEFAULT
															int iHeight,		// = D3DX_DEFAULT
															int iDepth,			// = D3DX_DEFAULT
															int iNumMIPLevels,	// = D3DX_DEFAULT
															D3DFORMAT Format,	// = D3DFMT_UNKNOWN
															DWORD dwUsage,		// = 0
															D3DPOOL Pool,		// = D3DPOOL_MANAGED
															DWORD dwFilter,		// = D3DX_DEFAULT
															DWORD dwMIPFilter,	// = D3DX_DEFAULT
															D3DCOLOR ColorKey)	// = 0x00000000
{
	tbVFile*				pVFile;
	PDIRECT3DVOLUMETEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(pMemory == NULL)					TB_ERROR_NULL_POINTER("pMemory", NULL);
	if(iMemorySize <= 0)				TB_ERROR_INVALID_VALUE("iMemorySize", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);


	// Eine virtuelle Datei aus dem Speicher erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Laden
	if(pVFile->Init(pMemory, iMemorySize))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", NULL);
	}

	// Die Methode zum Laden der Textur aus der virtuellen Datei aufrufen
	pTexture = LoadVolumeTexture(pVFile, iWidth, iHeight, iDepth, iNumMIPLevels, Format, dwUsage, Pool,
								 dwFilter, dwMIPFilter, ColorKey, "");
	if(pTexture == NULL)
	{
		// Fehler beim Laden der Textur!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Die Textur konnte nicht geladen werden!", NULL);
	}

	// Die virtuelle Datei löschen
	TB_SAFE_DELETE(pVFile);

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer gewöhnlichen Textur (Ressource)
PDIRECT3DTEXTURE9 tbTextureManager::LoadTexture(HMODULE hModule,
												char* pcResourceName,
												char* pcResourceType,
												int iWidth,				// = D3DX_DEFAULT
												int iHeight,			// = D3DX_DEFAULT
												int iNumMIPLevels,		// = D3DX_DEFAULT
												D3DFORMAT Format,		// = D3DFMT_UNKNOWN
												DWORD dwUsage,			// = 0
												D3DPOOL Pool,			// = D3DPOOL_MANAGED
												DWORD dwFilter,			// = D3DX_DEFAULT
												DWORD dwMIPFilter,		// = D3DX_DEFAULT
												D3DCOLOR ColorKey)		// = 0x00000000
{
	tbVFile*			pVFile;
	PDIRECT3DTEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(hModule == NULL)					TB_ERROR_NULL_POINTER("hModule", NULL);
	if(pcResourceName == NULL)			TB_ERROR_NULL_POINTER("pcResourceName", NULL);
	if(pcResourceType == NULL)			TB_ERROR_NULL_POINTER("pcResourceType", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);


	// Eine virtuelle Datei aus der Ressource erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Laden
	if(pVFile->Init(hModule, pcResourceName, pcResourceType))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", NULL);
	}

	// Die Methode zum Laden der Textur aus der virtuellen Datei aufrufen
	pTexture = LoadTexture(pVFile, iWidth, iHeight, iNumMIPLevels, Format, dwUsage, Pool,
		                   dwFilter, dwMIPFilter, ColorKey, "");
	if(pTexture == NULL)
	{
		// Fehler beim Laden der Textur!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Die Textur konnte nicht geladen werden!", NULL);
	}

	// Die virtuelle Datei löschen
	TB_SAFE_DELETE(pVFile);

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer Würfeltextur (Ressource)
PDIRECT3DCUBETEXTURE9 tbTextureManager::LoadCubeTexture(HMODULE hModule,
														char* pcResourceName,
														char* pcResourceType,
														int iSize,				// = D3DX_DEFAULT
														int iNumMIPLevels,		// = D3DX_DEFAULT
														D3DFORMAT Format,		// = D3DFMT_UNKNOWN
														DWORD dwUsage,			// = 0
														D3DPOOL Pool,			// = D3DPOOL_MANAGED
														DWORD dwFilter,			// = D3DX_DEFAULT
														DWORD dwMIPFilter,		// = D3DX_DEFAULT
														D3DCOLOR ColorKey)		// = 0x00000000
{
	tbVFile*				pVFile;
	PDIRECT3DCUBETEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(hModule == NULL)					TB_ERROR_NULL_POINTER("hModule", NULL);
	if(pcResourceName == NULL)			TB_ERROR_NULL_POINTER("pcResourceName", NULL);
	if(pcResourceType == NULL)			TB_ERROR_NULL_POINTER("pcResourceType", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);


	// Eine virtuelle Datei aus der Ressource erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Laden
	if(pVFile->Init(hModule, pcResourceName, pcResourceType))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", NULL);
	}

	// Die Methode zum Laden der Textur aus der virtuellen Datei aufrufen
	pTexture = LoadCubeTexture(pVFile, iSize, iNumMIPLevels, Format, dwUsage, Pool,
							   dwFilter, dwMIPFilter, ColorKey, "");
	if(pTexture == NULL)
	{
		// Fehler beim Laden der Textur!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Die Textur konnte nicht geladen werden!", NULL);
	}

	// Die virtuelle Datei löschen
	TB_SAFE_DELETE(pVFile);

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Laden einer Volumentextur (Ressource)
PDIRECT3DVOLUMETEXTURE9 tbTextureManager::LoadVolumeTexture(HMODULE hModule,
															char* pcResourceName,
															char* pcResourceType,
															int iWidth,				// = D3DX_DEFAULT
															int iHeight,			// = D3DX_DEFAULT
															int iDepth,				// = D3DX_DEFAULT
															int iNumMIPLevels,		// = D3DX_DEFAULT
															D3DFORMAT Format,		// = D3DFMT_UNKNOWN
															DWORD dwUsage,			// = 0
															D3DPOOL Pool,			// = D3DPOOL_MANAGED
															DWORD dwFilter,			// = D3DX_DEFAULT
															DWORD dwMIPFilter,		// = D3DX_DEFAULT
															D3DCOLOR ColorKey)		// = 0x00000000
{
	tbVFile*				pVFile;
	PDIRECT3DVOLUMETEXTURE9	pTexture;


	// Parameter prüfen und sicherstellen, dass eine tbDirect3D-Klasse existiert
	if(hModule == NULL)					TB_ERROR_NULL_POINTER("hModule", NULL);
	if(pcResourceName == NULL)			TB_ERROR_NULL_POINTER("pcResourceName", NULL);
	if(pcResourceType == NULL)			TB_ERROR_NULL_POINTER("pcResourceType", NULL);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", NULL);


	// Eine virtuelle Datei aus der Ressource erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);

	// Laden
	if(pVFile->Init(hModule, pcResourceName, pcResourceType))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", NULL);
	}

	// Die Methode zum Laden der Textur aus der virtuellen Datei aufrufen
	pTexture = LoadVolumeTexture(pVFile, iWidth, iHeight, iDepth, iNumMIPLevels, Format, dwUsage, Pool,
								 dwFilter, dwMIPFilter, ColorKey, "");
	if(pTexture == NULL)
	{
		// Fehler beim Laden der Textur!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Die Textur konnte nicht geladen werden!", NULL);
	}

	// Die virtuelle Datei löschen
	TB_SAFE_DELETE(pVFile);

	// Textur liefern
	return pTexture;
}

// ******************************************************************
// Finden oder Laden einer gewöhnlichen Textur (Datei)
PDIRECT3DTEXTURE9 tbTextureManager::GetTexture(char* pcFilename,
											   BOOL bDontCareForOptions,	// = TRUE
											   int iWidth,					// = D3DX_DEFAULT
											   int iHeight,					// = D3DX_DEFAULT
											   int iNumMIPLevels,			// = D3DX_DEFAULT
											   D3DFORMAT Format,			// = D3DFMT_UNKNOWN
											   DWORD dwUsage,				// = 0
											   D3DPOOL Pool,				// = D3DPOOL_MANAGED
											   DWORD dwFilter,				// = D3DX_DEFAULT
											   DWORD dwMIPFilter,			// = D3DX_DEFAULT
											   D3DCOLOR ColorKey)			// = 0x00000000
{
	int					iIndex;
	PDIRECT3DTEXTURE9	pTexture;


	// Parameter prüfen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", NULL);


	if(ColorKey != 0 && Format == D3DFMT_UNKNOWN) Format = D3DFMT_A8R8G8B8;

	// Gibt es die Textur vielleicht schon?
	iIndex = GetTextureIndex(pcFilename, bDontCareForOptions, iWidth, iHeight, 0, iNumMIPLevels,
		                     Format, dwUsage, Pool, dwFilter, dwMIPFilter, ColorKey);
	if(iIndex != -1)
	{
		// Die Textur gibt es schon!
		// Wir erhöhen ihren Referenzzähler und liefern sie zurück.
		m_pTextureList[iIndex].pTexture->AddRef();
		return (PDIRECT3DTEXTURE9)(m_pTextureList[iIndex].pTexture);
	}
	else
	{
		// Die Textur gibt es noch nicht!
		// Die Methode zum Laden aus einer Datei aufrufen
		pTexture = LoadTexture(pcFilename, iWidth, iHeight, iNumMIPLevels, Format, dwUsage, Pool,
			                   dwFilter, dwMIPFilter, ColorKey);
		if(pTexture == NULL)
		{
			// Fehler beim Laden!
			TB_ERROR("Fehler beim Laden der Textur!", NULL);
		}

		// Die neue Textur liefern
		return pTexture;
	}
}

// ******************************************************************
// Finden oder Laden einer Würfeltextur (Datei)
PDIRECT3DCUBETEXTURE9 tbTextureManager::GetCubeTexture(char* pcFilename,
													   BOOL bDontCareForOptions,	// = TRUE
													   int iSize,					// = D3DX_DEFAULT
													   int iNumMIPLevels,			// = D3DX_DEFAULT
													   D3DFORMAT Format,			// = D3DFMT_UNKNOWN
													   DWORD dwUsage,				// = 0
													   D3DPOOL Pool,				// = D3DPOOL_MANAGED
													   DWORD dwFilter,				// = D3DX_DEFAULT
													   DWORD dwMIPFilter,			// = D3DX_DEFAULT
													   D3DCOLOR ColorKey)			// = 0x00000000
{
	int						iIndex;
	PDIRECT3DCUBETEXTURE9	pTexture;


	// Parameter prüfen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", NULL);


	if(ColorKey != 0 && Format == D3DFMT_UNKNOWN) Format = D3DFMT_A8R8G8B8;

	// Gibt es die Textur vielleicht schon?
	iIndex = GetTextureIndex(pcFilename, bDontCareForOptions, iSize, iSize, 0, iNumMIPLevels,
		                     Format, dwUsage, Pool, dwFilter, dwMIPFilter, ColorKey);
	if(iIndex != -1)
	{
		// Die Textur gibt es schon!
		// Wir erhöhen ihren Referenzzähler und liefern sie zurück.
		m_pTextureList[iIndex].pTexture->AddRef();
		return (PDIRECT3DCUBETEXTURE9)(m_pTextureList[iIndex].pTexture);
	}
	else
	{
		// Die Textur gibt es noch nicht!
		// Die Methode zum Laden aus einer Datei aufrufen
		pTexture = LoadCubeTexture(pcFilename, iSize, iNumMIPLevels, Format, dwUsage, Pool,
								   dwFilter, dwMIPFilter, ColorKey);
		if(pTexture == NULL)
		{
			// Fehler beim Laden!
			TB_ERROR("Fehler beim Laden der Textur!", NULL);
		}

		// Die neue Textur liefern
		return pTexture;
	}
}

// ******************************************************************
// Finden oder Laden einer Volumentextur (Datei)
PDIRECT3DVOLUMETEXTURE9 tbTextureManager::GetVolumeTexture(char* pcFilename,
														   BOOL bDontCareForOptions,	// = TRUE
														   int iWidth,					// = D3DX_DEFAULT
														   int iHeight,					// = D3DX_DEFAULT
														   int iDepth,					// = D3DX_DEFAULT
														   int iNumMIPLevels,			// = D3DX_DEFAULT
														   D3DFORMAT Format,			// = D3DFMT_UNKNOWN
														   DWORD dwUsage,				// = 0
														   D3DPOOL Pool,				// = D3DPOOL_MANAGED
														   DWORD dwFilter,				// = D3DX_DEFAULT
														   DWORD dwMIPFilter,			// = D3DX_DEFAULT
														   D3DCOLOR ColorKey)			// = 0x00000000
{
	int						iIndex;
	PDIRECT3DVOLUMETEXTURE9	pTexture;


	// Parameter prüfen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", NULL);


	if(ColorKey != 0 && Format == D3DFMT_UNKNOWN) Format = D3DFMT_A8R8G8B8;

	// Gibt es die Textur vielleicht schon?
	iIndex = GetTextureIndex(pcFilename, bDontCareForOptions, iWidth, iHeight, iDepth, iNumMIPLevels,
		                     Format, dwUsage, Pool, dwFilter, dwMIPFilter, ColorKey);
	if(iIndex != -1)
	{
		// Die Textur gibt es schon!
		// Wir erhöhen ihren Referenzzähler und liefern sie zurück.
		m_pTextureList[iIndex].pTexture->AddRef();
		return (PDIRECT3DVOLUMETEXTURE9)(m_pTextureList[iIndex].pTexture);
	}
	else
	{
		// Die Textur gibt es noch nicht!
		// Die Methode zum Laden aus einer Datei aufrufen
		pTexture = LoadVolumeTexture(pcFilename, iWidth, iHeight, iDepth, iNumMIPLevels, Format, dwUsage, Pool,
									 dwFilter, dwMIPFilter, ColorKey);
		if(pTexture == NULL)
		{
			// Fehler beim Laden!
			TB_ERROR("Fehler beim Laden der Textur!", NULL);
		}

		// Die neue Textur liefern
		return pTexture;
	}
}

// ******************************************************************