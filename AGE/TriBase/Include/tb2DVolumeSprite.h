/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tb2DVolumeSprite.h
	==================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit 2D-Volumen-Sprites

	Autor:
	Snorky
	[24.9.2003]

********************************************************************/

#ifndef __TB2DVOLUMESPRITE__
#define __TB2DVOLUMESPRITE__

#pragma pack(1)


// Struktur für die Informationen über ein Volumensprite
struct TRIBASE_API tb2DVolumeSpriteInfo
{
	float		fTextureWidth;			// Breite der Textur
	float		fTextureHeight;			// Höhe der Textur
	float		fTextureDepth;			// Tiefe der Textur
	tbVector2	avTopLeftPix[256];		// Linke obere Koordinate jedes Sprites in Pixeln
	tbVector2	avBottomRightPix[256];	// Rechte untere Koordinate jedes Sprites in Pixeln
	tbVector2	avSpriteWidthPix[256];	// Breite und Höhe jedes Sprites in Pixeln
	tbVector2	avTopLeft[256];			// Linke obere Koordinate jedes Sprites
	tbVector2	avBottomRight[256];		// Rechte untere Koordinate jedes Sprites
	tbVector2	avSpriteWidth[256];		// Breite und Höhe jedes Sprites
};

// ******************************************************************
// Vertexformat
#define TB_2DVOLUMESPRITE_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0))
struct TRIBASE_API tb2DVolumeSpriteVertex
{
	tbVector3	vPosition;	// Positionsangabe
	float		fRHW;		// 1/w
	D3DCOLOR	Diffuse;	// Streufarbe
	tbVector3	vTexture;	// Texturkoordinaten
};

// ******************************************************************
// Flags zum Zeichnen von Volumensprites
#define TB_SVF_ALIGN_HLEFT		(0)		// Sprite linksbündig
#define TB_SVF_ALIGN_HCENTER	(1)		// Sprite horizontal zentrieren
#define TB_SVF_ALIGN_HRIGHT		(2)		// Sprite rechtsbündig
#define TB_SVF_ALIGN_VBOTTOM	(4)		// Sprite unten ausrichten
#define TB_SVF_ALIGN_VCENTER	(8)		// Sprite vertikal zentrieren
#define TB_SVF_ALIGN_VTOP		(16)	// Sprite oben ausrichten
#define TB_SVF_RELATIVE			(32)	// Koordinaten sind relativ
#define TB_SVF_RELATIVESCALING	(64)	// Skalierung ist relativ
#define TB_SVF_ALIGN_CENTER		(128)	// Sprite zentriert
#define TB_SVF_FULLMODE			(256)	// Als Vollbild anzeigen
#define TB_SVF_DEPTH_RELATIVE	(512)	// Texturtiefe ist relativ (0..1)
#define TB_SVF_DEPTH_ABSOLUTE	(1024)	// Texturtiefe ist absolut (ganze Zahlen = Texturtiefennummer, Nachkomma = Alphawert)

// ******************************************************************
// Klasse für ein Volumensprite
class TRIBASE_API tb2DVolumeSprite
{
private:
	// Variablen
	PDIRECT3DBASETEXTURE9	m_pTexture;		// Die Textur mit den Sprites
	tb2DVolumeSpriteInfo	m_2DSpriteInfo;	// Spriteinformationen
	tbEffect*				m_pEffect;		// Effekt
	int						m_iNumPasses;	// Anzahl der Durchgänge für den Effekt
	DWORD					m_dwOldFVF;		// Altes Vertexformat (zum Zeitpunkt von Begin)
	tb2DVolumeSpriteVertex	aVertex[600];	// Vertexliste der zu zeichnenden Sprites
	DWORD					dwVertexCursor;	// aktuelle Vertexposition in der Liste
	BOOL					m_bTexturmanager;	// Speichert, ober der Texturmanager verwendet wurde
	BOOL					m_bVolumeUse;	// Speichert, ob Volumentexturen unterstützt werden oder nicht
	
public:
	// Konstruktor und Destruktor
	tb2DVolumeSprite();
	~tb2DVolumeSprite();

	// Methoden
	// Laden aus Dateien
	tbResult	Init(char* pcINIFile,									// INI-Datei-Name
					 char* pcINISection,								// INI-Sektionseintrag
					 tbVector2 (*converter)(tbVector2 vParam) = NULL,	// Funktion zum konvertieren der INI-Werte von relative in absolute Koordinaten
					 char* pcTextureFile = NULL);						// INI-Eintrag nur bei NULL
	tbResult	Exit();
	tbResult	Begin();																			// Beginnen, Sprites zu zeichnen
	tbResult	End();																				// Beenden, Sprites zu zeichnen
	tbResult	DrawPuffer();																		// Alle Sprites im Puffer sofort malen

	// Liest Spritekoordinaten aus einer INI-Datei und rechnet sie ggf. um
	tbResult ReadINIVolumeSpriteInfo(tbINIReader* pINIReader,
									char* pcSection,
									char* pcKey,
									tb2DVolumeSpriteInfo* s2DSpriteInfo,
									tbVector2 (*converter)(tbVector2 vParam));

	// Sprite zeichnen (sollte 2er-Potenz Textur sein)
	tbResult DrawSprite(tbVector2 vPosition,
						float fDepth = 0.0f,
						DWORD dwNumber = 0,
						const DWORD dwFlags = TB_SVF_ALIGN_HLEFT | TB_SVF_ALIGN_VTOP | TB_SVF_DEPTH_ABSOLUTE,
						const tbColor& Color1 = tbColor(1.0f),
						tbVector2 vScaling = tbVector2(1.0f),
						float fRotating = 0.0f);	// rotate CW in RAD

	// Sprite zeichnen mit festen Fensterkoordinaten
	tbResult DrawSprite(tbVector4 vPosition,
						float fDepth = 0.0f,
						DWORD dwNumber = 0,
						const tbColor& Color1 = tbColor(1.0f));

	// Sprite mit Transformationsmatrix zeichnen (sollte ungerade Kantenlängen haben)
	tbResult DrawTransformedSprite(tbMatrix mTransformation,
								   float fDepth = 0.0f,
								   DWORD dwNumber = 0,
								   const DWORD dwFlags = TB_SVF_ALIGN_HLEFT | TB_SVF_ALIGN_VTOP | TB_SVF_DEPTH_ABSOLUTE,
								   const tbColor& Color1 = tbColor(1.0f));

	// Inline-Methoden
	PDIRECT3DBASETEXTURE9	GetTexture()	{return m_pTexture;}
	tb2DVolumeSpriteInfo	GetInfo()		{return m_2DSpriteInfo;}
	BOOL					GetTMUse()		{return m_bTexturmanager;}
	BOOL					GetVolumeUse()	{return m_bVolumeUse;}
};

// ******************************************************************

#endif __TB2DVOLUMESPRITE__
