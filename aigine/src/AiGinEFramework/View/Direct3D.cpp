
#include "../AiGinE.h"

// ******************************************************************
BOOL					Direct3D::m_bInitialized = FALSE;
HWND					Direct3D::m_hWindow = NULL;
BOOL					Direct3D::m_bOwnWindow = FALSE;
PDIRECT3D9				Direct3D::m_pD3D = NULL;
PDIRECT3DDEVICE9		Direct3D::m_pD3DDevice = NULL;
D3DCAPS9				Direct3D::m_Caps;
D3DPRESENT_PARAMETERS	Direct3D::m_PresentParams;
Vector2D				Direct3D::m_vScreenSize(0.0f);
D3DRS					Direct3D::m_RS;
D3DTSS					Direct3D::m_TSS;
D3DSS					Direct3D::m_SS;
DWORD					Direct3D::m_dwFVF = 0;
PDIRECT3DBASETEXTURE9	Direct3D::m_apTexture[8];
BOOL					Direct3D::m_abIsRS[205];
BOOL					Direct3D::m_abIsTSS[33];
BOOL					Direct3D::m_abIsSS[14];


// ******************************************************************
// Globale Variablen
HWND g_hWindow = NULL;

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
		g_bAppActive = LOWORD(WParam) != WA_INACTIVE;
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
Result Direct3D::Init(Config* pConfig,
						  char* pcWindowTitle,
						  HWND hOwnWindow,		// = NULL
						  HICON hIcon)			// = NULL
{
	if(hIcon == NULL) hIcon = LoadIcon(g_DLLHandle, MAKEINTRESOURCE(IDI_ICON1));

	HRESULT					hResult;
	D3DPRESENT_PARAMETERS	PresentParams;
	WNDCLASSEX				WindowClass = {sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0, 0,
										   g_DLLHandle, hIcon,
										   NULL, NULL, NULL, "TriBase Direct3D Window Class", NULL};


	// Parameter prüfen
	if(pConfig == NULL) ERROR_NULL_POINTER("pConfig", ERROR);


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

	INFO("Die Direct3D-Komponente wird initialisiert...");

	// Muss ein Fenster erstellt werden?
	if(g_hWindow == NULL)
	{
		if(!m_bOwnWindow)
		{
			INFO("Erstellen des Fensters...");
			
			// Die Fensterklasse registrieren
			if(!RegisterClassEx(&WindowClass))
			{
				// Fehler!
				ERROR("Fehler beim Registrieren der Fensterklasse!", ERROR);
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
									g_DLLHandle,
									NULL);
			if(m_hWindow == NULL)
			{
				// Fehler!
				ERROR("Erstellung des Fensters ist fehlgeschlagen!", ERROR);
			}
		}

		g_hWindow = m_hWindow;
	}
	else m_hWindow = g_hWindow;

	INFO("Initialisierung von Direct3D...");

	// IDirect3D9-Schnittstelle erzeugen
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(m_pD3D == NULL)
	{
		// Fehler!
		ERROR("Fehler beim Erstellen der IDirect3D9-Schnittstelle! DirectX 9 oder höher wird benötigt!", ERROR);
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
		ERROR_DIRECTX("m_pD3D->CreateDevice", hResult, ERROR);
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

	INFO("Die Initialisierung der Direct3D-Komponente ist abgeschlossen!");

	m_bInitialized = TRUE;

	return OK;
}

// ******************************************************************
// Herunterfahren
Result Direct3D::Exit()
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
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pD3DDevice);

	INFO("Die Direct3D-Komponente wurde heruntergefahren.");

	return OK;
}

// ******************************************************************
// Abfragen des aktuellen Status
Result Direct3D::Capture()
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

	return OK;
}

