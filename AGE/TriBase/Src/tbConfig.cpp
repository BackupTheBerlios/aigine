/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbConfig.cpp
	============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Abfragen der Engine-Einstellungen

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Beschreibungstexte
#define TB_D3DC_ADAPTER_DESCRIPTION					("Adapter:\r\n\r\nWählen Sie aus der Liste den Adapter aus, mit dem das Programm arbeiten soll. Ein Adapter ist das gleiche wie eine Grafikkarte oder ein Grafikchip. Im Normalfall ist nur ein einziger Adapter auf Ihrem System installiert.")
#define TB_D3DC_DEVICE_TYPE_DESCRIPTION				("Gerätetyp:\r\n\r\nDer Gerätetyp bestimmt, ob hardwarebeschleunigte Grafikausgabe zum Einsatz kommt (empfohlen). Eventuell gibt es noch weitere Möglichkeiten, die Sie nur dann in Erwägung ziehen sollten, wenn die Hardwarebeschleunigung nicht funktioniert.")
#define TB_D3DC_VIDEO_MODE_DESCRIPTION				("Videomodus:\r\n\r\nDer Videomodus bestimmt die Auflösung des Monitors im Vollbildmodus oder die Größe des Fensters im Fenstermodus. Je höher die Auflösung, desto mehr Details sind erkennbar und desto langsamer läuft die Anwendung. 800 x 600 Pixel ist ein guter Wert.\r\nAchten Sie auch auf die eventuell angegebene Bildwiederholfrequenz! Je höher, desto flimmerfreier ist das Bild. Im Fenstermodus spielen Format und Bildwiederholfrequenz keine Rolle - sie werden vom aktuellen Videomodus übernommen.")
#define TB_D3DC_WINDOWED_FULLSCREEN_DESCRIPTION		("Fenster-/Vollbildmodus:\r\n\r\nWählen Sie \"Fenstermodus\", damit das Programm in einem Fenster läuft. Sie können so einfacher zwischen Anwendungen wechseln. Der Vollbildmodus überlässt der Anwendung mehr Möglichkeiten und kann zu einem Geschwindigkeitsgewinn führen.")
#define TB_D3DC_BACK_BUFFER_FORMAT_DESCRIPTION		("Bildpufferformat:\r\n\r\nDas Bildpufferformat bestimmt, in welchem Format die Bilddaten gespeichert werden. Je größer die Zahlen, desto vollständiger ist das Farbspektrum abgedeckt.")
#define TB_D3DC_Z_STENCIL_BUFFER_FORMAT_DESCRIPTION	("Z-Stencil-Buffer-Format:\r\n\r\nWählen Sie hier das gewünschte Z-Stencil-Buffer-Format aus. Ein \"S\" im Formatnamen besagt, dass in diesem Format Platz für einen Stencil-Buffer ist, der für zusätzliche Grafikeffekte verwendet werden kann.\r\nJe höher die Zahl hinter dem \"D\", desto genauer kann die Tiefe eines 3D-Objekts gespeichert werden, was zu besserer Grafik führt.\r\nDie Zahl hinter dem \"S\" steht für die Anzahl der Bits für den Stencil-Buffer.")
#define TB_D3DC_MULTI_SAMPLING_TYPE_DESCRIPTION		("Multi-Sampling-Typ:\r\n\r\nDer Multi-Sampling-Typ bestimmt, in wie vielen Durchgängen jedes Bild geglättet wird. Je höher diese Anzahl ist, desto weniger Kanten erscheinen im fertigen Bild und desto langsamer läuft die Grafikausgabe.")
#define TB_D3DC_MULTI_SAMPLING_QUALITY_DESCRIPTION	("Multi-Sampling-Qualität:\r\n\r\nManche Adapter unterstützen Multi-Sampling in verschiedenen Qualitätsstufen. Je höher die Qualität, desto langsamer ist das Multi-Sampling und desto besser ist natürlich das Ergebnis.")
#define TB_D3DC_TNL_DESCRIPTION						("Transformation und Beleuchtung:\r\n\r\nViele Grafikkarten können selbst einen Großteil der Rechenarbeit erledigen, die vor dem eigentlichen Zeichnen des Bildes anfällt - Transformation und Beleuchtung.\r\nBei \"Hardware\" übernimmt die Hardware alle Operationen. Wird eine nicht unterstützt, ist sie nicht verfügbar.\r\nWählen Sie \"Software\", um alles per Software erledigen zu lassen, was natürlich langsamer ist.")
#define TB_DSC_DRIVERS_DESCRIPTION					("Soundtreiber:\r\n\r\nJedes Gerät, das Sound ausgeben kann (Soundkarte, Soundchip auf dem Mainboard) besitzt einen oder mehrere Treiber. \"Primary Sound Driver\" ist meist die beste Wahl.")
#define TB_DSC_BITS_PER_SAMPLE_DESCRIPTION			("Bits pro Sample:\r\n\r\nJe mehr Bits für ein Sample eines Sounds verwendet werden, desto besser ist die Soundqualität. CD-Qualität ist 16 Bits.")
#define TB_DSC_SAMPLING_FREQUENCY_DESCRIPTION		("Sampling-Frequenz:\r\n\r\nJe höher die Sampling-Frequenz ist, desto genauer können Sounds wiedergegeben werden. CD-Qualität ist 44100 Hz - das bedeutet, dass 44100 Samples pro Sekunde abgespielt werden.")

// ******************************************************************
// Globale Variablen
PDIRECT3D9	g_pD3DTemp;		// Temporäre IDirect3D9-Schnittstelle
tbConfig*	g_pConfig;		// Auszufüllende Konfigurationsstruktur
tbConfig	g_TempConfig;	// Temporäre Konfigurationsstruktur

//tbClient*	myClient;
//HWND mein_clientdialog = 0;

// ******************************************************************
// Funktionsdeklarationen
INT_PTR CALLBACK D3DEnumDialogProc(HWND hDlg, unsigned int uiMsg, WPARAM WParam, LPARAM LParam);

// ******************************************************************
// Diese Funktion löscht die Daten einer ganzen Liste.
tbResult DeleteListData(HWND hDlg,
						int iDialogItem)
{
	int		iNumElements;
	void*	pElementData;


	// Anzahl der Listenelemente abfragen
	iNumElements = (int)(SendDlgItemMessage(hDlg, iDialogItem, CB_GETCOUNT, 0, 0));
	if(iNumElements == CB_ERR) return TB_ERROR;

	// Alle Elemente durchgehen
	for(int iElement = 0; iElement < iNumElements; iElement++)
	{
		// Daten des Elements abfragen und freigeben
		pElementData = (void*)(SendDlgItemMessage(hDlg, iDialogItem, CB_GETITEMDATA, iElement, 0));
		if(pElementData != (void*)(CB_ERR)) TB_SAFE_MEMFREE(pElementData);
	}

	// Liste leeren
	SendDlgItemMessage(hDlg, iDialogItem, CB_RESETCONTENT, 0, 0);

	return TB_OK;
}

// ******************************************************************
// Funktion zum Abzählen aller Adapter
tbResult EnumAdapters(HWND hDlg)
{
	int						iNumAdapters;
	D3DADAPTER_IDENTIFIER9	AdapterInfo;

	
	// Liste löschen
	DeleteListData(hDlg, TB_IDC_D3DC_ADAPTERS);

	// Anzahl der Adapter abfragen
	iNumAdapters = g_pD3DTemp->GetAdapterCount();

	// Jeden Adapter durchgehen
	for(int iAdapter = 0; iAdapter < iNumAdapters; iAdapter++)
	{
		// Informationen abfragen
		g_pD3DTemp->GetAdapterIdentifier(iAdapter, 0, &AdapterInfo);

		// Einen neuen Eintrag in der Adapterliste anlegen, mit
		// dem Namen des Adapters
		SendDlgItemMessage(hDlg, TB_IDC_D3DC_ADAPTERS, CB_ADDSTRING,
			               0, (LPARAM)(AdapterInfo.Description));
	}

	// Den ersten Adapter in der Liste auswählen
	SendDlgItemMessage(hDlg, TB_IDC_D3DC_ADAPTERS, CB_SETCURSEL, 0, 0);

	return TB_OK;
}

