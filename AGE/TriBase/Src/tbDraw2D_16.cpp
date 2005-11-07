/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbDraw2D_16.cpp
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
D3DCOLOR tbMakeRGB16(const DWORD r,
					 const DWORD g,
					 const DWORD b)
{
	// Farbe zusammensetzen
	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

// ******************************************************************
// Farbe mit Alpha mischen
D3DCOLOR tbMakeRGBA16(const DWORD r,
					  const DWORD g,
					  const DWORD b,
					  const DWORD a)
{
	// Farbe zusammensetzen - Alpha ignorieren
	return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

// ******************************************************************
// Farbkomponenten abfragen
DWORD tbGetR16(const D3DCOLOR c) {return ((c >> 11) & 0x0000001F) << 3;}
DWORD tbGetG16(const D3DCOLOR c) {return ((c >> 5) & 0x0000003F) << 2;}
DWORD tbGetB16(const D3DCOLOR c) {return (c & 0x0000001F) << 3;}
DWORD tbGetA16(const D3DCOLOR c) {return 255;}

// ******************************************************************
// Pixel setzen
void tbSetPixel16(tbDraw2D* pThis,
				  const DWORD x,
				  const DWORD y,
				  const D3DCOLOR c)
{
	// Pixel setzen
	((WORD*)(pThis->m_pBits))[y * pThis->m_dwPitch + x] = (WORD)(c);
}

// ******************************************************************
// Pixel abfragen
D3DCOLOR tbGetPixel16(tbDraw2D* pThis,
					  const DWORD x,
					  const DWORD y)
{
	// Pixel abfragen
	return ((WORD*)(pThis->m_pBits))[y * pThis->m_dwPitch + x];
}

// ******************************************************************
// Horizontale Linie zeichnen
void tbDrawHLine16(tbDraw2D* pThis,
				   DWORD x1,
				   DWORD x2,
				   DWORD y,
				   D3DCOLOR c)
{
	// Eine horizontale Linie von Pixeln wird gezeichnet.
	if(x1 > x2) {DWORD dwTemp = x1; x1 = x2; x2 = dwTemp;}

	WORD wc = (WORD)(c);
	WORD* pwCursor = (WORD*)(pThis->m_pBits) + y * pThis->m_dwPitch + x1;

	for(DWORD x = x1; x <= x2; x++)
	{
		// Farbe setzen und Cursor erhöhen
		*(pwCursor++) = wc;
	}
}

// ******************************************************************
// Vertikale Linie zeichnen
void tbDrawVLine16(tbDraw2D* pThis,
				   DWORD x,
				   DWORD y1,
				   DWORD y2,
				   D3DCOLOR c)
{
	// Eine vertikale Linie von Pixeln wird gezeichnet.
	if(y1 > y2) {DWORD dwTemp = y1; y1 = y2; y2 = dwTemp;}

	WORD wc = (WORD)(c);
	WORD* pwCursor = (WORD*)(pThis->m_pBits) + y1 * pThis->m_dwPitch + x;

	for(DWORD y = y1; y <= y2; y++)
	{
		// Farbe setzen und Cursor zur nächsten Zeile fahren
		*pwCursor = wc;
		pwCursor += pThis->m_dwPitch;
	}
}

// ******************************************************************
// Bit-Block-Transfer durchführen
void tbBlt16(tbDraw2D* pSrc,
			 tbDraw2D* pDest,
			 DWORD dwDestX,
			 DWORD dwDestY,
			 DWORD dwSrcX,
			 DWORD dwSrcY,
			 DWORD dwWidth,
			 DWORD dwHeight)
{
	WORD* pwSrcCursor = (WORD*)(pSrc->m_pBits) + dwSrcY * pSrc->m_dwPitch + dwSrcX;
	WORD* pwDestCursor = (WORD*)(pDest->m_pBits) + dwDestY * pDest->m_dwPitch + dwDestX;

	// Zeile für Zeile kopieren
	for(DWORD y = 0; y < dwHeight; y++)
	{
		// Eine Zeile kopieren
		memcpy(pwDestCursor, pwSrcCursor, dwWidth << 1);

		pwSrcCursor += pSrc->m_dwPitch;
		pwDestCursor += pDest->m_dwPitch;
	}
}

// ******************************************************************