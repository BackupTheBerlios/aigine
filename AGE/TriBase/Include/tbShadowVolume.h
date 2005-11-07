/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`�__\ /\ \ \ \  __\ /\ __\_ /\  __\   /�__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbShadowVolume.h
	================
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Rendern von Echtzeitschatten

	Autor:
	David Scherfgen

********************************************************************/

// Vertexformat f�r das Schattenvolumen
#define TB_SHADOW_VOLUME_FVF (D3DFVF_XYZ)

// ******************************************************************
// Vertexformat f�r den Schatten
#define TB_SHADOW_FVF (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
struct TRIBASE_API tbShadowVertex
{
	tbVector3	vPosition;
	float		fRHW;
	D3DCOLOR	Color;
};

// ******************************************************************
// Struktur f�r eine Dreiecksseite
struct TRIBASE_API tbEdge
{
	DWORD dwPointA; // Punkt A dieser Seite (Index)
	DWORD dwPointB; // Punkt B dieser Seite (Index)
};

// ******************************************************************
// Klasse f�r ein Schattenvolumen
class TRIBASE_API tbShadowVolume
{
private:
	// Variablen
	tbModel*	m_pModel;				// Das Modell
	tbVector3*	m_pvTriangleCenters;	// Mittelpunkte der Dreiecke
	tbEdge*		m_pEdges;				// Liste der Dreiecksseiten
	DWORD		m_dwNumEdges;			// Anzahl der Dreiecksseiten
	tbEffect*	m_pShadowVolumeEffect;	// Effekt zum Rendern des Schattenvolumens
	tbEffect*	m_pShadowEffect;		// Effekt zum Rendern des Schattens
	tbVector3*	m_pvVertices;			// Die Vertizes f�r das Schattenvolumen
	DWORD		m_dwNumVertices;		// Anzahl der Vertizes

public:
	// Konstruktor und Destruktor
	tbShadowVolume();
	~tbShadowVolume();

	// Methoden
	tbResult	Init(tbModel* pModel);																													// Initialisierung
	void		AddEdge(const DWORD dwPointA, const DWORD dwPointB);																					// F�gt eine Dreiecksseite hinzu
	tbResult	ComputeVolume(const tbMatrix& mInvModelMatrix, const D3DLIGHT9& Light, const float fLength = 1000.0f, const BOOL bNormalize = FALSE);	// Berechnung des Schattenvolumens
	tbResult	RenderVolume(const tbMatrix& mModelMatrix);																								// Rendern des Schattenvolumens
	tbResult	RenderShadow(const tbColor& ShadowColor);																								// Rendern des Schattens

	// Inline-Methoden
	inline tbModel*		GetModel()				{return m_pModel;}
	inline tbEffect*	GetShadowVolumeEffect()	{return m_pShadowVolumeEffect;}
	inline tbEffect*	GetShadowEffect()		{return m_pShadowEffect;}
	inline tbVector3*	GetVertices()			{return m_pvVertices;}
};

// ******************************************************************