// ******************************************************************
// Abzählen der Gerätetypen des gerade ausgewählten Adapters
tbResult EnumDeviceTypes(HWND hDlg)
{
	int			iSelectedAdapter;
	D3DDEVTYPE	aDeviceType[] = {D3DDEVTYPE_HAL, D3DDEVTYPE_SW, D3DDEVTYPE_REF};
	char*		apcDeviceType[] = {"Hardwarebeschleunigung", "Softwaretreiber", "Referenz-Rasterizer"};
	D3DCAPS9	Caps;
	int			iIndex;
	D3DDEVTYPE*	pDeviceType;


	// Gewählten Adapter abfragen
	iSelectedAdapter = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_ADAPTERS, CB_GETCURSEL, 0, 0));

	// Gerätetypenliste leeren
	DeleteListData(hDlg, TB_IDC_D3DC_DEVICE_TYPES);

	// Gerätetypen prüfen
	for(int iDeviceType = 0; iDeviceType < 3; iDeviceType++)
	{
		// Fähigkeiten abfragen. Falls das funktioniert, ist der Gerätetyp
		// verfügbar und er wird zur Liste hinzugefügt.
		if(SUCCEEDED(g_pD3DTemp->GetDeviceCaps(iSelectedAdapter, aDeviceType[iDeviceType], &Caps)))
		{
			iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_ADDSTRING,
				                              0, (LPARAM)(apcDeviceType[iDeviceType])));

			// Speicher für den Gerätetyp reservieren und ihn dem Listeneintrag zuweisen.
			// Nur anhand des Listenindexes lässt sich der Typ nicht bestimmen.
			pDeviceType = (D3DDEVTYPE*)(tbMemAlloc(sizeof(D3DDEVTYPE)));
			*pDeviceType = aDeviceType[iDeviceType];
			
			// Daten dem Listeneintrag zuweisen
			SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_SETITEMDATA,
				               iIndex, (LPARAM)(pDeviceType));

			if(!(Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT))
			{
				g_TempConfig.Direct3D.dwFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
				EnableWindow(GetDlgItem(hDlg, TB_IDC_D3DC_HARDWARE_TNL), FALSE);
			}
		}
	}

	// Ersten Gerätetyp auswählen
	SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_SETCURSEL, 0, 0);

	return TB_OK;
}

// ******************************************************************
// Abzählen der Videomodi des Adapters und des Gerätetyps
tbResult EnumVideoModes(HWND hDlg)
{
	int				iSelectedAdapter;
	int				iNumVideoModes;
	D3DFORMAT		aFormat[] = {D3DFMT_X8R8G8B8, D3DFMT_R8G8B8, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5};
	D3DDISPLAYMODE*	pVideoMode;
	char			acFormatString[256];
	char			acVideoModeString[256];
	int				iIndex;


	// Gewählten Adapter abfragen
	iSelectedAdapter = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_ADAPTERS, CB_GETCURSEL, 0, 0));

	// Liste der Videomodi leeren
	DeleteListData(hDlg, TB_IDC_D3DC_VIDEO_MODES);

	// Anzahl der verfügbaren Videomodi erfragen.
	// Wir gehen dabei Format für Format durch.
	for(int iFormat = 0; iFormat < 3; iFormat++)
	{
		iNumVideoModes = g_pD3DTemp->GetAdapterModeCount(iSelectedAdapter, aFormat[iFormat]);
		
		// Jeden Videomodus durchgehen und Informationen über ihn abfragen
		for(int iVideoMode = 0; iVideoMode < iNumVideoModes; iVideoMode++)
		{
			// Speicher für diesen Videomodus reservieren
			pVideoMode = (D3DDISPLAYMODE*)(tbMemAlloc(sizeof(D3DDISPLAYMODE)));

			// Informationen abfragen
			g_pD3DTemp->EnumAdapterModes(iSelectedAdapter, aFormat[iFormat], iVideoMode, pVideoMode);

			// Textbeschreibung für diesen Videomodus generieren
			tbGetD3DFormatString(pVideoMode->Format, acFormatString);
			if(pVideoMode->RefreshRate != 0) sprintf(acVideoModeString, "%d x %d (%s, %d Hz)", pVideoMode->Width, pVideoMode->Height, acFormatString, pVideoMode->RefreshRate);
			else sprintf(acVideoModeString, "%d x %d (%s)", pVideoMode->Width, pVideoMode->Height, acFormatString);
			
			// Einen neuen Listeneintrag anlegen
			iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_ADDSTRING,
								              0, (LPARAM)(acVideoModeString)));

			// Daten dem Eintrag zuweisen
			SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_SETITEMDATA,
				               iIndex, (LPARAM)(pVideoMode));
		}
	}

	// Ersten Videomodus auswählen
	SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_SETCURSEL, 0, 0);

	return TB_OK;
}

// ******************************************************************
// Diese Funktion listet alle kompatiblen Bildpufferformate auf
tbResult EnumBackBufferFormats(HWND hDlg)
{
	int				iSelectedAdapter;
	int				iSelectedDeviceType;
	D3DDEVTYPE*		pSelectedDeviceType;
	int				iSelectedVideoMode;
	D3DDISPLAYMODE*	pSelectedVideoMode;
	BOOL			bWindowed;
	D3DDISPLAYMODE	CurrentVideoMode;
	BOOL			bHardwareFormatConversion;
	D3DFORMAT		aFormat[] = {D3DFMT_A2R10G10B10, D3DFMT_X8R8G8B8, D3DFMT_A8R8G8B8,
						         D3DFMT_R5G6B5, D3DFMT_X1R5G5B5, D3DFMT_A1R5G5B5};
	char			acBackBufferString[256];
	int				iIndex;
	D3DFORMAT*		pFormat;


	// Gewählten Adapter, Gerätetyp, Videomodus und Fenster-/Vollbildmodus abfragen
	iSelectedAdapter = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_ADAPTERS, CB_GETCURSEL, 0, 0));
	iSelectedDeviceType = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_GETCURSEL, 0, 0));
	pSelectedDeviceType = (D3DDEVTYPE*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_GETITEMDATA, iSelectedDeviceType, 0));
	iSelectedVideoMode = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_GETCURSEL, 0, 0));
	pSelectedVideoMode = (D3DDISPLAYMODE*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_GETITEMDATA, iSelectedVideoMode, 0));
	bWindowed = IsDlgButtonChecked(hDlg, TB_IDC_D3DC_WINDOWED_MODE) == BST_CHECKED;

	// Liste leeren
	DeleteListData(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS);

	// Aktuellen Videomodus abfragen
	g_pD3DTemp->GetAdapterDisplayMode(iSelectedAdapter, &CurrentVideoMode);

	// Jedes Format in der Liste durchgehen und prüfen, ob es verfügbar ist
	for(int iFormat = 0; iFormat < 6; iFormat++)
	{
		// Unterstützung abfragen
		if(SUCCEEDED(g_pD3DTemp->CheckDeviceFormat(iSelectedAdapter,
												   *pSelectedDeviceType,
												   bWindowed ? CurrentVideoMode.Format : pSelectedVideoMode->Format,
												   D3DUSAGE_RENDERTARGET,
												   D3DRTYPE_SURFACE,
												   aFormat[iFormat])))
		{
			// Prüfen, ob der Gerätetyp mit dieser Einstellung verfügbar ist
			if(SUCCEEDED(g_pD3DTemp->CheckDeviceType(iSelectedAdapter,
													 *pSelectedDeviceType,
													 bWindowed ? CurrentVideoMode.Format : pSelectedVideoMode->Format,
													 aFormat[iFormat],
													 bWindowed)))
			{
				// Wenn widersprüchliche Formate gewählt wurden, muss eine Formatkonvertierung
				// unterstützt sein.
				bHardwareFormatConversion = TRUE;
				if(bWindowed && aFormat[iFormat] != CurrentVideoMode.Format)
				{
					if(SUCCEEDED(g_pD3DTemp->CheckDeviceFormatConversion(iSelectedAdapter,
																		 *pSelectedDeviceType,
																		 CurrentVideoMode.Format,
																		 aFormat[iFormat])))
					{
						// Hardwarekonvertierung ist nicht möglich!
						bHardwareFormatConversion = FALSE;
					}
				}

				if(bHardwareFormatConversion)
				{
					// Formattext abfragen und Text generieren
					tbGetD3DFormatString(aFormat[iFormat], acBackBufferString);
					if(bWindowed)
					{
						sprintf(acBackBufferString, "%s %s", acBackBufferString,
								aFormat[iFormat] != CurrentVideoMode.Format ? "Hardwarekonv." : "");
					}

					// Listeneintrag generieren
					iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_ADDSTRING,
													  0, (LPARAM)(acBackBufferString)));

					// Dem Eintrag das Format zuweisen
					pFormat = (D3DFORMAT*)(tbMemAlloc(sizeof(D3DFORMAT)));
					*pFormat = aFormat[iFormat];
					SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_SETITEMDATA,
									   iIndex, (LPARAM)(pFormat));
				}
			}
		}
	}

	// Erstes Bildpufferformat auswählen
	SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_SETCURSEL, 0, 0);

	return TB_OK;
}

