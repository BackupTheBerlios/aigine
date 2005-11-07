/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tb2DVolumeSprite.cpp
	====================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit 2D-Volumen-Sprites

	Autor:
	Snorky
	[24.9.2003]

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tb2DVolumeSprite-Klasse
tb2DVolumeSprite::tb2DVolumeSprite()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tb2DVolumeSprite));
}

// ******************************************************************
// Destruktor der tb2DVolumeSprite-Klasse
tb2DVolumeSprite::~tb2DVolumeSprite()
{
	Exit();
}

// ******************************************************************
// alles freigeben und löschen
tbResult tb2DVolumeSprite::Exit()
{
	// Textur aus dem Effekt nehmen
	if(m_pEffect != NULL)
		m_pEffect->GetEffect()->SetTexture("Texture1", NULL);

	// Textur freigeben
	if(m_bTexturmanager)
	{
		tbTextureManager::ReleaseTexture(m_pTexture);
	}
	else
	{
		TB_SAFE_RELEASE(m_pTexture);
	}

	// Effekt freigeben
	TB_SAFE_DELETE(m_pEffect);

	// alles zurücksetzen
	ZeroMemory(this, sizeof(tb2DVolumeSprite));

	return TB_OK;
}

// ******************************************************************
// Liest Spritekoordinaten aus einer INI-Datei und rechnet sie ggf. um
tbResult tb2DVolumeSprite::ReadINIVolumeSpriteInfo(tbINIReader* pINIReader,
													char* pcSection,
													char* pcKey,
													tb2DVolumeSpriteInfo* s2DSpriteInfo,
													tbVector2 (*converter)(tbVector2 vParam))
{
	char		acKey[256];
	char		acString[256];
	ZeroMemory(acString, sizeof(acString));


	// Werte-Array durchgehen
	for(int i = 0; i < 256; i++) {
		// String lesen
		sprintf(acKey, "%s%d", pcKey, i);
		pINIReader->ReadINIString(pcSection, acKey, acString, 256);
		if(!strcmp(acString, "[NOT FOUND]")) return TB_NOT_FOUND;

		// Die Vektorkomponenten extrahieren
		int x1,x2,y1,y2;
		sscanf(acString, "%d, %d, %d, %d", &x1, &y1, &x2, &y2);
		
		// linke obere Ecke in Pixelkoordinaten
		s2DSpriteInfo->avTopLeftPix[i].x = (float)x1;
		s2DSpriteInfo->avTopLeftPix[i].y = (float)y1;

		// Breite in Pixelkoordinaten
		s2DSpriteInfo->avSpriteWidthPix[i].x = (float)(x2);
		s2DSpriteInfo->avSpriteWidthPix[i].y = (float)(y2);

		// linke obere Ecke in Texturkoordinaten
		s2DSpriteInfo->avTopLeft[i].x = (float)(x1 / (DWORD)(s2DSpriteInfo->fTextureWidth)) + (float)(x1 % (DWORD)(s2DSpriteInfo->fTextureWidth))/(s2DSpriteInfo->fTextureWidth -1.0f);
		s2DSpriteInfo->avTopLeft[i].y = (float)(y1 / (DWORD)(s2DSpriteInfo->fTextureWidth)) + (float)(y1 % (DWORD)(s2DSpriteInfo->fTextureHeight))/(s2DSpriteInfo->fTextureHeight -1.0f);
	
		// Breite in Texturkoordinaten
		s2DSpriteInfo->avSpriteWidth[i].x = (float)(x2 / (DWORD)(s2DSpriteInfo->fTextureWidth)) + (float)(x2 % (DWORD)(s2DSpriteInfo->fTextureWidth))/(s2DSpriteInfo->fTextureWidth -1.0f);
		s2DSpriteInfo->avSpriteWidth[i].y = (float)(y2 / (DWORD)(s2DSpriteInfo->fTextureHeight)) + (float)(y2 % (DWORD)(s2DSpriteInfo->fTextureHeight))/(s2DSpriteInfo->fTextureHeight -1.0f);

		// untere rechte Ecke anhand der Breiten berechnen
		s2DSpriteInfo->avBottomRight[i] = s2DSpriteInfo->avTopLeft[i] + s2DSpriteInfo->avSpriteWidth[i];
		s2DSpriteInfo->avBottomRightPix[i] = s2DSpriteInfo->avTopLeftPix[i] + s2DSpriteInfo->avSpriteWidthPix[i];

		// Werte convertieren, wenn nötig
		if(converter != NULL) 
		{
			s2DSpriteInfo->avTopLeftPix[i] = converter(s2DSpriteInfo->avTopLeftPix[i]);
			s2DSpriteInfo->avSpriteWidthPix[i] = converter(s2DSpriteInfo->avSpriteWidthPix[i]);
			s2DSpriteInfo->avBottomRightPix[i] = converter(s2DSpriteInfo->avBottomRightPix[i]);
		}
	}

	return TB_OK;
}

