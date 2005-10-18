//#include <TriBase.h>
#include "ageG_D3DRenderStates.h"
#include "ageG_D3DTextureStageStates.h"
#include "ageG_D3DSamplerStates.h"
#include "ageG_Direct3D.h"
#include "AGE.h"
// ******************************************************************
BOOL					ageG_Direct3D::m_bInitialized = FALSE;
HWND					ageG_Direct3D::m_hWindow = NULL;
BOOL					ageG_Direct3D::m_bOwnWindow = FALSE;
PDIRECT3D9				ageG_Direct3D::m_pD3D = NULL;
PDIRECT3DDEVICE9		ageG_Direct3D::m_pD3DDevice = NULL;
D3DCAPS9				ageG_Direct3D::m_Caps;
D3DPRESENT_PARAMETERS	ageG_Direct3D::m_PresentParams;
tbVector2				ageG_Direct3D::m_vScreenSize(0.0f);
tbD3DRS					ageG_Direct3D::m_RS;
tbD3DTSS				ageG_Direct3D::m_TSS;
tbD3DSS					ageG_Direct3D::m_SS;
DWORD					ageG_Direct3D::m_dwFVF = 0;
PDIRECT3DBASETEXTURE9	ageG_Direct3D::m_apTexture[8];
BOOL					ageG_Direct3D::m_abIsRS[205];
BOOL					ageG_Direct3D::m_abIsTSS[33];
BOOL					ageG_Direct3D::m_abIsSS[14];


// ******************************************************************
// Globale Variablen
HWND tb_g_hWindow = NULL;

// ******************************************************************
// Die Nachrichtenfunktion des Fensters
LRESULT WINAPI WindowProc(HWND hWindow,
						  unsigned int uiMessage,
						  WPARAM WParam,
						  LPARAM LParam)
{
	PAINTSTRUCT Paint;

	switch(uiMessage)
	{
	case WM_CREATE:
		break;

	case WM_ACTIVATE:
		// Aktivierungsstatus der Anwendung setzen
		tb_g_bAppActive = LOWORD(WParam) != WA_INACTIVE;
		break;

	case WM_PAINT:
		// Das Fenster mit Schwarz füllen
		BeginPaint(hWindow, &Paint);
		FillRect(Paint.hdc, &Paint.rcPaint, (HBRUSH)(GetStockObject(BLACK_BRUSH)));
		EndPaint(hWindow, &Paint);
		break;

	case WM_CLOSE:
		DestroyWindow(hWindow);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWindow, uiMessage, WParam, LParam);
	}

	return TRUE;
}

