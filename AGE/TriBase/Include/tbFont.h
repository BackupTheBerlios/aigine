/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbFont.h
	========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit Schriftarten

	Autor:
	David Scherfgen


********************************************************************/

#pragma pack(1)

// Struktur für die Informationen über eine Schriftart
struct TRIBASE_API tbFontInfo
{
	float		fWidth;				// Breite der Schriftart
	float		fHeight;			// Höhe der Schriftart
	float		fTextureWidth;		// Breite der Textur
	float		fTextureHeight;		// Höhe der Textur
	tbVector2	avTopLeft[256];		// Linke obere Koordinate jedes Zeichens
	tbVector2	avBottomRight[256];	// Rechte untere Koordinate jedes Zeichens
	float		afCharWidth[256];	// Breite jedes Zeichens in Pixeln
};

// ******************************************************************
// Vertexformat
#define TB_FONT_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
struct TRIBASE_API tbFontVertex
{
	tbVector3	vPosition;	// Positionsangabe
	float		fRHW;		// 1/w
	D3DCOLOR	Diffuse;	// Streufarbe
	tbVector2	vTexture;	// Texturkoordinaten
};

// ******************************************************************
// Flags zum Zeichnen von Text
#define TB_FF_ALIGN_HLEFT		(0)		// Text linksbündig
#define TB_FF_ALIGN_HCENTER		(1)		// Text zentrieren
#define TB_FF_ALIGN_HRIGHT		(2)		// Text rechtsbündig
#define TB_FF_ALIGN_VBOTTOM		(4)		// Text unten ausrichten
#define TB_FF_ALIGN_VCENTER		(8)		// Text vertikal zentrieren
#define TB_FF_ALIGN_VTOP		(16)	// Text oben ausrichten
#define TB_FF_RELATIVE			(32)	// Koordinaten sind relativ
#define TB_FF_RELATIVESCALING	(64)	// Skalierung ist relativ

// ******************************************************************
// Klasse für eine Schriftart
class TRIBASE_API tbFont
{
private:
	// Variablen
	PDIRECT3DTEXTURE9		m_pTexture;		// Die Textur mit den Buchstaben
	tbFontInfo				m_FontInfo;		// Schriftartinformationen
	tbEffect*				m_pEffect;		// Effekt
	int						m_iNumPasses;	// Anzahl der Durchgänge für den Effekt
	DWORD					m_dwOldFVF;		// Altes Vertexformat (zum Zeitpunkt von Begin)

public:
	// Konstruktor und Destruktor
	tbFont();
	~tbFont();

	// Methoden
	tbResult	Init(tbVFile* pTGAFile, tbVFile* pTBFFile);														// Laden aus virtuellen Dateien
	tbResult	Init(char* pcTGAFile, char* pcTBFFile);															// Laden aus echten Dateien
	tbResult	Init(void* pTGAFileMem, int iTGAFileMemSize, void* pTBFFileMem, int iTBFFileMemSize);			// Laden aus dem Speicher
	tbResult	Init(HMODULE hModule, char* pcTGAFileResName, char* pcTBFFileResName, char* pcResourceType);	// Laden aus einer Ressource
	float		ComputeTextWidth(const char* pcText, int iLength = -1, const float fSpace = -2.0f);				// Berechnet die Breite eines Texts
	tbResult	Begin();																						// Beginnen, mit der Schriftart zu zeichnen
	tbResult	End();																							// Beenden, mit der Schriftart zu zeichnen

	// Berechnet die Länge eines Texts exakt
	float ComputeRealTextWidth(tbVector2 vPosition,
							   const char* pcText,
					  		   const DWORD dwFlags = 0,
					  		   int iLength = -1,
					  		   tbVector2 vScaling = tbVector2(1.0f),
					  		   float fSpace = -2.0f,
					  		   float fItalic = 0.0f,
					  		   float fTabStops = 25.0f);
	
	// Text zeichnen
	tbResult DrawText(tbVector2 vPosition,
		              const char* pcText,
					  const DWORD dwFlags = TB_FF_ALIGN_HLEFT | TB_FF_ALIGN_VTOP,
					  int iLength = -1,
					  const tbColor& Color1 = tbColor(1.0f),
					  tbColor& Color2 = tbColor(-1.0f),
					  tbVector2 vScaling = tbVector2(1.0f),
					  float fSpace = -2.0f,
					  float fItalic = 0.0f,
  					  float fLineSpace = -5.0f,
					  float fTabStops = 25.0f,
					  float fXOffset = 0.0f);

	// Text mit Transformationsmatrix zeichnen
	tbResult DrawTransformedText(tbMatrix mTransformation,
		                         const char* pcText,
								 const DWORD dwFlags = 0,
								 int iLength = -1,
								 const tbColor& Color1 = tbColor(1.0f),
								 tbColor& Color2 = tbColor(-1.0f),
								 float fSpace = -2.0f,
								 float fItalic = 0.0f,
  								 float fLineSpace = -5.0f,
								 float fTabStops = 25.0f,
								 float fXOffset = 0.0f);

	// Inline-Methoden
	PDIRECT3DTEXTURE9	GetTexture()	{return m_pTexture;}
	tbFontInfo			GetInfo()		{return m_FontInfo;}
	float				GetWidth()		{return m_FontInfo.fWidth;}
	float				GetHeight()		{return m_FontInfo.fHeight;}
};

// ******************************************************************