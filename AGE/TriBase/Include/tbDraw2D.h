/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbDraw2D.h
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Zeichnen auf Oberflächen

	Autor:
	David Scherfgen

********************************************************************/

// Klasse für's Zeichnen
class TRIBASE_API tbDraw2D
{
public:
	// Variablen
	PDIRECT3DSURFACE9	m_pSurface;
	PDIRECT3DTEXTURE9	m_pTexture;
	void*				m_pMemory;
	DWORD				m_dwWidth;
	DWORD				m_dwHeight;
	D3DFORMAT			m_Format;
	DWORD				m_dwUsage;
	DWORD				m_dwBytesPerPixel;
	BOOL				m_bLocked;
	BOOL				m_bFromTexture;
	void*				m_pBits;
	DWORD				m_dwPitch;
	DWORD				m_dwLockedWidth;
	DWORD				m_dwLockedHeight;

private:
	// Funktionszeiger
	D3DCOLOR	(* m_pMakeRGB)		(const DWORD, const DWORD, const DWORD);																// RGB-Farbe in das Format der Oberfläche umwandeln
	D3DCOLOR	(* m_pMakeRGBA)		(const DWORD, const DWORD, const DWORD, const DWORD);													// RGBA-Farbe in das Format der Oberfläche umwandeln
	DWORD		(* m_pGetR)			(const D3DCOLOR);																						// Rot-Komponente abfragen
	DWORD		(* m_pGetG)			(const D3DCOLOR);																						// Grün-Komponente abfragen
	DWORD		(* m_pGetB)			(const D3DCOLOR);																						// Blau-Komponente abfragen
	DWORD		(* m_pGetA)			(const D3DCOLOR);																						// Alpha-Komponente abfragen
	void		(* m_pSetPixel)		(tbDraw2D*, const DWORD, const DWORD, const D3DCOLOR);													// Pixel zeichnen
	D3DCOLOR	(* m_pGetPixel)		(tbDraw2D*, const DWORD, const DWORD);																	// Pixel abfragen
	void		(* m_pDrawHLine)	(tbDraw2D*, const DWORD, const DWORD, const DWORD, const D3DCOLOR);										// Horizontale Linie zeichnen
	void		(* m_pDrawVLine)	(tbDraw2D*, const DWORD, const DWORD, const DWORD, const D3DCOLOR);										// Vertikale Linie zeichnen
	void		(* m_pBlt)			(tbDraw2D*, tbDraw2D*, const DWORD, const DWORD, const DWORD, const DWORD, const DWORD, const DWORD);	// Bit-Block-Transfer durchführen

public:
	// Konstruktor und Destruktor
	tbDraw2D();
	~tbDraw2D();

	// Methoden
	tbResult Init(PDIRECT3DSURFACE9 pSurface);																										// Aus Direct3D-Oberfläche erstellen
	tbResult Init(PDIRECT3DTEXTURE9 pTexture, DWORD dwLevel);																						// Aus Textur initialisieren
	tbResult Init(PDIRECT3DCUBETEXTURE9 pCubeTexture, D3DCUBEMAP_FACES Face, DWORD dwLevel);														// Aus Würfeltextur initialisieren
	tbResult Init(DWORD dwWidth, DWORD dwHeight, D3DFORMAT Format);																					// Neuen Speicher reservieren
	tbResult Init(tbVFile* pVFile, DWORD dwWidth, DWORD dwHeight, D3DFORMAT Format = D3DFMT_UNKNOWN);												// Lädt ein Bild aus einer virtuellen Datei
	tbResult Init(char* pcFilename, DWORD dwWidth, DWORD dwHeight, D3DFORMAT Format = D3DFMT_UNKNOWN);												// Lädt ein Bild aus einer Datei
	tbResult Init(void* pMemory, DWORD dwMemorySize, DWORD dwWidth, DWORD dwHeight, D3DFORMAT Format = D3DFMT_UNKNOWN);								// Lädt ein Bild aus dem Speicher
	tbResult Init(HMODULE hModule, char* pcResourceName, char* pcResourceType, DWORD dwWidth, DWORD dwHeight, D3DFORMAT Format = D3DFMT_UNKNOWN);	// Lädt ein Bild aus einer Ressource
	tbResult InitFunctionPointers();																												// Initialisiert die Funktionszeiger
	tbResult BeginDraw(RECT* pRect = NULL, DWORD dwFlags = 0);																						// Beginnt mit dem Zeichnen
	tbResult EndDraw();																																// Beendet den Zeichenvorgang
	void	 DrawLine(const DWORD x1, const DWORD y1, const DWORD x2, const DWORD y2, const D3DCOLOR c);											// Zeichnet eine Linie
	void	 DrawCircle(const DWORD cx, const DWORD cy, const DWORD r, const D3DCOLOR c);															// Zeichnet einen Kreis
	void	 DrawFilledCircle(const DWORD cx, const DWORD cy, const DWORD r, const D3DCOLOR c);														// Zeichnet einen gefüllten Kreis