// ******************************************************************
// Funktion zum Auflisten aller kompatiblen Z-Stencil-Buffer-Formate
tbResult EnumZStencilBufferFormats(HWND hDlg)
{
	int				iSelectedAdapter;
	int				iSelectedDeviceType;
	D3DDEVTYPE*		pSelectedDeviceType;
	int				iSelectedVideoMode;
	D3DDISPLAYMODE*	pSelectedVideoMode;
	BOOL			bWindowed;
	int				iSelectedBackBufferFormat;
	D3DFORMAT*		pSelectedBackBufferFormat;
	D3DDISPLAYMODE	CurrentVideoMode;
	D3DFORMAT		aFormat[] = {D3DFMT_D32,
		                         D3DFMT_D24X8, D3DFMT_D24S8, D3DFMT_D24X4S4,
								 D3DFMT_D16, D3DFMT_D16_LOCKABLE,
								 D3DFMT_D15S1};
	char			acZStencilBufferString[256];
	int				iIndex;
	D3DFORMAT*		pFormat;


	// Gewählten Adapter, Gerätetyp, Videomodus, Fenster-/Vollbildmodus und Bildpufferformat abfragen
	iSelectedAdapter = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_ADAPTERS, CB_GETCURSEL, 0, 0));
	iSelectedDeviceType = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_GETCURSEL, 0, 0));
	pSelectedDeviceType = (D3DDEVTYPE*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_GETITEMDATA, iSelectedDeviceType, 0));
	iSelectedVideoMode = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_GETCURSEL, 0, 0));
	pSelectedVideoMode = (D3DDISPLAYMODE*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_GETITEMDATA, iSelectedVideoMode, 0));
	bWindowed = IsDlgButtonChecked(hDlg, TB_IDC_D3DC_WINDOWED_MODE) == BST_CHECKED;
	iSelectedBackBufferFormat = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_GETCURSEL, 0, 0));
	pSelectedBackBufferFormat = (D3DFORMAT*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_GETITEMDATA, iSelectedBackBufferFormat, 0));

	// Liste leeren
	DeleteListData(hDlg, TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS);

	// Aktuellen Videomodus abfragen
	g_pD3DTemp->GetAdapterDisplayMode(iSelectedAdapter, &CurrentVideoMode);

	// Jedes Format in der Liste durchgehen und prüfen, ob es verfügbar ist
	for(int iFormat = 0; iFormat < 7; iFormat++)
	{
		// Unterstützung abfragen
		if(SUCCEEDED(g_pD3DTemp->CheckDeviceFormat(iSelectedAdapter,
			                                   *pSelectedDeviceType,
											   bWindowed ?   CurrentVideoMode.Format
												           : pSelectedVideoMode->Format,
											   D3DUSAGE_DEPTHSTENCIL,
											   D3DRTYPE_SURFACE,
											   aFormat[iFormat])))
		{
			// Passt das Format zum Bildpufferformat?
			if(SUCCEEDED(g_pD3DTemp->CheckDepthStencilMatch(iSelectedAdapter,
															*pSelectedDeviceType,
															bWindowed ? CurrentVideoMode.Format
																	    : pSelectedVideoMode->Format,
															*pSelectedBackBufferFormat,
															aFormat[iFormat])))
			{
				// Formattext abfragen und Text generieren
				tbGetD3DFormatString(aFormat[iFormat], acZStencilBufferString);

				// Listeneintrag generieren
				iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS, CB_ADDSTRING,
												  0, (LPARAM)(acZStencilBufferString)));

				// Dem Eintrag das Format zuweisen
				pFormat = (D3DFORMAT*)(tbMemAlloc(sizeof(D3DFORMAT)));
				*pFormat = aFormat[iFormat];
				SendDlgItemMessage(hDlg, TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS, CB_SETITEMDATA,
								   iIndex, (LPARAM)(pFormat));
			}
		}
	}

	// Erstes Z-Stencil-Buffer-Format auswählen
	SendDlgItemMessage(hDlg, TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS, CB_SETCURSEL, 0, 0);

	return TB_OK;
}

