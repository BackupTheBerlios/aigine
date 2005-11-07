/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbFont.cpp
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit Schriftarten

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbFont-Klasse
tbFont::tbFont()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbFont));
}

// ******************************************************************
// Destruktor der tbFont-Klasse
tbFont::~tbFont()
{
	// Textur aus dem Effekt nehmen
	m_pEffect->GetEffect()->SetTexture("Texture", NULL);

	// Textur und Effekt freigeben
	TB_SAFE_RELEASE(m_pTexture);
	TB_SAFE_DELETE(m_pEffect);
}

// ******************************************************************
// Initialisierung aus virtuellen Dateien
tbResult tbFont::Init(tbVFile* pTGAFile,
					  tbVFile* pTBFFile)
{
	HRESULT hResult;

	// Parameter prüfen und sicherstellen, dass tbDirect3D initialisiert wurde
	if(pTGAFile == NULL)				TB_ERROR_NULL_POINTER("pTGAFile", TB_ERROR);
	if(pTBFFile == NULL)				TB_ERROR_NULL_POINTER("pTBFFile", TB_ERROR);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Es muss zuerst eine tbDirect3D-Klasseninstanz erstellt werden!", TB_ERROR);


	// Textur laden
	if(FAILED(hResult = D3DXCreateTextureFromFileInMemoryEx(tbDirect3D::GetDevice(),
		                                                    (BYTE*)(pTGAFile->GetBuffer()) + pTGAFile->GetCursor(),
															pTGAFile->GetSize() - pTGAFile->GetCursor(),
															D3DX_DEFAULT, D3DX_DEFAULT, 1, 0,
															D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
															D3DX_DEFAULT, D3DX_DEFAULT,
															0, NULL, NULL, &m_pTexture)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("D3DXCreateTextureFromFileInMemoryEx", hResult, TB_ERROR);
	}

	// Informationen laden
	if(pTBFFile->Read(sizeof(tbFontInfo), &m_FontInfo))
	{
		// Fehler!
		TB_ERROR("Fehler beim Lesen der Schriftinformationen!", TB_ERROR);
	}

	// Effekt aus String erstellen
	m_pEffect = new tbEffect; if(m_pEffect == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	if(m_pEffect->Init("TEXTURE Texture;\n"
		               "\n"
		               "TECHNIQUE T1\n"
		               "{\n"
					   "	PASS P1\n"
					   "	{\n"
					   "        Texture[0]          = <Texture>;\n"
					   "		ZEnable				= False;\n"
					   "		ZWriteEnable		= False;\n"
					   "		ColorOp[0]			= Modulate;\n"
					   "		ColorArg1[0]		= Texture;\n"
					   "		ColorArg2[0]		= Current;\n"
					   "		ColorOp[1]			= Disable;\n"
					   "		AlphaOp[0]			= Modulate;\n"
					   "		AlphaArg1[0]		= Texture;\n"
					   "		AlphaArg2[0]		= Current;\n"
					   "		AlphaOp[1]			= Disable;\n"
					   "		AlphaBlendEnable	= True;\n"
					   "		SrcBlend			= SrcAlpha;\n"
					   "		DestBlend			= InvSrcAlpha;\n"
					   "	}\n"
					   "}\n", -1))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Effekts!", TB_ERROR);
	}

	// Textur setzen
	m_pEffect->GetEffect()->SetTexture("Texture", m_pTexture);

	return TB_OK;
}

// ******************************************************************
// Laden aus echten Dateien
tbResult tbFont::Init(char* pcTGAFile,
					  char* pcTBFFile)
{
	tbVFile* pTGAFile;
	tbVFile* pTBFFile;


	// Parameter prüfen
	if(pcTGAFile == NULL) TB_ERROR_NULL_POINTER("pcTGAFile", TB_ERROR);
	if(pcTBFFile == NULL) TB_ERROR_NULL_POINTER("pcTBFFile", TB_ERROR);


	// Virtuelle Dateien erstellen
	pTGAFile = new tbVFile;	if(pTGAFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	pTBFFile = new tbVFile;	if(pTBFFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Aus Datei laden
	if(pTGAFile->Init(pcTGAFile)) TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);
	if(pTBFFile->Init(pcTBFFile)) TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);

	// Die andere Methode aufrufen
	if(Init(pTGAFile, pTBFFile))
	{
		// Fehler!
		TB_ERROR("Fehler beim Laden der Schriftart!", TB_ERROR);
	}

	// Die virtuellen Dateien wieder freigeben
	TB_SAFE_DELETE(pTGAFile);
	TB_SAFE_DELETE(pTBFFile);

	return TB_OK;
}

// ******************************************************************
// Laden aus dem Speicher
tbResult tbFont::Init(void* pTGAFileMem,
					  int iTGAFileMemSize,
					  void* pTBFFileMem,
					  int iTBFFileMemSize)
{
	tbVFile* pTGAFile;
	tbVFile* pTBFFile;


	// Parameter prüfen
	if(pTGAFileMem == NULL)		TB_ERROR_NULL_POINTER("pTGAFileMem", TB_ERROR);
	if(iTGAFileMemSize <= 0)	TB_ERROR_INVALID_VALUE("iTGAFileMemSize", TB_ERROR);
	if(pTBFFileMem == NULL)		TB_ERROR_NULL_POINTER("pTBFFileMem", TB_ERROR);
	if(iTBFFileMemSize <= 0)	TB_ERROR_INVALID_VALUE("iTBFFileMemSize", TB_ERROR);


	// Virtuelle Dateien erstellen
	pTGAFile = new tbVFile; if(pTGAFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	if(pTGAFile->Init(pTGAFileMem, iTGAFileMemSize)) TB_ERROR("Fehler beim Erstellen der virtuellen TGA-Datei!", TB_ERROR);
	pTBFFile = new tbVFile; if(pTBFFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	if(pTBFFile->Init(pTBFFileMem, iTBFFileMemSize)) TB_ERROR("Fehler beim Erstellen der virtuellen TBF-Datei!", TB_ERROR);

	// Die andere Methode aufrufen
	if(Init(pTGAFile, pTBFFile))
	{
		// Fehler!
		TB_SAFE_DELETE(pTGAFile);
		TB_SAFE_DELETE(pTBFFile);
		TB_ERROR("Fehler beim Laden der Schriftart!", TB_ERROR);
	}

	// Die virtuellen Dateien freigeben
	TB_SAFE_DELETE(pTGAFile);
	TB_SAFE_DELETE(pTBFFile);

	return TB_OK;
}

// ******************************************************************
// Laden aus einer Ressource
tbResult tbFont::Init(HMODULE hModule,
					  char* pcTGAFileResName,
					  char* pcTBFFileResName,
					  char* pcResourceType)
{
	tbVFile* pTGAFile;
	tbVFile* pTBFFile;


	// Parameter prüfen
	if(hModule == NULL)				TB_ERROR_NULL_POINTER("pTGAFileMem", TB_ERROR);
	if(pcTGAFileResName == NULL)	TB_ERROR_NULL_POINTER("pcTGAFileResName", TB_ERROR);
	if(pcTBFFileResName == NULL)	TB_ERROR_NULL_POINTER("pcTBFFileResName", TB_ERROR);
	if(pcResourceType == NULL)		TB_ERROR_NULL_POINTER("pcResourceType", TB_ERROR);


	// Virtuelle Dateien erstellen
	pTGAFile = new tbVFile; if(pTGAFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	if(pTGAFile->Init(hModule, pcTGAFileResName, pcResourceType)) TB_ERROR("Fehler beim Erstellen der virtuellen TGA-Datei!", TB_ERROR);
	pTBFFile = new tbVFile; if(pTBFFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	if(pTBFFile->Init(hModule, pcTBFFileResName, pcResourceType)) TB_ERROR("Fehler beim Erstellen der virtuellen TBF-Datei!", TB_ERROR);

	// Die andere Methode aufrufen
	if(Init(pTGAFile, pTBFFile))
	{
		// Fehler!
		TB_SAFE_DELETE(pTGAFile);
		TB_SAFE_DELETE(pTBFFile);
		TB_ERROR("Fehler beim Laden der Schriftart!", TB_ERROR);
	}

	// Die virtuellen Dateien freigeben
	TB_SAFE_DELETE(pTGAFile);
	TB_SAFE_DELETE(pTBFFile);

	return TB_OK;
}

// ******************************************************************
// Diese Methode berechnet die Breite eines Texts.
float tbFont::ComputeTextWidth(const char* pcText,
							   int iLength,			// = -1
							   const float fSpace)	// = -2.0f
{
	// Parameter prüfen
	if(pcText == NULL) TB_ERROR_NULL_POINTER("pcText", 0.0f);
	if(strlen(pcText) == 0) return 0.0f;

	float fWidth = 0.0f;

	// Länge anpassen
	if(iLength == -1) iLength = (int)(strlen(pcText));
	if(iLength == 0) return 0.0f;

	// Jedes Zeichen durchgehen
	for(DWORD dwChar = 0; dwChar < (DWORD)(iLength); dwChar++)
	{
		// Breite des Zeichens addieren
		fWidth += m_FontInfo.afCharWidth[(BYTE)(pcText[dwChar])];
	}

	// n Zeichen: n - 1 Leerräume
	fWidth += (float)(iLength - 1) * fSpace;

	return fWidth;
}

// ******************************************************************
// Diese Methode berechnet reelle die Breite eines Texts.
float tbFont::ComputeRealTextWidth(tbVector2 vPosition,
								   const char* pcText,
								   const DWORD dwFlags,		// = 0
								   int iLength,				// = -1
						  		   tbVector2 vScaling,		// = tbVector2(1.0f)
						  		   float fSpace,			// = -2.0f
						  		   float fItalic,			// = 0.0f
						  		   float fTabStops)			// = 25.0f
{
	// Parameter prüfen
	if(pcText == NULL) TB_ERROR_NULL_POINTER(0.0f, TB_ERROR);
	if(strlen(pcText) == 0) return 0.0f;

	float fMin = 100000.0f;
	float fMax = -100000.0f;
	tbVector2 vCursor(vPosition);

	// Relative Koordinaten in absolute umrechnen
	if(dwFlags & TB_FF_RELATIVE)
	{
		vPosition.x *= tbDirect3D::GetScreenSize().x;
		vPosition.y *= tbDirect3D::GetScreenSize().y;
		fTabStops *= tbDirect3D::GetScreenSize().x / 640.0f;
	}

	// Relative Skalierung in absolute umrechnen
	if(dwFlags & TB_FF_RELATIVESCALING)
	{
		vScaling.x *= tbDirect3D::GetScreenSize().x / 640.0f;
		vScaling.y *= tbDirect3D::GetScreenSize().y / 480.0f;
	}

	// Zeichenhöhe berechnen
	float fCharHeight = m_FontInfo.fHeight * vScaling.y;

	// Cursor auf die horizontale Startposition setzen
	if(dwFlags & TB_FF_ALIGN_HCENTER)
	{
		// Zentrieren
		float fTextWidth = ComputeTextWidth(pcText, iLength, fSpace) * vScaling.x;
		vCursor.x = vPosition.x - 0.5f * fTextWidth;
	}
	else if(dwFlags & TB_FF_ALIGN_HRIGHT)
	{
		// Rechtsbündig
		float fTextWidth = ComputeTextWidth(pcText, iLength, fSpace) * vScaling.x;
		vCursor.x = vPosition.x - fTextWidth;
	}
	else
	{
		// Linksbündig
		vCursor.x = vPosition.x;
	}

	// Länge, "Kursivität" und Abstände anpassen (mit Skalierung multiplizieren)
	if(iLength == -1) iLength = (int)(strlen(pcText));
	fItalic *= vScaling.x;
	fSpace *= vScaling.x;

	// Cursor auf die vertikale Startposition setzen
	if(dwFlags & TB_FF_ALIGN_VCENTER)
	{
		// Text vertikal zentrieren
		vCursor.y = vPosition.y - 0.5f * m_FontInfo.fHeight * vScaling.y;
	}
	else if(dwFlags & TB_FF_ALIGN_VTOP)
	{
		// Text oben ausrichten
		vCursor.y = vPosition.y - m_FontInfo.fHeight * vScaling.y;
	}
	else
	{
		// Text unten ausrichten
		vCursor.y = vPosition.y;
	}

	// Startcursorposition speichern
	tbVector2 vStartCursor(vCursor);

	// Jedes einzelne Zeichen durchgehen
	for(DWORD dwChar = 0; dwChar < (DWORD)(iLength); dwChar++)
	{
		// Wenn das Zeichen ein Neue-Zeile-Zeichen ("\n") ist, dann wird vCursor.y
		// erhöht und vCursor.x auf vStartCursor.x zurückgesetzt.
		if(pcText[dwChar] == '\n')
		{
			vCursor.x = vStartCursor.x;
		}
		else if(pcText[dwChar] == '\t')
		{
			// Das Zeichen ist ein Tabulator!
			// vCursor.x wird bis zum nächsten Tab-Stop bewegt.
			vCursor.x += fTabStops - fmodf(vCursor.x, fTabStops);
		}
		else if(pcText[dwChar] == '\\')
		{
			if(dwChar < (DWORD)(iLength) - 1)
			{
				if(pcText[dwChar + 1] == 'n')
				{
					vCursor.x = vStartCursor.x;
					dwChar++;
				}
				else if(pcText[dwChar + 1] == 't')
				{
					vCursor.x += fTabStops - fmodf(vCursor.x, fTabStops);
					dwChar++;
				}
			}
		}
		else
		{
			// Es ist ein anzeigbares Zeichen!
			// Breite des Zeichens abfragen
			float fCharWidth = m_FontInfo.afCharWidth[(BYTE)(pcText[dwChar])] * vScaling.x;

			fMin = TB_MIN(fMin, vCursor.x);
			fMax = TB_MAX(fMax, vCursor.x + fCharWidth + fItalic);

			// Cursor aktualisieren
			vCursor.x += fCharWidth + fSpace;
		}
	}

	if(fMin == 100000.0f || fMax == -100000.0f) return 0.0f;

	return fMax - fMin;
}

// ******************************************************************
// Beginnt mit der Schriftart
tbResult tbFont::Begin()
{
	// Aktuellen Status speichern
	m_dwOldFVF = tbDirect3D::GetFVF();

	// Neues Vertexformat setzen
	tbDirect3D::SetFVF(TB_FONT_FVF);

	// Mit dem Effekt beginnen
	m_iNumPasses = m_pEffect->Begin();

	return TB_OK;
}

// ******************************************************************
// Beenden
tbResult tbFont::End()
{
	// Effekt beenden
	m_pEffect->End();

	// Wiederherstellen des alten Status
	tbDirect3D::SetFVF(m_dwOldFVF);

	return TB_OK;
}

// ******************************************************************
// Diese Methode zeichnet einen Text.
tbResult tbFont::DrawText(tbVector2 vPosition,
						  const char* pcText,
						  const DWORD dwFlags,		// = 0
						  int iLength,				// = -1
						  const tbColor& Color1,	// = tbColor(1.0f)
						  tbColor& Color2,			// = tbColor(-1.0f)
						  tbVector2 vScaling,		// = tbVector2(1.0f)
						  float fSpace,				// = -2.0f
						  float fItalic,			// = 0.0f
						  float fLineSpace,			// = -5.0f
						  float fTabStops,			// = 25.0f
						  float fXOffset)			// = 0.0f
{
	tbFontVertex	aVertex[600];
	DWORD			dwVertexCursor = 0;


	// Parameter prüfen
	if(pcText == NULL) TB_ERROR_NULL_POINTER("pcText", TB_ERROR);
	if(strlen(pcText) == 0) return TB_OK;

	if(Color2.r < 0.0f) Color2 = Color1;
	tbVector2 vCursor(vPosition);

	// Relative Koordinaten in absolute umrechnen
	if(dwFlags & TB_FF_RELATIVE)
	{
		vPosition.x *= tbDirect3D::GetScreenSize().x;
		vPosition.y *= tbDirect3D::GetScreenSize().y;
		fTabStops *= tbDirect3D::GetScreenSize().x / 640.0f;
	}

	// Relative Skalierung in absolute umrechnen
	if(dwFlags & TB_FF_RELATIVESCALING)
	{
		vScaling.x *= tbDirect3D::GetScreenSize().x / 640.0f;
		vScaling.y *= tbDirect3D::GetScreenSize().y / 480.0f;
	}

	// Zeichenhöhe berechnen
	float fCharHeight = m_FontInfo.fHeight * vScaling.y;

	// Cursor auf die horizontale Startposition setzen
	if(dwFlags & TB_FF_ALIGN_HCENTER)
	{
		// Zentrieren
		float fTextWidth = ComputeTextWidth(pcText, iLength, fSpace) * vScaling.x;
		vCursor.x = vPosition.x - 0.5f * fTextWidth;
	}
	else if(dwFlags & TB_FF_ALIGN_HRIGHT)
	{
		// Rechtsbündig
		float fTextWidth = ComputeTextWidth(pcText, iLength, fSpace) * vScaling.x;
		vCursor.x = vPosition.x - fTextWidth;
	}
	else
	{
		// Linksbündig
		vCursor.x = vPosition.x;
	}

	// Länge, "Kursivität" und Abstände anpassen (mit Skalierung multiplizieren)
	if(iLength == -1) iLength = (int)(strlen(pcText));
	fItalic *= vScaling.x;
	fSpace *= vScaling.x;
	fLineSpace *= vScaling.y;
	fXOffset *= vScaling.x;

	// Cursor auf die vertikale Startposition setzen
	if(dwFlags & TB_FF_ALIGN_VCENTER)
	{
		// Text vertikal zentrieren
		vCursor.y = vPosition.y - 0.5f * m_FontInfo.fHeight * vScaling.y;
	}
	else if(dwFlags & TB_FF_ALIGN_VBOTTOM)
	{
		// Text oben ausrichten
		vCursor.y = vPosition.y - m_FontInfo.fHeight * vScaling.y;
	}
	else
	{
		// Text unten ausrichten
		vCursor.y = vPosition.y;
	}

	// Startcursorposition speichern
	tbVector2 vStartCursor(vCursor);

	// Jedes einzelne Zeichen durchgehen
	for(DWORD dwChar = 0; dwChar < (DWORD)(iLength); dwChar++)
	{
		// Wenn das Zeichen ein Neue-Zeile-Zeichen ("\n") ist, dann wird vCursor.y
		// erhöht und vCursor.x auf vStartCursor.x zurückgesetzt.
		if(pcText[dwChar] == '\n')
		{
			vCursor.x = vStartCursor.x;
			vCursor.y += (m_FontInfo.fHeight * vScaling.y) + fLineSpace;
		}
		else if(pcText[dwChar] == '\t')
		{
			// Das Zeichen ist ein Tabulator!
			// vCursor.x wird bis zum nächsten Tab-Stop bewegt.
			vCursor.x += fTabStops - fmodf(vCursor.x, fTabStops);
		}
		else if(pcText[dwChar] == '\\')
		{
			if(dwChar < (DWORD)(iLength) - 1)
			{
				if(pcText[dwChar + 1] == 'n')
				{
					vCursor.x = vStartCursor.x;
					vCursor.y += (m_FontInfo.fHeight * vScaling.y) + fLineSpace;
					dwChar++;
				}
				else if(pcText[dwChar + 1] == 't')
				{
					vCursor.x += fTabStops - fmodf(vCursor.x, fTabStops);
					dwChar++;
				}
			}
		}
		else
		{
			// Es ist ein anzeigbares Zeichen!
			// Breite des Zeichens abfragen
			float fCharWidth = m_FontInfo.afCharWidth[(BYTE)(pcText[dwChar])] * vScaling.x;

			// Sechs Vertizes für das aktuelle Zeichen hinzufügen.
			// Dazu verwenden wir Dreieckslisten - jeweils sechs Vertizes
			// beschreiben ein Viereck.

			// Erster Vertex: links oben
			aVertex[dwVertexCursor].vPosition = tbVector3(vCursor.x + fItalic + fXOffset, vCursor.y, 0.0f);
			aVertex[dwVertexCursor].fRHW = 1.0f;
			aVertex[dwVertexCursor].Diffuse = tbColorInterpolate(Color1, Color2, (float)(dwChar) / (float)(iLength));
			aVertex[dwVertexCursor].vTexture = m_FontInfo.avTopLeft[(BYTE)(pcText[dwChar])];

			// Zweiter Vertex: rechts oben
			aVertex[dwVertexCursor + 1].vPosition = tbVector3(vCursor.x + fCharWidth + fItalic + fXOffset, vCursor.y, 0.0f);
			aVertex[dwVertexCursor + 1].fRHW = 1.0f;
			aVertex[dwVertexCursor + 1].Diffuse = tbColorInterpolate(Color1, Color2, (float)(dwChar + 1) / (float)(iLength));
			aVertex[dwVertexCursor + 1].vTexture.x = m_FontInfo.avBottomRight[(BYTE)(pcText[dwChar])].x;
			aVertex[dwVertexCursor + 1].vTexture.y = m_FontInfo.avTopLeft[(BYTE)(pcText[dwChar])].y;

			// Dritter Vertex: rechts unten
			aVertex[dwVertexCursor + 2].vPosition = tbVector3(vCursor.x + fCharWidth + fXOffset,
															  vCursor.y + fCharHeight,
															  0.0f);
			aVertex[dwVertexCursor + 2].fRHW = 1.0f;
			aVertex[dwVertexCursor + 2].Diffuse = aVertex[dwVertexCursor + 1].Diffuse;
			aVertex[dwVertexCursor + 2].vTexture = m_FontInfo.avBottomRight[(BYTE)(pcText[dwChar])];

			// Vierter Vertex = erster Vertex
			aVertex[dwVertexCursor + 3] = aVertex[dwVertexCursor];

			// Fünfter Vertex = dritter Vertex
			aVertex[dwVertexCursor + 4] = aVertex[dwVertexCursor + 2];

			// Sechster Vertex: links unten
			aVertex[dwVertexCursor + 5].vPosition = tbVector3(vCursor.x + fXOffset,
															  vCursor.y + fCharHeight,
															  0.0f);
			aVertex[dwVertexCursor + 5].fRHW = 1.0f;
			aVertex[dwVertexCursor + 5].Diffuse = aVertex[dwVertexCursor].Diffuse;
			aVertex[dwVertexCursor + 5].vTexture.x = m_FontInfo.avTopLeft[(BYTE)(pcText[dwChar])].x;
			aVertex[dwVertexCursor + 5].vTexture.y = m_FontInfo.avBottomRight[(BYTE)(pcText[dwChar])].y;

			// Vertexcursor aktualisieren
			dwVertexCursor += 6;

			// Cursor aktualisieren
			vCursor.x += fCharWidth + fSpace;
		}

		// Wenn das Vertex-Array voll oder der Text zu Ende ist, dann wird jetzt gezeichnet.
		if(dwVertexCursor >= 600 ||
		   dwChar == (DWORD)(iLength - 1))
		{
			// Jeden Durchgang des Effekts zeichnen
			for(int iPass = 0; iPass < m_iNumPasses; iPass++)
			{
				m_pEffect->BeginPass(iPass);
				tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
					                                         dwVertexCursor / 3,
															 aVertex,
															 sizeof(tbFontVertex));
				m_pEffect->EndPass();
			}

			// Vertexcursor zurücksetzen
			dwVertexCursor = 0;
		}
	}

	return TB_OK;
}

// ******************************************************************
// Text mit Transformationsmatrix zeichnen
tbResult tbFont::DrawTransformedText(tbMatrix mTransformation,
									 const char* pcText,
									 const DWORD dwFlags,	// = 0
									 int iLength,			// = -1
									 const tbColor& Color1,	// = tbColor(1.0f)
									 tbColor& Color2,		// = tbColor(-1.0f)
									 float fSpace,			// = -2.0f
									 float fItalic,			// = 0.0f
  									 float fLineSpace,		// = -5.0f
									 float fTabStops,		// = 25.0f
									 float fXOffset)		// = 0.0f
{
	tbFontVertex	aVertex[600];
	DWORD			dwVertexCursor = 0;


	if(Color2.r < 0.0f) Color2 = Color1;
	tbVector2 vCursor(0.0f);

	// Parameter prüfen
	if(pcText == NULL) TB_ERROR_NULL_POINTER("pcText", TB_ERROR);


	// Relative Koordinaten in absolute umrechnen
	if(dwFlags & TB_FF_RELATIVE)
	{
		mTransformation.m41 *= tbDirect3D::GetScreenSize().x;
		mTransformation.m42 *= tbDirect3D::GetScreenSize().y;
		fTabStops *= tbDirect3D::GetScreenSize().x / 640.0f;
	}

	// Relative Größe in absolute umrechnen
	if(dwFlags & TB_FF_RELATIVESCALING)
	{
		mTransformation.m11 *= tbDirect3D::GetScreenSize().x / 640.0f;
		mTransformation.m22 *= tbDirect3D::GetScreenSize().y / 480.0f;
	}

	// Länge anpassen
	if(iLength == -1) iLength = (int)(strlen(pcText));

	// Cursor auf die horizontale Startposition setzen
	if(dwFlags & TB_FF_ALIGN_HCENTER)
	{
		// Zentrieren
		float fTextWidth = ComputeTextWidth(pcText, iLength, fSpace);
		vCursor.x = -0.5f * fTextWidth;
	}
	else if(dwFlags & TB_FF_ALIGN_HRIGHT)
	{
		// Rechtsbündig
		float fTextWidth = ComputeTextWidth(pcText, iLength, fSpace);
		vCursor.x = -fTextWidth;
	}
	else
	{
		// Linksbündig
		vCursor.x = 0.0f;
	}

	// Cursor auf die vertikale Startposition setzen
	if(dwFlags & TB_FF_ALIGN_VCENTER)
	{
		// Text vertikal zentrieren
		vCursor.y = -0.5f * m_FontInfo.fHeight;
	}
	else if(dwFlags & TB_FF_ALIGN_VBOTTOM)
	{
		// Text oben ausrichten
		vCursor.y = -m_FontInfo.fHeight;
	}
	else
	{
		// Text unten ausrichten
		vCursor.y = 0.0f;
	}

	// Startcursorposition speichern
	tbVector2 vStartCursor(vCursor);

	// Jedes einzelne Zeichen durchgehen
	for(DWORD dwChar = 0; dwChar < (DWORD)(iLength); dwChar++)
	{
		// Wenn das Zeichen ein Neue-Zeile-Zeichen ("\n") ist, dann wird vCursor.y
		// erhöht und vCursor.x auf vStartCursor.x zurückgesetzt.
		if(pcText[dwChar] == '\n')
		{
			vCursor.x = vStartCursor.x;
			vCursor.y += m_FontInfo.fHeight + fLineSpace;
		}
		else if(pcText[dwChar] == '\t')
		{
			// Das Zeichen ist ein Tabulator!
			// vCursor.x wird bis zum nächsten Tab-Stop bewegt.
			vCursor.x += fTabStops - fmodf(vCursor.x, fTabStops);
		}
		else if(pcText[dwChar] == '\\')
		{
			if(dwChar < (DWORD)(iLength) - 1)
			{
				if(pcText[dwChar + 1] == 'n')
				{
					vCursor.x = vStartCursor.x;
					vCursor.y += m_FontInfo.fHeight + fLineSpace;
					dwChar++;
				}
				else if(pcText[dwChar + 1] == 't')
				{
					vCursor.x += fTabStops - fmodf(vCursor.x, fTabStops);
					dwChar++;
				}
			}
		}
		else
		{
			// Es ist ein anzeigbares Zeichen!
			// Breite des Zeichens abfragen
			float fCharWidth = m_FontInfo.afCharWidth[(BYTE)(pcText[dwChar])];

			// Sechs Vertizes für das aktuelle Zeichen hinzufügen.
			// Dazu verwenden wir Dreieckslisten - jeweils sechs Vertizes
			// beschreiben ein Viereck.

			// Erster Vertex: links oben
			aVertex[dwVertexCursor].vPosition = tbVector3(vCursor.x + fItalic + fXOffset, vCursor.y, 0.0f);
			aVertex[dwVertexCursor].vPosition = tbVector3TransformCoords(aVertex[dwVertexCursor].vPosition, mTransformation);
			aVertex[dwVertexCursor].fRHW = 1.0f;
			aVertex[dwVertexCursor].Diffuse = tbColorInterpolate(Color1, Color2, (float)(dwChar) / (float)(iLength));
			aVertex[dwVertexCursor].vTexture = m_FontInfo.avTopLeft[(BYTE)(pcText[dwChar])];

			// Zweiter Vertex: rechts oben
			aVertex[dwVertexCursor + 1].vPosition = tbVector3(vCursor.x + fCharWidth + fItalic + fXOffset, vCursor.y, 0.0f);
			aVertex[dwVertexCursor + 1].vPosition = tbVector3TransformCoords(aVertex[dwVertexCursor + 1].vPosition, mTransformation);
			aVertex[dwVertexCursor + 1].fRHW = 1.0f;
			aVertex[dwVertexCursor + 1].Diffuse = tbColorInterpolate(Color1, Color2, (float)(dwChar + 1) / (float)(iLength));
			aVertex[dwVertexCursor + 1].vTexture.x = m_FontInfo.avBottomRight[(BYTE)(pcText[dwChar])].x;
			aVertex[dwVertexCursor + 1].vTexture.y = m_FontInfo.avTopLeft[(BYTE)(pcText[dwChar])].y;

			// Dritter Vertex: rechts unten
			aVertex[dwVertexCursor + 2].vPosition = tbVector3(vCursor.x + fCharWidth + fXOffset,
															  vCursor.y + m_FontInfo.fHeight,
															  0.0f);
			aVertex[dwVertexCursor + 2].vPosition = tbVector3TransformCoords(aVertex[dwVertexCursor + 2].vPosition, mTransformation);
			aVertex[dwVertexCursor + 2].fRHW = 1.0f;
			aVertex[dwVertexCursor + 2].Diffuse = aVertex[dwVertexCursor + 1].Diffuse;
			aVertex[dwVertexCursor + 2].vTexture = m_FontInfo.avBottomRight[(BYTE)(pcText[dwChar])];

			// Vierter Vertex = erster Vertex
			aVertex[dwVertexCursor + 3] = aVertex[dwVertexCursor];

			// Fünfter Vertex = dritter Vertex
			aVertex[dwVertexCursor + 4] = aVertex[dwVertexCursor + 2];

			// Sechster Vertex: links unten
			aVertex[dwVertexCursor + 5].vPosition = tbVector3(vCursor.x + fXOffset,
															  vCursor.y + m_FontInfo.fHeight,
															  0.0f);
			aVertex[dwVertexCursor + 5].vPosition = tbVector3TransformCoords(aVertex[dwVertexCursor + 5].vPosition, mTransformation);
			aVertex[dwVertexCursor + 5].fRHW = 1.0f;
			aVertex[dwVertexCursor + 5].Diffuse = aVertex[dwVertexCursor].Diffuse;
			aVertex[dwVertexCursor + 5].vTexture.x = m_FontInfo.avTopLeft[(BYTE)(pcText[dwChar])].x;
			aVertex[dwVertexCursor + 5].vTexture.y = m_FontInfo.avBottomRight[(BYTE)(pcText[dwChar])].y;

			// Vertexcursor aktualisieren
			dwVertexCursor += 6;

			// Cursor aktualisieren
			vCursor.x += fCharWidth + fSpace;
		}

		// Wenn das Vertex-Array voll oder der Text zu Ende ist, dann wird jetzt gezeichnet.
		if(dwVertexCursor >= 600 ||
		   dwChar == (DWORD)(iLength - 1))
		{
			// Jeden Durchgang des Effekts zeichnen
			for(int iPass = 0; iPass < m_iNumPasses; iPass++)
			{
				m_pEffect->BeginPass(iPass);
				tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
					                                         dwVertexCursor / 3,
														     aVertex,
															 sizeof(tbFontVertex));
				m_pEffect->EndPass();
			}

			// Vertexcursor zurücksetzen
			dwVertexCursor = 0;
		}
	}

	return TB_OK;
}

// ******************************************************************