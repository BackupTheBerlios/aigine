// Config.h: interface for the Config class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONFIG_H
#define CONFIG_H

#include "../AiGinE.h"

#pragma pack(1)

// Struktur für die Einstellungen
struct Config
{
	struct
	{
		int					iAdapter;				// Adapter-ID
		D3DDEVTYPE			DeviceType;				// Gerätetyp
		BOOL				bWindowed;				// Fenstermodus?
		D3DDISPLAYMODE		VideoMode;				// Videomodus
		D3DFORMAT			BackBufferFormat;		// Bildpufferformat
		D3DFORMAT			ZStencilBufferFormat;	// Z-Stencil-Buffer-Format
		D3DMULTISAMPLE_TYPE	MultiSamplingType;		// Multi-Sampling-Typ
		DWORD				dwMultiSamplingQuality;	// Multi-Sampling-Qualität
		DWORD				dwFlags;				// Flags (Hardware oder Software?)
	} Direct3D;
	
	struct
	{
		GUID				SoundDriverGUID;		// GUID des Soundtreibers
		DWORD				dwSampleRate;			// Sampling-Frequenz
		DWORD				dwNumBitsPerSample;		// Anzahl der Bits pro Sample
	} DirectSound;
};

// ******************************************************************
// Funktionen deklarieren
Result DoConfigDialog(Config* pOut);	// Konfigurationsdialog aufrufen

#endif //CONFIG_H