// ******************************************************************
// Auflisten der Multi-Sampling-Typen
tbResult EnumMultiSamplingTypes(HWND hDlg)
{
	int						iSelectedAdapter;
	int						iSelectedDeviceType;
	D3DDEVTYPE*				pSelectedDeviceType;
	int						iSelectedVideoMode;
	D3DDISPLAYMODE*			pSelectedVideoMode;
	BOOL					bWindowed;
	int						iSelectedBackBufferFormat;
	D3DFORMAT*				pSelectedBackBufferFormat;
	int						iSelectedZStencilBufferFormat;
	D3DFORMAT*				pSelectedZStencilBufferFormat;
	D3DMULTISAMPLE_TYPE		aMultiSamplingType[] = {D3DMULTISAMPLE_NONE,
		                                            D3DMULTISAMPLE_2_SAMPLES,
													D3DMULTISAMPLE_3_SAMPLES,
													D3DMULTISAMPLE_4_SAMPLES,
													D3DMULTISAMPLE_5_SAMPLES,
													D3DMULTISAMPLE_6_SAMPLES,
													D3DMULTISAMPLE_7_SAMPLES,
													D3DMULTISAMPLE_8_SAMPLES,
													D3DMULTISAMPLE_9_SAMPLES,
													D3DMULTISAMPLE_10_SAMPLES,
													D3DMULTISAMPLE_11_SAMPLES,
													D3DMULTISAMPLE_12_SAMPLES,
													D3DMULTISAMPLE_13_SAMPLES,
													D3DMULTISAMPLE_14_SAMPLES,
													D3DMULTISAMPLE_15_SAMPLES,
													D3DMULTISAMPLE_16_SAMPLES};
	char					acMultiSamplingTypeString[256];
	int						iIndex;
	D3DMULTISAMPLE_TYPE*	pMultiSamplingType;
	DWORD					dwTemp;


	// Gewählte Optionen abfragen
	iSelectedAdapter = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_ADAPTERS, CB_GETCURSEL, 0, 0));
	iSelectedDeviceType = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_GETCURSEL, 0, 0));
	pSelectedDeviceType = (D3DDEVTYPE*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_GETITEMDATA, iSelectedDeviceType, 0));
	iSelectedVideoMode = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_GETCURSEL, 0, 0));
	pSelectedVideoMode = (D3DDISPLAYMODE*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_GETITEMDATA, iSelectedVideoMode, 0));
	bWindowed = IsDlgButtonChecked(hDlg, TB_IDC_D3DC_WINDOWED_MODE) == BST_CHECKED;
	iSelectedBackBufferFormat = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_GETCURSEL, 0, 0));
	pSelectedBackBufferFormat = (D3DFORMAT*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_GETITEMDATA, iSelectedBackBufferFormat, 0));
	iSelectedZStencilBufferFormat = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS, CB_GETCURSEL, 0, 0));
	pSelectedZStencilBufferFormat = (D3DFORMAT*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS, CB_GETITEMDATA, iSelectedZStencilBufferFormat, 0));

	// Liste leeren
	DeleteListData(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_TYPES);

	// Alle Multi-Sampling-Typen durchgehen und testen
	for(int iMultiSamplingType = 0; iMultiSamplingType < 16; iMultiSamplingType++)
	{
		// Kompatibilität mit dem Bildpufferformat prüfen
		if(SUCCEEDED(g_pD3DTemp->CheckDeviceMultiSampleType(iSelectedAdapter, 
															*pSelectedDeviceType,
															*pSelectedBackBufferFormat,
															bWindowed,
															aMultiSamplingType[iMultiSamplingType],
															&dwTemp)))
		{
			// Kompatibilität mit dem Z-Stencil-Buffer-Format prüfen
			if(SUCCEEDED(g_pD3DTemp->CheckDeviceMultiSampleType(iSelectedAdapter, 
																*pSelectedDeviceType,
																*pSelectedZStencilBufferFormat,
																bWindowed,
																aMultiSamplingType[iMultiSamplingType],
																&dwTemp)))
			{
				// Der Multi-Sampling-Typ ist OK!
				if(iMultiSamplingType == 0) strcpy(acMultiSamplingTypeString, "Kein Multi-Sampling");
				else sprintf(acMultiSamplingTypeString, "%d Samples", iMultiSamplingType + 1);

				pMultiSamplingType = (D3DMULTISAMPLE_TYPE*)(tbMemAlloc(sizeof(D3DMULTISAMPLE_TYPE)));
				*pMultiSamplingType = aMultiSamplingType[iMultiSamplingType];

				// Listeneintrag erzeugen
				iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_TYPES, CB_ADDSTRING,
					                              0, (LPARAM)(acMultiSamplingTypeString)));
				
				// Dem Eintrag den Multi-Sampling-Typ zuweisen
				SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_TYPES, CB_SETITEMDATA,
					               iIndex, (LPARAM)(pMultiSamplingType));
			}
		}
	}

	// Ersten Multi-Sampling-Typ auswählen
	SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_TYPES, CB_SETCURSEL, 0, 0);

	return TB_OK;
}

// ******************************************************************
// Auflisten aller Multi-Sampling-Qualitätsstufen
tbResult EnumMultiSamplingQualities(HWND hDlg)
{
	int						iSelectedAdapter;
	int						iSelectedDeviceType;
	D3DDEVTYPE*				pSelectedDeviceType;
	int						iSelectedVideoMode;
	D3DDISPLAYMODE*			pSelectedVideoMode;
	BOOL					bWindowed;
	int						iSelectedBackBufferFormat;
	D3DFORMAT*				pSelectedBackBufferFormat;
	int						iSelectedMultiSamplingType;
	D3DMULTISAMPLE_TYPE*	pSelectedMultiSamplingType;
	DWORD					dwNumMultiSamplingQualities;
	char					acMultiSamplingQualityString[256];


	// Gewählte Optionen abfragen
	iSelectedAdapter = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_ADAPTERS, CB_GETCURSEL, 0, 0));
	iSelectedDeviceType = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_GETCURSEL, 0, 0));
	pSelectedDeviceType = (D3DDEVTYPE*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_GETITEMDATA, iSelectedDeviceType, 0));
	iSelectedVideoMode = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_GETCURSEL, 0, 0));
	pSelectedVideoMode = (D3DDISPLAYMODE*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_GETITEMDATA, iSelectedVideoMode, 0));
	bWindowed = IsDlgButtonChecked(hDlg, TB_IDC_D3DC_WINDOWED_MODE) == BST_CHECKED;
	iSelectedBackBufferFormat = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_GETCURSEL, 0, 0));
	pSelectedBackBufferFormat = (D3DFORMAT*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_GETITEMDATA, iSelectedBackBufferFormat, 0));
	iSelectedMultiSamplingType = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_TYPES, CB_GETCURSEL, 0, 0));
	pSelectedMultiSamplingType = (D3DMULTISAMPLE_TYPE*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_TYPES, CB_GETITEMDATA, iSelectedMultiSamplingType, 0));

	// Liste leeren
	DeleteListData(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_QUALITIES);

	// Anzahl der Qualitätsstufen abfragen
	g_pD3DTemp->CheckDeviceMultiSampleType(iSelectedAdapter, 
										   *pSelectedDeviceType,
										   *pSelectedBackBufferFormat,
										   bWindowed,
										   *pSelectedMultiSamplingType,
										   &dwNumMultiSamplingQualities);

	// Listeneinträge erzeugen
	for(int iQuality = 0; iQuality < (int)(dwNumMultiSamplingQualities); iQuality++)
	{
		sprintf(acMultiSamplingQualityString, "Qualitätsstufe %d (von max. %d)",
			    iQuality + 1, dwNumMultiSamplingQualities);
		SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_QUALITIES, CB_ADDSTRING,
			               0, (LPARAM)(acMultiSamplingQualityString));
	}

	// Höchste Qualitätsstufe auswählen
	SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_QUALITIES, CB_SETCURSEL,
		               dwNumMultiSamplingQualities - 1, 0);

	return TB_OK;
}

// ******************************************************************
// Rückruffunktion für das Abzählen der Soundtreiber
BOOL CALLBACK DirectSoundEnumerateCallback(LPGUID pGUID,
										   LPCSTR pcDescription,
										   LPCSTR pcModule,
										   void* pContext)
{
	HWND	hDlg;
	int		iIndex;
	LPGUID	pStoredGUID;


	hDlg = (HWND)(pContext);

	// Listeneintrag erstellen
	iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_DSC_DRIVERS, CB_ADDSTRING, 0, (LPARAM)(pcDescription)));

	// GUID kopieren und dem Eintrag zuweisen
	pStoredGUID = (LPGUID)(tbMemAlloc(sizeof(GUID)));
	if(pGUID == NULL) *pStoredGUID = GUID_NULL;
	else *pStoredGUID = *pGUID;

	SendDlgItemMessage(hDlg, TB_IDC_DSC_DRIVERS, CB_SETITEMDATA, iIndex, (LPARAM)(pStoredGUID));

	return TRUE;
}