// ******************************************************************
// Initialisierung aus virtuellen Dateien
tbResult tb2DVolumeSprite::Init(char* pcINIFile,
								char* pcINISection,
								tbVector2 (*converter)(tbVector2 vParam),	// = NULL
								char* pcTextureFile)						// = NULL
{
	// sicherheitshalber alles vorher freigeben
	Exit();

	HRESULT		hResult;
	tbResult	tbRes;

	// Parameter prüfen und sicherstellen, dass tbDirect3D initialisiert wurde
	if(pcINIFile == NULL)				TB_ERROR_NULL_POINTER("pcINIFile", TB_ERROR);
	if(pcINISection == NULL)			TB_ERROR_NULL_POINTER("pcINISection", TB_ERROR);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Es muss zuerst eine tbDirect3D-Klasseninstanz erstellt werden!", TB_ERROR);

	
	// Informationsdatei vorbereiten
	tbINIReader* pINIReader;
	pINIReader = new tbINIReader; 	if(pINIReader == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	pINIReader->SetFilePath(pcINIFile);

	// ColorKey laden
	tbColor ColorKey = pINIReader->ReadINIColor(pcINISection, "ColorKey");
	if(ColorKey == tbColor(12345678.0f, 12345678.0f, 12345678.0f, 12345678.0f)) ColorKey = tbColor(0.0f, 0.0f, 0.0f, 1.0f);
	

	// Wenn kein Parameter übergeben nach INI-Eintrag suchen
	char acTextur[256];
	ZeroMemory(acTextur, sizeof(acTextur));
	if(pcTextureFile == NULL)
	{
		// Texturname aus INI laden wenn möglich
		pINIReader->ReadINIString(pcINISection, "Texture", acTextur, 256);
	
		// Kein INI-Eintrag gefunden?
		if(!strcmp(acTextur, "[NOT FOUND]"))
		{
			TB_ERROR("Fehler beim Laden der Sprite-Textur, kein INI-Eintrag gefunden!", TB_ERROR);
		}
	}
	else
	{
		strncpy(acTextur, pcTextureFile, 256);
	}

	// Texturmanager benutzen, wenn er initialisiert ist
	if(tbTextureManager::IsInitialized())
	{
		// Texturmanager benutzen merken
		m_bTexturmanager = TRUE;

		m_pTexture = tbTextureManager::GetVolumeTexture(acTextur, 
						TRUE, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
						1, D3DFMT_UNKNOWN, 0, D3DPOOL_MANAGED, 
						D3DX_DEFAULT, D3DX_DEFAULT, ColorKey);

		// werden keine Volumentexturen unterstützt?
		if(m_pTexture == NULL)
		{
			m_bVolumeUse = FALSE;
			TB_WARNING("Es werden keine Volumentexturen unterstützt!");

			// Alternativ-Texturname aus INI laden wenn möglich
			pINIReader->ReadINIString(pcINISection, "TextureAlt", acTextur, 256);
			// Kein INI-Eintrag gefunden?
			if(!strcmp(acTextur, "[NOT FOUND]")) TB_ERROR("Fehler beim Laden der alternativen Volumen-Textur, kein INI-Eintrag gefunden!", TB_ERROR);

			// Alternativbild laden
			m_pTexture = tbTextureManager::GetTexture(acTextur, 
						TRUE, D3DX_DEFAULT, D3DX_DEFAULT,
						1, D3DFMT_UNKNOWN, 0, D3DPOOL_MANAGED, 
						D3DX_DEFAULT, D3DX_DEFAULT, ColorKey);
		
			// Fehler?
			if(m_pTexture == NULL) TB_ERROR("Fehler beim Laden der alternativen Textur mit dem Texturmanager!",TB_ERROR);

			// Bilddimensionen speichern
			D3DSURFACE_DESC Desc;
			((IDirect3DTexture9*)m_pTexture)->GetLevelDesc(0, &Desc);
			m_2DSpriteInfo.fTextureHeight = (float)Desc.Height;
			m_2DSpriteInfo.fTextureWidth = (float)Desc.Width;
			m_2DSpriteInfo.fTextureDepth = 1.0f;
		}
		else
		{
			// volle Volumentexturunterstützung
			m_bVolumeUse = TRUE;

			// Bilddimensionen speichern
			D3DVOLUME_DESC Desc;
			((IDirect3DVolumeTexture9*)m_pTexture)->GetLevelDesc(0, &Desc);
			m_2DSpriteInfo.fTextureHeight = (float)Desc.Height;
			m_2DSpriteInfo.fTextureWidth = (float)Desc.Width;
			m_2DSpriteInfo.fTextureDepth = (float)Desc.Depth;
		}

	}
	else
	{

		// Texturmanager nicht benutzen merken
		m_bTexturmanager = FALSE;

		tbVFile*	pVFile;
		D3DXIMAGE_INFO dinfo;
		ZeroMemory(&dinfo, sizeof(dinfo));

		// Virtuelle Dateien erstellen
		pVFile = new tbVFile;
		if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

		// Aus Datei laden
		if(pVFile->Init(acTextur))
		{
			goto Label_LoadAlternativeVolumeTextur;
		}

		// Textur laden
		hResult = D3DXCreateVolumeTextureFromFileInMemoryEx(tbDirect3D::GetDevice(),
					(BYTE*)(pVFile->GetBuffer()) + pVFile->GetCursor(),
					pVFile->GetSize() - pVFile->GetCursor(),
					D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
					1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
					D3DX_DEFAULT, D3DX_DEFAULT,
					ColorKey, &dinfo, NULL, 
					(PDIRECT3DVOLUMETEXTURE9*)(&m_pTexture));
		if(FAILED(hResult) || m_pTexture == NULL)
		{
Label_LoadAlternativeVolumeTextur:
			TB_WARNING("Es werden keine Volumentexturen unterstützt!");
			// Alternativ-Texturname aus INI laden wenn möglich
			pINIReader->ReadINIString(pcINISection, "TextureAlt", acTextur, 256);
			// Kein INI-Eintrag gefunden?
			if(!strcmp(acTextur, "[NOT FOUND]")) TB_ERROR("Fehler beim Laden der alternativen Volumen-Textur, kein INI-Eintrag gefunden!", TB_ERROR);

			// Virtuelle Dateien erneut erstellen
			delete pVFile;
			pVFile = NULL;
			pVFile = new tbVFile;
			if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			// Aus Datei laden
			if(pVFile->Init(acTextur)) TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);

			hResult = D3DXCreateTextureFromFileInMemoryEx(tbDirect3D::GetDevice(),
					(BYTE*)(pVFile->GetBuffer()) + pVFile->GetCursor(),
					pVFile->GetSize() - pVFile->GetCursor(),
					D3DX_DEFAULT, D3DX_DEFAULT,
					1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
					D3DX_DEFAULT, D3DX_DEFAULT,
					ColorKey, &dinfo, NULL, 
					(PDIRECT3DTEXTURE9*)(&m_pTexture));
		
			// Fehler?
			if(FAILED(hResult) || m_pTexture == NULL) TB_ERROR_DIRECTX("D3DXCreateTextureFromFileInMemoryEx", hResult, TB_ERROR);
		}
		// Bilddimensionen speichern
		m_2DSpriteInfo.fTextureHeight = (float)dinfo.Height;
		m_2DSpriteInfo.fTextureWidth = (float)dinfo.Width;
		m_2DSpriteInfo.fTextureDepth = (float)dinfo.Depth;

		// Die virtuellen Dateien wieder freigeben
		TB_SAFE_DELETE(pVFile);
	}


	// Sprite-Informationen laden
	ReadINIVolumeSpriteInfo(pINIReader,
							pcINISection, 
							"Sprite", 
							&m_2DSpriteInfo,
							converter);


	// Effektname aus INI laden
	char acEffect[256];
	ZeroMemory(acEffect, sizeof(acEffect));
	pINIReader->ReadINIString(pcINISection, "Effect", acEffect, 256);
	if(!strcmp(acEffect, "[NOT FOUND]")) TB_ERROR("Fehler beim Laden des Sprite-Effekts!", TB_ERROR);
	// Effekt laden
	m_pEffect = new tbEffect; if(m_pEffect == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	if(TB_OK != (tbRes = m_pEffect->Init(acEffect))) TB_ERROR("Fehler beim Erstellen des Sprite-Effekts!", TB_ERROR);
	// Effekt setzen
	if(TB_OK != m_pEffect->SetTechnique())	TB_ERROR("Fehler beim Setzen des Sprite-Effekts!", TB_ERROR);

	
	// Textur setzen
	hResult = m_pEffect->GetEffect()->SetTexture("Texture1", m_pTexture);
	// Fehler?
	if(FAILED(hResult)) TB_ERROR_DIRECTX("SetTexture", hResult, TB_ERROR);


	// INIReader wieder freigeben
	TB_SAFE_DELETE(pINIReader);

	return TB_OK;
}

// ******************************************************************
// Beginnt mit den Sprites
tbResult tb2DVolumeSprite::Begin()
{
	// Aktuellen Status speichern
	m_dwOldFVF = tbDirect3D::GetFVF();

	// Neues Vertexformat setzen
	tbDirect3D::SetFVF(TB_2DVOLUMESPRITE_FVF);

	// Mit dem Effekt beginnen
	m_iNumPasses = m_pEffect->Begin();

	// Vertexcursor setzen
	dwVertexCursor = 0;

	return TB_OK;
}

// ******************************************************************
// Beenden
tbResult tb2DVolumeSprite::End()
{
	// Wenn noch was im Puffer steht, dann wird jetzt gezeichnet.
	if(dwVertexCursor > 0)
	{
		DrawPuffer();
	}

	// Effekt beenden
	m_pEffect->End();

	// Wiederherstellen des alten Status
	tbDirect3D::SetFVF(m_dwOldFVF);

	return TB_OK;
}

// ******************************************************************
// Alle Sprites im Puffer sofort malen
tbResult tb2DVolumeSprite::DrawPuffer()
{
	// Jeden Durchgang des Effekts zeichnen
	for(int iPass = 0; iPass < m_iNumPasses; iPass++)
	{
		m_pEffect->BeginPass(iPass);

		tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			                                     dwVertexCursor / 3,
												 aVertex,
												 sizeof(tb2DVolumeSpriteVertex));
		m_pEffect->EndPass();
	}

	// Vertexcursor zurücksetzen
	dwVertexCursor = 0;

	return TB_OK;
}

