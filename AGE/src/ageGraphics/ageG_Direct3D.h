class ageG_D3DRenderStates;

class ageG_D3DTextureStageStates;

class ageG_D3DSamplerStates;


// ******************************************************************
// Die tbDirect3D-Klasse
#include "AGE.h"

class ageG_Direct3D
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
	static ageG_Vector2				m_vScreenSize;

	static ageG_D3DRenderStates		    m_RS;	// Render-States
	static ageG_D3DTextureStageStates	m_TSS;	// Texturschicht-States
	static ageG_D3DSamplerStates        m_SS;	// Sampler-States

	static DWORD					m_dwFVF;		// Aktuelles Vertexformat
	static PDIRECT3DBASETEXTURE9	m_apTexture[8];	// Texturtabelle
	static BOOL						m_abIsRS[205];
	static BOOL						m_abIsTSS[33];
	static BOOL						m_abIsSS[14];

public:
	// Methoden
	static age_Result Init(tbConfig* pConfig, char* pcWindowTitle, HWND hOwnWindow = NULL, HICON hIcon = NULL);					// Fenster und Direct3D initialisieren
	static age_Result Exit();																										// Herunterfahren
	static age_Result Capture();																									// Aktualisiert die internen Tabellen und Pufferwerte
	static age_Result Clear(DWORD dwNumRects, const D3DRECT* pRects, DWORD dwFlags, D3DCOLOR Color, float z, DWORD dwStencil);	// Leert die Puffer
	static age_Result BeginScene();																								// Beginnt die Szene
	static age_Result EndScene();																									// Beendet die Szene
	static age_Result Present();																									// Zeigt den Bildpuffer an

	static age_Result SetTexture(DWORD dwStage, PDIRECT3DBASETEXTURE9 pTexture);													// Setzt eine Textur
	static PDIRECT3DBASETEXTURE9	GetTexture(DWORD dwStage);																					// Fragt eine Textur ab
	static age_Result SetTransform(D3DTRANSFORMSTATETYPE Transform, const ageG_Matrix& mMatrix);										// Setzt eine Matrix
	static ageG_Matrix					GetTransform(D3DTRANSFORMSTATETYPE Transform);																// Fragt eine Matrix ab
	static age_Result SetLight(DWORD dwLight, D3DLIGHT9* pLight);																	// Setzt ein Licht
	static age_Result LightEnable(DWORD dwLight, BOOL bEnable);																	// Aktiviert/deaktiviert ein Licht
	static age_Result GetLight(DWORD dwLight, D3DLIGHT9* pOut);																	// Fragt ein Licht ab
	static age_Result GetLightEnable(DWORD dwLight, BOOL* pbOut);																	// Fragt den Status eines Lichts ab
	static age_Result SetMaterial(D3DMATERIAL9* pMaterial);																		// Setzt ein Material
	static D3DMATERIAL9				GetMaterial();																								// Fragt das aktive Material ab
	static age_Result SetFVF(DWORD dwFVF);																						// Setzt ein Vertexformat

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