// ******************************************************************
// Abzählen der DirectSound-Treiber
tbResult EnumSoundDrivers(HWND hDlg)
{
	DeleteListData(hDlg, TB_IDC_DSC_DRIVERS);
	DirectSoundEnumerate(DirectSoundEnumerateCallback, (void*)(hDlg));

	// Den ersten Treiber in der Liste auswählen
	SendDlgItemMessage(hDlg, TB_IDC_DSC_DRIVERS, CB_SETCURSEL, 0, 0);

	return TB_OK;
}

// ******************************************************************
// Diese Funktion durchsucht eine Dialogliste nach einem bestimmten Element.
tbResult FindListElement(HWND hDlg,
						 int iDialogItem,
						 void* pData,
						 int iSizeOfData,
						 int* piOut)
{
	int		iNumElements;
	void*	pElementData;


	// Parameter prüfen
	if(pData == NULL) TB_ERROR_NULL_POINTER("pData", TB_ERROR);


	// Anzahl der Listenelemente abfragen
	iNumElements = (int)(SendDlgItemMessage(hDlg, iDialogItem, CB_GETCOUNT, 0, 0));
	if(iNumElements == CB_ERR) return TB_ERROR;

	// Alle Elemente durchgehen
	for(int iElement = 0; iElement < iNumElements; iElement++)
	{
		// Daten des Elements abfragen und mit den angegebenen Daten vergleichen
		pElementData = (void*)(SendDlgItemMessage(hDlg, iDialogItem, CB_GETITEMDATA, iElement, 0));
		if(pElementData != (void*)(CB_ERR))
		{
			if(!memcmp(pData, pElementData, iSizeOfData))
			{
				// Gefunden!
				*piOut = iElement;
				return TB_OK;
			}
		}
	}

	return TB_ERROR;
}

// ******************************************************************
// Diese Funktion aktiviert in einer Dialogliste ein bestimmtes Element.
tbResult SelectListElement(HWND hDlg,
						   int iDialogItem,
						   void* pData,
						   int iSizeOfData,
						   int* piOut)
{
	int iIndex;

	// Nach dem Element suchen
	if(FindListElement(hDlg, iDialogItem, pData, iSizeOfData, &iIndex)) return TB_ERROR;

	// Element gefunden! Auswählen.
	SendDlgItemMessage(hDlg, iDialogItem, CB_SETCURSEL, iIndex, 0);
	if(piOut != NULL) *piOut = iIndex;

	return TB_OK;
}

// ******************************************************************
// Diese Funktion speichert die Einstellungen in eine Struktur.
tbResult GetConfig(HWND hDlg,
				   tbConfig* pOut)
{
	int		iIndex;
	void*	pTemp;


	// Parameter prüfen
	if(pOut == NULL) TB_ERROR_NULL_POINTER("pOut", TB_ERROR);


	// Alle Parameter abfragen und speichern, vorher noch die Struktur leeren
	ZeroMemory(pOut, sizeof(tbConfig));
	
	// Adapter
	pOut->Direct3D.iAdapter = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_ADAPTERS, CB_GETCURSEL, 0, 0));
	
	// Gerätetyp
	iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_GETCURSEL, 0, 0));	
	pTemp = ((void*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_DEVICE_TYPES, CB_GETITEMDATA, iIndex, 0)));
	if(pTemp != (void*)(CB_ERR)) pOut->Direct3D.DeviceType = *((D3DDEVTYPE*)(pTemp));

	// Fenster-/Vollbildmodus
	pOut->Direct3D.bWindowed = IsDlgButtonChecked(hDlg, TB_IDC_D3DC_WINDOWED_MODE) == BST_CHECKED;

	// Videomodus
	iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_GETCURSEL, 0, 0));
	pTemp = ((void*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_VIDEO_MODES, CB_GETITEMDATA, iIndex, 0)));
	if(pTemp != (void*)(CB_ERR)) pOut->Direct3D.VideoMode = *((D3DDISPLAYMODE*)(pTemp));

	// Bildpufferformat
	iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_GETCURSEL, 0, 0));
	pTemp = ((void*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, CB_GETITEMDATA, iIndex, 0)));
	if(pTemp != (void*)(CB_ERR)) pOut->Direct3D.BackBufferFormat = *((D3DFORMAT*)(pTemp));

	// Z-Stencil-Buffer-Format
	iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS, CB_GETCURSEL, 0, 0));
	pTemp = ((void*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS, CB_GETITEMDATA, iIndex, 0)));
	if(pTemp != (void*)(CB_ERR)) pOut->Direct3D.ZStencilBufferFormat = *((D3DFORMAT*)(pTemp));

	// Multi-Sampling-Typ
	iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_TYPES, CB_GETCURSEL, 0, 0));
	pTemp = ((void*)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_TYPES, CB_GETITEMDATA, iIndex, 0)));
	if(pTemp != (void*)(CB_ERR)) pOut->Direct3D.MultiSamplingType = *((D3DMULTISAMPLE_TYPE*)(pTemp));

	// Multi-Sampling-Qualität
	pOut->Direct3D.dwMultiSamplingQuality = (DWORD)(SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_QUALITIES, CB_GETCURSEL, 0, 0));

	// Transformation und Beleuchtung
	pOut->Direct3D.dwFlags = D3DCREATE_MULTITHREADED;
	if(IsDlgButtonChecked(hDlg, TB_IDC_D3DC_HARDWARE_TNL) == BST_CHECKED) pOut->Direct3D.dwFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else pOut->Direct3D.dwFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	
	// DirectSound-Treiber
	iIndex = (int)(SendDlgItemMessage(hDlg, TB_IDC_DSC_DRIVERS, CB_GETCURSEL, 0, 0));
	if(iIndex != CB_ERR) pOut->DirectSound.SoundDriverGUID = *((LPGUID)(SendDlgItemMessage(hDlg, TB_IDC_DSC_DRIVERS, CB_GETITEMDATA, iIndex, 0)));

	// Anzahl der Bits pro Sample
	if(IsDlgButtonChecked(hDlg, TB_IDC_DSC_8_BITS) == BST_CHECKED) pOut->DirectSound.dwNumBitsPerSample = 8;
	else pOut->DirectSound.dwNumBitsPerSample = 16;

	// Sampling-Frequenz
	if(IsDlgButtonChecked(hDlg, TB_IDC_DSC_11025_HZ) == BST_CHECKED) pOut->DirectSound.dwSampleRate = 11025;
	else if(IsDlgButtonChecked(hDlg, TB_IDC_DSC_22050_HZ) == BST_CHECKED) pOut->DirectSound.dwSampleRate = 22050;
	else pOut->DirectSound.dwSampleRate = 44100;

	return TB_OK;
}