	// Inline-Methoden
	inline PDIRECT3DSURFACE9	GetSurface()																																		{return m_pSurface;}
	inline PDIRECT3DTEXTURE9	GetTexture()																																		{return m_pTexture;}
	inline void*				GetMemory()																																			{return m_pMemory;}
	inline D3DFORMAT			GetFormat()																																			{return m_Format;}
	inline DWORD				GetWidth()																																			{return m_dwWidth;}
	inline DWORD				GetHeight()																																			{return m_dwHeight;}
	inline D3DCOLOR				MakeRGB(const DWORD r, const DWORD g, const DWORD b)																								{return m_pMakeRGB(r, g, b);}
	inline D3DCOLOR				MakeRGBA(const DWORD r, const DWORD g, const DWORD b, const DWORD a)																				{return m_pMakeRGBA(r, g, b, a);}
	inline DWORD				GetR(const D3DCOLOR c)																																{return m_pGetR(c);}
	inline DWORD				GetG(const D3DCOLOR c)																																{return m_pGetG(c);}
	inline DWORD				GetB(const D3DCOLOR c)																																{return m_pGetB(c);}
	inline DWORD				GetA(const D3DCOLOR c)																																{return m_pGetA(c);}
	inline void					SetPixel(const DWORD x, const DWORD y, const D3DCOLOR c)																							{m_pSetPixel(this, x, y, c);}
	inline D3DCOLOR				GetPixel(const DWORD x, const DWORD y)																												{return m_pGetPixel(this, x, y);}
	inline void					DrawHLine(const DWORD x1, const DWORD x2, const DWORD y, const D3DCOLOR c)																			{m_pDrawHLine(this, x1, x2, y, c);}
	inline void					DrawVLine(const DWORD x, const DWORD y1, const DWORD y2, const D3DCOLOR c)																			{m_pDrawVLine(this, x, y1, y2, c);}
	inline void					DrawRect(const DWORD x1, const DWORD y1, const DWORD x2, const DWORD y2, const D3DCOLOR c)															{m_pDrawHLine(this, x1, x2, y1, c); m_pDrawHLine(this, x1, x2, y2, c); m_pDrawVLine(this, x1, y1, y2, c); m_pDrawVLine(this, x2, y1, y2, c);}
	inline void					DrawFilledRect(const DWORD x1, const DWORD y1, const DWORD x2, const DWORD y2, const D3DCOLOR c)													{for(DWORD y = y1; y <= y2; y++) m_pDrawHLine(this, x1, x2, y, c);}
	inline void					Blt(tbDraw2D* pDest, const DWORD dwDestX, const DWORD dwDestY, const DWORD dwSrcX, const DWORD dwSrcY, const DWORD dwWidth, const DWORD dwHeight)	{if(m_dwBytesPerPixel != pDest->m_dwBytesPerPixel) return; m_pBlt(this, pDest, dwDestX, dwDestY, dwSrcX, dwSrcY, dwWidth, dwHeight);}
};

// ******************************************************************
// Zeichenfunktionen
// 16-Bit-Zeichenfunktionen ohne Alpha
D3DCOLOR	tbMakeRGB16(const DWORD, const DWORD, const DWORD);
D3DCOLOR	tbMakeRGBA16(const DWORD, const DWORD, const DWORD, const DWORD);
DWORD		tbGetR16(const D3DCOLOR);
DWORD		tbGetG16(const D3DCOLOR);
DWORD		tbGetB16(const D3DCOLOR);
DWORD		tbGetA16(const D3DCOLOR);
void		tbSetPixel16(tbDraw2D*, const DWORD, const DWORD, const D3DCOLOR);
D3DCOLOR	tbGetPixel16(tbDraw2D*, const DWORD, const DWORD);
void		tbDrawHLine16(tbDraw2D*, const DWORD, const DWORD, const DWORD, const D3DCOLOR);
void		tbDrawVLine16(tbDraw2D*, const DWORD, const DWORD, const DWORD, const D3DCOLOR);
void		tbBlt16(tbDraw2D*, tbDraw2D*, const DWORD, const DWORD, const DWORD, const DWORD, const DWORD, const DWORD);

// 16-Bit-Zeichenfunktionen mit Alpha
D3DCOLOR	tbMakeRGB16A(const DWORD, const DWORD, const DWORD);
D3DCOLOR	tbMakeRGBA16A(const DWORD, const DWORD, const DWORD, const DWORD);
DWORD		tbGetR16A(const D3DCOLOR);
DWORD		tbGetG16A(const D3DCOLOR);
DWORD		tbGetB16A(const D3DCOLOR);
DWORD		tbGetA16A(const D3DCOLOR);

// 32-Bit-Zeichenfunktionen
D3DCOLOR	tbMakeRGB32(const DWORD, const DWORD, const DWORD);
D3DCOLOR	tbMakeRGBA32(const DWORD, const DWORD, const DWORD, const DWORD);
DWORD		tbGetR32(const D3DCOLOR);
DWORD		tbGetG32(const D3DCOLOR);
DWORD		tbGetB32(const D3DCOLOR);
DWORD		tbGetA32(const D3DCOLOR);
void		tbSetPixel32(tbDraw2D*, const DWORD, const DWORD, const D3DCOLOR);
D3DCOLOR	tbGetPixel32(tbDraw2D*, const DWORD, const DWORD);
void		tbDrawHLine32(tbDraw2D*, const DWORD, const DWORD, const DWORD, const D3DCOLOR);
void		tbDrawVLine32(tbDraw2D*, const DWORD, const DWORD, const DWORD, const D3DCOLOR);
void		tbBlt32(tbDraw2D*, tbDraw2D*, const DWORD, const DWORD, const DWORD, const DWORD, const DWORD, const DWORD);

// ******************************************************************