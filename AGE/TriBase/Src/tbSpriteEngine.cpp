/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbSpriteEngine.cpp
	==================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Zeichnen von Sprites

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbSpriteEngine-Klasse
tbSpriteEngine::tbSpriteEngine()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbSpriteEngine));
}

// ******************************************************************
// Destruktor der tbSpriteEngine-Klasse
tbSpriteEngine::~tbSpriteEngine()
{
	// Vertex-Buffer löschen
	TB_SAFE_DELETE(m_pVB);
}

// ******************************************************************
// Initialisiert die Sprite-Engine
tbResult tbSpriteEngine::Init(DWORD dwMaxSprites)
{
	// Parameter prüfen und sicher stellen, dass Direct3D initialisiert wurde
	if(dwMaxSprites == 0) TB_ERROR_INVALID_VALUE("dwMaxSprites", TB_ERROR);
	if(!tbDirect3D::IsInitialized()) TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Maximale Anzahl der Sprites übernehmen
	m_dwMaxSprites = dwMaxSprites;

	// Vertex-Buffer erstellen
	m_pVB = new tbVertexBuffer;
	if(m_pVB->Init(dwMaxSprites * 6 * sizeof(tbSpriteVertex), sizeof(tbSpriteVertex),
		           TB_SPRITE_FVF, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Vertex-Buffers!", TB_ERROR);
	}

	return TB_OK;
}

// ******************************************************************
// Findet einen freien Sprite-Typ
int tbSpriteEngine::FindFreeSpriteType()
{
	// Freien Typ suchen
	for(int iType = 0; iType < 1024; iType++) if(!m_aSpriteType[iType].bExists) return iType;

	// Nichts mehr frei!
	return -1;
}

// ******************************************************************
// Erstellt einen Sprite-Typ
tbResult tbSpriteEngine::CreateSpriteType(int iType,
										  tbVector2 vTopLeft,	// = tbVector2(0.0f, 0.0f)
										  tbVector2 vSize)		// = tbVector2(1.0f, 1.0f)
{
	// Parameter prüfen
	if(iType < 0 || iType >= 1024) TB_ERROR_INVALID_VALUE("iType", TB_ERROR);


	// Informationen eintragen
	m_aSpriteType[iType].bExists = TRUE;
	m_aSpriteType[iType].vTopLeft = vTopLeft;
	m_aSpriteType[iType].vTopRight.u = vTopLeft.u + vSize.u;
	m_aSpriteType[iType].vTopRight.v = vTopLeft.v;
	m_aSpriteType[iType].vBottomRight = vTopLeft + vSize;
	m_aSpriteType[iType].vBottomLeft.u = vTopLeft.u;
	m_aSpriteType[iType].vBottomLeft.v = vTopLeft.v + vSize.v;

	return TB_OK;
}

// ******************************************************************
// Beginnt mit den Sprites
tbResult tbSpriteEngine::Begin(tbMatrix mView,
							   tbVector3 vCamPos)
{
	// Zähler zurücksetzen
	m_dwNumSprites = 0;

	// Kameraachsen aus der Matrix abfragen
	m_vCamXAxis = tbVector3Normalize(tbVector3(mView.m11, mView.m21, mView.m31));
	m_vCamYAxis = tbVector3Normalize(tbVector3(mView.m12, mView.m22, mView.m32));
	m_vCamZAxis = tbVector3Normalize(tbVector3(mView.m13, mView.m23, mView.m33));
	m_vCamPos = vCamPos;

	// Cursorposition des Vertex-Buffers zurücksetzen
	m_pVB->SetCursor(0);

	m_bRunning = TRUE;

	return TB_OK;
}

// ******************************************************************
// Beenden
tbResult tbSpriteEngine::End()
{
	// Den Vertex-Buffer aktualisieren
	if(m_pVB->Update())
	{
		// Fehler!
		TB_ERROR("Fehler beim Aktualisieren des Vertex-Buffers!", TB_ERROR);
	}

	m_bRunning = FALSE;

	return TB_OK;
}

// ******************************************************************
// Fügt ein Sprite hinzu
int tbSpriteEngine::AddSprite(const int iType,
							  const tbVector3& vPosition,
							  const tbColor& Color,		// = tbColor(1.0f)
							  const tbVector2& vSize,	// = tbVector2(1.0f)
							  const float fRotation,	// = 0.0f
							  const BOOL bSimple)		// = TRUE
{
	tbSpriteVertex aVertex[4];

	if(!m_bRunning) return -1;

 	// Parameter prüfen
	if(iType < 0 || iType >= 1024) TB_ERROR_INVALID_VALUE("iType", -1);


	// Ist noch Platz?
	if(m_dwNumSprites >= m_dwMaxSprites) return -1;

	tbVector3 vSizeX;
	tbVector3 vSizeY;

	if(bSimple)
	{
		// Das Sprite soll im "simplen Modus" erstellt werden.
		// Das bedeutet, dass die Achsen des Sprite-Rechtecks ganz einfach denen der
		// Kamera entsprechen - bevor die Rotation per Sinus und Kosinus eingerechnet wird.

		// Die beiden Größenvektoren vSizeX und vSizeY berechnen
		vSizeX = ((m_vCamXAxis * cosf(-fRotation) * vSize.x) + (m_vCamYAxis * sinf(fRotation) * vSize.y)) * 0.5f;
		vSizeY = ((m_vCamXAxis * sinf(-fRotation) * vSize.x) + (m_vCamYAxis * cosf(-fRotation) * vSize.y)) * 0.5f;
	}
	else
	{
		// Das Sprite soll im erweiterten Modus erstellt werden.
		// Dreht sich die Kamera, drehen sich die Achsen des Sprites nicht mit.
		// Daher wird ein fester Achsenvektor benötigt, der in diesem Fall (0, 1, 0) ist -
		// bevor die Rotation hinzugerechnet wird.
		// Die Kameramatrix wurde auf ähnliche Weise erstellt.

		// Die beiden Achsen berechnen
		tbVector3 vYAxis(tbVector3Cross(tbVector3(sinf(fRotation), cosf(fRotation), 0.0f), m_vCamZAxis));
		tbVector3 vXAxis(tbVector3Cross(vYAxis, m_vCamZAxis));

		// Die beiden Größenvektoren vSizeX und vSizeY berechnen
		vSizeX = ((vXAxis * cosf(-fRotation) * vSize.x) + (vYAxis * sinf(fRotation) * vSize.y)) * 0.5f;
		vSizeY = ((vXAxis * sinf(-fRotation) * vSize.x) + (vYAxis * cosf(-fRotation) * vSize.y)) * 0.5f;
	}

	// Positionsangaben eintragen
	aVertex[0].vPosition = vPosition - vSizeX + vSizeY;	// Links oben
	aVertex[1].vPosition = vPosition + vSizeX + vSizeY;	// Rechts oben
	aVertex[2].vPosition = vPosition + vSizeX - vSizeY;	// Rechts unten
	aVertex[3].vPosition = vPosition - vSizeX - vSizeY;	// Links unten

	// Farben und Texturkoordinaten eintragen
	aVertex[0].Color = aVertex[1].Color = aVertex[2].Color = aVertex[3].Color = Color;
	aVertex[0].vTexture = m_aSpriteType[iType].vTopLeft;
	aVertex[1].vTexture = m_aSpriteType[iType].vTopRight;
	aVertex[2].vTexture = m_aSpriteType[iType].vBottomRight;
	aVertex[3].vTexture = m_aSpriteType[iType].vBottomLeft;

	// Die Vertizes zum Vertex-Buffer hinzufügen
	m_pVB->AddVertex(&aVertex[0]);
	m_pVB->AddVertex(&aVertex[1]);
	m_pVB->AddVertex(&aVertex[2]);
	m_pVB->AddVertex(&aVertex[0]);
	m_pVB->AddVertex(&aVertex[2]);
	m_pVB->AddVertex(&aVertex[3]);

	// Zähler erhöhen
	m_dwNumSprites++;

	// Index des neuen Sprites zurückliefern
	return (int)(m_dwNumSprites - 1);
}

// ******************************************************************
// Fügt ein Laser-Sprite zur Liste hinzu
int tbSpriteEngine::AddLaserSprite(const int iType,
                                   const tbVector3& vLaserStart,
                                   const tbVector3& vLaserEnd,
                                   const float fRadius,			// = 0.25f
                                   const tbColor& StartColor,	// = tbColor(1.0f)
                                   const tbColor& EndColor)		// = tbColor(1.0f)
{
	tbSpriteVertex aVertex[4];

	// Parameter prüfen
	if(iType < 0 || iType >= 1024) TB_ERROR_INVALID_VALUE("iType", -1);


	// Ist noch Platz?
	if(m_dwNumSprites >= m_dwMaxSprites) return -1;

	// Verbindungsvektor von der Kamera zur Mitte des Laserstrahls berechnen
	tbVector3 vCamToLaser(tbVector3Normalize((vLaserStart + vLaserEnd) * 0.5f - m_vCamPos));

	// Die z-Achse des Laserstrahls berechnen (Endpunkt - Startpunkt)
	tbVector3 vLaserZAxis(tbVector3Normalize(vLaserEnd - vLaserStart));

	// Die x-Achse des Laserstrahls, also des Rechtecks, muss folgende
	// Bedingungen erfüllen:
	// - Sie muss senkrecht auf der z-Achse des Laserstrahls stehen.
	// - Sie muss senkrecht auf der Verbindungslinie zwischen Kamera und Laser
	//   stehen, damit der stets "von oben" gesehen wird.
	// In Frage kommt also nur das Kreuzprodukt.
	// Die x-Achse mit dem Radius des Strahls multiplizieren
	tbVector3 vLaserXAxis(tbVector3Normalize(tbVector3Cross(vLaserZAxis, vCamToLaser)) * fRadius);

	// Nun können wir die Vertizes erzeugen:
	//
	//                 vLaserZAxis
	//           v0    ^    v1
	//             |---|---|  vLaserEnd
	//             |   |   |
	//             |   |   |
	//             |   |   |
	//             |   |   |
	//             |   |   |
	//             |   |   |
	//             |   |   |
	//             |   |   |
	//             |   |   |
	//             |...|---> vLaserXAxis
	//           v3         v2
	//                 vLaserStart

	// Linker Vertex am Strahlende
	aVertex[0].vPosition = vLaserEnd - vLaserXAxis;
	aVertex[0].Color = EndColor;
	aVertex[0].vTexture = m_aSpriteType[iType].vTopLeft;

	// Rechter Vertex am Strahlende
	aVertex[1].vPosition = vLaserEnd + vLaserXAxis;
	aVertex[1].Color = EndColor;
	aVertex[1].vTexture = m_aSpriteType[iType].vTopRight;

	// Rechter Vertex am Strahlbeginn
	aVertex[2].vPosition = vLaserStart + vLaserXAxis;
	aVertex[2].Color = StartColor;
	aVertex[2].vTexture = m_aSpriteType[iType].vBottomRight;

	// Linker Vertex am Strahlbeginn
	aVertex[3].vPosition = vLaserStart - vLaserXAxis;
	aVertex[3].Color = StartColor;
	aVertex[3].vTexture = m_aSpriteType[iType].vBottomLeft;

	// Die Vertizes zum Vertex-Buffer hinzufügen
	m_pVB->AddVertex(&aVertex[0]);
	m_pVB->AddVertex(&aVertex[1]);
	m_pVB->AddVertex(&aVertex[2]);
	m_pVB->AddVertex(&aVertex[0]);
	m_pVB->AddVertex(&aVertex[2]);
	m_pVB->AddVertex(&aVertex[3]);

	// Zähler erhöhen
	m_dwNumSprites++;

	// Index des Sprites zurückliefern
	return (int)(m_dwNumSprites - 1);
}

// ******************************************************************
// Rendert die Sprites
tbResult tbSpriteEngine::Render(int iFrom,	// = -1
								int iTo)	// = -1
{
 	// Parameter anpassen
	if(iFrom == -1) iFrom = 0;
	if(iTo == -1) iTo = (int)(m_dwNumSprites) - 1;
	if(iTo <= -1) return TB_OK;

 	// Parameter prüfen
	if(iFrom < 0 || iFrom >= (int)(m_dwNumSprites) || iFrom > iTo)	TB_ERROR_INVALID_VALUE("iFrom", TB_ERROR);
	if(iTo < 0 || iTo >= (int)(m_dwNumSprites)) 					TB_ERROR_INVALID_VALUE("iTo", TB_ERROR);


	// Vertex-Buffer als Datenquelle einsetzen
	tbDirect3D::GetDevice()->SetStreamSource(0, m_pVB->GetVB(), 0, sizeof(tbSpriteVertex));

	// Vertexformat setzen
	tbDirect3D::SetFVF(TB_SPRITE_FVF);

	// Vertizes als Dreiecksliste rendern
	tbDirect3D::GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, iFrom * 6, (iTo - iFrom + 1) * 2);

	return TB_OK;
}

// ******************************************************************