// ******************************************************************
// Diese Funktion aktiviert die Einstellungen aus einer Struktur
// so weit wie möglich.
tbResult SetConfig(HWND hDlg,
				   tbConfig* pConfig)
{
	// Alle Parameter in der Liste suchen und/oder auswählen
	SendDlgItemMessage(hDlg, TB_IDC_D3DC_ADAPTERS, CB_SETCURSEL, pConfig->Direct3D.iAdapter, 0);
	EnumDeviceTypes(hDlg); SelectListElement(hDlg, TB_IDC_D3DC_DEVICE_TYPES, &pConfig->Direct3D.DeviceType, sizeof(D3DDEVTYPE), NULL);
	CheckDlgButton(hDlg, TB_IDC_D3DC_WINDOWED_MODE, pConfig->Direct3D.bWindowed ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, TB_IDC_D3DC_FULLSCREEN_MODE, !pConfig->Direct3D.bWindowed ? BST_CHECKED : BST_UNCHECKED);
	EnumVideoModes(hDlg); SelectListElement(hDlg, TB_IDC_D3DC_VIDEO_MODES, &pConfig->Direct3D.VideoMode, sizeof(D3DDISPLAYMODE), NULL);
	EnumBackBufferFormats(hDlg); SelectListElement(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS, &pConfig->Direct3D.BackBufferFormat, sizeof(D3DFORMAT), NULL);
	EnumZStencilBufferFormats(hDlg); SelectListElement(hDlg, TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS, &pConfig->Direct3D.ZStencilBufferFormat, sizeof(D3DFORMAT), NULL);
	EnumMultiSamplingTypes(hDlg); SelectListElement(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_TYPES, &pConfig->Direct3D.MultiSamplingType, sizeof(D3DMULTISAMPLE_TYPE), NULL);
	EnumMultiSamplingQualities(hDlg); if(pConfig->Direct3D.dwMultiSamplingQuality != -1) SendDlgItemMessage(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_QUALITIES, CB_SETCURSEL, pConfig->Direct3D.dwMultiSamplingQuality, 0);
	CheckDlgButton(hDlg, TB_IDC_D3DC_HARDWARE_TNL, pConfig->Direct3D.dwFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, TB_IDC_D3DC_SOFTWARE_TNL, pConfig->Direct3D.dwFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING ? BST_CHECKED : BST_UNCHECKED);

	SelectListElement(hDlg, TB_IDC_DSC_DRIVERS, &pConfig->DirectSound.SoundDriverGUID, sizeof(GUID), NULL);
	CheckDlgButton(hDlg, TB_IDC_DSC_8_BITS, pConfig->DirectSound.dwNumBitsPerSample == 8 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, TB_IDC_DSC_16_BITS, pConfig->DirectSound.dwNumBitsPerSample == 16 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, TB_IDC_DSC_11025_HZ, pConfig->DirectSound.dwSampleRate == 11025 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, TB_IDC_DSC_22050_HZ, pConfig->DirectSound.dwSampleRate == 22050 ? BST_CHECKED : BST_UNCHECKED);
	CheckDlgButton(hDlg, TB_IDC_DSC_44100_HZ, pConfig->DirectSound.dwSampleRate == 44100 ? BST_CHECKED : BST_UNCHECKED);

	return TB_OK;
}


