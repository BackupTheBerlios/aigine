/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tb2DPrimitives.h
	================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Zeichnen von einfachen Primitiven

	Autor:
	Snorky
	[4.9.03]

********************************************************************/

#ifndef __TB2DPRIMITIVES__
#define __TB2DPRIMITIVES__

#pragma pack(1)


// ******************************************************************
// Vertexformat
#define TB_2DPRIMITIVES_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
struct TRIBASE_API tb2DPrimitivesVertex
{
	tbVector3	vPosition;	// Positionsangabe
	float		fRHW;		// 1/w
	D3DCOLOR	Diffuse;	// Streufarbe
};

// ******************************************************************
// Klasse für Primitiven
class TRIBASE_API tb2DPrimitives
{
protected:
	tbEffect*				m_pEffect;		// Effekt
	int						m_iNumPasses;	// Anzahl der Durchgänge für den Effekt
	DWORD					m_dwOldFVF;		// Altes Vertexformat (zum Zeitpunkt von Begin)
	tb2DPrimitivesVertex	aVertex[600];	// Vertexliste der zu zeichnenden Sprites
	DWORD					dwVertexCursor;	// aktuelle Vertexposition in der Liste

public:
	// Konstruktor und Destruktor
	tb2DPrimitives()	{ZeroMemory(this, sizeof(*this));}
	~tb2DPrimitives()	{Exit();}

	// Laden aus Dateien
	tbResult	Init(char* pcINIFile,				// INI-Datei-Name
					 char* pcINISection);			// INI-Sektionseintrag
	tbResult	Exit();
	tbResult	Begin();																			// Beginnen, Sprites zu zeichnen
	tbResult	End();																				// Beenden, Sprites zu zeichnen
	tbResult	DrawPuffer();																		// Alle Sprites im Puffer sofort malen

	tbResult	DrawLine(tbVector4 vCoords, 
						tbColor vColor1 = tbColor(1.0f),
						tbColor vColor2 = tbColor(-1.0f));
	tbResult	DrawLine(tbVector2 vStartCoords, 
						tbVector2 vZielCoords, 
						tbColor vColor1 = tbColor(1.0f),
						tbColor vColor2 = tbColor(-1.0f));
	tbResult	DrawLine(DWORD dwStartCoordX, 
						DWORD dwStartCoordY, 
						DWORD dwZielCoordX,
						DWORD dwZielCoordY,
						tbColor vColor1 = tbColor(1.0f),
						tbColor vColor2 = tbColor(-1.0f));
	tbResult	DrawLine(float fStartCoordX, 
						float fStartCoordY, 
						float fZielCoordX,
						float fZielCoordY,
						tbColor vColor1 = tbColor(1.0f),
						tbColor vColor2 = tbColor(-1.0f));
};

// ******************************************************************

#endif __TB2DPRIMITIVES__