// ******************************************************************
// Leert die Puffer
Result Direct3D::Clear(DWORD dwNumRects,
						   const D3DRECT* pRects,
						   DWORD dwFlags,
						   D3DCOLOR Color,
						   float z,
						   DWORD dwStencil)
{
	// Parameter prüfen
	if(dwNumRects != 0 && pRects == NULL)	ERROR_NULL_POINTER("pRects", ERROR);
	if(z < 0.0f || z > 1.0f)				ERROR_INVALID_VALUE("z", ERROR);


	// Direct3D-Methode aufrufen
	HRESULT hResult = m_pD3DDevice->Clear(dwNumRects, pRects, dwFlags, Color, z, dwStencil);
	if(FAILED(hResult)) ERROR_DIRECTX("m_pD3DDevice->Clear", hResult, ERROR);

	return OK;
}

// ******************************************************************
// Beginnt die Szene
Result Direct3D::BeginScene()
{
	HRESULT hResult = m_pD3DDevice->BeginScene();
	if(FAILED(hResult)) ERROR_DIRECTX("m_pD3DDevice->BeginScene", hResult, ERROR);

	return OK;
}

// ******************************************************************
// Beendet die Szene
Result Direct3D::EndScene()
{
	HRESULT hResult = m_pD3DDevice->EndScene();
	if(FAILED(hResult)) ERROR_DIRECTX("m_pD3DDevice->EndScene", hResult, ERROR);

	return OK;
}

// ******************************************************************
// Zeigt den Bildpuffer an
Result Direct3D::Present()
{
	HRESULT hResult = m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	if(FAILED(hResult)) ERROR_DIRECTX("m_pD3DDevice->Present", hResult, ERROR);

	return OK;
}

// ******************************************************************
// Setzen eines Render-States (Wert: DWORD)
Result Direct3D::SetRS(D3DRENDERSTATETYPE RS,
						   DWORD dwValue)
{
	// In der Debug-Version wird geprüft, ob RS in Ordnung ist.
#ifdef _DEBUG
	if(RS < 0 || RS > 204)
	{
		// Render-State ist nicht gültig!
		ERROR("Ungültiges Render-State!", ERROR);
	}
#endif

	// Wenn das Render-State schon gesetzt ist, direkt abbrechen
	if(m_RS.adwRS[RS] == dwValue) return OK;
	else
	{
		// Render-State setzen und in die Tabelle schreiben
		m_pD3DDevice->SetRenderState(RS, dwValue);
		m_RS.adwRS[RS] = dwValue;
	}

	return OK;
}

// ******************************************************************
// Setzen eines Render-States (Wert: float)
Result Direct3D::SetRSF(D3DRENDERSTATETYPE RS,
							float fValue)
{
	// In der Debug-Version wird geprüft, ob RS in Ordnung ist.
#ifdef _DEBUG
	if(RS < 0 || RS > 204)
	{
		// Render-State ist nicht gültig!
		ERROR("Ungültiges Render-State!", ERROR);
	}
#endif

	// Wenn das Render-State schon gesetzt ist, direkt abbrechen
	if(m_RS.afRS[RS] == fValue) return OK;
	else
	{
		// Render-State setzen und in die Tabelle schreiben
		m_pD3DDevice->SetRenderState(RS, *((DWORD*)(&fValue)));
		m_RS.afRS[RS] = fValue;
	}

	return OK;
}

// ******************************************************************
// Abfragen eines Render-States (Wert: DWORD)
DWORD Direct3D::GetRS(D3DRENDERSTATETYPE RS)
{
	// In der Debug-Version wird geprüft, ob RS in Ordnung ist.
#ifdef _DEBUG
	if(RS < 0 || RS > 204)
	{
		// Render-State ist nicht gültig!
		ERROR("Ungültiges Render-State!", 0xEEEEEEEE);
	}
#endif

	// Auf die Tabelle zugreifen
	return m_RS.adwRS[RS];
}

