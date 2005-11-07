/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbSkyBox.cpp
	============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Einfaches Erstellen von Sky-Boxes

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbSkyBox-Klasse
tbSkyBox::tbSkyBox()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbSkyBox));
}

// ******************************************************************
// Destruktor der tbSkyBox-Klasse
tbSkyBox::~tbSkyBox()
{
	// Vertex- und Index-Buffer löschen
	TB_SAFE_DELETE(m_pVB);
	TB_SAFE_DELETE(m_pIB);

	// Effekt löschen, vorher die Textur rausnehmen
	m_pEffect->GetEffect()->SetTexture("Texture", NULL);
	TB_SAFE_DELETE(m_pEffect);
}

// ******************************************************************
// Initialisiert die Sky-Box
tbResult tbSkyBox::Init(PDIRECT3DCUBETEXTURE9 pCubeMap)
{
	tbSkyBoxVertex aVertex[8];

	// Sicherstellen, dass Direct3D korrekt initialisiert wurde
	if(!tbDirect3D::IsInitialized()) TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Vertex-Buffer erstellen
	m_pVB = new tbVertexBuffer;
	if(m_pVB->Init(8 * sizeof(tbSkyBoxVertex), sizeof(tbSkyBoxVertex),
		           TB_SKYBOX_FVF))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Vertex-Buffers!", TB_ERROR);
	}

	// Index-Buffer erstellen
	m_pIB = new tbIndexBuffer;
	if(m_pIB->Init(36 * sizeof(WORD), sizeof(WORD), D3DFMT_INDEX16))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Index-Buffers!", TB_ERROR);
	}

	// ------------------------------------------------------------------

	// Die Vertizes erstellen
	aVertex[0].vPosition = tbVector3(-1.0f,  1.0f,  1.0f);
	aVertex[1].vPosition = tbVector3( 1.0f,  1.0f,  1.0f);
	aVertex[2].vPosition = tbVector3( 1.0f,  1.0f, -1.0f);
	aVertex[3].vPosition = tbVector3(-1.0f,  1.0f, -1.0f);
	aVertex[4].vPosition = tbVector3(-1.0f, -1.0f,  1.0f);
	aVertex[5].vPosition = tbVector3( 1.0f, -1.0f,  1.0f);
	aVertex[6].vPosition = tbVector3( 1.0f, -1.0f, -1.0f);
	aVertex[7].vPosition = tbVector3(-1.0f, -1.0f, -1.0f);

	// Die Texturkoordinaten entsprechen den Vertexpositionen
	for(int iVertex = 0; iVertex < 8; iVertex++)
	{
		// Texturkoordinate eintragen und Sky-Box skalieren
		aVertex[iVertex].vTexture = aVertex[iVertex].vPosition;
		aVertex[iVertex].vPosition *= 50.0f;
	}

	// Alle Vertizes eintragen und den Vertex-Buffer aktualisieren
	m_pVB->AddVertices(8, aVertex);
	if(m_pVB->Update()) TB_ERROR("Fehler beim Schreiben in den Vertex-Buffer!", TB_ERROR);

	// ------------------------------------------------------------------

	// Den Index-Buffer ausfüllen
	WORD awIndex[36] = {7, 3, 0,   4, 7, 0,		// Vorderseite
						5, 1, 2,   6, 5, 2,		// Hinterseite
						4, 0, 1,   5, 4, 1,		// Linke Seite
						6, 2, 3,   7, 6, 3,		// Rechte Seite
						2, 1, 0,   3, 2, 0,		// Oberseite
						4, 5, 6,   7, 4, 6};	// Unterseite
	m_pIB->AddIndices(36, awIndex);
	if(m_pIB->Update()) TB_ERROR("Fehler beim Schreiben in den Index-Buffer!", TB_ERROR);

	// ------------------------------------------------------------------

	// Textur kopieren
	m_pCubeMap = pCubeMap;

	// Effekt aus String erstellen
	m_pEffect = new tbEffect;
	if(m_pEffect->Init("TEXTURE Texture;\n"
		               "\n"
		               "TECHNIQUE T1\n"
		               "{\n"
					   "	PASS P1\n"
					   "	{\n"
					   "        Texture[0]		= <Texture>;\n"
					   "		ZEnable			= False;\n"
					   "		ZWriteEnable	= False;\n"
					   "		ColorOp[0]		= SelectArg1;\n"
					   "		ColorArg1[0]	= Texture;\n"
					   "		ColorOp[1]		= Disable;\n"
					   "		Lighting		= False;\n"
					   "		ColorVertex		= False;\n"
					   "	}\n"
					   "}\n", -1))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Effekts!", TB_ERROR);
	}

	// Textur setzen
	m_pEffect->GetEffect()->SetTexture("Texture", m_pCubeMap);

	return TB_OK;
}

// ******************************************************************
// Rendert die Sky-Box
tbResult tbSkyBox::Render(const tbVector3& vCameraPos)
{
	HRESULT hResult;

	// Weltmatrix erstellen, welche die Sky-Box auf die Position der Kamera bringt
	tbDirect3D::SetTransform(D3DTS_WORLD, tbMatrixTranslation(vCameraPos));

	// Vertex- und Index-Buffer als Datenquelle angeben und Vertexformat setzen
	tbDirect3D::GetDevice()->SetStreamSource(0, m_pVB->GetVB(), 0, sizeof(tbSkyBoxVertex));
	tbDirect3D::GetDevice()->SetIndices(m_pIB->GetIB());
	tbDirect3D::SetFVF(TB_SKYBOX_FVF);

	// Effekt starten
	int iNumPasses = m_pEffect->Begin();

	for(int iPass = 0; iPass < iNumPasses; iPass++)
	{
		m_pEffect->BeginPass(iPass);
	
		// Primitiven rendern
		hResult = tbDirect3D::GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
		if(FAILED(hResult))
		{
			// Fehler!
			m_pEffect->End();
			TB_ERROR("Fehler beim Rendern der Sky-Box!", TB_ERROR);
		}
		m_pEffect->EndPass();
	}

	// Effekt beenden
	m_pEffect->End();

	return TB_OK;
}

// ******************************************************************