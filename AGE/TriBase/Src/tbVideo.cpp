/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVideo.cpp
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Abspielen von Videos auf Texturen

	Autor:
	David Scherfgen

	Bearbeitet:
	Snorky
	[24.9.03]

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbVideoRenderer-Klasse
tbVideoRenderer::tbVideoRenderer(LPUNKNOWN pUnk,
								 HRESULT* phResult) : CBaseVideoRenderer(__uuidof(CLSID_TriBaseVideoRenderer),
																		 "TriBase Video Renderer", pUnk, phResult)
{
	// Alles zurücksetzen
	m_pTexture = NULL;
	ZeroMemory(&m_VideoInfo, sizeof(VIDEOINFO));
	ZeroMemory(&m_TexDesc, sizeof(D3DSURFACE_DESC));
	m_vBottomRightTex = tbVector2(0.0f);

	*phResult = S_OK;
}

// ******************************************************************
// Destruktor der tbVideoRenderer-Klasse
tbVideoRenderer::~tbVideoRenderer()
{
	// Textur freigeben
	TB_SAFE_RELEASE(m_pTexture);
}

// ******************************************************************
// Überprüfen eines Formats
HRESULT tbVideoRenderer::CheckMediaType(const CMediaType* pMediaType)
{
	// Es muss ein Videoformat sein!
	if(*(pMediaType->FormatType()) != FORMAT_VideoInfo) return E_INVALIDARG;

	// Wir akzeptieren nur RGB24-Videoformate.
	if(IsEqualGUID(*(pMediaType->Type()), MEDIATYPE_Video) &&
	   IsEqualGUID(*(pMediaType->Subtype()), MEDIASUBTYPE_RGB24))
	{
		return S_OK;
	}

	return E_INVALIDARG;
}

