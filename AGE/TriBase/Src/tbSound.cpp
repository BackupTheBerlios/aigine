/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbSound.cpp
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Einfacher Umgang mit Sounds

	Autor:
	David Scherfgen
	_________________________________________________
	|												|
	|	ACHTUNG:									|
	|	========									|
	|	DIESE DATEI MUSS MIT DER COMPILER-OPTION	|
	|	"STRUKTURMITGLIED AUF 1 BYTE AUSRICHTEN"	|
	|	KOMPILIERT WERDEN!							|
	|												|
********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbSound-Klasse
tbSound::tbSound()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbSound));
}

// ******************************************************************
// Destruktor der tbSound-Klasse
tbSound::~tbSound()
{
	// Alle Puffer stoppen
	StopAll();

	// Alle Puffer freigeben
	for(DWORD dwBuffer = 0; dwBuffer < m_dwNumBuffers; dwBuffer++)
	{
		if(m_dwFlags & DSBCAPS_CTRLFX) TB_SAFE_RELEASE(m_ppSoundBuffers8[dwBuffer]);
		if(m_dwFlags & DSBCAPS_CTRL3D) TB_SAFE_RELEASE(m_pp3DSoundBuffers[dwBuffer]);
		TB_SAFE_RELEASE(m_ppSoundBuffers[dwBuffer]);
	}

	// Die Listen löschen
	TB_SAFE_MEMFREE(m_ppSoundBuffers);
	TB_SAFE_MEMFREE(m_pdwTimeStamps);
	if(m_dwFlags & DSBCAPS_CTRLFX) TB_SAFE_MEMFREE(m_ppSoundBuffers8);
	if(m_dwFlags & DSBCAPS_CTRL3D)
	{
		TB_SAFE_MEMFREE(m_pp3DSoundBuffers);
		TB_SAFE_MEMFREE(m_pllTimeStamps);
		TB_SAFE_MEMFREE(m_pvStartVelocities);
		TB_SAFE_MEMFREE(m_pvOldPositions);
	}
}

