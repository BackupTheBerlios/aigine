/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbDirectSound.h
	===============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Einfacher Umgang mit DirectSound

	Autor:
	David Scherfgen

********************************************************************/

// Die tbDirectSound-Klasse
class TRIBASE_API tbDirectSound
{
private:
	// Variablen
	static BOOL						m_bInitialized;			// Initialisiert?
	static LPDIRECTSOUND8			m_pDSound;				// IDirectSound8-Schnittstelle
	static LPDIRECTSOUNDBUFFER		m_pPrimaryBuffer;		// Primärer Soundpuffer
	static LPDIRECTSOUND3DLISTENER8	m_pListener;			// Hörerschnittstelle
	static tbVector3				m_vOldListenerPos;		// Ehemalige Position des Hörers
	static LONGLONG					m_llListenerTimeStamp;	// Zeitstempel der SetListener-Methode

public:
	// Methoden
	static tbResult Init(tbConfig* pConfig, HWND hWindow = NULL, DWORD dwCoopLevel = DSSCL_PRIORITY, BOOL b3D = TRUE);												// Initialisierung
	static tbResult Exit();																																			// Herunterfahren
	static tbResult SetListener(const tbVector3& vListenerPos, const tbVector3& vListenerYAxis, const tbVector3& vListenerZAxis, const float fSpeedFactor = 1.0f);	// Aktualisieren des Hörers

	// Inline-Methoden
	static inline BOOL						IsInitialized()				{return m_bInitialized;}
	static inline LPDIRECTSOUND8			GetDSound()					{return m_pDSound;}
	static inline LPDIRECTSOUNDBUFFER		GetPrimaryBuffer()			{return m_pPrimaryBuffer;}
	static inline LPDIRECTSOUND3DLISTENER8	GetListener()				{return m_pListener;}
	static inline tbVector3					GetOldListenerPosition()	{return m_vOldListenerPos;}
};

// ******************************************************************