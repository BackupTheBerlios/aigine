/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbMusic.cpp
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Laden und Abspielen von Musikdateien

	Autor:
	David Scherfgen

	Bearbeitet:
	Snorky
	[21.8.03]

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbMusic-Klasse
tbMusic::tbMusic()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbMusic));
}

// ******************************************************************
// Destruktor der tbMusic-Klasse
tbMusic::~tbMusic()
{
	Exit();
}

// ******************************************************************
// Alles freigeben und zurücksetzen
tbResult tbMusic::Exit()
{
	// Wiedergabe stoppen
	if(IsPlaying()) Stop();

	// Alle Schnittstellen freigeben
	TB_SAFE_RELEASE(m_pMediaControl);
	TB_SAFE_RELEASE(m_pMediaSeeking);
	TB_SAFE_RELEASE(m_pBasicAudio);
	TB_SAFE_RELEASE(m_pGraph);

	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbMusic));

	return TB_OK;
}

// ******************************************************************
// Lädt die Musik aus einer Datei
tbResult tbMusic::Init(char* pcFilename)
{
	// Alles zurücksetzen
	Exit();

	HRESULT		hResult;
	LONGLONG	llDuration;
	WCHAR		awcFilename[256];

	// Graph erstellen
	hResult = CoCreateInstance(CLSID_FilterGraph,
		                       NULL,
							   CLSCTX_INPROC_SERVER,
							   IID_IGraphBuilder,
							   (void**)&m_pGraph);
	if(FAILED(hResult))
	{
		// Fehler!
		TB_ERROR_DIRECTX("CoCreateInstance", hResult, TB_ERROR);
	}

	// Graph für die Datei erstellen
	MultiByteToWideChar(CP_ACP, 0, pcFilename, -1, awcFilename, 256);
	if(FAILED(hResult = m_pGraph->RenderFile(awcFilename, NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pGraph->RenderFile", hResult, TB_ERROR);
	}

	// Die drei Schnittstellen für die Kontrolle und das Suchen in der Musik abfragen
	m_pGraph->QueryInterface(IID_IMediaControl, (void**)(&m_pMediaControl));
	m_pGraph->QueryInterface(IID_IMediaSeeking, (void**)(&m_pMediaSeeking));
	m_pGraph->QueryInterface(IID_IBasicAudio, (void**)(&m_pBasicAudio));
	if(m_pMediaControl == NULL ||
	   m_pMediaSeeking == NULL ||
	   m_pBasicAudio == NULL)
	{
		// Fehler!
		TB_ERROR("Fehler beim Abfragen der DirectShow-Musikschnittstellen!", TB_ERROR);
	}

	// Zeitformat setzen
	m_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);

	// Die Länge der Musik abfragen
	m_pMediaSeeking->GetDuration(&llDuration);
	m_dwDuration = (DWORD)(llDuration / 10000);

	return TB_OK;
}

// ******************************************************************
// Spielt die Musik ab
tbResult tbMusic::Play(BOOL bLooping) // = TRUE
{
	// Musik zurücksetzen
	SetCurrentPosition(0);

	// Musik abspielen
	HRESULT hResult = m_pMediaControl->Run();
	if(FAILED(hResult))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pMediaControl->Run", hResult, TB_ERROR);
	}

	// Looping kopieren
	SetLooping(bLooping);

	m_bPlaying = TRUE;

	return TB_OK;
}

// ******************************************************************
// Pausiert die Musik
tbResult tbMusic::Pause()
{
	// Pause
	m_pMediaControl->Pause();

	return TB_OK;
}

// ******************************************************************
// Stoppt die Musik
tbResult tbMusic::Stop()
{
	// Musik stoppen
	m_pMediaControl->Stop();
	m_bPlaying = FALSE;

	return TB_OK;
}

// ******************************************************************
// Fragt den Status der Musik ab
OAFilterState tbMusic::GetState()
{
	OAFilterState Temp;

	m_pMediaControl->GetState(10, &Temp);
	return Temp;
}

// ******************************************************************
// Verarbeitet die Musik
tbResult tbMusic::Process()
{
	if(m_bPlaying)
	{
		if(GetCurrentPosition() >= (int)(m_dwDuration))
		{
			if(m_bLooping)
			{
				// Wenn Looping eingeschaltet ist und die Musik zu 
				// Ende ist, dann spielen wir sie erneut ab.
				return Play(TRUE);
			}
			else
			{
				// Ansonsten wird nicht mehr gespielt
				m_bPlaying = FALSE;
				return TB_OK;
			}
		}
	}

	return TB_OK;
}

// ******************************************************************
// Setzt die aktuelle Position
tbResult tbMusic::SetCurrentPosition(DWORD dwPosition)
{
	LONGLONG llPosition;

	// Neue Position setzen
	llPosition = (LONGLONG)(dwPosition) * 10000;
	HRESULT hResult = m_pMediaSeeking->SetPositions(&llPosition, AM_SEEKING_AbsolutePositioning,
													NULL, AM_SEEKING_NoPositioning);
	if(FAILED(hResult))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pMediaSeeking->SetPositions", hResult, TB_ERROR);
	}

	return TB_OK;
}

// ******************************************************************
// Fragt die aktuelle Position ab
int tbMusic::GetCurrentPosition()
{
	LONGLONG llPosition;

	// Position abfragen
	HRESULT hResult = m_pMediaSeeking->GetPositions(&llPosition, NULL);
	if(FAILED(hResult))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pMediaSeeking->GetPositions", hResult, -1);
	}

	// Position zurückliefern
	return (int)(llPosition / 10000);
}

// ******************************************************************
// Setzt die Abspielgeschwindigkeit
tbResult tbMusic::SetPlaybackRate(float fRate)
{
	// Geschwindigkeit setzen
	HRESULT hResult = m_pMediaSeeking->SetRate((double)(fRate));
	if(FAILED(hResult))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pMediaSeeking->SetRate", hResult, TB_ERROR);
	}

	return TB_OK;
}

// ******************************************************************
// Fragt die Abspielgeschwindigkeit ab
float tbMusic::GetPlaybackRate()
{
	double dRate;

	// Geschwindigkeit abfragen
	HRESULT hResult = m_pMediaSeeking->GetRate(&dRate);
	if(FAILED(hResult))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pMediaSeeking->GetRate", hResult, -1.0f);
	}

	return (float)(dRate);
}

// ******************************************************************