// ******************************************************************
// Die Initialisierungsmethode
age_Result ageG_Direct3D::Init(tbConfig* pConfig,
						  char* pcWindowTitle,
						  HWND hOwnWindow,		// = NULL
						  HICON hIcon)			// = NULL
{
	if(hIcon == NULL) hIcon = LoadIcon(tb_g_DLLHandle, MAKEINTRESOURCE(TB_IDI_ICON1));

	HRESULT					hResult;
	D3DPRESENT_PARAMETERS	PresentParams;
	WNDCLASSEX				WindowClass = {sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0, 0,
										   tb_g_DLLHandle, hIcon,
										   NULL, NULL, NULL, "TriBase Direct3D Window Class", NULL};


	// Parameter prüfen
	if(pConfig == NULL) TB_ERROR_NULL_POINTER("pConfig", TB_ERROR);


	memset(m_abIsRS, 255, 205 * sizeof(BOOL));
	memset(m_abIsTSS, 255, 33 * sizeof(BOOL));
	memset(m_abIsSS, 255, 14 * sizeof(BOOL));

	m_abIsRS[0] = FALSE;
	m_abIsRS[1] = FALSE;
	m_abIsRS[2] = FALSE;
	m_abIsRS[3] = FALSE;
	m_abIsRS[4] = FALSE;
	m_abIsRS[5] = FALSE;
	m_abIsRS[6] = FALSE;
	m_abIsRS[10] = FALSE;
	m_abIsRS[11] = FALSE;
	m_abIsRS[12] = FALSE;
	m_abIsRS[13] = FALSE;
	m_abIsRS[17] = FALSE;
	m_abIsRS[18] = FALSE;
	m_abIsRS[21] = FALSE;
	m_abIsRS[30] = FALSE;
	m_abIsRS[31] = FALSE;
	m_abIsRS[32] = FALSE;
	m_abIsRS[33] = FALSE;
	m_abIsRS[39] = FALSE;
	m_abIsRS[40] = FALSE;
	m_abIsRS[41] = FALSE;
	m_abIsRS[42] = FALSE;
	m_abIsRS[43] = FALSE;
	m_abIsRS[44] = FALSE;
	m_abIsRS[45] = FALSE;
	m_abIsRS[46] = FALSE;
	m_abIsRS[49] = FALSE;
	m_abIsRS[50] = FALSE;
	m_abIsRS[51] = FALSE;
	for(DWORD dwRS = 61; dwRS <= 127; dwRS++) m_abIsRS[dwRS] = FALSE;
	m_abIsRS[144] = FALSE;
	m_abIsRS[149] = FALSE;
	m_abIsRS[150] = FALSE;
	m_abIsRS[153] = FALSE;
	m_abIsRS[164] = FALSE;
	m_abIsRS[169] = FALSE;
	m_abIsRS[177] = FALSE;
	m_abIsRS[196] = FALSE;
	m_abIsRS[197] = FALSE;
	
	m_abIsTSS[0] = FALSE;
	m_abIsTSS[12] = FALSE;
	m_abIsTSS[13] = FALSE;
	m_abIsTSS[14] = FALSE;
	m_abIsTSS[15] = FALSE;
	m_abIsTSS[16] = FALSE;
	m_abIsTSS[17] = FALSE;
	m_abIsTSS[18] = FALSE;
	m_abIsTSS[19] = FALSE;
	m_abIsTSS[20] = FALSE;
	m_abIsTSS[21] = FALSE;
	m_abIsTSS[25] = FALSE;
	m_abIsTSS[29] = FALSE;
	m_abIsTSS[30] = FALSE;
	m_abIsTSS[31] = FALSE;

	m_abIsSS[0] = FALSE;

	// Eintragen, ob der Benutzer sein eigenes Fenster hat oder nicht.
	// Wenn ja, das eigene Fenster eintragen.
	m_bOwnWindow = hOwnWindow != NULL;
	if(m_bOwnWindow) m_hWindow = hOwnWindow;

	TB_INFO("Die Direct3D-Komponente wird initialisiert...");

	// Muss ein Fenster erstellt werden?
	if(tb_g_hWindow == NULL)
	{
		if(!m_bOwnWindow)
		{
			TB_INFO("Erstellen des Fensters...");
			
			// Die Fensterklasse registrieren
			if(!RegisterClassEx(&WindowClass))
			{
				// Fehler!
				TB_ERROR("Fehler beim Registrieren der Fensterklasse!", TB_ERROR);
			}

			// Fenster erstellen und prüfen
			m_hWindow = CreateWindow("TriBase Direct3D Window Class",
									pcWindowTitle,
									WS_VISIBLE | WS_OVERLAPPEDWINDOW,
									GetSystemMetrics(SM_CXSCREEN) / 2 - pConfig->Direct3D.VideoMode.Width / 2,
									GetSystemMetrics(SM_CYSCREEN) / 2 - pConfig->Direct3D.VideoMode.Height / 2,
									pConfig->Direct3D.VideoMode.Width,
									pConfig->Direct3D.VideoMode.Height,
									NULL,
									NULL,
									tb_g_DLLHandle,
									NULL);
			if(m_hWindow == NULL)
			{
				// Fehler!
				TB_ERROR("Erstellung des Fensters ist fehlgeschlagen!", TB_ERROR);
			}
		}

		tb_g_hWindow = m_hWindow;
	}
	else m_hWindow = tb_g_hWindow;

	TB_INFO("Initialisierung von Direct3D...");

	// IDirect3D9-Schnittstelle erzeugen
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der IDirect3D9-Schnittstelle! DirectX 9 oder höher wird benötigt!", TB_ERROR);
	}

	// Präsentationsstruktur ausfüllen
	ZeroMemory(&PresentParams, sizeof(D3DPRESENT_PARAMETERS));
	PresentParams.BackBufferWidth				= pConfig->Direct3D.VideoMode.Width;
	PresentParams.BackBufferHeight				= pConfig->Direct3D.VideoMode.Height;
	PresentParams.BackBufferFormat				= pConfig->Direct3D.BackBufferFormat;
	PresentParams.BackBufferCount				= 1;
	PresentParams.MultiSampleType				= pConfig->Direct3D.MultiSamplingType;
	PresentParams.MultiSampleQuality			= pConfig->Direct3D.dwMultiSamplingQuality;
	PresentParams.SwapEffect					= D3DSWAPEFFECT_DISCARD;
	PresentParams.hDeviceWindow					= m_hWindow;
	PresentParams.Windowed						= pConfig->Direct3D.bWindowed;
	PresentParams.EnableAutoDepthStencil		= TRUE;
	PresentParams.AutoDepthStencilFormat		= pConfig->Direct3D.ZStencilBufferFormat;
	PresentParams.Flags							= pConfig->Direct3D.ZStencilBufferFormat != D3DFMT_D16_LOCKABLE ? D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL : 0;
	PresentParams.FullScreen_RefreshRateInHz	= pConfig->Direct3D.bWindowed ? D3DPRESENT_RATE_DEFAULT : pConfig->Direct3D.VideoMode.RefreshRate;
	PresentParams.PresentationInterval			= pConfig->Direct3D.bWindowed ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_ONE;

	// Und nun die Geräteschnittstelle generieren
	if(FAILED(hResult = m_pD3D->CreateDevice(pConfig->Direct3D.iAdapter,
								             pConfig->Direct3D.DeviceType,
											 m_hWindow,
											 pConfig->Direct3D.dwFlags,
											 &PresentParams,
											 &m_pD3DDevice)))
	{
		// Fehler beim Generieren der Schnittstelle!
		TB_ERROR_DIRECTX("m_pD3D->CreateDevice", hResult, TB_ERROR);
	}

	// Parameter eintragen
	m_PresentParams = PresentParams;

	// Die Gerätefähigkeiten abfragen und den aktuellen Status speichern
	m_pD3DDevice->GetDeviceCaps(&m_Caps);
	Capture();

	// Cursor im Vollbildmodus ausblenden
	if(!pConfig->Direct3D.bWindowed) ShowCursor(FALSE);

	// Standardeinstellungen
	SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
	SetRS(D3DRS_DITHERENABLE, TRUE);
	for(DWORD dwStage = 0; dwStage < m_Caps.MaxSimultaneousTextures; dwStage++)
	{
		if(m_Caps.PrimitiveMiscCaps & D3DPTFILTERCAPS_MINFANISOTROPIC) SetSS(dwStage, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
		else SetSS(dwStage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		if(m_Caps.PrimitiveMiscCaps & D3DPTFILTERCAPS_MAGFANISOTROPIC) SetSS(dwStage, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
		else SetSS(dwStage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		SetSS(dwStage, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
		SetSS(dwStage, D3DSAMP_MAXANISOTROPY, m_Caps.MaxAnisotropy);
	}

	m_vScreenSize.x = (float)(PresentParams.BackBufferWidth);
	m_vScreenSize.y = (float)(PresentParams.BackBufferHeight);

	TB_INFO("Die Initialisierung der Direct3D-Komponente ist abgeschlossen!");

	m_bInitialized = TRUE;

	return TB_OK;
}

// ******************************************************************
// Herunterfahren
age_Result ageG_Direct3D::Exit()
{
	m_bInitialized = FALSE;

	if(m_pD3DDevice != NULL)
	{
		// Alle Texturen zurücksetzen
		for(DWORD dwStage = 0; dwStage < m_Caps.MaxSimultaneousTextures; dwStage++)
		{
			// Die Textur deaktivieren, damit ihr Referenzzähler verringert wird.
			// Andernfalls könnten Speicherlecks entstehen.
			m_pD3DDevice->SetTexture(dwStage, NULL);
		}

		// Vertex- und Index-Buffer zurücksetzen
		m_pD3DDevice->SetStreamSource(0, NULL, 0, 0);
		m_pD3DDevice->SetIndices(NULL);
	}

	// Schnittstellen freigeben
	TB_SAFE_RELEASE(m_pD3D);
	TB_SAFE_RELEASE(m_pD3DDevice);

	TB_INFO("Die Direct3D-Komponente wurde heruntergefahren.");

	return TB_OK;
}

// ******************************************************************
// Abfragen des aktuellen Status
age_Result ageG_Direct3D::Capture()
{
	// Render-States abfragen und speichern
	for(DWORD dwRS = 0; dwRS < 195; dwRS++)
	{
		if(m_abIsRS[dwRS])
		{
			m_pD3DDevice->GetRenderState((D3DRENDERSTATETYPE)(dwRS),
				                         &m_RS.adwRS[dwRS]);
		}
	}

	// Für jede Texturschicht...
	for(DWORD dwStage = 0; dwStage < m_Caps.MaxSimultaneousTextures; dwStage++)
	{
		// Texturschicht-States abfragen
		for(DWORD dwTSS = 0; dwTSS < 29; dwTSS++)
		{
			if(m_abIsTSS[dwTSS])
			{
				m_pD3DDevice->GetTextureStageState(dwStage, (D3DTEXTURESTAGESTATETYPE)(dwTSS),
					                               &m_TSS.adwTSS[dwStage][dwTSS]);
			}
		}

		// Sampler-States abfragen
		for(DWORD dwSS = 0; dwSS < 14; dwSS++)
		{
			if(m_abIsSS[dwSS])
			{
				m_pD3DDevice->GetSamplerState(dwStage, (D3DSAMPLERSTATETYPE)(dwSS),
					                          &m_SS.adwSS[dwStage][dwSS]);
			}
		}

		// Textur abfragen
		m_pD3DDevice->GetTexture(dwStage, &m_apTexture[dwStage]);
	}

	// Vertexformat abfragen
	m_pD3DDevice->GetFVF(&m_dwFVF);

	return TB_OK;
}

// ******************************************************************
// Leert die Puffer
age_Result ageG_Direct3D::Clear(DWORD dwNumRects,
						   const D3DRECT* pRects,
						   DWORD dwFlags,
						   D3DCOLOR Color,
						   float z,
						   DWORD dwStencil)
{
	// Parameter prüfen
	if(dwNumRects != 0 && pRects == NULL)	TB_ERROR_NULL_POINTER("pRects", TB_ERROR);
	if(z < 0.0f || z > 1.0f)				TB_ERROR_INVALID_VALUE("z", TB_ERROR);


	// Direct3D-Methode aufrufen
	HRESULT hResult = m_pD3DDevice->Clear(dwNumRects, pRects, dwFlags, Color, z, dwStencil);
	if(FAILED(hResult)) TB_ERROR_DIRECTX("m_pD3DDevice->Clear", hResult, TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Beginnt die Szene
age_Result ageG_Direct3D::BeginScene()
{
	HRESULT hResult = m_pD3DDevice->BeginScene();
	if(FAILED(hResult)) TB_ERROR_DIRECTX("m_pD3DDevice->BeginScene", hResult, TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Beendet die Szene
age_Result ageG_Direct3D::EndScene()
{
	HRESULT hResult = m_pD3DDevice->EndScene();
	if(FAILED(hResult)) TB_ERROR_DIRECTX("m_pD3DDevice->EndScene", hResult, TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Zeigt den Bildpuffer an
age_Result ageG_Direct3D::Present()
{
	HRESULT hResult = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	if(FAILED(hResult)) TB_ERROR_DIRECTX("m_pD3DDevice->Present", hResult, TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Setzen eines Render-States (Wert: DWORD)

// ******************************************************************
// Setzen eines Render-States (Wert: float)

// ******************************************************************
// Abfragen eines Render-States (Wert: DWORD)

// ******************************************************************
// Abfragen eines Render-States (Wert: float)

// ******************************************************************
// Setzen eines Texturschicht-States (Wert: DWORD)

// ******************************************************************
// Setzen eines Texturschicht-States (Wert: float)

// ******************************************************************
// Abfragen eines Texturschicht-States (Wert: DWORD)

// ******************************************************************
// Abfragen eines Texturschicht-States (Wert: float)

// ******************************************************************
// Setzen eines Sampler-States (Wert: DWORD)

// ******************************************************************
// Setzen eines Sampler-States (Wert: float)

// ******************************************************************
// Abfragen eines Sampler-States (Wert: DWORD)

// ******************************************************************
// Abfragen eines Sampler-States (Wert: float)

// ******************************************************************
// Setzen einer Textur
age_Result ageG_Direct3D::SetTexture(DWORD dwStage,
								PDIRECT3DBASETEXTURE9 pTexture)
{
	// In der Debug-Version wird geprüft, ob dwStage in Ordnung ist.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8)
	{
		// Texturschicht ist nicht gültig!
		TB_ERROR("Ungültige Texturschicht!", TB_ERROR);
	}
#endif

	// Prüfen, ob die Textur vielleicht schon gesetzt wurde.
	// Falls ja, wird direkt abgebrochen.
	if(m_apTexture[dwStage] == pTexture) return TB_OK;
	else
	{
		// Die Textur setzen und die Tabelle aktualisieren
		m_pD3DDevice->SetTexture(dwStage, pTexture);
		m_apTexture[dwStage] = pTexture;
	}

	return TB_OK;
}

// ******************************************************************
// Abfragen einer Textur
PDIRECT3DBASETEXTURE9 ageG_Direct3D::GetTexture(DWORD dwStage)
{
	// In der Debug-Version wird geprüft, ob dwStage in Ordnung ist.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8)
	{
		// Texturschicht ist nicht gültig!
		TB_ERROR("Ungültige Texturschicht!", NULL);
	}
#endif

	// Auf die Tabelle zugreifen
	return m_apTexture[dwStage];
}

// ******************************************************************
// Setzen einer Matrix
tbResult ageG_Direct3D::SetTransform(D3DTRANSFORMSTATETYPE Transform,
								  const tbMatrix& mMatrix)
{
	// Matrix setzen
	return FAILED(m_pD3DDevice->SetTransform(Transform, (D3DMATRIX*)(&mMatrix))) ? TB_ERROR : TB_OK;
}

// ******************************************************************
// Abfragen einer Matrix
tbMatrix ageG_Direct3D::GetTransform(D3DTRANSFORMSTATETYPE Transform)
{
	tbMatrix mTemp;

	// Matrix abfragen
	m_pD3DDevice->GetTransform(Transform, (D3DMATRIX*)(&mTemp));

	return mTemp;
}

// ******************************************************************
// Setzen eines Lichts
age_Result ageG_Direct3D::SetLight(DWORD dwLight,
							  D3DLIGHT9* pLight)
{
	// Parameter prüfen
	if(pLight == NULL) TB_ERROR_NULL_POINTER("pLight", TB_ERROR);


	HRESULT hResult = m_pD3DDevice->SetLight(dwLight, pLight);
	if(FAILED(hResult)) TB_ERROR_DIRECTX("m_pD3DDevice->SetLight", hResult, TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Setzt den Status eines Lichts
age_Result ageG_Direct3D::LightEnable(DWORD dwLight,
								 BOOL bEnable)
{
	HRESULT hResult = m_pD3DDevice->LightEnable(dwLight, bEnable);
	if(FAILED(hResult)) TB_ERROR_DIRECTX("m_pD3DDevice->LightEnable", hResult, TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Fragt ein Licht ab
age_Result ageG_Direct3D::GetLight(DWORD dwLight,
							  D3DLIGHT9* pOut)
{
	// Parameter prüfen
	if(pOut == NULL) TB_ERROR_NULL_POINTER("pOut", TB_ERROR);


	HRESULT hResult = m_pD3DDevice->GetLight(dwLight, pOut);
	if(FAILED(hResult)) TB_ERROR_DIRECTX("m_pD3DDevice->GetLight", hResult, TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Fragt den Status eines Lichts ab
age_Result ageG_Direct3D::GetLightEnable(DWORD dwLight,
									BOOL* pbOut)
{
	// Parameter prüfen
	if(pbOut == NULL) TB_ERROR_NULL_POINTER("pbOut", TB_ERROR);


	HRESULT hResult = m_pD3DDevice->GetLightEnable(dwLight, pbOut);
	if(FAILED(hResult)) TB_ERROR_DIRECTX("m_pD3DDevice->GetLightEnable", hResult, TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Setzt ein Material
age_Result ageG_Direct3D::SetMaterial(D3DMATERIAL9* pMaterial)
{
	// Parameter prüfen
	if(pMaterial == NULL) TB_ERROR_NULL_POINTER("pMaterial", TB_ERROR);


	HRESULT hResult = m_pD3DDevice->SetMaterial(pMaterial);
	if(FAILED(hResult)) TB_ERROR_DIRECTX("m_pD3DDevice->SetMaterial", hResult, TB_ERROR);

	return TB_OK;
}

// ******************************************************************
// Fragt das aktive Material ab
D3DMATERIAL9 ageG_Direct3D::GetMaterial()
{
	D3DMATERIAL9 Temp;

	m_pD3DDevice->GetMaterial(&Temp);

	return Temp;
}

// ******************************************************************
// Setzen eines Vertexformats
age_Result ageG_Direct3D::SetFVF(DWORD dwFVF)
{
	// Vertexformat setzen
	HRESULT r = m_pD3DDevice->SetFVF(dwFVF);
	if(FAILED(r))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pD3DDevice->SetFVF", r, TB_ERROR);
	}

	return TB_OK;
}

// ******************************************************************