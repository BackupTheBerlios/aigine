
#include "../AiGinE.h"

// Unions für Render-States, Texturschicht-States und Sampler-States
union D3DRS
{
	DWORD adwRS[205];	// Render-State-Tabelle mit DWORD-Werten
	float afRS[205];	// Render-State-Tabelle mit float-Werten
};

union D3DTSS
{
	DWORD adwTSS[8][33];	// Texturschicht-State-Tabelle mit DWORD-Werten
	float afTSS[8][33];		// Texturschicht-State-Tabelle mit float-Werten
};

union D3DSS
{
	DWORD adwSS[8][14];	// Sampler-State-Tabelle mit DWORD-Werten
	float afSS[8][14];	// Sampler-State-Tabelle mit float-Werten
};

// ******************************************************************
// Die Direct3D-Klasse
class Direct3D
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
	static Vector2D					m_vScreenSize;

	static D3DRS					m_RS;	// Render-States
	static D3DTSS					m_TSS;	// Texturschicht-States
	static D3DSS					m_SS;	// Sampler-States

	static DWORD					m_dwFVF;		// Aktuelles Vertexformat
	static PDIRECT3DBASETEXTURE9	m_apTexture[8];	// Texturtabelle
	static BOOL						m_abIsRS[205];
	static BOOL						m_abIsTSS[33];
	static BOOL						m_abIsSS[14];

public:
	// Methoden
	static Result					Init(Config* pConfig, char* pcWindowTitle, HWND hOwnWindow = NULL, HICON hIcon = NULL);					// Fenster und Direct3D initialisieren
	static Result					Exit();																										// Herunterfahren
	static Result					Capture();																									// Aktualisiert die internen Tabellen und Pufferwerte
	static Result					Clear(DWORD dwNumRects, const D3DRECT* pRects, DWORD dwFlags, D3DCOLOR Color, float z, DWORD dwStencil);	// Leert die Puffer
	static Result					BeginScene();																								// Beginnt die Szene
	static Result					EndScene();																									// Beendet die Szene
	static Result					Present();																									// Zeigt den Bildpuffer an
	static Result					SetRS(D3DRENDERSTATETYPE RS, DWORD dwValue);																// Setzt ein Render-State
	static Result					SetRSF(D3DRENDERSTATETYPE RS, float fValue);																// Setzt ein Render-State
	static DWORD					GetRS(D3DRENDERSTATETYPE RS);																				// Fragt ein Render-State ab
	static float					GetRSF(D3DRENDERSTATETYPE RS);																				// Fragt ein Render-State ab
	static Result					SetTSS(DWORD dwStage, D3DTEXTURESTAGESTATETYPE TSS, DWORD dwValue);											// Setzt ein Texturschicht-State
	static Result					SetTSSF(DWORD dwStage, D3DTEXTURESTAGESTATETYPE TSS, float fValue);											// Setzt ein Texturschicht-State
	static DWORD					GetTSS(DWORD dwStage, D3DTEXTURESTAGESTATETYPE TSS);														// Fragt ein Texturschicht-State ab
	static float					GetTSSF(DWORD dwStage, D3DTEXTURESTAGESTATETYPE TSS);														// Fragt ein Texturschicht-State ab
	static Result					SetSS(DWORD dwStage, D3DSAMPLERSTATETYPE SS, DWORD dwValue);												// Setzt ein Sampler-State
	static Result					SetSSF(DWORD dwStage, D3DSAMPLERSTATETYPE SS, float fValue);												// Setzt ein Sampler-State
	static DWORD					GetSS(DWORD dwStage, D3DSAMPLERSTATETYPE SS);																// Fragt ein Sampler-State ab
	static float					GetSSF(DWORD dwStage, D3DSAMPLERSTATETYPE SS);																// Fragt ein Sampler-State ab
	static Result					SetTexture(DWORD dwStage, PDIRECT3DBASETEXTURE9 pTexture);													// Setzt eine Textur
	static PDIRECT3DBASETEXTURE9	GetTexture(DWORD dwStage);																					// Fragt eine Textur ab
	static Result					SetTransform(D3DTRANSFORMSTATETYPE Transform, const Matrix4x4& mMatrix);										// Setzt eine Matrix
	static Matrix4x4				GetTransform(D3DTRANSFORMSTATETYPE Transform);																// Fragt eine Matrix ab
	static Result					SetLight(DWORD dwLight, D3DLIGHT9* pLight);																	// Setzt ein Licht
	static Result					LightEnable(DWORD dwLight, BOOL bEnable);																	// Aktiviert/deaktiviert ein Licht
	static Result					GetLight(DWORD dwLight, D3DLIGHT9* pOut);																	// Fragt ein Licht ab
	static Result					GetLightEnable(DWORD dwLight, BOOL* pbOut);																	// Fragt den Status eines Lichts ab
	static Result					SetMaterial(D3DMATERIAL9* pMaterial);																		// Setzt ein Material
	static D3DMATERIAL9				GetMaterial();																								// Fragt das aktive Material ab
	static Result					SetFVF(DWORD dwFVF);																						// Setzt ein Vertexformat

	// Inline-Methoden
	static inline BOOL				IsInitialized()	{return m_bInitialized;}
	static inline HWND				GetWindow()		{return m_hWindow;}
	static inline BOOL				GetOwnWindow()	{return m_bOwnWindow;}
	static inline PDIRECT3D9		GetD3D()		{return m_pD3D;}
	static inline PDIRECT3DDEVICE9	GetDevice()		{return m_pD3DDevice;}
	static inline DWORD				GetFVF()		{return m_dwFVF;}
	static inline D3DCAPS9			GetCaps()		{return m_Caps;}
	static inline Vector2D			GetScreenSize()	{return m_vScreenSize;}
	static inline float				GetAspect()		{return m_vScreenSize.x / m_vScreenSize.y;}
};

// ******************************************************************