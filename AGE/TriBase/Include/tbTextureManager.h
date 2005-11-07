/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbTextureManager.h
	==================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Implementierung des Texturmanagers

	Autor:
	David Scherfgen

********************************************************************/

// Struktur für einen Texturlisteneintrag
struct TRIBASE_API tbTextureListEntry
{
	BOOL					bExists;				// Existiert diese Textur?
	PDIRECT3DBASETEXTURE9	pTexture;				// Die Texturschnittstelle
	char					acSourceFilename[256];	// Quelldateiname
	int						iWidth;					// Breite der Textur
	int						iDepth;					// Tiefe der Textur
	int						iHeight;				// Höhe der Textur
	int						iNumMIPLevels;			// Anzahl der MIP-Mapping-Ebenen
	D3DFORMAT				Format;					// Oberflächenformat der Textur
	DWORD					dwUsage;				// Verwendungszweck
	D3DPOOL					Pool;					// Speicherklasse
	DWORD					dwFilter;				// Bildfilter (beim Laden)
	DWORD					dwMIPFilter;			// MIP-Map-Filter (beim Laden)
	D3DCOLOR				ColorKey;				// Color-Key (beim Laden)
};

// ******************************************************************
// Klasse des Texturmanagers
class TRIBASE_API tbTextureManager
{
private:
	// Variablen
	static BOOL					m_bInitialized;	// Initialisiert?
	static int					m_iNumTextures;	// Anzahl der Texturen
	static int					m_iListSize;	// Größe der Texturliste
	static tbTextureListEntry*	m_pTextureList;	// Die Texturliste

public:
	// Methoden
	static tbResult	Init(int iInitialListSize = 256);					// Initialisierung
	static tbResult Exit();												// Herunterfahren
	static tbResult	SetListSize(int iNewSize);							// Verändert die Listengröße
	static int		GetTextureIndex(PDIRECT3DBASETEXTURE9 pTexture);	// Liefert den Index einer Textur in der Liste
	static int		GetNewIndex();										// Liefert den nächsten freien Index
	static tbResult	ReleaseTexture(PDIRECT3DBASETEXTURE9 pTexture);		// Gibt eine Textur frei
	static tbResult	DeleteAllTextures();								// Löscht alle Texturen

	// Sucht nach einer Textur (erweitert)
	static int GetTextureIndex(char* pcSourceFilename,
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
							   D3DCOLOR ColorKey);

	// Fügt eine Textur der Liste hinzu
	static int AddTextureToList(PDIRECT3DBASETEXTURE9 pTexture,
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
								D3DCOLOR ColorKey);

	// Laden einer gewöhnlichen Textur (virtuelle Datei)
	static PDIRECT3DTEXTURE9 LoadTexture(tbVFile* pVFile,
										 int iWidth = D3DX_DEFAULT,
										 int iHeight = D3DX_DEFAULT,
										 int iNumMIPLevels = D3DX_DEFAULT,
										 D3DFORMAT Format = D3DFMT_UNKNOWN,
										 DWORD dwUsage = 0,
										 D3DPOOL Pool = D3DPOOL_MANAGED,
										 DWORD dwFilter = D3DX_DEFAULT,
										 DWORD dwMIPFilter = D3DX_DEFAULT,
										 D3DCOLOR ColorKey = 0x00000000,
 										 char* pcSourceFilename = NULL);

	// Laden einer Würfeltextur (virtuelle Datei)
	static PDIRECT3DCUBETEXTURE9 LoadCubeTexture(tbVFile* pVFile,
												 int iSize = D3DX_DEFAULT,
												 int iNumMIPLevels = D3DX_DEFAULT,
												 D3DFORMAT Format = D3DFMT_UNKNOWN,
												 DWORD dwUsage = 0,
												 D3DPOOL Pool = D3DPOOL_MANAGED,
												 DWORD dwFilter = D3DX_DEFAULT,
												 DWORD dwMIPFilter = D3DX_DEFAULT,
												 D3DCOLOR ColorKey = 0x00000000,
												 char* pcSourceFilename = NULL);

	// Laden einer Volumentextur (virtuelle Datei)
	static PDIRECT3DVOLUMETEXTURE9 LoadVolumeTexture(tbVFile* pVFile,
													 int iWidth = D3DX_DEFAULT,
													 int iHeight = D3DX_DEFAULT,
													 int iDepth = D3DX_DEFAULT,
													 int iNumMIPLevels = D3DX_DEFAULT,
													 D3DFORMAT Format = D3DFMT_UNKNOWN,
													 DWORD dwUsage = 0,
													 D3DPOOL Pool = D3DPOOL_MANAGED,
													 DWORD dwFilter = D3DX_DEFAULT,
													 DWORD dwMIPFilter = D3DX_DEFAULT,
													 D3DCOLOR ColorKey = 0x00000000,
													 char* pcSourceFilename = NULL);

