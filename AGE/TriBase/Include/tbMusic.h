/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbMusik.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Laden und Abspielen von Musikdateien

	Autor:
	David Scherfgen

	Bearbeitet:
	Snorky
	[21.8.03]

********************************************************************/

// Die tbMusic-Klasse
class TRIBASE_API tbMusic
{
private:
	// Variablen
	IGraphBuilder*	m_pGraph;
	IMediaControl*	m_pMediaControl;
	IMediaSeeking*	m_pMediaSeeking;
	IBasicAudio*	m_pBasicAudio;
	BOOL			m_bPlaying;
	BOOL			m_bLooping;
	DWORD			m_dwDuration;

public:
	// Konstruktor und Destruktor
	tbMusic();
	~tbMusic();

	// Methoden
	tbResult		Exit();									// Alles freigeben und zurücksetzen
	tbResult		Init(char* pcFilename);					// Lädt die Musik aus einer Datei
	tbResult		Play(BOOL bLooping = TRUE);				// Spielt die Musik ab
	tbResult		Pause();								// Pausiert die Musik
	tbResult		Stop();									// Stoppt die Musik
	OAFilterState	GetState();								// Fragt den aktuellen Status ab
	tbResult		Process();								// Erledigt z.B. das Looping
	tbResult		SetCurrentPosition(DWORD dwPosition);	// Setzt die aktuelle Position
	int				GetCurrentPosition();					// Fragt die aktuelle Position ab	
	tbResult		SetPlaybackRate(float fRate);			// Setzt die Wiedergabegeschwindigkeit
	float			GetPlaybackRate();						// Fragt die Wiedergabegeschwindigkeit ab

	// Inline-Methoden
	inline IGraphBuilder*	GetGraph()					{return m_pGraph;}
	inline IMediaControl*	GetMediaControl()			{return m_pMediaControl;}
	inline IMediaSeeking*	GetMediaSeeking()			{return m_pMediaSeeking;}
	inline IBasicAudio*		GetBasicAudio()				{return m_pBasicAudio;}
	inline void				SetLooping(BOOL bLooping)	{m_bLooping = bLooping;}
	inline BOOL				GetLooping()				{return m_bLooping;}
	inline DWORD			GetDuration()				{return m_dwDuration;}
	inline void				SetVolume(LONG lVolume)		{m_pBasicAudio->put_Volume(lVolume);}
	inline LONG				GetVolume()					{static LONG lVolume; m_pBasicAudio->get_Volume(&lVolume); return lVolume;}
	inline void				SetPan(LONG lPan)			{m_pBasicAudio->put_Balance(lPan);}
	inline LONG				GetPan()					{static LONG lPan; m_pBasicAudio->get_Balance(&lPan); return lPan;}
	inline BOOL				IsPlaying()					{return m_bPlaying;}
};

// ******************************************************************