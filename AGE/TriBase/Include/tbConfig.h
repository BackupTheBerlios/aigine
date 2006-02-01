/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbConfig.h
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Abfragen der Engine-Einstellungen

	Autor:
	David Scherfgen

********************************************************************/

#pragma pack(1)

// Struktur für die Einstellungen
struct TRIBASE_API tbConfig
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

	struct
	{
		int					status;
		char				sessionname[64];
		char				hostName[64];
		int					portnumber;
		int					maxplayer;
//		int					hr;
	} DirectPlay;
};

// ******************************************************************
// Funktionen deklarieren
TRIBASE_API tbResult tbDoConfigDialog(tbConfig* pOut);	// Konfigurationsdialog aufrufen

// ******************************************************************