// ******************************************************************
// Laden einer WAV-Datei
LPDIRECTSOUNDBUFFER tbLoadWAVFile(LPDIRECTSOUND8 pDSound,
								  tbVFile* pVFile,
								  DWORD dwFlags,
								  GUID GUID3DAlgorithm)
{
	HRESULT				hResult;
	tbRIFFHeader		RIFFHeader;
	tbWAVChunkHeader	ChunkHeader;
	BOOL				bFormatChunkRead = FALSE;
	BOOL				bDataChunkRead = FALSE;
	DWORD				dwNumBytesToRead;
	WAVEFORMATEX		WaveFormat;
	void*				pSoundData = NULL;
	DSBUFFERDESC		BufferDesc;
	LPDIRECTSOUNDBUFFER	pSoundBuffer;
	void*				pSoundBufferData;
	DWORD				dwSoundBufferDataSize;


	// Parameter prüfen
	if(pDSound == NULL)	TB_ERROR_NULL_POINTER("pDSound", NULL);
	if(pVFile == NULL)	TB_ERROR_NULL_POINTER("pVFile", NULL);


    // RIFF-Header lesen und prüfen
	if(pVFile->Read(sizeof(tbRIFFHeader), &RIFFHeader)) TB_ERROR("Fehler beim Lesen des RIFF-Headers!", NULL);
	if(strnicmp(RIFFHeader.acRIFF, "RIFF", 4)) TB_ERROR("RIFF-Signatur nicht gefunden!", NULL);
	if(strnicmp(RIFFHeader.acFormat, "WAVE", 4)) TB_ERROR("WAVE-Signatur nicht gefunden - die Datei ist keine WAV-Datei!", NULL);

	// ------------------------------------------------------------------

	// WAV-Chunks einlesen
	while(TRUE)
	{
		// Chunk-Header lesen. Bei einem Lesefehler haben wir das Dateiende erreicht.
		if(pVFile->Read(sizeof(tbWAVChunkHeader), &ChunkHeader)) break;

		if(!strnicmp(ChunkHeader.acType, "fmt ", 4))
		{
			// Es ist der Format-Chunk!
			// Prüfen, ob die Größe der Daten in Ordnung ist. Wenn sie größer ist als
			// die Größe der WAVEFORMATEX-Struktur, dann lassen wir die restlichen Bytes weg.
			dwNumBytesToRead = TB_MIN(ChunkHeader.dwDataSize, sizeof(WAVEFORMATEX));
			if(pVFile->Read(dwNumBytesToRead, &WaveFormat)) TB_ERROR("Fehler beim Lesen des Soundformats!", NULL);
			bFormatChunkRead = TRUE;
		}
		else if(!strnicmp(ChunkHeader.acType, "data", 4))
		{
			// Es ist der Daten-Chunk!
			// Genügend Speicher reservieren und dann die Daten lesen.
			pSoundData = tbMemAlloc(ChunkHeader.dwDataSize);
			if(pSoundData == NULL) TB_ERROR_OUT_OF_MEMORY(NULL);
			if(pVFile->Read(ChunkHeader.dwDataSize, pSoundData)) TB_ERROR("Fehler beim Lesen der Sounddaten!", NULL);
			bDataChunkRead = TRUE;
		}
		else
		{
			// Unbekannter Chunk - überspringen
			pVFile->Seek(TB_VFSO_CURSOR, ChunkHeader.dwDataSize);
		}
	}

	// Prüfen, ob alle wichtigen Chunks vorhanden waren
	if(!(bFormatChunkRead && bDataChunkRead))
	{
		// Fehler!
		TB_SAFE_MEMFREE(pSoundData);
		TB_ERROR("Die Chunks \"fmt \" und \"data\" müssen beide vorhanden sein!", NULL);
	}

	// ------------------------------------------------------------------

	// DSBUFFERDESC-Struktur ausfüllen
	BufferDesc.dwSize			= sizeof(DSBUFFERDESC);
	BufferDesc.dwFlags			= dwFlags;
	BufferDesc.dwBufferBytes	= tbMemGetSize(pSoundData);
	BufferDesc.dwReserved		= 0;
	BufferDesc.lpwfxFormat		= &WaveFormat;
	BufferDesc.guid3DAlgorithm	= GUID3DAlgorithm;

	// Puffer erstellen
	if(FAILED(hResult = pDSound->CreateSoundBuffer(&BufferDesc,
		                                           &pSoundBuffer,
												   NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("pDSound->CreateSoundBuffer", hResult, NULL);
	}

	// ------------------------------------------------------------------

	// Puffer komplett sperren
	hResult = pSoundBuffer->Lock(0, 0, &pSoundBufferData, &dwSoundBufferDataSize, NULL, NULL, DSBLOCK_ENTIREBUFFER);
	if(FAILED(hResult))
	{
		// Fehler!
		TB_SAFE_MEMFREE(pSoundData);
		TB_SAFE_RELEASE(pSoundBuffer);
		TB_ERROR_DIRECTX("pSoundBuffer->Lock", hResult, NULL);
	}

	// Die Daten hineinschreiben und den Puffer entsperren
	memcpy(pSoundBufferData, pSoundData, TB_MIN(tbMemGetSize(pSoundData), (int)(dwSoundBufferDataSize)));
	pSoundBuffer->Unlock(pSoundBufferData, dwSoundBufferDataSize, NULL, 0);

	// Die Sounddaten freigeben
	TB_SAFE_MEMFREE(pSoundData);

	// Den Soundpuffer zurückliefern
	return pSoundBuffer;
}

// ******************************************************************
// Initialisierung aus einer virtuellen Datei
tbResult tbSound::Init(tbVFile* pVFile,
					   DWORD dwFlags,			// = DSBCAPS_STATIC | DSBCAPS_LOCDEFER
					   GUID GUID3DAlgorithm,	// = GUID_NULL
					   DWORD dwNumBuffers)		// = 4
{
	HRESULT	hResult;
	DWORD	dwVFileCursor;


	// Parameter prüfen
	if(pVFile == NULL)		TB_ERROR_NULL_POINTER("pVFile", TB_ERROR);
	if(dwNumBuffers == 0)	TB_ERROR_INVALID_VALUE("dwNumBuffers", TB_ERROR);

	// Sicherstellen, dass DirectSound initialisiert wurde
	if(!tbDirectSound::IsInitialized()) TB_ERROR("DirectSound wurde noch nicht initialisiert!", TB_ERROR);


	// Platz für die Soundpufferschnittstellen machen
	m_ppSoundBuffers = (LPDIRECTSOUNDBUFFER*)(tbMemAlloc(dwNumBuffers * sizeof(LPDIRECTSOUNDBUFFER)));
	if(m_ppSoundBuffers == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Speicher für DWORD-Zeitstempel reservieren
	m_pdwTimeStamps = (DWORD*)(tbMemAlloc(dwNumBuffers * sizeof(DWORD)));
	if(m_pdwTimeStamps == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Wenn Effekte mit im Spiel sind, brauchen wir auch eine Liste der 8er-Versionen.
	if(dwFlags & DSBCAPS_CTRLFX)
	{
		m_ppSoundBuffers8 = (LPDIRECTSOUNDBUFFER8*)(tbMemAlloc(dwNumBuffers * sizeof(LPDIRECTSOUNDBUFFER8)));
		if(m_ppSoundBuffers8 == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Wenn es 3D-Schnittstellen sind, brauchen wir noch drei weitere Listen:
	// - eine für die 3D-Soundpufferschnittstellen
	// - Zeitstempel für SetPosition
	// - eine für die ehemaligen Positionen
	if(dwFlags & DSBCAPS_CTRL3D)
	{
		m_pp3DSoundBuffers = (LPDIRECTSOUND3DBUFFER*)(tbMemAlloc(dwNumBuffers * sizeof(LPDIRECTSOUND3DBUFFER)));
		m_pllTimeStamps = (LONGLONG*)(tbMemAlloc(dwNumBuffers * sizeof(LONGLONG)));
		m_pvStartVelocities = (tbVector3*)(tbMemAlloc(dwNumBuffers * sizeof(tbVector3)));
		m_pvOldPositions = (tbVector3*)(tbMemAlloc(dwNumBuffers * sizeof(tbVector3)));
		if(m_pp3DSoundBuffers == NULL || m_pllTimeStamps == NULL || m_pvOldPositions == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Die Flags und die Anzahl der Puffer kopieren
	m_dwFlags = dwFlags;
	m_dwNumBuffers = dwNumBuffers;

	// ------------------------------------------------------------------

	if(!(dwFlags & DSBCAPS_CTRLFX))
	{
		// Bei einem Sound, der ohne Effekte abgespielt wird, reicht es, den Soundpuffer
		// einmal zu erstellen und ihn dann immer wieder zu "klonen".
		m_ppSoundBuffers[0] = tbLoadWAVFile(tbDirectSound::GetDSound(), pVFile, dwFlags, GUID3DAlgorithm);
		if(m_ppSoundBuffers[0] == NULL) TB_ERROR("Fehler beim Laden der WAV-Datei!", TB_ERROR);

		if(m_dwNumBuffers > 1)
		{
			// Die restlichen Puffer klonen
			for(DWORD dwBuffer = 1; dwBuffer < m_dwNumBuffers; dwBuffer++)
			{
				hResult = tbDirectSound::GetDSound()->DuplicateSoundBuffer(m_ppSoundBuffers[0],
										 							       &m_ppSoundBuffers[dwBuffer]);
				if(FAILED(hResult)) TB_ERROR_DIRECTX("tbDirectSound::GetDSound()->DuplicateSoundBuffer",
					                                 hResult, TB_ERROR);
			}
		}
	}
	else
	{
		// Es sind Effekte erwünscht!
		// Dazu brauchen wir nun auch noch die 8er-Schnittstellen und Klonen der
		// Soundpuffer durch DuplicateSoundBuffer ist nicht mehr möglich.

		// Den Cursor der virtuellen Datei speichern, denn wenn wir den Sound mehrfach
		// laden, würde der Cursor über das Ende der Datei hinaus geraten.
		dwVFileCursor = pVFile->GetCursor();

		// Für jeden Puffer die WAV-Datei neu laden
		for(DWORD dwBuffer = 0; dwBuffer < m_dwNumBuffers; dwBuffer++)
		{
			// Cursorposition wiederherstellen
			pVFile->Seek(TB_VFSO_START, dwVFileCursor);

			// WAV-Datei laden
			m_ppSoundBuffers[dwBuffer] = tbLoadWAVFile(tbDirectSound::GetDSound(), pVFile, dwFlags, GUID3DAlgorithm);
			if(m_ppSoundBuffers[dwBuffer] == NULL) TB_ERROR("Fehler beim Laden der WAV-Datei!", TB_ERROR);

			// 8er-Schnittstelle erstellen
			m_ppSoundBuffers[dwBuffer]->QueryInterface(IID_IDirectSoundBuffer8,
				                                       (void**)(&m_ppSoundBuffers8[dwBuffer]));
		}
	}

	// Wenn 3D-Sound erwünscht ist, erstellen wir zu jedem Sound noch eine 3D-Schnittstelle.
	if(dwFlags & DSBCAPS_CTRL3D)
	{
		for(DWORD dwBuffer = 0; dwBuffer < m_dwNumBuffers; dwBuffer++)
		{
			// Schnittstelle abfragen
			m_ppSoundBuffers[dwBuffer]->QueryInterface(IID_IDirectSound3DBuffer,
				                                       (void**)(&m_pp3DSoundBuffers[dwBuffer]));
		}
	}

	return TB_OK;
}

// ******************************************************************
// Initialisierung aus einer echten Datei
tbResult tbSound::Init(char* pcFilename,
					   DWORD dwFlags,			// = DSBCAPS_STATIC | DSBCAPS_LOCDEFER
					   GUID GUID3DAlgorithm,	// = GUID_NULL
					   DWORD dwNumBuffers)		// = 4
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", TB_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Aus Datei laden
	if(pVFile->Init(pcFilename))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, dwFlags, GUID3DAlgorithm, dwNumBuffers))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Sounds!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Initialisierung aus dem Speicher
tbResult tbSound::Init(void* pMemory,
					   DWORD dwMemorySize,
					   DWORD dwFlags,			// = DSBCAPS_STATIC | DSBCAPS_LOCDEFER
					   GUID GUID3DAlgorithm,	// = GUID_NULL
					   DWORD dwNumBuffers)		// = 4
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(pMemory == NULL)		TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);
	if(dwMemorySize == 0)	TB_ERROR_INVALID_VALUE("dwMemorySize", TB_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Aus Speicher erstellen
	if(pVFile->Init(pMemory, dwMemorySize))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, dwFlags, GUID3DAlgorithm, dwNumBuffers))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Sounds!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Initialisierung aus einer Ressource
tbResult tbSound::Init(HMODULE hModule,
					   char* pcResourceName,
					   char* pcResourceType,
					   DWORD dwFlags,			// = DSBCAPS_STATIC | DSBCAPS_LOCDEFER
					   GUID GUID3DAlgorithm,	// = GUID_NULL
					   DWORD dwNumBuffers)		// = 4
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(hModule == NULL)			TB_ERROR_NULL_POINTER("hModule", TB_ERROR);
	if(pcResourceName == NULL)	TB_ERROR_INVALID_VALUE("pcResourceName", TB_ERROR);
	if(pcResourceType == NULL)	TB_ERROR_INVALID_VALUE("pcResourceType", TB_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Aus Ressource erstellen
	if(pVFile->Init(hModule, pcResourceName, pcResourceType))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, dwFlags, GUID3DAlgorithm, dwNumBuffers))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Sounds!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Liefert den ersten verfügbaren Soundpuffer zurück
int tbSound::GetAvailableBuffer()
{
	DWORD dwStatus;

	// Alle Puffer durchgehen
	for(DWORD dwBuffer = 0; dwBuffer < m_dwNumBuffers; dwBuffer++)
	{
		// Wenn der Puffer gerade nicht abgespielt wird, dann ist er frei.
		// Wegen der Latenz darf man sich aber erst auf den Status verlassen,
		// wenn das Abspielen des Sounds eine Weile her ist.
		if(timeGetTime() > m_pdwTimeStamps[dwBuffer] + 50)
		{
			m_ppSoundBuffers[dwBuffer]->GetStatus(&dwStatus);
			if(!(dwStatus & DSBSTATUS_PLAYING)) return (int)(dwBuffer);
		}
	}

	// Kein freier Puffer!
	return -1;
}

// ******************************************************************
// Spielt einen Soundpuffer ab
tbResult tbSound::Play(DWORD dwBuffer,
					   DWORD dwPriority,			// = 0
					   DWORD dwFlags,				// = 0
					   tbVector3 vStartVelocity)	// = tbVector3(0.0f)
{
	HRESULT hResult;

	// Parameter prüfen
	if(dwBuffer >= m_dwNumBuffers) TB_ERROR_INVALID_VALUE("dwBuffer", TB_ERROR);


	// Puffer an den Anfang setzen
	m_ppSoundBuffers[dwBuffer]->SetCurrentPosition(0);

	// Laustärke, Balance und Frequenz zurücksetzen
	if(m_dwFlags & DSBCAPS_CTRLVOLUME) m_ppSoundBuffers[dwBuffer]->SetVolume(0);
	if(m_dwFlags & DSBCAPS_CTRLPAN) m_ppSoundBuffers[dwBuffer]->SetPan(0);
	if(m_dwFlags & DSBCAPS_CTRLFREQUENCY) m_ppSoundBuffers[dwBuffer]->SetFrequency(DSBFREQUENCY_ORIGINAL);

	// Startgeschwindigkeit setzen
	if(m_dwFlags & DSBCAPS_CTRL3D) m_pvStartVelocities[dwBuffer] = vStartVelocity;
	
	// Abspielen
	hResult = m_ppSoundBuffers[dwBuffer]->Play(0, dwPriority, dwFlags);
	if(hResult == DSERR_BUFFERLOST)
	{
		// Puffer ging verloren! Wiederherstellen und erneut abspielen.
		m_ppSoundBuffers[dwBuffer]->Restore();
		hResult = m_ppSoundBuffers[dwBuffer]->Play(0, dwPriority, dwFlags);
	}

	if(FAILED(hResult))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_ppSoundBuffers[dwBuffer]->Play", hResult, TB_ERROR);
	}

	// Zeitstempel zurücksetzen
	if(m_dwFlags & DSBCAPS_CTRL3D) m_pllTimeStamps[dwBuffer] = 0;
	m_bJustStarted = TRUE;

	// DWORD-Zeitstempel setzen
	m_pdwTimeStamps[dwBuffer] = timeGetTime();

	return TB_OK;
}

// ******************************************************************
// Spielt einen verfügbaren Puffer ab und liefert den Index
int tbSound::PlayAvailableBuffer(DWORD dwPriority,			// = 0
								 DWORD dwFlags,				// = 0
								 tbVector3 vStartVelocity)	// = tbVector3(0.0f)
{
	// Freien Puffer finden
	int iBuffer = GetAvailableBuffer();
	if(iBuffer == -1) return -1;

	// Die normale Abspielmethode aufrufen
	if(Play((DWORD)(iBuffer), dwPriority, dwFlags, vStartVelocity)) return -1;

	return iBuffer;
}

// ******************************************************************
// Stoppt einen Soundpuffer
tbResult tbSound::Stop(DWORD dwBuffer)
{
	// Parameter prüfen
	if(dwBuffer >= m_dwNumBuffers) TB_ERROR_INVALID_VALUE("dwBuffer", TB_ERROR);


	// Puffer stoppen
	m_ppSoundBuffers[dwBuffer]->Stop();

	return TB_OK;
}

// ******************************************************************
// Stoppt alle Soundpuffer
tbResult tbSound::StopAll()
{
	// Alle Puffer durchgehen und anhalten
	for(DWORD dwBuffer = 0; dwBuffer < m_dwNumBuffers; dwBuffer++)
	{
		// Anhalten
		m_ppSoundBuffers[dwBuffer]->Stop();
	}

	return TB_OK;
}

// ******************************************************************
// Setzt die Position eines Sounds und berechnet die Geschwindigkeit
tbResult tbSound::SetPosition(const DWORD dwBuffer,
							  const tbVector3& vPosition,
							  const float fSpeedFactor)	// = 1.0f
{
	// Parameter prüfen
	if(dwBuffer >= m_dwNumBuffers) TB_ERROR_INVALID_VALUE("dwBuffer", TB_ERROR);

	// Prüfen, ob es überhaupt ein 3D-Sound ist
	if(!(m_dwFlags & DSBCAPS_CTRL3D)) TB_ERROR("Dies ist kein 3D-Sound!", TB_ERROR);
	LPDIRECTSOUND3DBUFFER pSoundBuffer(m_pp3DSoundBuffers[dwBuffer]);


	// Position setzen
	pSoundBuffer->SetPosition(vPosition.x, vPosition.y, vPosition.z, DS3D_DEFERRED);

	// Den Geschwindigkeitsvektor berechnen.
	// m_pllTimeStamps[dwBuffer] enthält den Zeitpunkt, zu dem SetListener das letzte
	// Mal aufgerufen wurde. Ist m_bJustStarted TRUE, dann ist das hier das erste Mal
	// und die Geschwindigkeit wird auf null gesetzt.
	if(m_bJustStarted)
	{
		pSoundBuffer->SetVelocity(m_pvStartVelocities[dwBuffer].x,
			                      m_pvStartVelocities[dwBuffer].y,
								  m_pvStartVelocities[dwBuffer].z,
								  DS3D_DEFERRED);

		// Aktuelle Zeit und aktuelle Position als ehemalige eintragen
		QueryPerformanceCounter((LARGE_INTEGER*)(&m_pllTimeStamps[dwBuffer]));
		m_pvOldPositions[dwBuffer] = vPosition;

		m_bJustStarted = FALSE;
	}
	else
	{
		LONGLONG llCurrentTime;

		// Differenz zwischen aktueller und alter Zeit berechnen
		QueryPerformanceCounter((LARGE_INTEGER*)(&llCurrentTime));
		double dNumSecsPassed = (double)(llCurrentTime - m_pllTimeStamps[dwBuffer]) / tb_g_dFrequency;

		// Wenn diese Zeit nur knapp von der Zeit seit dem letzten Frame abweicht,
		// wird dies als kleiner Fehler gewertet und die tatsächliche Zeit wird benutzt.
		if(fabs(dNumSecsPassed - tb_g_dNumSecsPassed) < 0.1) dNumSecsPassed = tb_g_dNumSecsPassed;

		// Zurückgelegte Strecke durch die Zeit teilen. Dadurch erhalten
		// wir die Geschwindigkeit.
		tbVector3 vVelocity(((vPosition - m_pvOldPositions[dwBuffer]) / (float)(dNumSecsPassed)) * fSpeedFactor);
		pSoundBuffer->SetVelocity(vVelocity.x, vVelocity.y, vVelocity.z, DS3D_DEFERRED);

		// Aktuelle Zeit und aktuelle Position als ehemalige eintragen
		m_pllTimeStamps[dwBuffer] = llCurrentTime;
		m_pvOldPositions[dwBuffer] = vPosition;
	}

	return TB_OK;
}

// ******************************************************************
// Setzt minimale und maximale Entfernung
tbResult tbSound::SetDistances(float fMinDistance,
							   float fMaxDistance)
{
	// Ist es überhaupt ein 3D-Sound?
	if(!(m_dwFlags & DSBCAPS_CTRL3D)) TB_ERROR("Dies ist kein 3D-Sound!", TB_ERROR);

	// Distanzen für alle 3D-Soundpuffer setzen
	for(DWORD dwBuffer = 0; dwBuffer < m_dwNumBuffers; dwBuffer++)
	{
		// Distanzen setzen
		m_pp3DSoundBuffers[dwBuffer]->SetMinDistance(fMinDistance, DS3D_IMMEDIATE);
		m_pp3DSoundBuffers[dwBuffer]->SetMaxDistance(fMaxDistance, DS3D_IMMEDIATE);
	}

	return TB_OK;
}

// ******************************************************************
// Liefert den Status eines Soundpuffers
DWORD tbSound::GetStatus(const DWORD dwBuffer)
{
	DWORD dwStatus;

	// Parameter prüfen
	if(dwBuffer >= m_dwNumBuffers) TB_ERROR_INVALID_VALUE("dwBuffer", TB_ERROR);


	// Wegen der Latenz kann man sich erst nach kurzer Zeit auf den
	// Status verlassen.
	if(timeGetTime() >= m_pdwTimeStamps[dwBuffer] &&
       timeGetTime() < m_pdwTimeStamps[dwBuffer] + 50) return DSBSTATUS_PLAYING;

	// Status abfragen und liefern
	m_ppSoundBuffers[dwBuffer]->GetStatus(&dwStatus);
	return dwStatus;
}

// ******************************************************************