	// Laden einer gewöhnlichen Textur (Datei)
	static PDIRECT3DTEXTURE9 LoadTexture(char* pcFilename,
										 int iWidth = D3DX_DEFAULT,
										 int iHeight = D3DX_DEFAULT,
										 int iNumMIPLevels = D3DX_DEFAULT,
										 D3DFORMAT Format = D3DFMT_UNKNOWN,
										 DWORD dwUsage = 0,
										 D3DPOOL Pool = D3DPOOL_MANAGED,
										 DWORD dwFilter = D3DX_DEFAULT,
										 DWORD dwMIPFilter = D3DX_DEFAULT,
										 D3DCOLOR ColorKey = 0x00000000);

	// Laden einer Würfeltextur (Datei)
	static PDIRECT3DCUBETEXTURE9 LoadCubeTexture(char* pcFilename,
												 int iSize = D3DX_DEFAULT,
												 int iNumMIPLevels = D3DX_DEFAULT,
												 D3DFORMAT Format = D3DFMT_UNKNOWN,
												 DWORD dwUsage = 0,
												 D3DPOOL Pool = D3DPOOL_MANAGED,
												 DWORD dwFilter = D3DX_DEFAULT,
												 DWORD dwMIPFilter = D3DX_DEFAULT,
												 D3DCOLOR ColorKey = 0x00000000);

	// Laden einer Volumentextur (Datei)
	static PDIRECT3DVOLUMETEXTURE9 LoadVolumeTexture(char* pcFilename,
													 int iWidth = D3DX_DEFAULT,
													 int iHeight = D3DX_DEFAULT,
													 int iDepth = D3DX_DEFAULT,
													 int iNumMIPLevels = D3DX_DEFAULT,
													 D3DFORMAT Format = D3DFMT_UNKNOWN,
													 DWORD dwUsage = 0,
													 D3DPOOL Pool = D3DPOOL_MANAGED,
													 DWORD dwFilter = D3DX_DEFAULT,
													 DWORD dwMIPFilter = D3DX_DEFAULT,
													 D3DCOLOR ColorKey = 0x00000000);

	// Laden einer gewöhnlichen Textur (Speicher)
	static PDIRECT3DTEXTURE9 LoadTexture(void* pMemory,
										 int iMemorySize,
										 int iWidth = D3DX_DEFAULT,
										 int iHeight = D3DX_DEFAULT,
										 int iNumMIPLevels = D3DX_DEFAULT,
										 D3DFORMAT Format = D3DFMT_UNKNOWN,
										 DWORD dwUsage = 0,
										 D3DPOOL Pool = D3DPOOL_MANAGED,
										 DWORD dwFilter = D3DX_DEFAULT,
										 DWORD dwMIPFilter = D3DX_DEFAULT,
										 D3DCOLOR ColorKey = 0x00000000);

	// Laden einer Würfeltextur (Speicher)
	static PDIRECT3DCUBETEXTURE9 LoadCubeTexture(void* pMemory,
												 int iMemorySize,
												 int iSize = D3DX_DEFAULT,
												 int iNumMIPLevels = D3DX_DEFAULT,
												 D3DFORMAT Format = D3DFMT_UNKNOWN,
												 DWORD dwUsage = 0,
												 D3DPOOL Pool = D3DPOOL_MANAGED,
												 DWORD dwFilter = D3DX_DEFAULT,
												 DWORD dwMIPFilter = D3DX_DEFAULT,
												 D3DCOLOR ColorKey = 0x00000000);

	// Laden einer Volumentextur (Speicher)
	static PDIRECT3DVOLUMETEXTURE9 LoadVolumeTexture(void* pMemory,
													 int iMemorySize,
													 int iWidth = D3DX_DEFAULT,
													 int iHeight = D3DX_DEFAULT,
													 int iDepth = D3DX_DEFAULT,
													 int iNumMIPLevels = D3DX_DEFAULT,
													 D3DFORMAT Format = D3DFMT_UNKNOWN,
													 DWORD dwUsage = 0,
													 D3DPOOL Pool = D3DPOOL_MANAGED,
													 DWORD dwFilter = D3DX_DEFAULT,
													 DWORD dwMIPFilter = D3DX_DEFAULT,
													 D3DCOLOR ColorKey = 0x00000000);