// ******************************************************************
// Diese Methode zeichnet ein Sprite.
tbResult tb2DVolumeSprite::DrawSprite(tbVector2 vPosition,
								float fDepth,				// = 0.0f
								DWORD dwNumber,				// = 0
								const DWORD dwFlags,		// = TB_SVF_ALIGN_HLEFT | TB_SVF_ALIGN_VTOP | TB_SVF_DEPTH_ABSOLUTE
								const tbColor& Color1,		// = tbColor(1.0f)
								tbVector2 vScaling,			// = tbVector2(1.0f)
								float fRotating)			// = 0.0f
{
	// Wenn rotiert werden soll muss transformated Sprite her
	if(fRotating != 0.0f)
	{
		tbMatrix	mTransformation;

		mTransformation = tbMatrixScaling(tbVector3(vScaling.x, vScaling.y, 1.0f));
		mTransformation *= tbMatrixRotationZ(fRotating);
		mTransformation *= tbMatrixTranslation(tbVector3(vPosition.x, vPosition.y, 0.0f));
		
		return DrawTransformedSprite(mTransformation, fDepth, dwNumber, dwFlags, Color1);
	}


	// Absolute Texturtiefen in Relative umrechnen
	if(dwFlags & TB_SVF_DEPTH_ABSOLUTE)
	{
		fDepth = fDepth / m_2DSpriteInfo.fTextureDepth;
	}


	// Relative Koordinaten in absolute umrechnen
	if(dwFlags & TB_SVF_RELATIVE)
	{
		vPosition.x *= tbDirect3D::GetScreenSize().x;
		vPosition.y *= tbDirect3D::GetScreenSize().y;
	}

	// Relative Skalierung in absolute umrechnen
	if(dwFlags & TB_SVF_RELATIVESCALING)
	{
		vScaling.x *= tbDirect3D::GetScreenSize().x / 640.0f;
		vScaling.y *= tbDirect3D::GetScreenSize().y / 480.0f;
	}


	tbVector2 vCursor(vPosition);
	// Breite des Sprites für den Bildschirm
	float fSpriteWidth = m_2DSpriteInfo.avSpriteWidthPix[dwNumber].x * vScaling.x;
	float fSpriteHight = m_2DSpriteInfo.avSpriteWidthPix[dwNumber].y * vScaling.y;
	// Breite des Sprites als Textur
	tbVector3 vTexturStart = tbVector3(m_2DSpriteInfo.avTopLeft[dwNumber].x, m_2DSpriteInfo.avTopLeft[dwNumber].y, fDepth);
	tbVector3 vTexturEnd = tbVector3(m_2DSpriteInfo.avBottomRight[dwNumber].x, m_2DSpriteInfo.avBottomRight[dwNumber].y, fDepth);


	// Cursor auf die horizontale Startposition setzen
	if((dwFlags & TB_SVF_ALIGN_HCENTER) || (dwFlags & TB_SVF_ALIGN_CENTER))
	{
		// Zentrieren
		vCursor.x -= 0.5f * (fSpriteWidth);
	}
	else if(dwFlags & TB_SVF_ALIGN_HRIGHT)
	{
		// Rechtsbündig
		vCursor.x -= fSpriteWidth-1;
	}
	else
	{
		// Linksbündig
	}


	// Cursor auf die vertikale Startposition setzen
	if((dwFlags & TB_SVF_ALIGN_VCENTER) || (dwFlags & TB_SVF_ALIGN_CENTER))
	{
		// vertikal zentrieren
		vCursor.y -= 0.5f * fSpriteHight;
	}
	else if(dwFlags & TB_SVF_ALIGN_VBOTTOM)
	{
		// unten ausrichten
		vCursor.y -= fSpriteHight-1;
	}
	else
	{
		// oben ausrichten
	}


	// Vollbildmodus umrechnen
	if(dwFlags & TB_SF_FULLMODE)
	{
		vCursor = tbVector2(0.0f, 0.0f);
		fSpriteWidth = tbDirect3D::GetScreenSize().x;
		fSpriteHight = tbDirect3D::GetScreenSize().y;
		vTexturStart = tbVector3(0.0f, 0.0f, fDepth);
		vTexturEnd = tbVector3(1.0f, 1.0f, fDepth);
	}

	// Texturausgleich, wenn Position im Negativen, führt sonst zu Grafikfehlern, 
	// Wert beruht auf Praxistests, Ursache ist unbekannt
	if(vCursor.x < 0.0f || vCursor.y < 0.0f)
	{
		vTexturStart.x += 0.0000001f;
		vTexturStart.y += 0.0000001f;
	}

	// Sechs Vertizes für das Sprite hinzufügen.
	// Dazu verwenden wir Dreieckslisten - jeweils sechs Vertizes
	// beschreiben ein Viereck.

	// Erster Vertex: links oben
	aVertex[dwVertexCursor].vPosition = tbVector3(vCursor.x, vCursor.y, 0.0f);
	aVertex[dwVertexCursor].fRHW = 1.0f;
	aVertex[dwVertexCursor].Diffuse = Color1;
	aVertex[dwVertexCursor].vTexture = vTexturStart;

	// Zweiter Vertex: rechts oben
	aVertex[dwVertexCursor + 1].vPosition = tbVector3(vCursor.x + fSpriteWidth, vCursor.y, 0.0f);
	aVertex[dwVertexCursor + 1].fRHW = 1.0f;
	aVertex[dwVertexCursor + 1].Diffuse = Color1;
	aVertex[dwVertexCursor + 1].vTexture.x = vTexturEnd.x;
	aVertex[dwVertexCursor + 1].vTexture.y = vTexturStart.y;
	aVertex[dwVertexCursor + 1].vTexture.z = fDepth;

	// Dritter Vertex: rechts unten
	aVertex[dwVertexCursor + 2].vPosition = tbVector3(vCursor.x + fSpriteWidth, vCursor.y + fSpriteHight, 0.0f);
	aVertex[dwVertexCursor + 2].fRHW = 1.0f;
	aVertex[dwVertexCursor + 2].Diffuse = Color1;
	aVertex[dwVertexCursor + 2].vTexture = vTexturEnd;

	// Vierter Vertex = erster Vertex
	aVertex[dwVertexCursor + 3] = aVertex[dwVertexCursor];

	// Fünfter Vertex = dritter Vertex
	aVertex[dwVertexCursor + 4] = aVertex[dwVertexCursor + 2];

	// Sechster Vertex: links unten
	aVertex[dwVertexCursor + 5].vPosition = tbVector3(vCursor.x, vCursor.y + fSpriteHight, 0.0f);
	aVertex[dwVertexCursor + 5].fRHW = 1.0f;
	aVertex[dwVertexCursor + 5].Diffuse = Color1;
	aVertex[dwVertexCursor + 5].vTexture.x = vTexturStart.x;
	aVertex[dwVertexCursor + 5].vTexture.y = vTexturEnd.y;
	aVertex[dwVertexCursor + 5].vTexture.z = fDepth;

	// Vertexcursor aktualisieren
	dwVertexCursor += 6;


	// Wenn das Vertex-Array voll ist, dann wird jetzt gezeichnet.
	if(dwVertexCursor >= 600)
	{
		DrawPuffer();
	}

	return TB_OK;
}