// ******************************************************************
// Abfragen eines Render-States (Wert: float)
float Direct3D::GetRSF(D3DRENDERSTATETYPE RS)
{
	// In der Debug-Version wird geprüft, ob RS in Ordnung ist.
#ifdef _DEBUG
	if(RS < 0 || RS > 204)
	{
		// Render-State ist nicht gültig!
		ERROR("Ungültiges Render-State!", -10000000.0f);
	}
#endif

	// Auf die Tabelle zugreifen
	return m_RS.afRS[RS];
}

// ******************************************************************
// Setzen eines Texturschicht-States (Wert: DWORD)
Result Direct3D::SetTSS(DWORD dwStage,
							D3DTEXTURESTAGESTATETYPE TSS,
							DWORD dwValue)
{
	// In der Debug-Version wird geprüft, ob dwStage und TSS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   TSS < 0 || TSS > 32)
	{
		// Texturschicht-State ist nicht gültig!
		ERROR("Ungültiges Texturschicht-State!", ERROR);
	}
#endif

	// Wenn das Texturschicht-State schon gesetzt ist, direkt abbrechen
	if(m_TSS.adwTSS[dwStage][TSS] == dwValue) return OK;
	else
	{
		// Texturschicht-State setzen und in die Tabelle schreiben
		m_pD3DDevice->SetTextureStageState(dwStage, TSS, dwValue);
		m_TSS.adwTSS[dwStage][TSS] = dwValue;
	}

	return OK;
}

// ******************************************************************
// Setzen eines Texturschicht-States (Wert: float)
Result Direct3D::SetTSSF(DWORD dwStage,
							 D3DTEXTURESTAGESTATETYPE TSS,
							 float fValue)
{
	// In der Debug-Version wird geprüft, ob dwStage und TSS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   TSS < 0 || TSS > 32)
	{
		// Texturschicht-State ist nicht gültig!
		ERROR("Ungültiges Texturschicht-State!", ERROR);
	}
#endif

	// Wenn das Texturschicht-State schon gesetzt ist, direkt abbrechen
	if(m_TSS.afTSS[dwStage][TSS] == fValue) return OK;
	else
	{
		// Texturschicht-State setzen und in die Tabelle schreiben
		m_pD3DDevice->SetTextureStageState(dwStage, TSS, *((DWORD*)(&fValue)));
		m_TSS.afTSS[dwStage][TSS] = fValue;
	}

	return OK;
}

// ******************************************************************
// Abfragen eines Texturschicht-States (Wert: DWORD)
DWORD Direct3D::GetTSS(DWORD dwStage,
						 D3DTEXTURESTAGESTATETYPE TSS)
{
	// In der Debug-Version wird geprüft, ob dwStage und TSS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   TSS < 0 || TSS > 32)
	{
		// Texturschicht-State ist nicht gültig!
		ERROR("Ungültiges Texturschicht-State!", 0xEEEEEEEE);
	}
#endif

	// Auf die Tabelle zugreifen
	return m_TSS.adwTSS[dwStage][TSS];
}

// ******************************************************************
// Abfragen eines Texturschicht-States (Wert: float)
float Direct3D::GetTSSF(DWORD dwStage,
						  D3DTEXTURESTAGESTATETYPE TSS)
{
	// In der Debug-Version wird geprüft, ob dwStage und TSS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   TSS < 0 || TSS > 32)
	{
		// Texturschicht-State ist nicht gültig!
		ERROR("Ungültiges Texturschicht-State!", -10000000.0f);
	}
#endif

	// Auf die Tabelle zugreifen
	return m_TSS.afTSS[dwStage][TSS];
}