	// Laden einer gewöhnlichen Textur (Ressource)
	static PDIRECT3DTEXTURE9 LoadTexture(HMODULE hModule,
										 char* pcResourceName,
										 char* pcResourceType,
										 int iWidth = D3DX_DEFAULT,
										 int iHeight = D3DX_DEFAULT,
										 int iNumMIPLevels = D3DX_DEFAULT,
										 D3DFORMAT Format = D3DFMT_UNKNOWN,
										 DWORD dwUsage = 0,
										 D3DPOOL Pool = D3DPOOL_MANAGED,
										 DWORD dwFilter = D3DX_DEFAULT,
										 DWORD dwMIPFilter = D3DX_DEFAULT,
										 D3DCOLOR ColorKey = 0x00000000);

	// Laden einer Würfeltextur (Ressource)
	static PDIRECT3DCUBETEXTURE9 LoadCubeTexture(HMODULE hModule,
												 char* pcResourceName,
												 char* pcResourceType,
												 int iSize = D3DX_DEFAULT,
												 int iNumMIPLevels = D3DX_DEFAULT,
												 D3DFORMAT Format = D3DFMT_UNKNOWN,
												 DWORD dwUsage = 0,
												 D3DPOOL Pool = D3DPOOL_MANAGED,
												 DWORD dwFilter = D3DX_DEFAULT,
												 DWORD dwMIPFilter = D3DX_DEFAULT,
												 D3DCOLOR ColorKey = 0x00000000);

	// Laden einer Volumentextur (Ressource)
	static PDIRECT3DVOLUMETEXTURE9 LoadVolumeTexture(HMODULE hModule,
													 char* pcResourceName,
													 char* pcResourceType,
													 int iWidth = D3DX_DEFAULT,
													 int iHeight = D3DX_DEFAULT,
													 int iDepth = D3DX_DEFAULT,
													 int iNumMIPLevels = D3DX_DEFAULT,
													 D3DFORMAT Format = D3DFMT_UNKNOWN,
													 DWORD dwUsage = 0,
													 D3DPOOL Pool = D3DPOOL_MANAGED,
													 DWORD dwFilter = D3DX_DEFAULT,
													 DWORD dwMIPFilter = D3DX_DEFAULT,
													 D3DCOLOR ColorKey = 0x00000000);
	
	// Finden oder Laden einer gewöhnlichen Textur (Datei)
	static PDIRECT3DTEXTURE9 GetTexture(char* pcFilename,
										BOOL bDontCareForOptions = TRUE,
										int iWidth = D3DX_DEFAULT,
										int iHeight = D3DX_DEFAULT,
										int iNumMIPLevels = D3DX_DEFAULT,
										D3DFORMAT Format = D3DFMT_UNKNOWN,
										DWORD dwUsage = 0,
										D3DPOOL Pool = D3DPOOL_MANAGED,
										DWORD dwFilter = D3DX_DEFAULT,
										DWORD dwMIPFilter = D3DX_DEFAULT,
										D3DCOLOR ColorKey = 0x00000000);

	// Finden oder Laden einer Würfeltextur (Datei)
	static PDIRECT3DCUBETEXTURE9 GetCubeTexture(char* pcFilename,
												BOOL bDontCareForOptions = TRUE,
												int iSize = D3DX_DEFAULT,
												int iNumMIPLevels = D3DX_DEFAULT,
												D3DFORMAT Format = D3DFMT_UNKNOWN,
												DWORD dwUsage = 0,
												D3DPOOL Pool = D3DPOOL_MANAGED,
												DWORD dwFilter = D3DX_DEFAULT,
												DWORD dwMIPFilter = D3DX_DEFAULT,
												D3DCOLOR ColorKey = 0x00000000);

	// Finden oder Laden einer Volumentextur (Datei)
	static PDIRECT3DVOLUMETEXTURE9 GetVolumeTexture(char* pcFilename,
													BOOL bDontCareForOptions = TRUE,
													int iWidth = D3DX_DEFAULT,
													int iHeight = D3DX_DEFAULT,
													int iDepth = D3DX_DEFAULT,
													int iNumMIPLevels = D3DX_DEFAULT,
													D3DFORMAT Format = D3DFMT_UNKNOWN,
													DWORD dwUsage = 0,
													D3DPOOL Pool = D3DPOOL_MANAGED,
													DWORD dwFilter = D3DX_DEFAULT,
													DWORD dwMIPFilter = D3DX_DEFAULT,
													D3DCOLOR ColorKey = 0x00000000);

	// Inline-Methoden
	static inline BOOL					IsInitialized()			{return m_bInitialized;}
	static inline tbTextureListEntry*	GetTextureList()		{return m_pTextureList;}
	static inline int					GetNumTextures()		{return m_iNumTextures;}
	static inline int					GetTextureListSize()	{return m_iListSize;}
};

// ******************************************************************