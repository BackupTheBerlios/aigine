/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbSkyBox.h
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Einfaches Erstellen von Sky-Boxes

	Autor:
	David Scherfgen

********************************************************************/

// Vertexformat
#define TB_SKYBOX_FVF (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0))
struct TRIBASE_API tbSkyBoxVertex
{
	tbVector3	vPosition;
	tbVector3	vTexture;
};

// ******************************************************************
// Klasse für eine Sky-Box
class TRIBASE_API tbSkyBox
{
private:
	// Variablen
	tbVertexBuffer*			m_pVB;		// Vertex-Buffer
	tbIndexBuffer*			m_pIB;		// Index-Buffer
	tbEffect*				m_pEffect;	// Effekt für die Sky-Box
	PDIRECT3DCUBETEXTURE9	m_pCubeMap;	// Die Textur

public:
	// Konstruktur und Destruktor
	tbSkyBox();
	~tbSkyBox();

	// Methoden
	tbResult Init(PDIRECT3DCUBETEXTURE9 pCubeMap);	// Initialisiert die Sky-Box
	tbResult Render(const tbVector3& vCameraPos);	// Rendert die Sky-Box

	// Inline-Methoden
	inline tbVertexBuffer*			GetVertexBuffer()	{return m_pVB;}
	inline tbIndexBuffer*			GetIndexBuffer()	{return m_pIB;}
	inline tbEffect*				GetEffect()			{return m_pEffect;}
	inline PDIRECT3DCUBETEXTURE9	GetCubeMap()		{return m_pCubeMap;}
};

// ******************************************************************