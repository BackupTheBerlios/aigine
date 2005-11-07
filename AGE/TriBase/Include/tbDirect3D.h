/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbDirect3D.h
	============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Einfacher Umgang mit Direct3D

	Autor:
	David Scherfgen

********************************************************************/

// Unions für Render-States, Texturschicht-States und Sampler-States
union TRIBASE_API tbD3DRS
{
	DWORD adwRS[205];	// Render-State-Tabelle mit DWORD-Werten
	float afRS[205];	// Render-State-Tabelle mit float-Werten
};

union TRIBASE_API tbD3DTSS
{
	DWORD adwTSS[8][33];	// Texturschicht-State-Tabelle mit DWORD-Werten
	float afTSS[8][33];		// Texturschicht-State-Tabelle mit float-Werten
};

union TRIBASE_API tbD3DSS
{
	DWORD adwSS[8][14];	// Sampler-State-Tabelle mit DWORD-Werten
	float afSS[8][14];	// Sampler-State-Tabelle mit float-Werten
};

// ******************************************************************
// Die tbDirect3D-Klasse
class TRIBASE_API tbDirect3D
{
private:
	// Variablen
	static BOOL						m_bInitialized;		// Initialisiert?
	static HWND						m_hWindow;			// Fensterhandle
	static BOOL						m_bOwnWindow;		// Möchte der Benutzer sein eigenes Fenster verwenden?
	static PDIRECT3D9				m_pD3D;				// IDirect3D9-Schnittstelle
	static PDIRECT3DDEVICE9			m_pD3DDevice;		// Geräteschnittstelle IDirect3DDevice9
	static D3DCAPS9					m_Caps;				// Fähigkeiten der Hardware
	static D3DPRESENT_PARAMETERS	m_PresentParams;
	static tbVector2				m_vScreenSize;

	static tbD3DRS					m_RS;	// Render-States
	static tbD3DTSS					m_TSS;	// Texturschicht-States
	static tbD3DSS					m_SS;	// Sampler-States

	static DWORD					m_dwFVF;		// Aktuelles Vertexformat
	static PDIRECT3DBASETEXTURE9	m_apTexture[8];	// Texturtabelle
	static BOOL						m_abIsRS[205];
	static BOOL						m_abIsTSS[33];
	static BOOL						m_abIsSS[14];

public:
	// Methoden
	static tbResult					Init(tbConfig* pConfig, char* pcWindowTitle, HWND hOwnWindow = NULL, HICON hIcon = NULL);					// Fenster und Direct3D initialisieren
	static tbResult					Exit();																										// Herunterfahren
	static tbResult					Capture();																									// Aktualisiert die internen Tabellen und Pufferwerte
	static tbResult					Clear(DWORD dwNumRects, const D3DRECT* pRects, DWORD dwFlags, D3DCOLOR Color, float z, DWORD dwStencil);	// Leert die Puffer
	static tbResult					BeginScene();																								// Beginnt die Szene
	static tbResult					EndScene();																									// Beendet die Szene
	static tbResult					Present();																									// Zeigt den Bildpuffer an
	static tbResult					SetRS(D3DRENDERSTATETYPE RS, DWORD dwValue);																// Setzt ein Render-State
	static tbResult					SetRSF(D3DRENDERSTATETYPE RS, float fValue);																// Setzt ein Render-State
	static DWORD					GetRS(D3DRENDERSTATETYPE RS);																				// Fragt ein Render-State ab
	static float					GetRSF(D3DRENDERSTATETYPE RS);																				// Fragt ein Render-State ab
	static tbResult					SetTSS(DWORD dwStage, D3DTEXTURESTAGESTATETYPE TSS, DWORD dwValue);											// Setzt ein Texturschicht-State
	static tbResult					SetTSSF(DWORD dwStage, D3DTEXTURESTAGESTATETYPE TSS, float fValue);											// Setzt ein Texturschicht-State
	static DWORD					GetTSS(DWORD dwStage, D3DTEXTURESTAGESTATETYPE TSS);														// Fragt ein Texturschicht-State ab
	static float					GetTSSF(DWORD dwStage, D3DTEXTURESTAGESTATETYPE TSS);														// Fragt ein Texturschicht-State ab
	static tbResult					SetSS(DWORD dwStage, D3DSAMPLERSTATETYPE SS, DWORD dwValue);												// Setzt ein Sampler-State
	static tbResult					SetSSF(DWORD dwStage, D3DSAMPLERSTATETYPE SS, float fValue);												// Setzt ein Sampler-State
	static DWORD					GetSS(DWORD dwStage, D3DSAMPLERSTATETYPE SS);																// Fragt ein Sampler-State ab
	static float					GetSSF(DWORD dwStage, D3DSAMPLERSTATETYPE SS);																// Fragt ein Sampler-State ab
	static tbResult					SetTexture(DWORD dwStage, PDIRECT3DBASETEXTURE9 pTexture);													// Setzt eine Textur
	static PDIRECT3DBASETEXTURE9	GetTexture(DWORD dwStage);																					// Fragt eine Textur ab
	static tbResult					SetTransform(D3DTRANSFORMSTATETYPE Transform, const tbMatrix& mMatrix);										// Setzt eine Matrix
	static tbMatrix					GetTransform(D3DTRANSFORMSTATETYPE Transform);																// Fragt eine Matrix ab
	static tbResult					SetLight(DWORD dwLight, D3DLIGHT9* pLight);																	// Setzt ein Licht
	static tbResult					LightEnable(DWORD dwLight, BOOL bEnable);																	// Aktiviert/deaktiviert ein Licht
	static tbResult					GetLight(DWORD dwLight, D3DLIGHT9* pOut);																	// Fragt ein Licht ab
	static tbResult					GetLightEnable(DWORD dwLight, BOOL* pbOut);																	// Fragt den Status eines Lichts ab
	static tbResult					SetMaterial(D3DMATERIAL9* pMaterial);																		// Setzt ein Material
	static D3DMATERIAL9				GetMaterial();																								// Fragt das aktive Material ab
	static tbResult					SetFVF(DWORD dwFVF);																						// Setzt ein Vertexformat

	// Inline-Methoden
	static inline BOOL				IsInitialized()	{return m_bInitialized;}
	static inline HWND				GetWindow()		{return m_hWindow;}
	static inline BOOL				GetOwnWindow()	{return m_bOwnWindow;}
	static inline PDIRECT3D9		GetD3D()		{return m_pD3D;}
	static inline PDIRECT3DDEVICE9	GetDevice()		{return m_pD3DDevice;}
	static inline DWORD				GetFVF()		{return m_dwFVF;}
	static inline D3DCAPS9			GetCaps()		{return m_Caps;}
	static inline tbVector2			GetScreenSize()	{return m_vScreenSize;}
	static inline float				GetAspect()		{return m_vScreenSize.x / m_vScreenSize.y;}
};

// ******************************************************************