// ******************************************************************
// Format wurde festgelegt
HRESULT tbVideoRenderer::SetMediaType(const CMediaType* pMediaType)
{
	HRESULT			r;
	D3DCAPS9		D3DCaps;
	int				iVideoWidth;
	int				iVideoHeight;
	int				iTexWidth;
	int				iTexHeight;
	D3DFORMAT		aFormat[6] = {D3DFMT_X8R8G8B8,
								  D3DFMT_A8R8G8B8,
								  D3DFMT_R5G6B5,
								  D3DFMT_X1R5G5B5,
								  D3DFMT_A1R5G5B5,
								  D3DFMT_R8G8B8};
	D3DLOCKED_RECT	LockedRect;


	// Videoinfos kopieren
	m_VideoInfo = *((VIDEOINFO*)(pMediaType->Format()));

	// Breite und Höhe des Videos kopieren
	iVideoWidth = m_VideoInfo.bmiHeader.biWidth;
	iVideoHeight = m_VideoInfo.bmiHeader.biHeight;

	// Die Caps der Direct3D-Geräteschnittstelle abfragen
	D3DCaps = tbDirect3D::GetCaps();

	// Unterstützt das Gerät Texturen, deren Breite und Höhe keine 2er-Potenzen sind?
	if(D3DCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL)
	{
		// Ja! Dann setzen wir Breite und Höhe auf die Dimensionen des Videos.
		iTexWidth = iVideoWidth;
		iTexHeight = iVideoHeight;
	}
	else
	{
		// Breite und Höhe müssen 2er-Potenzen sein. Wir suchen jeweils die nächste.
		if(iVideoWidth > 4096) iTexWidth = 4096;
		if(iVideoWidth < 4096) iTexWidth = 4096;
		if(iVideoWidth < 2048) iTexWidth = 2048;
		if(iVideoWidth < 1024) iTexWidth = 1024;
		if(iVideoWidth < 512) iTexWidth = 512;
		if(iVideoWidth < 256) iTexWidth = 256;
		if(iVideoWidth < 128) iTexWidth = 128;
		if(iVideoWidth < 64) iTexWidth = 64;
		if(iVideoWidth < 32) iTexWidth = 32;
		if(iVideoWidth < 16) iTexWidth = 16;
		if(iVideoWidth < 8) iTexWidth = 8;
		if(iVideoWidth < 4) iTexWidth = 4;
		if(iVideoHeight > 4096) iTexHeight = 4096;
		if(iVideoHeight < 4096) iTexHeight = 4096;
		if(iVideoHeight < 2048) iTexHeight = 2048;
		if(iVideoHeight < 1024) iTexHeight = 1024;
		if(iVideoHeight < 512) iTexHeight = 512;
		if(iVideoHeight < 256) iTexHeight = 256;
		if(iVideoHeight < 128) iTexHeight = 128;
		if(iVideoHeight < 64) iTexHeight = 64;
		if(iVideoHeight < 32) iTexHeight = 32;
		if(iVideoHeight < 16) iTexHeight = 16;
		if(iVideoHeight < 8) iTexHeight = 8;
		if(iVideoHeight < 4) iTexHeight = 4;
	}

	// Müssen alle Texturen quadratisch sein? Falls ja, dann setzen wir Breite
	// und Höhe auf das Maximum. 256x512 wird dann zum Beispiel zu 512x512.
	if(D3DCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY)
	{
		iTexWidth = TB_MAX(iTexWidth, iTexHeight);
		iTexHeight = iTexWidth;
	}

	// Alle sechs unterstützten Formate durchgehen und ausprobieren
	for(int i = 0; i < 6; i++)
	{
		// Textur erstellen. Wenn möglich, eine dynamische.
		if(D3DCaps.Caps2 & D3DCAPS2_DYNAMICTEXTURES)
		{
			// Dynamische Textur erstellen
			r = tbDirect3D::GetDevice()->CreateTexture(iTexWidth, iTexHeight, 1,
				                                       D3DUSAGE_DYNAMIC, aFormat[i], D3DPOOL_DEFAULT,
													   &m_pTexture, NULL);
			if(FAILED(r))
			{
				// Statische Textur erstellen
				r = tbDirect3D::GetDevice()->CreateTexture(iTexWidth, iTexHeight, 1,
					                                       0, aFormat[i], D3DPOOL_MANAGED,
														   &m_pTexture, NULL);
			}
		}
		else
		{
			// Statische Textur erstellen
			r = tbDirect3D::GetDevice()->CreateTexture(iTexWidth, iTexHeight, 1,
				                                       0, aFormat[i], D3DPOOL_MANAGED,
													   &m_pTexture, NULL);
		}

		// Hat es funktioniert?
		if(SUCCEEDED(r))
		{
			// Ja! Abbrechen.
			break;
		}
	}

	// Gab es einen Fehler?
	if(FAILED(r))
	{
		// Ja! Es wurde kein Texturformat gefunden.
		TB_ERROR("Es wurde kein unterstütztes Texturformat gefunden!", E_FAIL);
	}

	// Beschreibung der Textur abfragen
	m_pTexture->GetLevelDesc(0, &m_TexDesc);

	// Überprüfen, ob das Format in Ordnung ist
	if(m_TexDesc.Format != D3DFMT_R8G8B8 &&
	   m_TexDesc.Format != D3DFMT_A8R8G8B8 &&
	   m_TexDesc.Format != D3DFMT_X8R8G8B8 &&
	   m_TexDesc.Format != D3DFMT_R5G6B5 &&
	   m_TexDesc.Format != D3DFMT_A1R5G5B5 &&
	   m_TexDesc.Format != D3DFMT_X1R5G5B5)
	{
		// Falsches Format!
		TB_SAFE_RELEASE(m_pTexture);
		TB_ERROR("Die Textur hat ein nicht unterstütztes Format!", E_FAIL);
	}

	// Texturkoordinaten für die rechte untere Ecke des Videos berechnen
	m_vBottomRightTex.x = (float)(iVideoWidth) / (float)(m_TexDesc.Width);
	m_vBottomRightTex.y = (float)(iVideoHeight) / (float)(m_TexDesc.Height);

	// Die Textur sperren und mit schwarzer Farbe füllen
	m_pTexture->LockRect(0, &LockedRect, NULL, 0);
	for(DWORD y = 0; y < m_TexDesc.Height; y++) ZeroMemory((BYTE*)(LockedRect.pBits) + y * LockedRect.Pitch, LockedRect.Pitch);
	m_pTexture->UnlockRect(0);

	return S_OK;
}

