/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbDraw2D_16A.cpp
	================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Zeichnen auf Oberflächen

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Farbe mischen
D3DCOLOR tbMakeRGB16A(const DWORD r,
					  const DWORD g,
					  const DWORD b)
{
	// Farbe zusammensetzen - vierte Komponente auf 1 setzen
	return 0x8000 | ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3);
}

// ******************************************************************
// Farbe mit Alpha mischen
D3DCOLOR tbMakeRGBA16A(const DWORD r,
					   const DWORD g,
					   const DWORD b,
					   const DWORD a)
{
	// Farbe zusammensetzen
	return (a > 127 ? 0x8000 : 0) | ((r >> 3) << 10) | ((g >> 3) << 5) | (b >> 3);
}

// ******************************************************************
// Farbkomponenten abfragen
DWORD tbGetR16A(const D3DCOLOR c) {return ((c >> 10) & 0x0000001F) << 3;}
DWORD tbGetG16A(const D3DCOLOR c) {return ((c >> 5) & 0x0000001F) << 3;}
DWORD tbGetB16A(const D3DCOLOR c) {return (c & 0x0000001F) << 3;}
DWORD tbGetA16A(const D3DCOLOR c) {return c >= 0x8000 ? 255 : 0;}

// ******************************************************************