// ******************************************************************
// Sprite zeichnen mit festen Fensterkoordinaten
tbResult tb2DVolumeSprite::DrawSprite(tbVector4 vPosition,
					float fDepth,			// = 0.0f
					DWORD dwNumber,			// = 0
					const tbColor& Color1)	// = tbColor(1.0f)
{
	// Vorberechnungen
	tbVector3 vTexturStart = tbVector3(m_2DSpriteInfo.avTopLeft[dwNumber].x, m_2DSpriteInfo.avTopLeft[dwNumber].y, fDepth);
	tbVector3 vTexturEnd = tbVector3(m_2DSpriteInfo.avBottomRight[dwNumber].x, m_2DSpriteInfo.avBottomRight[dwNumber].y, fDepth);
	// Texturausgleich, wenn Position im Negativen, führt sonst zu Grafikfehlern, 
	// Wert beruht auf Praxistests, Ursache ist unbekannt
	if(vPosition.x < 0.0f || vPosition.y < 0.0f)
	{
		vTexturStart.x += 0.0000001f;
		vTexturStart.y += 0.0000001f;
	}

	// Erster Vertex: links oben
	aVertex[dwVertexCursor].vPosition = tbVector3(vPosition.x1, vPosition.y1, 0.0f);
	aVertex[dwVertexCursor].fRHW = 1.0f;
	aVertex[dwVertexCursor].Diffuse = Color1;
	aVertex[dwVertexCursor].vTexture = vTexturStart;

	// Zweiter Vertex: rechts oben
	aVertex[dwVertexCursor + 1].vPosition = tbVector3(vPosition.x2, vPosition.y1, 0.0f);
	aVertex[dwVertexCursor + 1].fRHW = 1.0f;
	aVertex[dwVertexCursor + 1].Diffuse = Color1;
	aVertex[dwVertexCursor + 1].vTexture.x = vTexturEnd.x;
	aVertex[dwVertexCursor + 1].vTexture.y = vTexturStart.y;
	aVertex[dwVertexCursor + 1].vTexture.z = fDepth;

	// Dritter Vertex: rechts unten
	aVertex[dwVertexCursor + 2].vPosition = tbVector3(vPosition.x2, vPosition.y2, 0.0f);
	aVertex[dwVertexCursor + 2].fRHW = 1.0f;
	aVertex[dwVertexCursor + 2].Diffuse = Color1;
	aVertex[dwVertexCursor + 2].vTexture = vTexturEnd;

	// Vierter Vertex = erster Vertex
	aVertex[dwVertexCursor + 3] = aVertex[dwVertexCursor];

	// Fünfter Vertex = dritter Vertex
	aVertex[dwVertexCursor + 4] = aVertex[dwVertexCursor + 2];

	// Sechster Vertex: links unten
	aVertex[dwVertexCursor + 5].vPosition = tbVector3(vPosition.x1, vPosition.y2, 0.0f);
	aVertex[dwVertexCursor + 5].fRHW = 1.0f;
	aVertex[dwVertexCursor + 5].Diffuse = Color1;
	aVertex[dwVertexCursor + 5].vTexture.x = vTexturStart.x;
	aVertex[dwVertexCursor + 5].vTexture.y = vTexturEnd.y;
	aVertex[dwVertexCursor + 5].vTexture.z = fDepth;

	// Vertexcursor aktualisieren
	dwVertexCursor += 6;


	// Wenn das Vertex-Array voll ist, dann wird jetzt gezeichnet.
	if(dwVertexCursor >= 600)
	{
		DrawPuffer();
	}

	return TB_OK;
}

