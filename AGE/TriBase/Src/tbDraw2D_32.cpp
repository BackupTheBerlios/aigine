/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbDraw2D_32.cpp
	===============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Zeichnen auf Oberflächen

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Farbe mischen
D3DCOLOR tbMakeRGB32(const DWORD r,
					 const DWORD g,
					 const DWORD b)
{
	// Farbe zusammensetzen - vierte Komponente auf 255 setzen
	return 0xFF000000 | (r << 16) | (g << 8) | b;
}

// ******************************************************************
// Farbe mit Alpha mischen
D3DCOLOR tbMakeRGBA32(const DWORD r,
					  const DWORD g,
					  const DWORD b,
					  const DWORD a)
{
	// Farbe zusammensetzen
	return (a << 24) | (r << 16) | (g << 8) | b;
}

// ******************************************************************
// Farbkomponenten abfragen
DWORD tbGetR32(const D3DCOLOR c) {return (c >> 16) & 0x000000FF;}
DWORD tbGetG32(const D3DCOLOR c) {return (c >> 8) & 0x000000FF;}
DWORD tbGetB32(const D3DCOLOR c) {return c & 0x000000FF;}
DWORD tbGetA32(const D3DCOLOR c) {return (c >> 24) & 0x000000FF;}

// ******************************************************************
// Pixel setzen
void tbSetPixel32(tbDraw2D* pThis,
				  const DWORD x,
				  const DWORD y,
				  const D3DCOLOR c)
{
	// Pixel setzen
	((DWORD*)(pThis->m_pBits))[y * pThis->m_dwPitch + x] = c;
}

// ******************************************************************
// Pixel abfragen
D3DCOLOR tbGetPixel32(tbDraw2D* pThis,
					  const DWORD x,
					  const DWORD y)
{
	// Pixel abfragen
	return ((DWORD*)(pThis->m_pBits))[y * pThis->m_dwPitch + x];
}

// ******************************************************************
// Horizontale Linie zeichnen
void tbDrawHLine32(tbDraw2D* pThis,
				   DWORD x1,
				   DWORD x2,
				   DWORD y,
				   D3DCOLOR c)
{
	// Eine horizontale Linie von Pixeln wird gezeichnet.
	if(x1 > x2) {DWORD dwTemp = x1; x1 = x2; x2 = dwTemp;}

	DWORD dwc = c;
	DWORD* pdwCursor = (DWORD*)(pThis->m_pBits) + y * pThis->m_dwPitch + x1;

	for(DWORD x = x1; x <= x2; x++)
	{
		// Farbe setzen und Cursor erhöhen
		*(pdwCursor++) = dwc;
	}
}

// ******************************************************************
// Vertikale Linie zeichnen
void tbDrawVLine32(tbDraw2D* pThis,
				   DWORD x,
				   DWORD y1,
				   DWORD y2,
				   D3DCOLOR c)
{
	// Eine vertikale Linie von Pixeln wird gezeichnet.
	if(y1 > y2) {DWORD dwTemp = y1; y1 = y2; y2 = dwTemp;}

	DWORD dwc = c;
	DWORD* pdwCursor = (DWORD*)(pThis->m_pBits) + y1 * pThis->m_dwPitch + x;

	for(DWORD y = y1; y <= y2; y++)
	{
		// Farbe setzen und Cursor zur nächsten Zeile fahren
		*pdwCursor = dwc;
		pdwCursor += pThis->m_dwPitch;
	}
}

// ******************************************************************
// Bit-Block-Transfer durchführen
void tbBlt32(tbDraw2D* pSrc,
			 tbDraw2D* pDest,
			 DWORD dwDestX,
			 DWORD dwDestY,
			 DWORD dwSrcX,
			 DWORD dwSrcY,
			 DWORD dwWidth,
			 DWORD dwHeight)
{
	DWORD* pdwSrcCursor = (DWORD*)(pSrc->m_pBits) + dwSrcY * pSrc->m_dwPitch + dwSrcX;
	DWORD* pdwDestCursor = (DWORD*)(pDest->m_pBits) + dwDestY * pDest->m_dwPitch + dwDestX;

	// Zeile für Zeile kopieren
	for(DWORD y = 0; y < dwHeight; y++)
	{
		// Eine Zeile kopieren
		memcpy(pdwDestCursor, pdwSrcCursor, dwWidth << 2);

		pdwSrcCursor += pSrc->m_dwPitch;
		pdwDestCursor += pDest->m_dwPitch;
	}
}

// ******************************************************************