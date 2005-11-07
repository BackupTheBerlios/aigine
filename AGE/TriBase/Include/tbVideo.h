/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVideo.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Abspielen von Videos auf einer Textur

	Autor:
	David Scherfgen

	Bearbeitet:
	Snorky
	[24.9.03]

********************************************************************/

// GUID für den Video-Renderer
struct __declspec(uuid("{16231934-1369-32FA-AA31-1AAA4BE1277F}")) CLSID_TriBaseVideoRenderer;

// ******************************************************************
// Klasse für den Video-Renderer
class tbVideoRenderer : public CBaseVideoRenderer
{
public:
	// Variablen
	PDIRECT3DTEXTURE9	m_pTexture;
	D3DSURFACE_DESC		m_TexDesc;
	VIDEOINFO			m_VideoInfo;
	tbVector2			m_vBottomRightTex;
	BOOL				m_bTexLocked;

	// Konstruktor und Destruktor
    tbVideoRenderer(LPUNKNOWN pUnk, HRESULT* phResult);
    ~tbVideoRenderer();

	// Methoden für CBaseVideoRenderer
    HRESULT CheckMediaType(const CMediaType* pMediaType);	// Testen eines Formats
    HRESULT SetMediaType(const CMediaType* pMediaType);		// Format wurde festgelegt
    HRESULT DoRenderSample(IMediaSample* pMediaSample);		// Neues Sample kommt an

	// Inline-Methoden
	inline PDIRECT3DTEXTURE9	GetTexture()		{return m_pTexture;}
	inline D3DSURFACE_DESC		GetTextureDesc()	{return m_TexDesc;}
	inline VIDEOINFO			GetVideoInfo()		{return m_VideoInfo;}
	inline tbVector2			GetBottomRightTex()	{return m_vBottomRightTex;}
	inline BOOL					IsTextureLocked()	{return m_bTexLocked;}
};

// ******************************************************************
// Vertexformat
#define TB_VIDEO_FVF (D3DFVF_XYZRHW | D3DFVF_TEX1)
struct SVideoVertex
{
	tbVector3	vPosition;
	float		fRHW;
	tbVector2	vTex0;
};

// ******************************************************************
// Klasse für ein Video
class TRIBASE_API tbVideo
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
	IBaseFilter*	m_pRenderer;
	DWORD			m_dwOldFVF;

public:
	// Konstruktor und Destruktor
	tbVideo();
	~tbVideo();

	// Methoden
	tbResult		Exit();									// Alles freigeben
	tbResult		Init(char* pcFilename);					// Lädt das Video aus einer Datei
	tbResult		Play(BOOL bLooping = TRUE);				// Spielt das Video ab
	tbResult		Pause();								// Pausiert das Video
	tbResult		Stop();									// Stoppt das Video
	OAFilterState	GetState();								// Fragt den aktuellen Status ab
	tbResult		Process();								// Erledigt z.B. das Looping
	tbResult		SetCurrentPosition(DWORD dwPosition);	// Setzt die aktuelle Position
	int				GetCurrentPosition();					// Fragt die aktuelle Position ab	
	tbResult		SetPlaybackRate(float fRate);			// Setzt die Wiedergabegeschwindigkeit
	float			GetPlaybackRate();						// Fragt die Wiedergabegeschwindigkeit ab
	tbResult		DrawVideo(tbVector4 vRect = tbVector4(0.0f));	// Zeichnet das Video auf den Monitor
	
	// Inline-Methoden
	inline IGraphBuilder*		GetGraph()					{return m_pGraph;}
	inline IMediaControl*		GetMediaControl()			{return m_pMediaControl;}
	inline IMediaSeeking*		GetMediaSeeking()			{return m_pMediaSeeking;}
	inline IBasicAudio*			GetBasicAudio()				{return m_pBasicAudio;}
	inline void					SetLooping(BOOL bLooping)	{m_bLooping = bLooping;}
	inline BOOL					GetLooping()				{return m_bLooping;}
	inline DWORD				GetDuration()				{return m_dwDuration;}
	inline void					SetVolume(LONG lVolume)		{if(m_pBasicAudio != NULL) m_pBasicAudio->put_Volume(lVolume);}
	inline LONG					GetVolume()					{if(m_pBasicAudio != NULL) {static LONG lVolume; m_pBasicAudio->get_Volume(&lVolume); return lVolume;} else return -1;}
	inline void					SetPan(LONG lPan)			{if(m_pBasicAudio != NULL) m_pBasicAudio->put_Balance(lPan);}
	inline LONG					GetPan()					{if(m_pBasicAudio != NULL) {static LONG lPan; m_pBasicAudio->get_Balance(&lPan); return lPan;} else return -1;}
	inline tbVideoRenderer*		GetRenderer()				{return (tbVideoRenderer*)(m_pRenderer);}
	inline BOOL					IsPlaying()					{return m_bPlaying;}
};

// ******************************************************************
