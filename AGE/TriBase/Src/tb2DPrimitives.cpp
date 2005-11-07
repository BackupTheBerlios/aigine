/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tb2DPrimitives.cpp
	==================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit 2D-Sprites

	Autor:
	Snorky
	[21.8.2003]

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// alles freigeben und löschen
tbResult tb2DPrimitives::Exit()
{
	TB_SAFE_DELETE(m_pEffect);

	// Alles zurücksetzen
	ZeroMemory(this, sizeof(*this));

	return TB_OK;
}

// ******************************************************************
// Initialisierung aus virtuellen Dateien
tbResult tb2DPrimitives::Init(char* pcINIFile,
						  char* pcINISection)
{
	// vorsichtshalber alles vorher freigeben
	Exit();

	tbResult	tbRes;

	// Parameter prüfen und sicherstellen, dass tbDirect3D initialisiert wurde
	if(pcINIFile == NULL)				TB_ERROR_NULL_POINTER("pcINIFile", TB_ERROR);
	if(pcINISection == NULL)			TB_ERROR_NULL_POINTER("pcINISection", TB_ERROR);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Es muss zuerst eine tbDirect3D-Klasseninstanz erstellt werden!", TB_ERROR);

	
	// Informationsdatei vorbereiten
	tbINIReader* pINIReader;
	pINIReader = new tbINIReader; 	if(pINIReader == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	pINIReader->SetFilePath(pcINIFile);

	// Effektname aus INI laden
	char acEffect[256];
	ZeroMemory(acEffect, sizeof(acEffect));
	pINIReader->ReadINIString(pcINISection, "Effect", acEffect, 256);
	if(!strcmp(acEffect, "[NOT FOUND]")) TB_ERROR("Fehler beim Laden des Primitives-Effekts!", TB_ERROR);

	// Effekt laden
	m_pEffect = new tbEffect; if(m_pEffect == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	if(TB_OK != (tbRes = m_pEffect->Init(acEffect))) TB_ERROR("Fehler beim Erstellen des Sprite-Effekts!", TB_ERROR);
	// Effekt setzen
	if(TB_OK != m_pEffect->SetTechnique())	TB_ERROR("Fehler beim Setzen des Sprite-Effekts!", TB_ERROR);

	// INIReader wieder freigeben
	TB_SAFE_DELETE(pINIReader);

	return TB_OK;
}

// ******************************************************************
// Beginnt mit dem Zeichnen
tbResult tb2DPrimitives::Begin()
{
	// Aktuellen Status speichern
	m_dwOldFVF = tbDirect3D::GetFVF();

	// Neues Vertexformat setzen
	tbDirect3D::SetFVF(TB_2DPRIMITIVES_FVF);

	// Mit dem Effekt beginnen
	m_iNumPasses = m_pEffect->Begin();

	// Vertexcursor setzen
	dwVertexCursor = 0;

	return TB_OK;
}

// ******************************************************************
// Beenden
tbResult tb2DPrimitives::End()
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
// Alle Primitiven im Puffer sofort malen
tbResult tb2DPrimitives::DrawPuffer()
{
	// Jeden Durchgang des Effekts zeichnen
	for(int iPass = 0; iPass < m_iNumPasses; iPass++)
	{
		m_pEffect->BeginPass(iPass);

		tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_LINELIST,
			                                     dwVertexCursor / 2,
												 aVertex,
												 sizeof(tb2DPrimitivesVertex));
		m_pEffect->EndPass();
	}

	// Vertexcursor zurücksetzen
	dwVertexCursor = 0;

	return TB_OK;
}

// ******************************************************************
// Zeichnen einer Linie
tbResult tb2DPrimitives::DrawLine(tbVector4 vCoords, 
								tbColor vColor1,		// = tbColor(1.0f)
								tbColor vColor2)		// = tbColor(-1.0f)
{
	// Parameter prüfen
	if(vColor2 == tbColor(-1.0f)) vColor2 = vColor1;


	// StartVertex:
	aVertex[dwVertexCursor].vPosition = tbVector3(vCoords.x1, vCoords.y1, 0.0f);
	aVertex[dwVertexCursor].fRHW = 1.0f;
	aVertex[dwVertexCursor].Diffuse = vColor1;

	// ZielVertex:
	aVertex[dwVertexCursor + 1].vPosition = tbVector3(vCoords.x2, vCoords.y2, 0.0f);
	aVertex[dwVertexCursor + 1].fRHW = 1.0f;
	aVertex[dwVertexCursor + 1].Diffuse = vColor2;

	// Vertexcursor aktualisieren
	dwVertexCursor += 2;


	// Wenn das Vertex-Array voll ist, dann wird jetzt gezeichnet.
	if(dwVertexCursor >= 600)
	{
		DrawPuffer();
	}

	return TB_OK;
}

// ******************************************************************
// Zeichnen einer Linie
tbResult tb2DPrimitives::DrawLine(tbVector2 vStartCoords, 
								tbVector2 vZielCoords, 
								tbColor vColor1,		// = tbColor(1.0f)
								tbColor vColor2)		// = tbColor(-1.0f)
{
	return DrawLine(tbVector4(vStartCoords, vZielCoords), vColor1, vColor2);
}

// ******************************************************************
// Zeichnen einer Linie
tbResult tb2DPrimitives::DrawLine(DWORD dwStartCoordX, 
								DWORD dwStartCoordY, 
								DWORD dwZielCoordX,
								DWORD dwZielCoordY,
								tbColor vColor1,		// = tbColor(1.0f)
								tbColor vColor2)		// = tbColor(-1.0f)
{
	return DrawLine(tbVector4((float)dwStartCoordX, (float)dwStartCoordY, (float)dwZielCoordX, (float)dwZielCoordY), vColor1, vColor2);
}

// ******************************************************************
// Zeichnen einer Linie
tbResult tb2DPrimitives::DrawLine(float fStartCoordX, 
								float fStartCoordY, 
								float fZielCoordX,
								float fZielCoordY,
								tbColor vColor1,		// = tbColor(1.0f)
								tbColor vColor2)		// = tbColor(-1.0f)
{
	return DrawLine(tbVector4(fStartCoordX, fStartCoordY, fZielCoordX, fZielCoordY), vColor1, vColor2);
}

// ******************************************************************

