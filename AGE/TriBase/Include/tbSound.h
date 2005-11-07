/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbSound.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Einfacher Umgang mit Sounds

	Autor:
	David Scherfgen

********************************************************************/

#pragma pack(1)

// Struktur für den Header einer RIFF-Datei
struct TRIBASE_API tbRIFFHeader
{
	char	acRIFF[4];		// = "RIFF"
	DWORD	dwSize;			// Dateigröße - 8 Bytes
	char	acFormat[4];	// Zum Beispiel "WAVE"
};

// ******************************************************************
// Allgemeiner WAV-Chunk-Header
struct TRIBASE_API tbWAVChunkHeader
{
	char	acType[4];	// Typ des Chunks (FOURCC)
	DWORD	dwDataSize;	// Größe der folgenden Chunkdaten
};

// ******************************************************************
// Die tbSound-Klasse
class TRIBASE_API tbSound
{
private:
	// Variablen
	LPDIRECTSOUNDBUFFER*	m_ppSoundBuffers;		// Die Soundpuffer
	LPDIRECTSOUNDBUFFER8*	m_ppSoundBuffers8;		// Die 8er-Soundpuffer
	LPDIRECTSOUND3DBUFFER*	m_pp3DSoundBuffers;		// Die 3D-Soundpuffer
	tbVector3*				m_pvStartVelocities;	// Startgeschwindigkeiten
	tbVector3*				m_pvOldPositions;		// Ehemalige Positionen
	LONGLONG*				m_pllTimeStamps;		// Zeitstempel der SetPosition-Methode für jeden Puffer
	DWORD*					m_pdwTimeStamps;		// Zeitstempel
	DWORD					m_dwNumBuffers;			// Anzahl der Soundpuffer
	DWORD					m_dwFlags;				// Erstellungsflags
	BOOL					m_bJustStarted;			// Hast der Sound gerade erst angefangen?

public:
	// Konstruktor und Destruktor
	tbSound();
	~tbSound();

	// Methoden
	tbResult	Init(tbVFile* pVFile, DWORD dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCDEFER, GUID GUID3DAlgorithm = GUID_NULL, DWORD dwNumBuffers = 4);												// Lädt den Sound aus einer virtuellen Datei
	tbResult	Init(char* pcFilename, DWORD dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCDEFER, GUID GUID3DAlgorithm = GUID_NULL, DWORD dwNumBuffers = 4);											// Lädt den Sound aus einer Datei
	tbResult	Init(void* pMemory, DWORD dwMemorySize, DWORD dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCDEFER, GUID GUID3DAlgorithm = GUID_NULL, DWORD dwNumBuffers = 4);							// Lädt den Sound aus dem Speicher
	tbResult	Init(HMODULE hModule, char* pcResourceName, char* pcResourceType, DWORD dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCDEFER, GUID GUID3DAlgorithm = GUID_NULL, DWORD dwNumBuffers = 4);	// Lädt den Sound aus einer Ressource
	int			GetAvailableBuffer();																																							// Findet den ersten verfügbaren Soundpuffer
	tbResult	Play(DWORD dwBuffer, DWORD dwPriority = 0, DWORD dwFlags = 0, tbVector3 vStartVelocity = tbVector3(0.0f));																		// Spielt einen Sound ab
	int			PlayAvailableBuffer(DWORD dwPriority = 0, DWORD dwFlags = 0, tbVector3 vStartVelocity = tbVector3(0.0f));																		// Spielt den ersten verfügbaren Soundpuffer ab
	tbResult	Stop(DWORD dwBuffer);																																							// Hält einen Sound an
	tbResult	StopAll();																																										// Hält alle Soundpuffer an
	tbResult	SetPosition(const DWORD dwBuffer, const tbVector3& vPosition, const float fSpeedFactor = 1.0f);																					// Setzt die Position eines Sounds und berechnet den Geschwindigkeitsvektor
	tbResult	SetDistances(float fMinDistance, float fMaxDistance);																															// Setzt minimale und maximale Entfernung
	DWORD		GetStatus(const DWORD dwBuffer);																																				// Liefert den Status eines Soundpuffers

	// Inline-Methoden
	inline LPDIRECTSOUNDBUFFER		GetSoundBuffer(DWORD dwBuffer)					{return m_ppSoundBuffers[dwBuffer];}
	inline LPDIRECTSOUNDBUFFER8		GetSoundBuffer8(DWORD dwBuffer)					{return m_ppSoundBuffers8[dwBuffer];}
	inline LPDIRECTSOUND3DBUFFER	Get3DSoundBuffer(DWORD dwBuffer)				{return m_pp3DSoundBuffers[dwBuffer];}
	inline tbVector3				GetOldPosition(DWORD dwBuffer)					{return m_pvOldPositions[dwBuffer];}
	inline tbResult					SetVolume(DWORD dwBuffer, LONG lVolume)			{HRESULT r = m_ppSoundBuffers[dwBuffer]->SetVolume(lVolume); return FAILED(r) ? TB_ERROR : TB_OK;}
	inline LONG						GetVolume(DWORD dwBuffer)						{LONG lVolume = 0; m_ppSoundBuffers[dwBuffer]->GetVolume(&lVolume); return lVolume;}
	inline tbResult					SetPan(DWORD dwBuffer, LONG lPan)				{HRESULT r = m_ppSoundBuffers[dwBuffer]->SetPan(lPan); return FAILED(r) ? TB_ERROR : TB_OK;}
	inline LONG						GetPan(DWORD dwBuffer)							{LONG lPan = 0; m_ppSoundBuffers[dwBuffer]->GetPan(&lPan); return lPan;}
	inline tbResult					SetFrequency(DWORD dwBuffer, DWORD dwFrequency)	{HRESULT r = m_ppSoundBuffers[dwBuffer]->SetFrequency(dwFrequency); return FAILED(r) ? TB_ERROR : TB_OK;}
	inline DWORD					GetFrequency(DWORD dwBuffer)					{DWORD dwFrequency = 0; m_ppSoundBuffers[dwBuffer]->GetFrequency(&dwFrequency); return dwFrequency;}
	inline DWORD					GetFlags()										{return m_dwFlags;}
};

// ******************************************************************