// ******************************************************************
// Setzen eines Sampler-States (Wert: DWORD)
Result Direct3D::SetSS(DWORD dwStage,
						   D3DSAMPLERSTATETYPE SS,
						   DWORD dwValue)
{
	// In der Debug-Version wird geprüft, ob dwStage und SS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   SS < 0 || SS > 13)
	{
		// Sampler-State ist nicht gültig!
		ERROR("Ungültiges Sampler-State!", ERROR);
	}
#endif

	// Wenn das Sampler-State schon gesetzt ist, direkt abbrechen
	if(m_SS.adwSS[dwStage][SS] == dwValue) return OK;
	else
	{
		// Sampler-State setzen und in die Tabelle schreiben
		m_pD3DDevice->SetSamplerState(dwStage, SS, dwValue);
		m_SS.adwSS[dwStage][SS] = dwValue;
	}

	return OK;
}

// ******************************************************************
// Setzen eines Sampler-States (Wert: float)
Result Direct3D::SetSSF(DWORD dwStage,
							D3DSAMPLERSTATETYPE SS,
							float fValue)
{
	// In der Debug-Version wird geprüft, ob dwStage und SS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   SS < 0 || SS > 13)
	{
		// Sampler-State ist nicht gültig!
		ERROR("Ungültiges Sampler-State!", ERROR);
	}
#endif

	// Wenn das Sampler-State schon gesetzt ist, direkt abbrechen
	if(m_SS.afSS[dwStage][SS] == fValue) return OK;
	else
	{
		// Sampler-State setzen und in die Tabelle schreiben
		m_pD3DDevice->SetSamplerState(dwStage, SS, *((DWORD*)(&fValue)));
		m_SS.afSS[dwStage][SS] = fValue;
	}

	return OK;
}

// ******************************************************************
// Abfragen eines Sampler-States (Wert: DWORD)
DWORD Direct3D::GetSS(DWORD dwStage,
						D3DSAMPLERSTATETYPE SS)
{
	// In der Debug-Version wird geprüft, ob dwStage und SS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   SS < 0 || SS > 13)
	{
		// Sampler-State ist nicht gültig!
		ERROR("Ungültiges Sampler-State!", 0xEEEEEEEE);
	}
#endif

	// Auf die Tabelle zugreifen
	return m_SS.adwSS[dwStage][SS];
}

// ******************************************************************
// Abfragen eines Sampler-States (Wert: float)
float Direct3D::GetSSF(DWORD dwStage,
						 D3DSAMPLERSTATETYPE SS)
{
	// In der Debug-Version wird geprüft, ob dwStage und SS in Ordnung sind.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8 ||
	   SS < 0 || SS > 13)
	{
		// Sampler-State ist nicht gültig!
		ERROR("Ungültiges Sampler-State!", -10000000.0f);
	}
#endif

	// Auf die Tabelle zugreifen
	return m_SS.afSS[dwStage][SS];
}

// ******************************************************************
// Setzen einer Textur
Result Direct3D::SetTexture(DWORD dwStage,
								PDIRECT3DBASETEXTURE9 pTexture)
{
	// In der Debug-Version wird geprüft, ob dwStage in Ordnung ist.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8)
	{
		// Texturschicht ist nicht gültig!
		ERROR("Ungültige Texturschicht!", ERROR);
	}
#endif

	// Prüfen, ob die Textur vielleicht schon gesetzt wurde.
	// Falls ja, wird direkt abgebrochen.
	if(m_apTexture[dwStage] == pTexture) return OK;
	else
	{
		// Die Textur setzen und die Tabelle aktualisieren
		m_pD3DDevice->SetTexture(dwStage, pTexture);
		m_apTexture[dwStage] = pTexture;
	}

	return OK;
}

// ******************************************************************
// Abfragen einer Textur
PDIRECT3DBASETEXTURE9 Direct3D::GetTexture(DWORD dwStage)
{
	// In der Debug-Version wird geprüft, ob dwStage in Ordnung ist.
#ifdef _DEBUG
	if(dwStage < 0 || dwStage > 8)
	{
		// Texturschicht ist nicht gültig!
		ERROR("Ungültige Texturschicht!", NULL);
	}
#endif

	// Auf die Tabelle zugreifen
	return m_apTexture[dwStage];
}