void chatlisten_initialisieren( HWND hDlg)
	{
	HWND lst;
	LVCOLUMN lvc;

	//server
	lst = GetDlgItem(hDlg, IDC_CHATLISTE_SERVER);
	ListView_SetExtendedListViewStyleEx( lst, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	lvc.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_FMT;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 120;
	lvc.pszText = "Name";
	ListView_InsertColumn( lst, 0, &lvc);
	lvc.cx = 360;
	lvc.pszText = "Text";
	ListView_InsertColumn( lst, 1, &lvc);

	//client
	lst = GetDlgItem(hDlg, IDC_CHATLISTE_CLIENT);
	ListView_SetExtendedListViewStyleEx( lst, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	lvc.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_FMT;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 120;
	lvc.pszText = "Name";
	ListView_InsertColumn( lst, 0, &lvc);
	lvc.cx = 300;
	lvc.pszText = "Text";
	ListView_InsertColumn( lst, 1, &lvc);

	}



void spielerlisten_initialisieren( HWND hDlg)
	{
	HWND lst;
	LVCOLUMN lvc;
	//server
	lst = GetDlgItem(hDlg, IDC_SPIELERLISTE_SERVER);
	ListView_SetExtendedListViewStyleEx( lst, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	lvc.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_FMT;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 36;
	lvc.pszText = "Nr";
	ListView_InsertColumn( lst, 0, &lvc);
	lvc.cx = 96;
	lvc.pszText = "Name";
	ListView_InsertColumn( lst, 1, &lvc);
	lvc.fmt = LVCFMT_RIGHT;
	lvc.cx = 96;
	lvc.pszText = "Joystick X";
	ListView_InsertColumn( lst, 2, &lvc);
	lvc.cx = 96;
	lvc.pszText = "Joystick Z";
	ListView_InsertColumn( lst, 3, &lvc);

	//client
	lst = GetDlgItem(hDlg, IDC_SPIELERLISTE_CLIENT);
	ListView_SetExtendedListViewStyleEx( lst, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	lvc.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_FMT;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 30;
	lvc.pszText = "Nr";
	ListView_InsertColumn( lst, 0, &lvc);
	lvc.cx = 180;
	lvc.pszText = "Name";
	ListView_InsertColumn( lst, 1, &lvc);
	}

void serverliste_initialisieren( HWND hDlg)
	{
	HWND lst;
	LVCOLUMN lvc;

	lst = GetDlgItem(hDlg, IDC_HOSTLISTE);
	ListView_SetExtendedListViewStyleEx( lst, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES, LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	lvc.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_FMT;
	lvc.fmt = LVCFMT_LEFT;
	lvc.cx = 120;
	lvc.pszText = "Session";
	ListView_InsertColumn( lst, 0, &lvc);
	lvc.cx = 120;
	lvc.pszText = "IP-Adresse";
	ListView_InsertColumn( lst, 1, &lvc);
	lvc.cx = 120;
	lvc.pszText = "Portnummer";
	ListView_InsertColumn( lst, 2, &lvc);
	lvc.cx = 80;
	lvc.pszText = "Latenzzeit";
	ListView_InsertColumn( lst, 3, &lvc);
	}

void listen_initialisieren(HWND hDlg) {
	chatlisten_initialisieren(hDlg);
	spielerlisten_initialisieren(hDlg);
	serverliste_initialisieren(hDlg);
}





/*
void anmeldung( HWND hDlg)
	{
	HWND lst;
	LVITEM lvi;
	int ix;
	host *h;
	HRESULT hr;

	lst = GetDlgItem(hDlg, IDC_HOSTLISTE);
	if( ListView_GetSelectedCount( lst) == 1)
		{
		ix = ListView_GetSelectionMark( lst);
		GetDlgItemText( hDlg, IDC_SPIELER, myClient->spielername, 64);
		if( isalnum(myClient->spielername[0]))
			{
			lvi.iItem = ix;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_PARAM;
			ListView_GetItem( lst, &lvi);
			h = (host *)lvi.lParam;
			hr = myClient->anmelden( h);
			if( hr == S_OK)
				{
				SetDlgItemText( hDlg, IDC_ANMELDEN, "Abmelden");
				EnableWindow( GetDlgItem(hDlg, IDC_SPIELER), FALSE);
				EnableWindow( GetDlgItem(hDlg, IDC_CHAT_CLIENT), TRUE);
				SetDlgItemText( hDlg, IDC_SESSION, h->sessionname);
				}
//			else
				//MessageBox( hDlg, DXGetErrorDescription8( hr), "Duell-Meldung", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			}
//		else
			//MessageBox( hDlg, "Gib erst einen Spielernamen ein!", "Duell-Meldung", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
		}
//	else
		//MessageBox( hDlg, "Kein Server ausgewählt!", "Duell-Meldung", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
	}

void abmeldung( HWND hDlg)
	{
	myClient->lock();
	myClient->reset();
	myClient->init( client_messagehandler);
	serverliste_aktualisieren( hDlg);
	spielerliste_aktualisieren( hDlg);
	myClient->unlock();
	SetDlgItemText( hDlg, IDC_ANMELDEN, "Anmelden");
    EnableWindow( GetDlgItem(hDlg, IDC_SPIELER), TRUE);
    EnableWindow( GetDlgItem(hDlg, IDC_CHAT_CLIENT), FALSE);
    SetDlgItemText( hDlg, IDC_SESSION, "");
    SetDlgItemText( hDlg, IDC_SPIELERINDEX, "");
	}

*/


// ******************************************************************
// Dialogfunktion
INT_PTR CALLBACK D3DEnumDialogProc(HWND hDlg,
								   unsigned int uiMsg,
								   WPARAM WParam,
								   LPARAM LParam)
{
	char  acFilename[256];
	FILE* pFile;

	char buf[256];
	DWORD p;


	// Nachricht verarbeiten
	switch(uiMsg)
	{
	case WM_INITDIALOG:
		// Optionskästchen einstellen
		CheckDlgButton(hDlg, TB_IDC_D3DC_FULLSCREEN_MODE, BST_CHECKED);
		CheckDlgButton(hDlg, TB_IDC_D3DC_HARDWARE_TNL, BST_CHECKED);
		CheckDlgButton(hDlg, TB_IDC_DSC_16_BITS, BST_CHECKED);
		CheckDlgButton(hDlg, TB_IDC_DSC_44100_HZ, BST_CHECKED);

		// Alles abzählen
		EnumAdapters(hDlg);
		EnumDeviceTypes(hDlg);
		EnumVideoModes(hDlg);
		EnumBackBufferFormats(hDlg);
		EnumZStencilBufferFormats(hDlg);
		EnumMultiSamplingTypes(hDlg);
		EnumMultiSamplingQualities(hDlg);

		listen_initialisieren(hDlg);
		display_serverstate(hDlg);

		tbClient::mein_clientdialog = hDlg;
		tbServer::mein_serverdialog = hDlg;

		// DirectSound-Treiber auflisten
		EnumSoundDrivers(hDlg);
		break;
    case WM_SERVERLISTE_AKTUALISIEREN:
		serverliste_aktualisieren( hDlg);
		break;
    case WM_SPIELER_AKTUALISIEREN:
 		display_spieler( hDlg);
	    break;
    case WM_SPIELERLISTE_AKTUALISIEREN:
		spielerliste_aktualisieren( hDlg);
		break;
    case WM_SPIELERINDEX_AKTUALISIEREN:
		spielerindex_aktualisieren( hDlg);
		break;
	case WM_SERVER_TERMINATE:
		MessageBox( hDlg, "Abbruch der Server-Verbindung", "Duell-Meldung", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
		abmeldung( hDlg);
		break;

	case WM_CLOSE:
		// Code 2 zurückliefern; das bedeutet: Dialog abgebrochen
		EndDialog(hDlg, 2);
		break;

	case WM_DESTROY:
		// Aufräumen
		DeleteListData(hDlg, TB_IDC_D3DC_ADAPTERS);
		DeleteListData(hDlg, TB_IDC_D3DC_DEVICE_TYPES);
		DeleteListData(hDlg, TB_IDC_D3DC_VIDEO_MODES);
		DeleteListData(hDlg, TB_IDC_D3DC_BACK_BUFFER_FORMATS);
		DeleteListData(hDlg, TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS);
		DeleteListData(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_TYPES);
		DeleteListData(hDlg, TB_IDC_D3DC_MULTI_SAMPLING_QUALITIES);
		DeleteListData(hDlg, TB_IDC_DSC_DRIVERS);
		break;

	case WM_COMMAND:		
		switch(LOWORD(WParam))
		{
		case IDC_START:
			next_serverstate( hDlg);
			display_serverstate( hDlg);
			display_spieler( hDlg);
			break;
		case IDC_KILL:
			kill_players( hDlg);
			break;
		case IDC_CHAT_SERVER:
			msg_chat cm;
			cm.msgid = MSG_CHAT;
			cm.spielerindex = -1; // Server
			GetDlgItemText( hDlg, IDC_CHATMESSAGE_SERVER, cm.text, 256);
			tbServer::send_chatmessage( &cm);
			tbServer::lock();
			server_chatliste_aktualisieren( hDlg, &cm);
			tbServer::unlock();
			SetDlgItemText( hDlg, IDC_CHATMESSAGE_SERVER, "");
			break;
		case IDC_CHAT_CLIENT:
			GetDlgItemText( hDlg, IDC_CHATMESSAGE_CLIENT, buf, 256);
			tbClient::chat( buf);
			SetDlgItemText( hDlg, IDC_CHATMESSAGE_CLIENT, "");
			break;
		case IDC_SUCHEN:
			GetDlgItemText( hDlg, IDC_SERVER_IP, buf, 64);
			p = GetDlgItemInt( hDlg, IDC_PORTNUMMER, 0, FALSE);
			tbClient::host_suchen(buf[0] ? buf : 0, p);
			break;
		case IDC_ANMELDEN:
			SetCursor( LoadCursor(NULL, IDC_WAIT));
			tbClient::client->CancelAsyncOperation( NULL, DPNCANCEL_ENUM);
			if( !tbClient::myhost)
				anmeldung( hDlg);
			else
				abmeldung( hDlg);
			SetCursor( LoadCursor(NULL, IDC_ARROW));
			break;
		case TB_IDC_D3DC_ADAPTERS:
			if(HIWORD(WParam) == CBN_SELCHANGE)
			{
				// Es wurde ein neuer Adapter gewählt! Gerätetypen neu auflisten.
				GetConfig(hDlg, &g_TempConfig);
				EnumDeviceTypes(hDlg);
				EnumVideoModes(hDlg);
				EnumBackBufferFormats(hDlg);
				EnumZStencilBufferFormats(hDlg);
				EnumMultiSamplingTypes(hDlg);
				EnumMultiSamplingQualities(hDlg);
				SetConfig(hDlg, &g_TempConfig);
			}
			else
			{
				// Informationstext anzeigen
				SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_D3DC_ADAPTER_DESCRIPTION);
			}
			break;

		case TB_IDC_D3DC_DEVICE_TYPES:
			if(HIWORD(WParam) == CBN_SELCHANGE)
			{
				// Es wurde ein neuer Gerätetyp gewählt! Videomodi neu auflisten.
				GetConfig(hDlg, &g_TempConfig);
				EnumVideoModes(hDlg);
				EnumBackBufferFormats(hDlg);
				EnumZStencilBufferFormats(hDlg);
				EnumMultiSamplingTypes(hDlg);
				EnumMultiSamplingQualities(hDlg);
				SetConfig(hDlg, &g_TempConfig);
			}
			else
			{
				// Informationstext anzeigen
				SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_D3DC_DEVICE_TYPE_DESCRIPTION);
			}
			break;

		case TB_IDC_D3DC_WINDOWED_MODE:
		case TB_IDC_D3DC_FULLSCREEN_MODE:
			if(HIWORD(WParam) == BN_CLICKED)
			{
				// Es wurde zwischen Fenster- und Vollbildmodus gewechselt!
				// Die Bildpufferformate müssen neu aufgelistet werden.
				GetConfig(hDlg, &g_TempConfig);
				EnumVideoModes(hDlg);
				EnumBackBufferFormats(hDlg);
				EnumZStencilBufferFormats(hDlg);
				EnumMultiSamplingTypes(hDlg);
				EnumMultiSamplingQualities(hDlg);
				SetConfig(hDlg, &g_TempConfig);
			}

			// Informationstext anzeigen
			SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_D3DC_WINDOWED_FULLSCREEN_DESCRIPTION);
			break;

		case TB_IDC_D3DC_VIDEO_MODES:
			if(HIWORD(WParam) == CBN_SELCHANGE)
			{
				// Es wurde ein neuer Videomodus gewählt! Bildpufferformate neu auflisten.
				GetConfig(hDlg, &g_TempConfig);
				EnumBackBufferFormats(hDlg);
				EnumZStencilBufferFormats(hDlg);
				EnumMultiSamplingTypes(hDlg);
				EnumMultiSamplingQualities(hDlg);
				SetConfig(hDlg, &g_TempConfig);
			}
			else
			{
				// Informationstext anzeigen
				SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_D3DC_VIDEO_MODE_DESCRIPTION);
			}
			break;

		case TB_IDC_D3DC_BACK_BUFFER_FORMATS:
			if(HIWORD(WParam) == CBN_SELCHANGE)
			{
				// Es wurde ein neues Bildpufferformat gewählt! Z-Stencil-Buffer-Formate neu auflisten.
				GetConfig(hDlg, &g_TempConfig);
				EnumZStencilBufferFormats(hDlg);
				EnumMultiSamplingTypes(hDlg);
				EnumMultiSamplingQualities(hDlg);
				SetConfig(hDlg, &g_TempConfig);
			}
			else
			{
				// Informationstext anzeigen
				SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_D3DC_BACK_BUFFER_FORMAT_DESCRIPTION);
			}
			break;

		case TB_IDC_D3DC_Z_STENCIL_BUFFER_FORMATS:
			if(HIWORD(WParam) == CBN_SELCHANGE)
			{
				// Es wurde ein neues Z-Stencil-Buffer-Format gewählt! Multi-Sampling-Typen neu auflisten.
				GetConfig(hDlg, &g_TempConfig);
				EnumMultiSamplingTypes(hDlg);
				EnumMultiSamplingQualities(hDlg);
				SetConfig(hDlg, &g_TempConfig);
			}
			else
			{
				// Informationstext anzeigen
				SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_D3DC_Z_STENCIL_BUFFER_FORMAT_DESCRIPTION);
			}
			break;

		case TB_IDC_D3DC_MULTI_SAMPLING_TYPES:
			if(HIWORD(WParam) == CBN_SELCHANGE)
			{
				// Es wurde ein neuer Multi-Sampling-Typ gewählt! Qualitätsstufen neu auflisten.
				GetConfig(hDlg, &g_TempConfig);
				EnumMultiSamplingQualities(hDlg);
				SetConfig(hDlg, &g_TempConfig);
			}
			else
			{
				// Informationstext anzeigen
				SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_D3DC_MULTI_SAMPLING_TYPE_DESCRIPTION);
			}
			break;

		case TB_IDC_D3DC_MULTI_SAMPLING_QUALITIES:
			if(HIWORD(WParam) != CBN_SELCHANGE)
			{
				// Informationstext anzeigen
				SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_D3DC_MULTI_SAMPLING_QUALITY_DESCRIPTION);
			}
			break;

		case TB_IDC_D3DC_HARDWARE_TNL:
		case TB_IDC_D3DC_SOFTWARE_TNL:
			// Informationstext anzeigen
			SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_D3DC_TNL_DESCRIPTION);
			break;

		case TB_IDC_DSC_DRIVERS:
			// Informationstext anzeigen
			SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_DSC_DRIVERS_DESCRIPTION);
			break;

		case TB_IDC_DSC_8_BITS:
		case TB_IDC_DSC_16_BITS:
			// Informationstext anzeigen
			SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_DSC_BITS_PER_SAMPLE_DESCRIPTION);
			break;

		case TB_IDC_DSC_11025_HZ:
		case TB_IDC_DSC_22050_HZ:
		case TB_IDC_DSC_44100_HZ:
			// Informationstext anzeigen
			SetDlgItemText(hDlg, TB_IDC_C_DESCRIPTION, TB_DSC_SAMPLING_FREQUENCY_DESCRIPTION);
			break;

		case TB_IDC_C_SAVE:
			{
				strcpy(acFilename, "*.tbc");
				OPENFILENAME Filename = {sizeof(OPENFILENAME), hDlg, NULL, "TBC-Dateien\0.tbc", NULL,
										 0, 1, acFilename, 255, NULL, 0, NULL, "TBC-Datei speichern",
										 OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
										 0, 1, ".tbc", 0, NULL, NULL};

				if(GetSaveFileName(&Filename))
				{
					// Parameter abfragen und speichern
					GetConfig(hDlg, &g_TempConfig);
					pFile = fopen(acFilename, "wc");
					fwrite(&g_TempConfig, sizeof(tbConfig), 1, pFile);
					fclose(pFile);
				}
			}
			break;

		case TB_IDC_C_LOAD:
			{
				strcpy(acFilename, "*.tbc");
				OPENFILENAME Filename = {sizeof(OPENFILENAME), hDlg, NULL, "TBC-Dateien\0.tbc\0Alle Dateien\0*.*", NULL,
										 0, 1, acFilename, 255, NULL, 0, NULL, "TBC-Datei laden",
										 OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR,
										 0, 1, ".tbc", 0, NULL, NULL};
				
				if(GetOpenFileName(&Filename))
				{
					// Parameter einlesen und aktivieren
					pFile = fopen(acFilename, "r");
					GetConfig(hDlg, &g_TempConfig);
					fread(&g_TempConfig, sizeof(tbConfig), 1, pFile);
					fclose(pFile);
					SetConfig(hDlg, &g_TempConfig);
				}
			}
			break;

		case TB_IDC_C_OK:
			// Parameter abfragen und einen eventuellen Fehler vermeiden
			GetConfig(hDlg, g_pConfig);
			if(g_pConfig->Direct3D.DeviceType != D3DDEVTYPE_HAL &&
			   !(g_pConfig->Direct3D.dwFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING))
			{
				if(g_pConfig->Direct3D.dwFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) g_pConfig->Direct3D.dwFlags ^= D3DCREATE_HARDWARE_VERTEXPROCESSING;
				g_pConfig->Direct3D.dwFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			}
			
			// Code 0 zurückliefern; das bedeutet: alles OK.
			EndDialog(hDlg, 0);
			break;

		case TB_IDC_C_CANCEL:
			// Code 2 zurückliefern; das bedeutet: Dialog abgebrochen
			EndDialog(hDlg, 2);
			break;
		}
		break;

	default:
		// Standardverarbeitung der Nachricht
		return FALSE;
		break;
	}

	// Nachricht wurde verarbeitet.
	return TRUE;
}