// ******************************************************************
// Sprite mit Transformationsmatrix zeichnen
tbResult tb2DVolumeSprite::DrawTransformedSprite(tbMatrix mTransformation,
										   float fDepth,			// = 0.0f
										   DWORD dwNumber,			// = 0
										   const DWORD dwFlags,		// = TB_SF_ALIGN_HLEFT | TB_SF_ALIGN_VTOP | TB_SVF_DEPTH_ABSOLUTE
										   const tbColor& Color1)	// = tbColor(1.0f)
{
	// Relative Texturtiefen in absolute umrechnen
	if(dwFlags & TB_SVF_DEPTH_ABSOLUTE)
	{
		fDepth = fDepth / m_2DSpriteInfo.fTextureDepth;
	}

	// Relative Koordinaten in absolute umrechnen
	if(dwFlags & TB_SVF_RELATIVE)
	{
		mTransformation.m41 *= tbDirect3D::GetScreenSize().x;
		mTransformation.m42 *= tbDirect3D::GetScreenSize().y;
	}

	// Relative Größe in absolute umrechnen
	if(dwFlags & TB_SVF_RELATIVESCALING)
	{
		mTransformation.m11 *= tbDirect3D::GetScreenSize().x / 640.0f;
		mTransformation.m22 *= tbDirect3D::GetScreenSize().y / 480.0f;
	}

	tbVector2 vCursor(0.0f);
	// Breite des Sprites für den Bildschirm
	float fSpriteWidth = m_2DSpriteInfo.avSpriteWidthPix[dwNumber].x;
	float fSpriteHight = m_2DSpriteInfo.avSpriteWidthPix[dwNumber].y;
	// Breite des Sprites als Textur
	tbVector3 vTexturStart = tbVector3(m_2DSpriteInfo.avTopLeft[dwNumber].x, m_2DSpriteInfo.avTopLeft[dwNumber].y, fDepth);
	tbVector3 vTexturEnd = tbVector3(m_2DSpriteInfo.avBottomRight[dwNumber].x, m_2DSpriteInfo.avBottomRight[dwNumber].y, fDepth);

	// Cursor auf die horizontale Startposition setzen
	if((dwFlags & TB_SVF_ALIGN_HCENTER) || (dwFlags & TB_SVF_ALIGN_CENTER))
	{
		// Zentrieren
		vCursor.x = -0.5f * (fSpriteWidth);
	}
	else if(dwFlags & TB_SVF_ALIGN_HRIGHT)
	{
		// Rechtsbündig
		vCursor.x = -(fSpriteWidth-1);
	}
	else
	{
		// Linksbündig
	}

	// Cursor auf die vertikale Startposition setzen
	if((dwFlags & TB_SVF_ALIGN_VCENTER) || (dwFlags & TB_SVF_ALIGN_CENTER))
	{
		// vertikal zentrieren
		vCursor.y = -0.5f * (fSpriteHight);
	}
	else if(dwFlags & TB_SVF_ALIGN_VTOP)
	{
		// oben ausrichten
		vCursor.y = -(fSpriteHight-1);
	}
	else
	{
		// unten ausrichten
	}


	// Vollbildmodus umrechnen
	if(dwFlags & TB_SF_FULLMODE)
	{
		vCursor = tbVector2(0.0f, 0.0f);
		fSpriteWidth = tbDirect3D::GetScreenSize().x;
		fSpriteHight = tbDirect3D::GetScreenSize().y;
		vTexturStart = tbVector3(0.0f, 0.0f, fDepth);
		vTexturEnd = tbVector3(1.0f, 1.0f, fDepth);
	}

	// Texturausgleich, wenn Position im Negativen, führt sonst zu Grafikfehlern, 
	// Wert beruht auf Praxistests, Ursache ist unbekannt
	if(vCursor.x < 0.0f || vCursor.y < 0.0f)
	{
		vTexturStart.x += 0.0000001f;
		vTexturStart.y += 0.0000001f;
	}

	// Sechs Vertizes für das Sprite hinzufügen.
	// Dazu verwenden wir Dreieckslisten - jeweils sechs Vertizes
	// beschreiben ein Viereck.

	// Erster Vertex: links oben
	aVertex[dwVertexCursor].vPosition = tbVector3(vCursor.x, vCursor.y, 0.0f);
	aVertex[dwVertexCursor].vPosition = tbVector3TransformCoords(aVertex[dwVertexCursor].vPosition, mTransformation);
	aVertex[dwVertexCursor].fRHW = 1.0f;
	aVertex[dwVertexCursor].Diffuse = Color1;
	aVertex[dwVertexCursor].vTexture = vTexturStart;

	// Zweiter Vertex: rechts oben
	aVertex[dwVertexCursor + 1].vPosition = tbVector3(vCursor.x + fSpriteWidth, vCursor.y, 0.0f);
	aVertex[dwVertexCursor + 1].vPosition = tbVector3TransformCoords(aVertex[dwVertexCursor + 1].vPosition, mTransformation);
	aVertex[dwVertexCursor + 1].fRHW = 1.0f;
	aVertex[dwVertexCursor + 1].Diffuse = Color1;
	aVertex[dwVertexCursor + 1].vTexture.x = vTexturEnd.x;
	aVertex[dwVertexCursor + 1].vTexture.y = vTexturStart.y;
	aVertex[dwVertexCursor + 1].vTexture.z = fDepth;

	// Dritter Vertex: rechts unten
	aVertex[dwVertexCursor + 2].vPosition = tbVector3(vCursor.x + fSpriteWidth, vCursor.y + fSpriteHight, 0.0f);
	aVertex[dwVertexCursor + 2].vPosition = tbVector3TransformCoords(aVertex[dwVertexCursor + 2].vPosition, mTransformation);
	aVertex[dwVertexCursor + 2].fRHW = 1.0f;
	aVertex[dwVertexCursor + 2].Diffuse = Color1;
	aVertex[dwVertexCursor + 2].vTexture = vTexturEnd;

	// Vierter Vertex = erster Vertex
	aVertex[dwVertexCursor + 3] = aVertex[dwVertexCursor];

	// Fünfter Vertex = dritter Vertex
	aVertex[dwVertexCursor + 4] = aVertex[dwVertexCursor + 2];

	// Sechster Vertex: links unten
	aVertex[dwVertexCursor + 5].vPosition = tbVector3(vCursor.x, vCursor.y + fSpriteHight, 0.0f);
	aVertex[dwVertexCursor + 5].vPosition = tbVector3TransformCoords(aVertex[dwVertexCursor + 5].vPosition, mTransformation);
	aVertex[dwVertexCursor + 5].fRHW = 1.0f;
	aVertex[dwVertexCursor + 5].Diffuse = Color1;
	aVertex[dwVertexCursor + 5].vTexture.x = vTexturStart.x;
	aVertex[dwVertexCursor + 5].vTexture.y = vTexturEnd.y;
	aVertex[dwVertexCursor + 5].vTexture.z = fDepth;

	// Vertexcursor aktualisieren
	dwVertexCursor += 6;

	// Wenn das Vertex-Array voll ist, dann wird jetzt gezeichnet.
	if(dwVertexCursor >= 600)
	{
		DrawPuffer();
	}

	return TB_OK;
}

// ******************************************************************
