/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbSpriteEngine.h
	================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Zeichnen von Sprites

	Autor:
	David Scherfgen

********************************************************************/

// Vertexformat
#define TB_SPRITE_FVF (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct TRIBASE_API tbSpriteVertex
{
	tbVector3	vPosition;
	D3DCOLOR	Color;
	tbVector2	vTexture;
};

// ******************************************************************
// Sprite-Typ
struct TRIBASE_API tbSpriteType
{
	BOOL		bExists;
	tbVector2	vTopLeft;		// Linke obere Texturkoordinaten
	tbVector2	vTopRight;		// Rechte obere Texturkoordinaten
	tbVector2	vBottomRight;	// Rechte untere Texturkoordinaten
	tbVector2	vBottomLeft;	// Linke untere Texturkoordinaten
};

// ******************************************************************
// Klasse für die Sprite-Engine
class TRIBASE_API tbSpriteEngine
{
private:
	// Variablen
	tbVertexBuffer*	m_pVB;					// Vertex-Buffer für die Sprites
	DWORD			m_dwNumSprites;			// Anzahl der Sprites
	DWORD			m_dwMaxSprites;			// Maximale Anzahl von Sprites
	tbVector3		m_vCamPos;				// Kameraposition
	tbVector3		m_vCamXAxis;			// x-Achse der Kamera
	tbVector3		m_vCamYAxis;			// y-Achse der Kamera
	tbVector3		m_vCamZAxis;			// z-Achse der Kamera
	tbSpriteType	m_aSpriteType[1024];	// Informationen über die Sprite-Typen
	BOOL			m_bRunning;				// Läuft die Sprite-Engine?

public:
	// Konstruktor und Destruktor
	tbSpriteEngine();
	~tbSpriteEngine();

	// Methoden
	tbResult	Init(DWORD dwMaxSprites);					// Initialisiert die Sprite-Engine
	int			FindFreeSpriteType();						// Sucht einen freien Sprite-Typ
	tbResult	Begin(tbMatrix mView, tbVector3 vCamPos);	// Fängt von vorne an, leert die Sprites
	tbResult	End();										// Beenden
	tbResult	Render(int iFrom = -1, int iTo = -1);		// Rendert die Sprites

	// Erstellt einen Sprite-Typ	
	tbResult CreateSpriteType(int iType,
		                      tbVector2 vTopLeft = tbVector2(0.0f, 0.0f),
							  tbVector2 vSize = tbVector2(1.0f, 1.0f));
	
	// Fügt ein Sprite zur Liste hinzu
	int AddSprite(const int iType,
				  const tbVector3& vPosition,
		          const tbColor& Color = tbColor(1.0f),
				  const tbVector2& vSize = tbVector2(1.0f),
				  const float fRotation = 0.0f,
				  const BOOL bSimple = TRUE);
	
	// Fügt einen Laserstrahl hinzu
	int AddLaserSprite(const int iType,
        			   const tbVector3& vLaserStart,
 					   const tbVector3& vLaserEnd,
		               const float fRadius = 0.25f,
		               const tbColor& StartColor = tbColor(1.0f),
		               const tbColor& EndColor = tbColor(1.0f));

	// Inline-Methoden
	inline void				DeleteSpriteType(int iType)	{m_aSpriteType[iType].bExists = FALSE;}
	inline tbVertexBuffer*	GetVertexBuffer()			{return m_pVB;}
	inline DWORD			GetNumSprites()				{return m_dwNumSprites;}
	inline DWORD			GetMaxSprites()				{return m_dwMaxSprites;}
	inline tbSpriteType*	GetSpriteTypes()			{return m_aSpriteType;}
};

// ******************************************************************
