/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbShadowVolume.cpp
	==================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Rendern von Echtzeitschatten

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>


// ******************************************************************
// Konstruktor der tbShadowVolume-Klasse
tbShadowVolume::tbShadowVolume()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbShadowVolume));
}

// ******************************************************************
// Destruktor der tbShadowVolume-Klasse
tbShadowVolume::~tbShadowVolume()
{
	// Effekte löschen
	TB_SAFE_DELETE(m_pShadowVolumeEffect);
	TB_SAFE_DELETE(m_pShadowEffect);

	// Speicher freigeben
	TB_SAFE_MEMFREE(m_pEdges);
	TB_SAFE_MEMFREE(m_pvTriangleCenters);
	TB_SAFE_MEMFREE(m_pvVertices);
}

// ******************************************************************
// Initialisiert das Schattenvolumen
tbResult tbShadowVolume::Init(tbModel* pModel)
{
	// Parameter prüfen
	if(pModel == NULL)			TB_ERROR_NULL_POINTER("pModel", TB_ERROR);
	if(!pModel->m_bExtraData)	TB_ERROR("Das Modell hat keine Extradaten!", TB_ERROR);


	// Modell kopieren
	m_pModel = pModel;

	// Effekt für das Schattenvolumen erstellen
	m_pShadowVolumeEffect = new tbEffect;
	if(m_pShadowVolumeEffect->Init("TECHNIQUE T1\n"
								   "{\n"
								   "	PASS P1\n"
								   "	{\n"
								   "        Texture[0]			= Null;\n"
								   "		ZEnable				= True;\n"
								   "		ZWriteEnable		= False;\n"
								   "		ShadeMode			= Flat;\n"
								   "        FogEnable			= False;\n"
								   "		ColorOp[0]			= SelectArg1;\n"
								   "		ColorArg1[0]		= Current;\n"
								   "		ColorOp[1]			= Disable;\n"
								   "		Lighting			= False;\n"
								   "		StencilEnable		= True;\n"
								   "		StencilFunc			= Always;\n"
								   "		StencilFail			= Keep;\n"
								   "		StencilZFail		= Keep;\n"
								   "		StencilPass			= Decr;\n"
								   "		StencilMask			= 0xFFFFFFFF;\n"
								   "		StencilWriteMask	= 0xFFFFFFFF;\n"
								   "		TwoSidedStencilMode	= True;\n"
								   "		CCW_StencilFunc		= Always;\n"
								   "		CCW_StencilFail		= Keep;\n"
								   "		CCW_StencilZFail	= Keep;\n"
								   "		CCW_StencilPass		= Incr;\n"
								   "		CullMode			= None;\n"
								   "		ColorWriteEnable	= 0;\n"
								   "	}\n"
								   "}\n"
								   "\n"
								   "TECHNIQUE T2\n"
								   "{\n"
								   "	PASS P1\n"
								   "	{\n"
								   "        Texture[0]			= Null;\n"
								   "		ZEnable				= True;\n"
								   "		ZWriteEnable		= False;\n"
								   "		ShadeMode			= Flat;\n"
								   "        FogEnable			= False;\n"
								   "		ColorOp[0]			= SelectArg1;\n"
								   "		ColorArg1[0]		= Current;\n"
								   "		ColorOp[1]			= Disable;\n"
								   "		Lighting			= False;\n"
								   "		StencilEnable		= True;\n"
								   "		StencilFunc			= Always;\n"
								   "		StencilFail			= Keep;\n"
								   "		StencilZFail		= Keep;\n"
								   "		StencilPass			= Decr;\n"
								   "		StencilMask			= 0xFFFFFFFF;\n"
								   "		StencilWriteMask	= 0xFFFFFFFF;\n"
								   "		TwoSidedStencilMode	= True;\n"
								   "		CCW_StencilFunc		= Always;\n"
								   "		CCW_StencilFail		= Keep;\n"
								   "		CCW_StencilZFail	= Keep;\n"
								   "		CCW_StencilPass		= Incr;\n"
								   "		CullMode			= None;\n"
								   "		AlphaBlendEnable	= True;\n"
								   "		SrcBlend			= Zero;\n"
								   "		DestBlend			= One;\n"
								   "	}\n"
								   "}\n"
								   "\n"
								   "TECHNIQUE T3\n"
								   "{\n"
								   "	PASS P1\n"
								   "	{\n"
								   "        Texture[0]			= Null;\n"
								   "		ZEnable				= True;\n"
								   "		ZWriteEnable		= False;\n"
								   "		ShadeMode			= Flat;\n"
								   "        FogEnable			= False;\n"
								   "		ColorOp[0]			= SelectArg1;\n"
								   "		ColorArg1[0]		= Current;\n"
								   "		ColorOp[1]			= Disable;\n"
								   "		Lighting			= False;\n"
								   "		StencilEnable		= True;\n"
								   "		StencilFunc			= Always;\n"
								   "		StencilFail			= Keep;\n"
								   "		StencilZFail		= Keep;\n"
								   "		StencilPass			= Decr;\n"
								   "		StencilMask			= 0xFFFFFFFF;\n"
								   "		StencilWriteMask	= 0xFFFFFFFF;\n"
								   "		CullMode			= CCW;\n"
								   "		ColorWriteEnable	= 0;\n"
								   "	}\n"
								   "\n"
								   "	PASS P2\n"
								   "	{\n"
								   "		StencilPass			= Incr;\n"
								   "		CullMode			= CW;\n"
								   "	}\n"
								   "}\n"
								   "\n"
								   "TECHNIQUE T4\n"
								   "{\n"
								   "	PASS P1\n"
								   "	{\n"
								   "        Texture[0]			= Null;\n"
								   "		ZEnable				= True;\n"
								   "		ZWriteEnable		= False;\n"
								   "		ShadeMode			= Flat;\n"
								   "        FogEnable			= False;\n"
								   "		ColorOp[0]			= SelectArg1;\n"
								   "		ColorArg1[0]		= Current;\n"
								   "		ColorOp[1]			= Disable;\n"
								   "		Lighting			= False;\n"
								   "		StencilEnable		= True;\n"
								   "		StencilFunc			= Always;\n"
								   "		StencilFail			= Keep;\n"
								   "		StencilZFail		= Keep;\n"
								   "		StencilPass			= Decr;\n"
								   "		StencilMask			= 0xFFFFFFFF;\n"
								   "		StencilWriteMask	= 0xFFFFFFFF;\n"
								   "		CullMode			= CCW;\n"
								   "		AlphaBlendEnable	= True;\n"
								   "		SrcBlend			= Zero;\n"
								   "		DestBlend			= One;\n"
								   "	}\n"
								   "\n"
								   "	PASS P2\n"
								   "	{\n"
								   "		StencilPass			= Incr;\n"
								   "		CullMode			= CW;\n"
								   "	}\n"
								   "}\n", -1))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Effekts für das Schattenvolumen!", TB_ERROR);
	}

	// Effekt für den Schatten erstellen
	m_pShadowEffect = new tbEffect;
	if(m_pShadowEffect->Init("TECHNIQUE T1\n"
							 "{\n"
							 "	  PASS P1\n"
							 "	  {\n"
							 "        Texture[0]		= Null;\n"
							 "		  ZEnable			= False;\n"
							 "		  ZWriteEnable		= False;\n"
							 "		  StencilEnable		= True;\n"
							 "		  StencilFunc		= NotEqual;\n"
							 "		  StencilRef		= 0;\n"
						     "		  StencilFail		= Keep;\n"
						     "		  StencilZFail		= Keep;\n"
						     "		  StencilPass		= Keep;\n"
							 "		  StencilMask		= 0xFFFFFFFF;\n"
							 "		  ShadeMode			= Flat;\n"
 							 "        FogEnable			= False;\n"
							 "		  ColorOp[0]		= SelectArg1;\n"
							 "		  ColorArg1[0]		= Current;\n"
							 "		  ColorOp[1]		= Disable;\n"
							 "		  Lighting			= False;\n"
							 "		  ColorVertex		= True;\n"
							 "		  AlphaBlendEnable	= True;\n"
							 "		  SrcBlend			= SrcAlpha;\n"
							 "		  DestBlend			= InvSrcAlpha;\n"
							 "	  }\n"
						     "}\n", -1))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Effekts für den Schatten!", TB_ERROR);
	}

	// Speicher für die Dreiecksseiten reservieren
	m_pEdges = (tbEdge*)(tbMemAlloc(m_pModel->m_dwNumIndices * sizeof(tbEdge)));
	if(m_pEdges == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Speicher für die Mittelpunkte der Dreiecke reservieren
	m_pvTriangleCenters = (tbVector3*)(tbMemAlloc(m_pModel->m_dwNumIndices / 3 * sizeof(tbVector3)));
	if(m_pvTriangleCenters == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Speicher für die Vertizes des Schattenvolumens reservieren
	m_pvVertices = (tbVector3*)(tbMemAlloc(m_pModel->m_dwNumIndices * 6 * sizeof(tbVector3)));
	if(m_pvVertices == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// ------------------------------------------------------------------

	// Die Mittelpunkte aller Dreiecke berechnen
	for(DWORD t = 0; t < m_pModel->m_dwNumIndices / 3; t++)
	{
		// Mittelwert aus den drei Vektoren dieses Dreiecks berechnen
		m_pvTriangleCenters[t] = (m_pModel->m_pvVectors[m_pModel->m_pdwIndices[t * 3]] +
			                      m_pModel->m_pvVectors[m_pModel->m_pdwIndices[t * 3 + 1]] +
								  m_pModel->m_pvVectors[m_pModel->m_pdwIndices[t * 3 + 2]]) / 3.0f;
	}

	return TB_OK;
}

// ******************************************************************
// Hinzufügen einer Dreiecksseite
void tbShadowVolume::AddEdge(const DWORD dwPointA,
							 const DWORD dwPointB)
{
	// Gibt es vielleicht schon eine Seite mit diesen beiden Punkten?
	for(DWORD dwEdge = 0; dwEdge < m_dwNumEdges; dwEdge++)
	{
		if((m_pEdges[dwEdge].dwPointA == dwPointA && m_pEdges[dwEdge].dwPointB == dwPointB) ||
		   (m_pEdges[dwEdge].dwPointA == dwPointB && m_pEdges[dwEdge].dwPointB == dwPointA))
		{
			// Ja! Dann muss diese Seite entfernt werden.
			if(m_dwNumEdges > 1) m_pEdges[dwEdge] = m_pEdges[m_dwNumEdges - 1];
			m_dwNumEdges--;
			return;
		}
	}

	// Diese Seite existiert noch nicht!
	// Wir fertigen einen neuen Listeneintrag an.
	m_pEdges[m_dwNumEdges].dwPointA = dwPointA;
	m_pEdges[m_dwNumEdges].dwPointB = dwPointB;
	m_dwNumEdges++;
}

// ******************************************************************
// Berechnung des Schattenvolumens
tbResult tbShadowVolume::ComputeVolume(const tbMatrix& mInvModelMatrix,
									   const D3DLIGHT9& Light,
									   const float fLength,		// = 1000.0f
									   const BOOL bNormalize)	// = FALSE
{
	tbVector3	vLight;
	tbVector3	vDir;
	tbEdge*		pEdge;
	tbVector3	vPointA;
	tbVector3	vPointB;
	tbVector3	vPointC;
	tbVector3	vPointD;


	if(Light.Type == D3DLIGHT_DIRECTIONAL)
	{
		// Ein Richtungslicht kann gesondert behandelt werden.
		// Erst transformieren wir die Richtung des Lichts mit der
		// inversen Transformationsmatrix des Modells.
		vDir = tbVector3TransformNormal(tbVector3NormalizeEx(Light.Direction), mInvModelMatrix);

		// Alle Dreiecke durchgehen und das Punktprodukt aus dem
		// Dreiecksnormalvektor und vDir berechnen. Nur wenn ein Wert größer
		// gleich null herauskommt, ist das Dreieck aus der Sicht des
		// Lichts sichtbar (Culling!).
		m_dwNumEdges = 0;
		for(DWORD t = 0; t < m_pModel->m_dwNumIndices / 3; t++)
		{
			// Punktprodukt berechnen und daraus die Sichtbarkeit bestimmen
			if(tbVector3Dot(m_pModel->m_pTrianglePlanes[t * 4].n, vDir) >= 0.0f)
			{
				// Die drei Seiten dieses Dreiecks hinzufügen
				AddEdge(m_pModel->m_pdwIndices[t * 3], m_pModel->m_pdwIndices[t * 3 + 1]);
				AddEdge(m_pModel->m_pdwIndices[t * 3 + 1], m_pModel->m_pdwIndices[t * 3 + 2]);
				AddEdge(m_pModel->m_pdwIndices[t * 3 + 2], m_pModel->m_pdwIndices[t * 3]);
			}
		}

		// Die verbliebenen Seiten durchgehen
		vDir *= fLength;
		m_dwNumVertices = 0;
		for(DWORD dwEdge = 0; dwEdge < m_dwNumEdges; dwEdge++)
		{
			pEdge = &m_pEdges[dwEdge];

			// Jetzt fügen wir 6 Vertizes zur Vertexliste hinzu,
			// um die Seite zu verlängern.
			vPointA = m_pModel->m_pvVectors[pEdge->dwPointA];
			vPointB = m_pModel->m_pvVectors[pEdge->dwPointB];
			vPointC = vPointB + vDir;
			vPointD = vPointA + vDir;
			m_pvVertices[m_dwNumVertices++] = vPointA;
			m_pvVertices[m_dwNumVertices++] = vPointD;
			m_pvVertices[m_dwNumVertices++] = vPointC;
			m_pvVertices[m_dwNumVertices++] = vPointA;
			m_pvVertices[m_dwNumVertices++] = vPointC;
			m_pvVertices[m_dwNumVertices++] = vPointB;
		}
	}
	else
	{
		// Bei den anderen Lichttypen hat das Licht eine Position.
		// Das heißt, dass der Richtungsvektor vom Licht zu den Dreiecken
		// bei jedem Dreieck unterschiedlich ist.

		// Die Lichtposition in das Modellkoordinatensystem umrechnen
		vLight = tbVector3TransformCoords(Light.Position, mInvModelMatrix);

		// Alle Dreiecke durchgehen. Wir verfahren fast wie oben, nur dass
		// vDir bei jedem Dreieck neu berechnet wird.
		m_dwNumEdges = 0;
		for(DWORD t = 0; t < m_pModel->m_dwNumIndices / 3; t++)
		{
			// Richtungsvektor vom Licht zum Mittelpunkt des Dreiecks berechnen
			vDir = tbVector3NormalizeEx(m_pvTriangleCenters[t] - vLight);

			// Punktprodukt berechnen und daraus die Sichtbarkeit bestimmen
			if(tbVector3Dot(m_pModel->m_pTrianglePlanes[t * 4].n, vDir) >= 0.0f)
			{
				// Die drei Seiten dieses Dreiecks hinzufügen
				AddEdge(m_pModel->m_pdwIndices[t * 3], m_pModel->m_pdwIndices[t * 3 + 1]);
				AddEdge(m_pModel->m_pdwIndices[t * 3 + 1], m_pModel->m_pdwIndices[t * 3 + 2]);
				AddEdge(m_pModel->m_pdwIndices[t * 3 + 2], m_pModel->m_pdwIndices[t * 3]);
			}
		}

		// Die verbliebenen Seiten durchgehen
		m_dwNumVertices = 0;
		if(bNormalize)
		{
			for(DWORD dwEdge = 0; dwEdge < m_dwNumEdges; dwEdge++)
			{
				pEdge = &m_pEdges[dwEdge];

				// Jetzt fügen wir 6 Vertizes zur Vertexliste hinzu,
				// um diese Seite zu verlängern.
				vPointA = m_pModel->m_pvVectors[pEdge->dwPointA];
				vPointB = m_pModel->m_pvVectors[pEdge->dwPointB];
				vPointC = vPointB + tbVector3NormalizeEx(vPointB - vLight) * fLength;
				vPointD = vPointA + tbVector3NormalizeEx(vPointA - vLight) * fLength;
				m_pvVertices[m_dwNumVertices++] = vPointA;
				m_pvVertices[m_dwNumVertices++] = vPointD;
				m_pvVertices[m_dwNumVertices++] = vPointC;
				m_pvVertices[m_dwNumVertices++] = vPointA;
				m_pvVertices[m_dwNumVertices++] = vPointC;
				m_pvVertices[m_dwNumVertices++] = vPointB;
			}
		}
		else
		{
			for(DWORD dwEdge = 0; dwEdge < m_dwNumEdges; dwEdge++)
			{
				pEdge = &m_pEdges[dwEdge];

				// Jetzt fügen wir 6 Vertizes zur Vertexliste hinzu,
				// um diese Seite zu verlängern.
				vPointA = m_pModel->m_pvVectors[pEdge->dwPointA];
				vPointB = m_pModel->m_pvVectors[pEdge->dwPointB];
				vPointC = vPointB + (vPointB - vLight) * fLength;
				vPointD = vPointA + (vPointA - vLight) * fLength;
				m_pvVertices[m_dwNumVertices++] = vPointA;
				m_pvVertices[m_dwNumVertices++] = vPointD;
				m_pvVertices[m_dwNumVertices++] = vPointC;
				m_pvVertices[m_dwNumVertices++] = vPointA;
				m_pvVertices[m_dwNumVertices++] = vPointC;
				m_pvVertices[m_dwNumVertices++] = vPointB;
			}
		}
	}

	return TB_OK;
}

// ******************************************************************
// Rendern des Schattenvolumens
tbResult tbShadowVolume::RenderVolume(const tbMatrix& mModelMatrix)
{
	if(m_dwNumVertices == 0) return TB_OK;

	// Vertexformat setzen
	tbDirect3D::SetFVF(TB_SHADOW_VOLUME_FVF);

	// Transformationsmatrix setzen
	tbDirect3D::SetTransform(D3DTS_WORLD, mModelMatrix);

	// Schattenvolumen rendern
	int iNumPasses = m_pShadowVolumeEffect->Begin();

	for(int iPass = 0; iPass < iNumPasses; iPass++)
	{
		m_pShadowVolumeEffect->BeginPass(iPass);

		// Primitiven rendern
		tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		                                         m_dwNumVertices / 3,
											     m_pvVertices,
												 sizeof(tbVector3));
		m_pShadowVolumeEffect->EndPass();
	}

	// Effekt beenden
	m_pShadowVolumeEffect->End();

	return TB_OK;
}

// ******************************************************************
// Rendern des Schattens
tbResult tbShadowVolume::RenderShadow(const tbColor& ShadowColor)
{
	tbShadowVertex aVertex[4];

	// Die vier Vertizes erstellen (Rechteck)
	// Links unten
	aVertex[0].vPosition = tbVector3(0.0f, tbDirect3D::GetScreenSize().y, 0.5f);
	aVertex[0].fRHW = 1.0f;
	aVertex[0].Color = ShadowColor;

	// Links oben
	aVertex[1].vPosition = tbVector3(0.0f, 0.0f, 0.5f);
	aVertex[1].fRHW = 1.0f;
	aVertex[1].Color = ShadowColor;

	// Rechts unten
	aVertex[2].vPosition = tbVector3(tbDirect3D::GetScreenSize().x, tbDirect3D::GetScreenSize().y, 0.5f);
	aVertex[2].fRHW = 1.0f;
	aVertex[2].Color = ShadowColor;

	// Rechts oben
	aVertex[3].vPosition = tbVector3(tbDirect3D::GetScreenSize().x, 0.0f, 0.5f);
	aVertex[3].fRHW = 1.0f;
	aVertex[3].Color = ShadowColor;

	// Vertexformat setzen
	tbDirect3D::SetFVF(TB_SHADOW_FVF);

	// Als Dreiecksfolge zeichnen
	int iNumPasses = m_pShadowEffect->Begin();
	for(int iPass = 0; iPass < iNumPasses; iPass++)
	{
		m_pShadowEffect->BeginPass(iPass);
		tbDirect3D::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2,
			                                     aVertex, sizeof(tbShadowVertex));
		m_pShadowEffect->EndPass();
	}

	m_pShadowEffect->End();

	return TB_OK;
}

// ******************************************************************