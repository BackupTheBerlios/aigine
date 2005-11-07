/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbDirectSound.cpp
	=================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Einfacher Umgang mit DirectSound

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
BOOL						tbDirectSound::m_bInitialized = FALSE;
LPDIRECTSOUND8				tbDirectSound::m_pDSound = NULL;
LPDIRECTSOUNDBUFFER			tbDirectSound::m_pPrimaryBuffer = NULL;
LPDIRECTSOUND3DLISTENER8	tbDirectSound::m_pListener = NULL;
tbVector3					tbDirectSound::m_vOldListenerPos(0.0f);
LONGLONG					tbDirectSound::m_llListenerTimeStamp = 0;

// ******************************************************************
// Initialisierung von DirectSound
tbResult tbDirectSound::Init(tbConfig* pConfig,
							 HWND hWindow,		// = NULL
							 DWORD dwCoopLevel,	// = DSSCL_PRIORITY
							 BOOL b3D)			// = TRUE
{
	HRESULT			hResult;
	DSBUFFERDESC	BufferDesc;
	WAVEFORMATEX	WaveFormat;


	// Parameter prüfen/anpassen
	TB_INFO("Die DirectSound-Komponente wird initialisiert...");
	if(pConfig == NULL) TB_ERROR_NULL_POINTER("pConfig", TB_ERROR);
	if(hWindow == NULL)
	{
		// Das Direct3D-Fenster benutzen!
		if(!tbDirect3D::IsInitialized()) {TB_ERROR("Kein Fenster angegeben und Direct3D wurde noch nicht initialisiert!", TB_ERROR);}
		else hWindow = tbDirect3D::GetWindow();
	}


	// IDirectSound8-Schnittstelle erstellen
	TB_INFO("DirectSound-Schnittstelle wird generiert...");
	if(FAILED(hResult = DirectSoundCreate8(&pConfig->DirectSound.SoundDriverGUID,
						                   &m_pDSound,
										   NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("DirectSoundCreate8", hResult, TB_ERROR);
	}


	// Primären Soundpuffer erstellen
	TB_INFO("Generiere primären Soundpuffer...");
	ZeroMemory(&BufferDesc, sizeof(DSBUFFERDESC));
	BufferDesc.dwSize = sizeof(DSBUFFERDESC);
	BufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | (b3D ? DSBCAPS_CTRL3D : 0);
	if(FAILED(hResult = m_pDSound->CreateSoundBuffer(&BufferDesc,
		                                             &m_pPrimaryBuffer,
													 NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pDSound->CreateSoundBuffer", hResult, TB_ERROR);
	}

	// Kooperationsebene setzen
	m_pDSound->SetCooperativeLevel(hWindow, dwCoopLevel);

	// Format des primären Puffers setzen, falls DSSCL_PRIORITY die Kooperationsebene ist
	if(dwCoopLevel & DSSCL_PRIORITY)
	{
		// Das Format wird aus der Konfigurationsstruktur zusammengebaut.
		WaveFormat.wFormatTag		= WAVE_FORMAT_PCM;
		WaveFormat.nChannels		= 2;
		WaveFormat.nSamplesPerSec	= pConfig->DirectSound.dwSampleRate;
		WaveFormat.wBitsPerSample	= (WORD)(pConfig->DirectSound.dwNumBitsPerSample);
		WaveFormat.nBlockAlign		= WaveFormat.nChannels * (WaveFormat.wBitsPerSample >> 3);
		WaveFormat.nAvgBytesPerSec	= WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
		WaveFormat.cbSize			= 0;

		// Format setzen
		if(FAILED(hResult = m_pPrimaryBuffer->SetFormat(&WaveFormat)))
		{
			// Warnung ausgeben
			TB_WARNING("Format des primären Soundpuffers konnte nicht gesetzt werden!");
		}
	}


	// 3D-Hörerschnittstelle abfragen
	if(b3D)
	{
		TB_INFO("Initialisierung des 3D-Sounds...");
		m_pPrimaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (void**)(&m_pListener));
	}

	// Damit sind wir fertig!
	TB_INFO("Die DirectSound-Komponente wurde erfolgreich initialisiert!");

	m_bInitialized = TRUE;

	return TB_OK;
}

// ******************************************************************
// Herunterfahren
tbResult tbDirectSound::Exit()
{
	m_bInitialized = FALSE;

	// Schnittstellen freigeben
	TB_SAFE_RELEASE(m_pListener);
	TB_SAFE_RELEASE(m_pDSound);

	TB_INFO("Die DirectSound-Komponente wurde heruntergefahren.");

	return TB_OK;
}

// ******************************************************************
// Setzt die Position und die Orientierung des Hörers und errechnet die Geschwindigkeit
tbResult tbDirectSound::SetListener(const tbVector3& vListenerPos,
									const tbVector3& vListenerYAxis,
									const tbVector3& vListenerZAxis,
									const float fSpeedFactor)	// = 1.0f
{
	// Prüfen, ob der Hörer überhaupt existiert (wenn nicht, dann wurde die
	// Klasseninstanz nicht für 3D-Sounds erstellt)
	if(m_pListener == NULL)
	{
		// Fehler!
		TB_ERROR("Der Hörer ist nicht verfügbar, da die tbDirectSound-Klasse nicht im 3D-Modus erstellt wurde!", TB_ERROR);
	}

	// Position und Orientierung setzen
	m_pListener->SetPosition(vListenerPos.x, vListenerPos.y, vListenerPos.z, DS3D_DEFERRED);
	m_pListener->SetOrientation(vListenerZAxis.x, vListenerZAxis.y, vListenerZAxis.z,
		                        vListenerYAxis.x, vListenerYAxis.y, vListenerYAxis.z,
								DS3D_DEFERRED);

	// Den Geschwindigkeitsvektor berechnen.
	// m_llListenerTimeStamp enthält den Zeitpunkt, zu dem SetListener das letzte
	// Mal aufgerufen wurde. Ist die Variable 0, dann ist das hier das erste Mal
	// und die Geschwindigkeit wird auf null gesetzt.
	if(m_llListenerTimeStamp == 0)
	{
		m_pListener->SetVelocity(0.0f, 0.0f, 0.0f, DS3D_DEFERRED);

		// Aktuelle Zeit und aktuelle Position als ehemalige eintragen
		QueryPerformanceCounter((LARGE_INTEGER*)(&m_llListenerTimeStamp));
		m_vOldListenerPos = vListenerPos;
	}
	else
	{
		LONGLONG llCurrentTime;

		// Differenz zwischen aktueller und alter Zeit berechnen
		QueryPerformanceCounter((LARGE_INTEGER*)(&llCurrentTime));
		double dNumSecsPassed = (double)(llCurrentTime - m_llListenerTimeStamp) / tb_g_dFrequency;

		// Wenn diese Zeit nur knapp von der Zeit seit dem letzten Frame abweicht,
		// wird dies als kleiner Fehler gewertet und die tatsächliche Zeit wird benutzt.
		if(fabs(dNumSecsPassed - tb_g_dNumSecsPassed) < 0.1) dNumSecsPassed = tb_g_dNumSecsPassed;

		// Zurückgelegte Strecke durch die Zeit teilen. Dadurch erhalten
		// wir die Geschwindigkeit.
		tbVector3 vVelocity(((vListenerPos - m_vOldListenerPos) / (float)(dNumSecsPassed)) * fSpeedFactor);
		m_pListener->SetVelocity(vVelocity.x, vVelocity.y, vVelocity.z, DS3D_DEFERRED);

		// Aktuelle Zeit und aktuelle Position als ehemalige eintragen
		m_llListenerTimeStamp = llCurrentTime;
		m_vOldListenerPos = vListenerPos;
	}

	return TB_OK;
}

// ******************************************************************