// ******************************************************************
// Videodaten kommen an und müssen verarbeitet werden
HRESULT tbVideoRenderer::DoRenderSample(IMediaSample* pSample)
{
	HRESULT r;

	if(pSample == NULL) return E_FAIL;
	BYTE* pTemp;
	pSample->GetPointer(&pTemp); if(pTemp == NULL) return E_FAIL;

	// Breite und Höhe des Videos kopieren
	DWORD dwVideoWidth = m_VideoInfo.bmiHeader.biWidth;
	DWORD dwVideoHeight = m_VideoInfo.bmiHeader.biHeight;

	// Texturbereich sperren, auf dem das Video abgespielt wird
	RECT Rect;
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = dwVideoWidth - 1;
	Rect.bottom = dwVideoHeight - 1;

	m_bTexLocked = TRUE;

	BOOL bDiscard = m_TexDesc.Usage & D3DUSAGE_DYNAMIC;
	D3DLOCKED_RECT LockedRect;
	if(FAILED(r = m_pTexture->LockRect(0, &LockedRect, bDiscard ? NULL : &Rect,
		                               bDiscard ? D3DLOCK_DISCARD : 0)))
	{
		// Fehler beim Sperren!
		m_bTexLocked = FALSE;
		TB_ERROR_DIRECTX("m_pTexture->LockRect", r, E_FAIL);
	}

	// Daten kopieren
	if(m_TexDesc.Format == D3DFMT_R8G8B8)
	{
		BYTE* pVideo;
		pSample->GetPointer(&pVideo);
		pVideo += dwVideoWidth * (dwVideoHeight - 1) * 3;
		BYTE* pTexture = (BYTE*)(LockedRect.pBits);
		DWORD dwPitch = LockedRect.Pitch;

		for(DWORD y = 0; y < dwVideoHeight; y++)
		{
			memcpy(pTexture, pVideo, dwVideoWidth * 3);
			pVideo -= dwVideoWidth * 3;
			pTexture += dwPitch;
		}
	}
	else if(m_TexDesc.Format == D3DFMT_X8R8G8B8)
	{
		BYTE* pVideo;
		pSample->GetPointer(&pVideo);
		pVideo += dwVideoWidth * (dwVideoHeight - 1) * 3;
		DWORD* pdwTexture = (DWORD*)(LockedRect.pBits);
		DWORD dwPitch = LockedRect.Pitch / 4;

		if(!(dwVideoWidth % 4))
		{
			for(DWORD y = 0; y < dwVideoHeight; y++)
			{
				for(DWORD x = 0; x < dwVideoWidth / 4; x++)
				{
					*pdwTexture++ = ((DWORD)(pVideo[2]) << 16) | ((DWORD)(pVideo[1]) << 8) | pVideo[0];
					*pdwTexture++ = ((DWORD)(pVideo[5]) << 16) | ((DWORD)(pVideo[4]) << 8) | pVideo[3];
					*pdwTexture++ = ((DWORD)(pVideo[8]) << 16) | ((DWORD)(pVideo[7]) << 8) | pVideo[6];
					*pdwTexture++ = ((DWORD)(pVideo[11]) << 16) | ((DWORD)(pVideo[10]) << 8) | pVideo[9];
					pVideo += 12;
				}

				pdwTexture += dwPitch - dwVideoWidth;
				pVideo -= dwVideoWidth * 6;
			}
		}
		else
		{
			for(DWORD y = 0; y < dwVideoHeight; y++)
			{
				for(DWORD x = 0; x < dwVideoWidth; x++)
				{
					*pdwTexture++ = ((DWORD)(pVideo[2]) << 16) |
									((DWORD)(pVideo[1]) << 8) |
									pVideo[0];
					pVideo += 3;
				}

				pdwTexture += dwPitch - dwVideoWidth;
				pVideo -= dwVideoWidth * 6;
			}
		}
	}
	else if(m_TexDesc.Format == D3DFMT_A8R8G8B8)
	{
		BYTE* pVideo;
		pSample->GetPointer(&pVideo);
		pVideo += dwVideoWidth * (dwVideoHeight - 1) * 3;
		DWORD* pdwTexture = (DWORD*)(LockedRect.pBits);
		DWORD dwPitch = LockedRect.Pitch / 4;

		if(!(dwVideoWidth % 4))
		{
			for(DWORD y = 0; y < dwVideoHeight; y++)
			{
				for(DWORD x = 0; x < dwVideoWidth / 4; x++)
				{
					*pdwTexture++ = 0xFF000000 | ((DWORD)(pVideo[2]) << 16) | ((DWORD)(pVideo[1]) << 8) | pVideo[0];
					*pdwTexture++ = 0xFF000000 | ((DWORD)(pVideo[5]) << 16) | ((DWORD)(pVideo[4]) << 8) | pVideo[3];
					*pdwTexture++ = 0xFF000000 | ((DWORD)(pVideo[8]) << 16) | ((DWORD)(pVideo[7]) << 8) | pVideo[6];
					*pdwTexture++ = 0xFF000000 | ((DWORD)(pVideo[11]) << 16) | ((DWORD)(pVideo[10]) << 8) | pVideo[9];
					pVideo += 12;
				}

				pdwTexture += dwPitch - dwVideoWidth;
				pVideo -= dwVideoWidth * 6;
			}
		}
		else
		{
			for(DWORD y = 0; y < dwVideoHeight; y++)
			{
				for(DWORD x = 0; x < dwVideoWidth; x++)
				{
					*pdwTexture++ = 0xFF000000 |
									((DWORD)(pVideo[2]) << 16) |
									((DWORD)(pVideo[1]) << 8) |
									pVideo[0];
					pVideo += 3;
				}

				pdwTexture += dwPitch - dwVideoWidth;
				pVideo -= dwVideoWidth * 6;
			}
		}
	}
	else if(m_TexDesc.Format == D3DFMT_R5G6B5)
	{
		BYTE* pVideo;
		pSample->GetPointer(&pVideo);
		pVideo += dwVideoWidth * (dwVideoHeight - 1) * 3;
		WORD* pwTexture = (WORD*)(LockedRect.pBits);
		DWORD dwPitch = LockedRect.Pitch / 2;

		if(!(dwVideoWidth % 4))
		{
			for(DWORD y = 0; y < dwVideoHeight; y++)
			{
				for(DWORD x = 0; x < dwVideoWidth / 4; x++)
				{
					*pwTexture++ = (((WORD)(pVideo[2] >> 3)) << 11) | (((WORD)(pVideo[1] >> 2)) << 5) | pVideo[0] >> 3;
					*pwTexture++ = (((WORD)(pVideo[5] >> 3)) << 11) | (((WORD)(pVideo[4] >> 2)) << 5) | pVideo[3] >> 3;
					*pwTexture++ = (((WORD)(pVideo[8] >> 3)) << 11) | (((WORD)(pVideo[7] >> 2)) << 5) | pVideo[6] >> 3;
					*pwTexture++ = (((WORD)(pVideo[11] >> 3)) << 11) | (((WORD)(pVideo[10] >> 2)) << 5) | pVideo[9] >> 3;
					pVideo += 12;
				}

				pwTexture += dwPitch - dwVideoWidth;
				pVideo -= dwVideoWidth * 6;
			}
		}
		else
		{
			for(DWORD y = 0; y < dwVideoHeight; y++)
			{
				for(DWORD x = 0; x < dwVideoWidth; x++)
				{
					*pwTexture++ = (((WORD)(pVideo[2] >> 3)) << 11) |
								   (((WORD)(pVideo[1] >> 2)) << 5) |
								   pVideo[0] >> 3;
					pVideo += 3;
				}

				pwTexture += dwPitch - dwVideoWidth;
				pVideo -= dwVideoWidth * 6;
			}
		}
	}
	else if(m_TexDesc.Format == D3DFMT_X1R5G5B5 ||
	        m_TexDesc.Format == D3DFMT_A1R5G5B5)
	{
		BYTE* pVideo;
		pSample->GetPointer(&pVideo);
		pVideo += dwVideoWidth * (dwVideoHeight - 1) * 3;
		WORD* pwTexture = (WORD*)(LockedRect.pBits);
		DWORD dwPitch = LockedRect.Pitch / 2;

		if(!(dwVideoWidth % 4))
		{
			for(DWORD y = 0; y < dwVideoHeight; y++)
			{
				for(DWORD x = 0; x < dwVideoWidth / 4; x++)
				{
					*pwTexture++ = 0x8000 | (((WORD)(pVideo[2] >> 3)) << 10) | (((WORD)(pVideo[1] >> 3)) << 5) | pVideo[0] >> 3;
					*pwTexture++ = 0x8000 | (((WORD)(pVideo[5] >> 3)) << 10) | (((WORD)(pVideo[4] >> 3)) << 5) | pVideo[3] >> 3;
					*pwTexture++ = 0x8000 | (((WORD)(pVideo[8] >> 3)) << 10) | (((WORD)(pVideo[7] >> 3)) << 5) | pVideo[6] >> 3;
					*pwTexture++ = 0x8000 | (((WORD)(pVideo[11] >> 3)) << 10) | (((WORD)(pVideo[10] >> 3)) << 5) | pVideo[9] >> 3;
					pVideo += 12;
				}

				pwTexture += dwPitch - dwVideoWidth;
				pVideo -= dwVideoWidth * 6;
			}
		}
		else
		{
			for(DWORD y = 0; y < dwVideoHeight; y++)
			{
				for(DWORD x = 0; x < dwVideoWidth; x++)
				{
					*pwTexture++ = 0x8000 |
								   (((WORD)(pVideo[2] >> 3)) << 10) |
								   (((WORD)(pVideo[1] >> 3)) << 5) |
								   pVideo[0] >> 3;
					pVideo += 3;
				}

				pwTexture += dwPitch - dwVideoWidth;
				pVideo -= dwVideoWidth * 6;
			}
		}
	}

	// Entsperren
	m_pTexture->UnlockRect(0);

	m_bTexLocked = FALSE;

	return S_OK;
}

