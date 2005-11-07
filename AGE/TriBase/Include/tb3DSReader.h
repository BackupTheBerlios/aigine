/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tb3DSReader.h
	=============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Lesen von 3DS-Dateien

	Autor:
	David Scherfgen

********************************************************************/

#pragma pack(1)

// Struktur für einen Chunk-Headers aus einer 3DS-Datei
struct TRIBASE_API tb3DSChunkHeader
{
	WORD	wChunkID;
	DWORD	dwChunkSize;
};

// ******************************************************************
// Struktur für ein 3DS-Material
struct TRIBASE_API tb3DSMaterial
{
	BOOL	bHasFaces;			// Hat dieses Material Dreiecke?
	char	acName[256];		// Name des Materials
	char	acTextureMap[256];	// Name der Texturdatei
	char	acOpacityMap[256];	// Name der Opazitätstextur
	tbColor	DiffuseColor;		// Streufarbe
	tbColor	AmbientColor;		// Indirekte Farbe
	tbColor	SpecularColor;		// Glanzfarbe
	tbColor	SelfIllumination;	// Eigenfarbe
	float	fShininess;			// Glanzstärke
	float	fSpecularPower;		// Glanzmultiplikator
	float	fSelfIllumination;	// Selbstbeleuchtung
	float	fOpacity;			// Opazität (0 bis 1)
	BOOL	bTwoSided;			// Zweiseitiges Material?
	BOOL	bWire;				// Drahtmaterial?
	BOOL	bBlended;			// Muss Alpha-Blending verwendet werden?
	BOOL	bSelfIllumColor;	// Eigenfarbe oder Eigenhelligkeit?
	BOOL	bNoShading;			// Shading
};

// ******************************************************************
// Struktur für ein 3DS-Dreieck
struct TRIBASE_API tb3DSFace
{
	union
	{
		struct
		{
			WORD a;	// Vertex A
			WORD b;	// Vertex B
			WORD c;	// Vertex C
		};

        WORD i[3];	// Indizes		
	};

	DWORD	dwSmoothingGroup;	// Glättungsgruppe
	int		iMaterial;			// Material dieses Dreiecks
};

// ******************************************************************
// Struktur für einen 3DS-Vertex ohne Normalvektor
struct TRIBASE_API tb3DSVertex
{
	tbVector3 p;	// Position
	tbVector2 t;	// Texturkoordinaten
};

// ******************************************************************
// Struktur für einen 3DS-Vertex mit Normalvektor
struct TRIBASE_API tb3DSVertexEx
{
	tbVector3 p;	// Position
	tbVector3 n;	// Normalvektor
	tbVector2 t;	// Texturkoordinaten
};

// ******************************************************************
// Struktur für ein 3DS-Modellobjekt
struct TRIBASE_API tb3DSModel
{
	char			acName[256];		// Name
	WORD			wNumVertices;		// Anzahl der Vertizes
	WORD			wNumFaces;			// Anzahl der Dreiecke
	tb3DSVertex*	pVertices;			// Vertizes
	tb3DSFace*		pFaces;				// Dreiecke
	tbMatrix		mTransformation;	// Transformationsmatrix
};

// ******************************************************************
// 3DS-Lichttypen
enum TRIBASE_API tb3DSLightType
{
	TB_3DSLT_POINT,	// Punktlicht
	TB_3DSLT_SPOT,	// Spotlicht
	TB_3DSLT_MAX
};

// ******************************************************************
// Struktur für ein 3DS-Licht
struct TRIBASE_API tb3DSLight
{
	char			acName[256];	// Name
	tb3DSLightType	Type;			// Lichttyp
	tbVector3		vPosition;		// Position
	tbVector3		vTarget;		// Ziel
	tbColor			Color;			// Farbe des Lichts
	float			fMultiplier;	// Multiplikator für die Farbe
	float			fInnerCone;		// Innerer Lichtkegel
	float			fOuterCone;		// Äußerer Lichtkegel
	float			fRange;			// Reichweite
	BOOL			bAttenuate;		// Licht auf Entfernung abschwächen?
	BOOL			bShadowed;		// Schatten für dieses Licht berechnen?
};

// ******************************************************************
// Klasse für 3DS-Dateien
class TRIBASE_API tb3DSFile
{
private:
	// Variablen
	int				m_iNumMaterials;			// Anzahl der Materialien
	int				m_iNumModels;				// Anzahl der Modelle
	int				m_iNumLights;				// Anzahl der Lichter
	tb3DSMaterial*	m_pMaterials;				// Materialien
	tb3DSModel*		m_pModels;					// Modelle
	tb3DSLight*		m_pLights;					// Lichter
	float			m_fBoundingSphereRadius;	// Radius der umgebenden Kugel
	tbVector3		m_vBoundingBoxMin;			// Minimumpunkt des umgebenden Quaders
	tbVector3		m_vBoundingBoxMax;			// Maximumpunkt des umgebenden Quaders

public:
	// Konstruktor und Destruktor
	tb3DSFile();
	~tb3DSFile();

	// Methoden
	tbResult	Init(tbVFile* pVFile);												// Aus virtueller Datei erstellen
	tbResult	Init(char* pcFilename);												// Aus echter Datei erstellen
	tbResult	Init(void* pMemory, int iSize);										// Aus Speicher erstellen
	tbResult	Init(HMODULE hModule, char* pcResourceName, char* pcResourceType);	// Aus Ressource erstellen
	int			GetTotalNumVertices();												// Gesamtanzahl der Vertizes abfragen
	int			GetTotalNumFaces();													// Gesamtanzahl der Dreiecke abfragen

	// "Rohe" Daten ausgeben
	tbResult GenerateRawData(int iMaterial,
		                     int iBaseIndex,
							 tb3DSVertexEx** ppVerticesOut,
							 WORD** ppwIndicesOut,
							 DWORD* pdwNumVerticesOut,
							 DWORD* pdwNumIndicesOut);

	// Inline-Methoden
	inline int				GetNumMaterials()			{return m_iNumMaterials;}
	inline int				GetNumModels()				{return m_iNumModels;}
	inline int				GetNumLights()				{return m_iNumLights;}
	inline tb3DSMaterial*	GetMaterials()				{return m_pMaterials;}
	inline tb3DSModel*		GetModels()					{return m_pModels;}
	inline tb3DSLight*		GetLights()					{return m_pLights;}
	inline float			GetBoundingSphereRadius()	{return m_fBoundingSphereRadius;}
	inline tbVector3		GetBoundingBoxMin()			{return m_vBoundingBoxMin;}
	inline tbVector3		GetBoundingBoxMax()			{return m_vBoundingBoxMax;}

	// friend-Funktionen
	friend tbResult Read3DSModelChunk(tbVFile* pVFile, tb3DSFile* p3DSFile, tb3DSModel* pOut);
};

// ******************************************************************
// Funktionen deklarieren
TRIBASE_API tbResult tbConvert3DSMaterialToD3DMaterial(tb3DSMaterial* p3DSMaterial, D3DMATERIAL9* pOut);		// 3DS-Material in ein Direct3D-Material umwandeln
TRIBASE_API tbResult tbConvert3DSLightToD3DLight(tb3DSLight* p3DSLight, D3DLIGHT9* pOut);					// 3DS-Licht in ein Direct3D-Licht umwandeln

// ******************************************************************