// ******************************************************************
// Diese Methode startet den Konfigurationsdialog und speichert die Parameter.
TRIBASE_API tbResult tbDoConfigDialog(tbConfig* pOut)
{
	int iResult;
	int hr;
	
//	tbClientInit();
//	tbServerInit();

	// Parameter prüfen
	if(pOut == NULL) TB_ERROR_NULL_POINTER("pOut", TB_ERROR);

	TB_INFO("Der Konfigurationsdialog wird aufgerufen...");

	// Den globalen Zeiger auf die Struktur setzen, damit sie später
	// gefüllt werden kann.
	g_pConfig = pOut;

	// Eine IDirect3D9-Schnittstelle generieren, die für die Abzählung gebraucht wird.
	g_pD3DTemp = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3DTemp == NULL)
	{
		// Fehler!
		MessageBox(NULL, "Die IDirect3D9-Schnittstelle konnte nicht generiert werden! DirectX 9 oder höher wird benötigt!", "Fehler", MB_OK | MB_ICONEXCLAMATION);
		TB_ERROR("Die IDirect3D9-Schnittstelle konnte nicht generiert werden! DirectX 9 oder höher wird benötigt!", TB_ERROR);
	}


	// Dialogfenster aufrufen
	iResult = (int)(DialogBox(tb_g_DLLHandle,
							  MAKEINTRESOURCE(TB_IDD_CONFIG),
						      NULL,
						      D3DEnumDialogProc));

	// Die Schnittstelle wieder freigeben
	TB_SAFE_RELEASE(g_pD3DTemp);

	// Rückgabewert des Dialogfensters auswerten
	if(iResult == 0) return TB_OK;					// Alles OK!
	else if(iResult == 1) return TB_ERROR;			// Fehler!
	else /*if(iResult == 2)*/ return TB_CANCELED;	// Dialog abgebrochen
}