// ******************************************************************
// Konstruktor der tbVideoRenderer-Klasse
tbVideo::tbVideo()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbVideo));
}

// ******************************************************************
// Destruktor der tbVideo-Klasse
tbVideo::~tbVideo()
{
	Exit();
}

// ******************************************************************
// alles zurücksetzen
tbResult tbVideo::Exit()
{
	// Wiedergabe stoppen
	if(IsPlaying()) Stop();

	// Den Video-Renderer entfernen
	if(m_pGraph != NULL && m_pRenderer != NULL)
		m_pGraph->RemoveFilter(m_pRenderer);

	// Alle Schnittstellen freigeben
//	if(m_pRenderer != NULL)
//		delete ((tbVideoRenderer*)m_pRenderer);
	m_pRenderer = NULL;
	TB_SAFE_RELEASE(m_pMediaControl);
	TB_SAFE_RELEASE(m_pMediaSeeking);
	TB_SAFE_RELEASE(m_pBasicAudio);
	TB_SAFE_RELEASE(m_pGraph);
	
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbVideo));

	return TB_OK;
}

// ******************************************************************
// Initialisiert das Video aus einer Datei
tbResult tbVideo::Init(char* pcFilename)
{
	// Alles zurücksetzen
	Exit();

	HRESULT			r;
	IBaseFilter*	pSrcFilter;			// Quellfilter
	IPin*			pSrcFilterOut;		// Ausgangspin des Quellfilters
	LONGLONG		llDuration;			// Dauer des Videos
	WCHAR			awcFilename[256];	// Dateiname

	// Wurde Direct3D initialisiert?
	if(!tbDirect3D::IsInitialized()) TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Graph erstellen
	if(FAILED(r = CoCreateInstance(CLSID_FilterGraph,
								   NULL,
								   CLSCTX_INPROC_SERVER,
								   IID_IGraphBuilder,
								   (void**)&m_pGraph)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("CoCreateInstance", r, TB_ERROR);
	}

	// Neuen Video-Renderer erstellen
	m_pRenderer = new tbVideoRenderer(NULL, &r);
	if(FAILED(r))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erzeugen des Video-Renderers!", TB_ERROR);
	}

	// Den Video-Renderer hinzufügen
	if(FAILED(r = m_pGraph->AddFilter(m_pRenderer, L"Renderer")))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pGraph->AddFilter", r, TB_ERROR);
	}

	// Quellfilter hinzufügen
	MultiByteToWideChar(CP_ACP, 0, pcFilename, -1, awcFilename, 256);
	if(FAILED(r = m_pGraph->AddSourceFilter(awcFilename, L"Source", &pSrcFilter)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("m_pGraph->AddSourceFilter", r, TB_ERROR);
	}

	// Den Ausgangspin des Quellfilters abfragen
	if(FAILED(r = pSrcFilter->FindPin(L"Output", &pSrcFilterOut)))
	{
		// Fehler!
		pSrcFilter->Release();
		TB_ERROR_DIRECTX("pSrcFilter->FindPin", r, TB_ERROR);
    }

	// Den Ausgangspin rendern. DirectShow wird unseren Video-Renderer
	// finden und ihn verwenden. Außerdem wird DirectShow - wenn nötig -
	// automatisch für das Abspielen der Audiodaten sorgen.
	if(FAILED(r = m_pGraph->Render(pSrcFilterOut)))
	{
		// Fehler!
		pSrcFilter->Release();
		pSrcFilterOut->Release();
		TB_ERROR_DIRECTX("m_pGraph->Render", r, TB_ERROR);
	}
	// Die Schnittstellen des Quellfilters freigeben
	pSrcFilter->Release();
	pSrcFilterOut->Release();

	// Die drei Schnittstellen für die Kontrolle und das Suchen im Video abfragen
	m_pGraph->QueryInterface(IID_IMediaControl, (void**)(&m_pMediaControl));
	m_pGraph->QueryInterface(IID_IMediaSeeking, (void**)(&m_pMediaSeeking));
	m_pGraph->QueryInterface(IID_IBasicAudio, (void**)(&m_pBasicAudio));
	if(m_pMediaControl == NULL ||
	   m_pMediaSeeking == NULL)
	{
		// Fehler!
		TB_ERROR("Fehler beim Abfragen der DirectShow-Videoschnittstellen!", TB_ERROR);
	}

	// Zeitformat setzen
	m_pMediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);

	// Die Länge des Videos abfragen
	m_pMediaSeeking->GetDuration(&llDuration);
	m_dwDuration = (DWORD)(llDuration / 10000);

	return TB_OK;
}

