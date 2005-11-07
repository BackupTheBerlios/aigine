/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbDraw2D.cpp
	============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Zeichnen auf Oberflächen

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbDraw2D-Klasse
tbDraw2D::tbDraw2D()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbDraw2D));
}

// ******************************************************************
// Destruktor der tbDraw2D-Klasse
tbDraw2D::~tbDraw2D()
{
	// Wenn wir eine Direct3D-Oberfläche haben: entsperren und freigeben
	if(m_pSurface != NULL)
	{
		if(m_bLocked) EndDraw();
		if(m_bFromTexture) m_pSurface->Release();
		TB_SAFE_RELEASE(m_pSurface);
		TB_SAFE_RELEASE(m_pTexture);
	}
	else if(m_pMemory != NULL)
	{
		// Speicher freigeben
		TB_SAFE_MEMFREE(m_pMemory);
	}
}

// ******************************************************************
// Initialisierung aus einer Direct3D-Oberfläche
tbResult tbDraw2D::Init(PDIRECT3DSURFACE9 pSurface)
{
	D3DSURFACE_DESC SurfaceDesc;

	// Parameter prüfen
	if(pSurface == NULL) TB_ERROR_NULL_POINTER("pSurface", TB_ERROR);


	// Referenzzähler der Oberfläche erhöhen
	pSurface->AddRef();

	// Eigenschaften der Oberfläche abfragen und speichern
	pSurface->GetDesc(&SurfaceDesc);
	m_dwWidth = SurfaceDesc.Width;
	m_dwHeight = SurfaceDesc.Height;
	m_Format = SurfaceDesc.Format;
	m_dwUsage = SurfaceDesc.Usage;
	m_pSurface = pSurface;

	// Bytes pro Pixel abfragen
	m_dwBytesPerPixel = tbGetD3DFormatBPP(m_Format) / 8;
	if(m_dwBytesPerPixel == 0) TB_ERROR("Nicht unterstütztes Format!", TB_ERROR);

	// Funktionszeiger ausfüllen
	if(InitFunctionPointers()) TB_ERROR("Fehler beim Ausfüllen der Funktionszeiger!", TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Initialisierung aus einer Textur
tbResult tbDraw2D::Init(PDIRECT3DTEXTURE9 pTexture,
						DWORD dwLevel)
{
	HRESULT				hResult;
	PDIRECT3DSURFACE9	pSurface;


	// Parameter prüfen
	if(pTexture == NULL) TB_ERROR_NULL_POINTER("pTexture", TB_ERROR);


	// Die Oberfläche abfragen
	hResult = pTexture->GetSurfaceLevel(dwLevel, &pSurface);
	if(FAILED(hResult)) TB_ERROR_DIRECTX("pTexture->GetSurfaceLevel", hResult, TB_ERROR);

	// Wir merken uns, dass die Oberfläche aus einer Textur stammt.
	m_bFromTexture = TRUE;

	// Die andere Methode aufrufen
	return Init(pSurface);
}

// ******************************************************************
// Initialisierung aus einer Würfeltextur
tbResult tbDraw2D::Init(PDIRECT3DCUBETEXTURE9 pCubeTexture,
						D3DCUBEMAP_FACES Face,
						DWORD dwLevel)
{
	HRESULT				hResult;
	PDIRECT3DSURFACE9	pSurface;


	// Parameter prüfen
	if(pCubeTexture == NULL) TB_ERROR_NULL_POINTER("pCubeTexture", TB_ERROR);


	// Die Oberfläche abfragen
	hResult = pCubeTexture->GetCubeMapSurface(Face, dwLevel, &pSurface);
	if(FAILED(hResult)) TB_ERROR_DIRECTX("pCubeTexture->GetCubeMapSurface", hResult, TB_ERROR);

	// Wir merken uns, dass die Oberfläche aus einer Textur stammt.
	m_bFromTexture = TRUE;

	// Die andere Methode aufrufen
	return Init(pSurface);
}

// ******************************************************************
// Initialisierung durch neuen Speicherbereich
tbResult tbDraw2D::Init(DWORD dwWidth,
						DWORD dwHeight,
						D3DFORMAT Format)
{
	// Parameter prüfen
	if(dwWidth == 0)	TB_ERROR_INVALID_VALUE("dwWidth", TB_ERROR);
	if(dwHeight == 0)	TB_ERROR_INVALID_VALUE("dwHeight", TB_ERROR);


	// Angaben kopieren
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;
	m_Format = Format;

	// Bytes pro Pixel abfragen
	m_dwBytesPerPixel = tbGetD3DFormatBPP(m_Format) / 8;
	if(m_dwBytesPerPixel == 0) TB_ERROR("Nicht unterstütztes Format!", TB_ERROR);

	// Speicher reservieren
	m_pMemory = tbMemAlloc(m_dwWidth * m_dwHeight * m_dwBytesPerPixel);
	if(m_pMemory == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Funktionszeiger ausfüllen
	if(InitFunctionPointers()) TB_ERROR("Fehler beim Ausfüllen der Funktionszeiger!", TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Lädt ein Bild aus einer virtuellen Datei
tbResult tbDraw2D::Init(tbVFile* pVFile,
						DWORD dwWidth,
						DWORD dwHeight,
						D3DFORMAT Format) // = D3DFMT_UNKNOWN
{
	HRESULT				hResult;
	PDIRECT3DTEXTURE9	pTexture;


	// Parameter prüfen
	if(pVFile == NULL) TB_ERROR_NULL_POINTER("pVFile", TB_ERROR);

	// Sicherstellen, dass Direct3D initialisiert wurde
	if(!tbDirect3D::IsInitialized()) TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Textur laden
	hResult = D3DXCreateTextureFromFileInMemoryEx(tbDirect3D::GetDevice(),
		                                          (BYTE*)(pVFile->GetBuffer()) + pVFile->GetCursor(),
												  pVFile->GetSize() - pVFile->GetCursor(),
												  dwWidth, dwHeight, 1,
												  0, Format, D3DPOOL_SYSTEMMEM,
												  D3DX_DEFAULT, D3DX_DEFAULT,
												  0, NULL, NULL, &pTexture);
	if(FAILED(hResult)) TB_ERROR_DIRECTX("D3DXCreateTextureFromFileInMemoryEx", hResult, TB_ERROR);
	m_pTexture = pTexture;

	// Die andere Methode aufrufen
	return Init(pTexture, 0);
}

// ******************************************************************
// Lädt ein Bild aus einer Datei
tbResult tbDraw2D::Init(char* pcFilename,
						DWORD dwWidth,
						DWORD dwHeight,
						D3DFORMAT Format)	// = D3DFMT_UNKNOWN
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", TB_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new tbVFile;
	if(pVFile->Init(pcFilename)) TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);

	// Die andere Methode aufrufen
	if(Init(pVFile, dwWidth, dwHeight, Format))
	{
		// Fehler!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Laden des Bilds!", TB_ERROR);
	}

	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Lädt ein Bild aus einem Speicherbereich
tbResult tbDraw2D::Init(void* pMemory,
						DWORD dwMemorySize,
						DWORD dwWidth,
						DWORD dwHeight,
						D3DFORMAT Format) // = D3DFMT_UNKNOWN
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(pMemory == NULL)		TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);
	if(dwMemorySize == 0)	TB_ERROR_INVALID_VALUE("dwMemorySize", TB_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new tbVFile;
	if(pVFile->Init(pMemory, dwMemorySize)) TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);

	// Die andere Methode aufrufen
	if(Init(pVFile, dwWidth, dwHeight, Format))
	{
		// Fehler!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Laden des Bilds!", TB_ERROR);
	}

	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Lädt ein Bild aus einer Ressource
tbResult tbDraw2D::Init(HMODULE hModule,
						char* pcResourceType,
						char* pcResourceName,
						DWORD dwWidth,
						DWORD dwHeight,
						D3DFORMAT Format) // = D3DFMT_UNKNOWN
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(hModule == NULL)			TB_ERROR_NULL_POINTER("hModule", TB_ERROR);
	if(pcResourceName == NULL)	TB_ERROR_NULL_POINTER("pcResourceName", TB_ERROR);
	if(pcResourceType == NULL)	TB_ERROR_NULL_POINTER("pcResourceType", TB_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new tbVFile;
	if(pVFile->Init(hModule, pcResourceName, pcResourceType)) TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);

	// Die andere Methode aufrufen
	if(Init(pVFile, dwWidth, dwHeight, Format))
	{
		// Fehler!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Laden des Bilds!", TB_ERROR);
	}

	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Initialisiert die Funktionszeiger
tbResult tbDraw2D::InitFunctionPointers()
{
	// Funktionszeiger ausfüllen - je nach Format
	switch(m_Format)
	{
	case D3DFMT_R5G6B5:
		// 16-Bit-Format ohne Alpha
		m_pMakeRGB		= tbMakeRGB16;
		m_pMakeRGBA		= tbMakeRGBA16;
		m_pGetR			= tbGetR16;
		m_pGetG			= tbGetG16;
		m_pGetB			= tbGetB16;
		m_pGetA			= tbGetA16;
		m_pSetPixel		= tbSetPixel16;
		m_pGetPixel		= tbGetPixel16;
		m_pDrawHLine	= tbDrawHLine16;
		m_pDrawVLine	= tbDrawVLine16;
		m_pBlt			= tbBlt16;
		break;

	case D3DFMT_X1R5G5B5:
	case D3DFMT_A1R5G5B5:
		// 16-Bit-Formate mit Alpha
		m_pMakeRGB		= tbMakeRGB16A;
		m_pMakeRGBA		= tbMakeRGBA16A;
		m_pGetR			= tbGetR16A;
		m_pGetG			= tbGetG16A;
		m_pGetB			= tbGetB16A;
		m_pGetA			= tbGetA16A;
		m_pSetPixel		= tbSetPixel16;
		m_pGetPixel		= tbGetPixel16;
		m_pDrawHLine	= tbDrawHLine16;
		m_pDrawVLine	= tbDrawVLine16;
		m_pBlt			= tbBlt16;
		break;

	case D3DFMT_X8R8G8B8:
	case D3DFMT_A8R8G8B8:
		// 32-Bit-Formate
		m_pMakeRGB		= tbMakeRGB32;
		m_pMakeRGBA		= tbMakeRGBA32;
		m_pGetR			= tbGetR32;
		m_pGetG			= tbGetG32;
		m_pGetB			= tbGetB32;
		m_pGetA			= tbGetA32;
		m_pSetPixel		= tbSetPixel32;
		m_pGetPixel		= tbGetPixel32;
		m_pDrawHLine	= tbDrawHLine32;
		m_pDrawVLine	= tbDrawVLine32;
		m_pBlt			= tbBlt32;
		break;

	default:
		// Standardfunktionen zuweisen
		if(m_dwBytesPerPixel == 2)
		{
			m_pMakeRGB		= tbMakeRGB16;
			m_pMakeRGBA		= tbMakeRGBA16;
			m_pGetR			= tbGetR16;
			m_pGetG			= tbGetG16;
			m_pGetB			= tbGetB16;
			m_pGetA			= tbGetA16;
			m_pSetPixel		= tbSetPixel16;
			m_pGetPixel		= tbGetPixel16;
			m_pDrawHLine	= tbDrawHLine16;
			m_pDrawVLine	= tbDrawVLine16;
			m_pBlt			= tbBlt16;
		}
		else if(m_dwBytesPerPixel == 4)
		{
			m_pMakeRGB		= tbMakeRGB32;
			m_pMakeRGBA		= tbMakeRGBA32;
			m_pSetPixel		= tbSetPixel32;
			m_pGetPixel		= tbGetPixel32;
			m_pDrawHLine	= tbDrawHLine32;
			m_pDrawVLine	= tbDrawVLine32;
			m_pBlt			= tbBlt32;
		}
		else
		{
			// Nicht unterstütztes Format!
			TB_ERROR("Für dieses Format existieren keine Zeichenfunktionen!", TB_ERROR);
		}
		break;
	}

	return TB_OK;
}

// ******************************************************************
// Beginnt mit dem Zeichnen
tbResult tbDraw2D::BeginDraw(RECT* pRect,	// = NULL
							 DWORD dwFlags)	// = 0
{
	HRESULT			hResult;
	D3DLOCKED_RECT	LockedRect;


	// Schon gesperrt?
	if(m_bLocked) TB_ERROR("Doppeltes Sperren nicht möglich!", TB_ERROR);

	if(m_pSurface != NULL)
	{
		if(dwFlags & D3DLOCK_DISCARD && (!(m_dwUsage & D3DUSAGE_DYNAMIC) ||
		                                 pRect != NULL))
		{
			// Flag entfernen!
			dwFlags ^= D3DLOCK_DISCARD;
		}

		// Oberfläche sperren
		hResult = m_pSurface->LockRect(&LockedRect, pRect, dwFlags);
		if(FAILED(hResult)) TB_ERROR_DIRECTX("m_pSurface->LockRect", hResult, TB_ERROR);

		// Angaben kopieren
		m_pBits = LockedRect.pBits;
		m_dwPitch = LockedRect.Pitch / m_dwBytesPerPixel;
	}
	else
	{
		// Zeiger erstellen
		if(pRect == NULL) m_pBits = m_pMemory;
		else m_pBits = (void*)((BYTE*)(m_pMemory) + ((pRect->top * m_dwWidth) + pRect->left) * m_dwBytesPerPixel);

		// Der Zeilenabstand ist immer gleich der Breite.
		m_dwPitch = m_dwWidth;
	}

	// Gesperrt!
	m_bLocked = TRUE;

	// Größe des gesperrten Bereichs berechnen
	if(pRect == NULL)
	{
		m_dwLockedWidth = m_dwWidth;
		m_dwLockedHeight = m_dwHeight;
	}
	else
	{
		m_dwLockedWidth = (pRect->right - pRect->left) + 1;
		m_dwLockedHeight = (pRect->bottom - pRect->top) + 1;
	}

	return TB_OK;
}

// ******************************************************************
// Beendet den Zeichenvorgang
tbResult tbDraw2D::EndDraw()
{
	if(m_bLocked)
	{
		if(m_pSurface != NULL)
		{
			// Oberfläche entsperren
			m_pSurface->UnlockRect();
		}

		m_bLocked = FALSE;
	}

	return TB_OK;
}

// ******************************************************************
// Zeichnet eine Linie
void tbDraw2D::DrawLine(DWORD x1,
						DWORD y1,
						DWORD x2,
						DWORD y2,
						D3DCOLOR c)
{
	// Diese Funktion zeichnet eine Linie nach
	// Bresenhams Linienalgorithmus.

	int dx = (int)(abs((int)(x1) - (int)(x2)));
	int dy = (int)(abs((int)(y1) - (int)(y2)));
	int ix = x1 < x2 ? 1 : -1;
	int iy = y1 < y2 ? 1 : -1;

	if(dx == 0) {m_pDrawVLine(this, x1, iy == 1 ? y1 : y2, iy == 1 ? y2 : y1, c); return;}
	if(dy == 0) {m_pDrawHLine(this, ix == 1 ? x1 : x2, ix == 1 ? x2 : x1, y1, c); return;}

	if(dx > dy)
	{
		int dpr = dy << 1;
		int dpu = dpr - (dx << 1);
		int p = dpr - dx;

		for(; dx >= 0; dx--)
		{
			m_pSetPixel(this, x1, y1, c);
			if(p > 0)
			{ 
				x1 += ix;
				y1 += iy;
				p += dpu;
			}
			else
			{
				x1 += ix;
				p += dpr;
			}
		}		
	}
	else
	{
		int dpr = dx << 1;
		int dpu = dpr - (dy << 1);
		int p =	dpr - dy;

		for(; dy >= 0; dy--)
		{
			m_pSetPixel(this, x1, y1, c);

			if(p > 0)
			{ 
				x1 += ix;
				y1 += iy;
				p += dpu;
			}
			else
			{
				y1 += iy;
				p += dpr;
			}
		}		
	}		
}

// ******************************************************************
// Zeichnet einen Kreis
void tbDraw2D::DrawCircle(DWORD cx,
						  DWORD cy,
						  DWORD r,
						  D3DCOLOR c)
{
	// Diese Funktion zeichnet einen hohlen Kreis nach
	// Bresenhams Kreisalgorithmus.

	DWORD x = 0;
	DWORD y = r;
	int p = 3 - (r << 2);

	while(x < y)
	{
		m_pSetPixel(this, cx - x, cy - y, c);
		m_pSetPixel(this, cx + x, cy - y, c);
		m_pSetPixel(this, cx - x, cy + y, c);
		m_pSetPixel(this, cx + x, cy + y, c);
		m_pSetPixel(this, cx - y, cy - x, c);
		m_pSetPixel(this, cx + y, cy - x, c);
		m_pSetPixel(this, cx - y, cy + x, c);
		m_pSetPixel(this, cx + y, cy + x, c);
 
		if(p < 0) p = p + (x << 2) + 6;
		else
		{
			p = p + ((x - y) << 2) + 10;
			y--;
		}

		x++;
	}

	if(x == y)
	{
		m_pSetPixel(this, cx - x, cy - x, c);
		m_pSetPixel(this, cx + x, cy - x, c);
		m_pSetPixel(this, cx - x, cy + x, c);
		m_pSetPixel(this, cx + x, cy + x, c);
	}
}

// ******************************************************************
// Zeichnet einen gefüllten Kreis
void tbDraw2D::DrawFilledCircle(DWORD cx,
								DWORD cy,
								DWORD r,
								D3DCOLOR c)
{
	// Diese Funktion zeichnet einen gefüllten Kreis nach
	// Bresenhams Kreisalgorithmus.

	DWORD x = 0;
	DWORD y = r;
	int p = 3 - (r << 2);

	while(x < y)
	{
		m_pDrawHLine(this, cx - x, cx + x, cy - y, c);
		m_pDrawHLine(this, cx - x, cx + x, cy + y, c);
		m_pDrawHLine(this, cx - y, cx + y, cy - x, c);
		m_pDrawHLine(this, cx - y, cx + y, cy + x, c);
 
		if(p < 0) p = p + (x << 2) + 6;
		else
		{
			p = p + ((x - y) << 2) + 10;
			y--;
		}

		x++;
	}

	if(x == y)
	{
		m_pDrawHLine(this, cx - x, cx + x, cy - x, c);
		m_pDrawHLine(this, cx - x, cx + x, cy + x, c);
	}
}

// ******************************************************************