// ******************************************************************
// Setzen einer Matrix
Result Direct3D::SetTransform(D3DTRANSFORMSTATETYPE Transform,
								  const Matrix4x4& mMatrix)
{
	// Matrix setzen
	return FAILED(m_pD3DDevice->SetTransform(Transform, (D3DMATRIX*)(&mMatrix))) ? ERROR : OK;
}

// ******************************************************************
// Abfragen einer Matrix
Matrix4x4 Direct3D::GetTransform(D3DTRANSFORMSTATETYPE Transform)
{
	Matrix4x4 mTemp;

	// Matrix abfragen
	m_pD3DDevice->GetTransform(Transform, (D3DMATRIX*)(&mTemp));

	return mTemp;
}

// ******************************************************************
// Setzen eines Lichts
Result Direct3D::SetLight(DWORD dwLight,
							  D3DLIGHT9* pLight)
{
	// Parameter prüfen
	if(pLight == NULL) ERROR_NULL_POINTER("pLight", ERROR);


	HRESULT hResult = m_pD3DDevice->SetLight(dwLight, pLight);
	if(FAILED(hResult)) ERROR_DIRECTX("m_pD3DDevice->SetLight", hResult, ERROR);

	return OK;
}

// ******************************************************************
// Setzt den Status eines Lichts
Result Direct3D::LightEnable(DWORD dwLight,
								 BOOL bEnable)
{
	HRESULT hResult = m_pD3DDevice->LightEnable(dwLight, bEnable);
	if(FAILED(hResult)) ERROR_DIRECTX("m_pD3DDevice->LightEnable", hResult, ERROR);

	return OK;
}

// ******************************************************************
// Fragt ein Licht ab
Result Direct3D::GetLight(DWORD dwLight,
							  D3DLIGHT9* pOut)
{
	// Parameter prüfen
	if(pOut == NULL) ERROR_NULL_POINTER("pOut", ERROR);


	HRESULT hResult = m_pD3DDevice->GetLight(dwLight, pOut);
	if(FAILED(hResult)) ERROR_DIRECTX("m_pD3DDevice->GetLight", hResult, ERROR);

	return OK;
}

// ******************************************************************
// Fragt den Status eines Lichts ab
Result Direct3D::GetLightEnable(DWORD dwLight,
									BOOL* pbOut)
{
	// Parameter prüfen
	if(pbOut == NULL) ERROR_NULL_POINTER("pbOut", ERROR);


	HRESULT hResult = m_pD3DDevice->GetLightEnable(dwLight, pbOut);
	if(FAILED(hResult)) ERROR_DIRECTX("m_pD3DDevice->GetLightEnable", hResult, ERROR);

	return OK;
}

// ******************************************************************
// Setzt ein Material
Result Direct3D::SetMaterial(D3DMATERIAL9* pMaterial)
{
	// Parameter prüfen
	if(pMaterial == NULL) ERROR_NULL_POINTER("pMaterial", ERROR);


	HRESULT hResult = m_pD3DDevice->SetMaterial(pMaterial);
	if(FAILED(hResult)) ERROR_DIRECTX("m_pD3DDevice->SetMaterial", hResult, ERROR);

	return OK;
}

// ******************************************************************
// Fragt das aktive Material ab
D3DMATERIAL9 Direct3D::GetMaterial()
{
	D3DMATERIAL9 Temp;

	m_pD3DDevice->GetMaterial(&Temp);

	return Temp;
}

// ******************************************************************
// Setzen eines Vertexformats
Result Direct3D::SetFVF(DWORD dwFVF)
{
	// Vertexformat setzen
	HRESULT r = m_pD3DDevice->SetFVF(dwFVF);
	if(FAILED(r))
	{
		// Fehler!
		ERROR_DIRECTX("m_pD3DDevice->SetFVF", r, ERROR);
	}

	return OK;
}

// ******************************************************************