// ******************************************************************
// Spielt das Video ab
tbResult tbVideo::Play(BOOL bLooping) // = TRUE
{
	// Video zurücksetzen
	SetCurrentPosition(0);

	// Video abspielen
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
// Pausiert das Video
tbResult tbVideo::Pause()
{
	// Pause
	m_pMediaControl->Pause();

	return TB_OK;
}

// ******************************************************************
// Stoppt das Video
tbResult tbVideo::Stop()
{
	// Video stoppen
	m_pMediaControl->Stop();
	m_bPlaying = FALSE;

	return TB_OK;
}

// ******************************************************************
// Fragt den Status des Videos ab
OAFilterState tbVideo::GetState()
{
	OAFilterState Temp;

	m_pMediaControl->GetState(10, &Temp);
	return Temp;
}

// ******************************************************************
// Verarbeitet das Video
tbResult tbVideo::Process()
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
tbResult tbVideo::SetCurrentPosition(DWORD dwPosition)
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
int tbVideo::GetCurrentPosition()
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
tbResult tbVideo::SetPlaybackRate(float fRate)
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
float tbVideo::GetPlaybackRate()
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
// Zeichnen des Videos auf den Backbuffer
tbResult tbVideo::DrawVideo(tbVector4 vRect)
{
	// Wenn Video nicht gestartet wurde, kann auch nichts gemalt werden
	if(GetState() != State_Running) return TB_OK;

	SVideoVertex aVertex[4];

	// Parameter prüfen
	if(vRect == tbVector4(0.0f)) vRect = tbVector4(0.0f, 0.0f, tbDirect3D::GetScreenSize().x, tbDirect3D::GetScreenSize().y);

	// Wenn die Videotextur gerade gesperrt ist, warten wir so lange,
	// bis sie es nicht mehr ist.
	while(((tbVideoRenderer*)m_pRenderer)->IsTextureLocked()) {}

	// Aktuellen Status speichern
	m_dwOldFVF = tbDirect3D::GetFVF();
	// Vertexformat und Videotextur setzen, Z-Buffer aus
	tbDirect3D::SetFVF(TB_VIDEO_FVF);
	tbDirect3D::SetTexture(0, ((tbVideoRenderer*)m_pRenderer)->GetTexture());
	tbDirect3D::SetRS(D3DRS_ZENABLE, D3DZB_FALSE);

	// Die vier Vertizes erstellen (Rechteck)
	// Links unten
	aVertex[0].vPosition = tbVector3(vRect.left, vRect.bottom, 0.0f);
	aVertex[0].fRHW = 1.0f;
	aVertex[0].vTex0 = tbVector2(0.0f, ((tbVideoRenderer*)m_pRenderer)->GetBottomRightTex().y);

	// Links oben
	aVertex[1].vPosition = tbVector3(vRect.left, vRect.top, 0.0f);
	aVertex[1].fRHW = 1.0f;
	aVertex[1].vTex0 = tbVector2(0.0f, 0.0f);

	// Rechts unten
	aVertex[2].vPosition = tbVector3(vRect.right, vRect.bottom, 0.0f);
	aVertex[2].fRHW = 1.0f;
	aVertex[2].vTex0 = ((tbVideoRenderer*)m_pRenderer)->GetBottomRightTex();

	// Rechts oben
	aVertex[3].vPosition = tbVector3(vRect.right, vRect.top, 0.0f);
	aVertex[3].fRHW = 1.0f;
	aVertex[3].vTex0 = tbVector2(((tbVideoRenderer*)m_pRenderer)->GetBottomRightTex().x, 0.0f);

	// Als Dreiecksfolge zeichnen
	tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, aVertex, sizeof(SVideoVertex));

	// Wiederherstellen des alten Status
	tbDirect3D::SetFVF(m_dwOldFVF);
	tbDirect3D::SetTexture(0, NULL);

	return TB_OK;
}

// ******************************************************************
