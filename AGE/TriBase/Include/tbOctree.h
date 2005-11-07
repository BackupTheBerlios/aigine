/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbOctree.h
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Rendern von Octrees

	Autor:
	David Scherfgen

********************************************************************/

#pragma pack(1)

// Aufzählung für die verschiedenen Chunk-Typen
enum TRIBASE_API tbOctreeChunkType
{
	TB_CT_OCTREE_VERTICES,	// Vertexdaten
	TB_CT_OCTREE_INDICES,	// Indexdaten
	TB_CT_OCTREE_EFFECTS,	// Effektdaten,
	TB_CT_OCTREE_LIGHTING,	// Beleuchtungsdaten
	TB_CT_OCTREE_TREE,		// Octree-Daten
	TB_CT_OCTREE_USER = 255	// Benutzerdefiniert (TB_CT_USER + x)
};

// ******************************************************************
// Struktur für einen Chunk-Header
struct TRIBASE_API tbOctreeChunkHeader
{
	tbOctreeChunkType	ChunkType;	// Typ des Chunks
	DWORD				dwDataSize;	// Größe der Daten des Chunks
};

// ******************************************************************
// Struktur für einen Knoten-Header
struct TRIBASE_API tbOctreeNodeHeader
{
	BOOL		bIsLeaf;			// Ist es ein Endknoten?
	DWORD		dwStartIndex;		// Startindex
	DWORD		dwNumIndices;		// Anzahl der Indizes
	tbVector3	vBoundingBoxMin;	// Minimaler Vektor der Bounding-Box
	tbVector3	vBoundingBoxMax;	// Maximaler Vektor der Bounding-Box
};

// ******************************************************************
// Struktur für den Header des TB_CT_OCTREE_VERTICES-Chunks
struct TRIBASE_API tbOctreeVerticesChunkHeader
{
	DWORD dwFVF;			// Vertexformat
	DWORD dwVertexSize;		// Größe eines einzelnen Vertex
	DWORD dwNumVertices;	// Anzahl der Vertizes
};

// ******************************************************************
// Struktur für den Header des TB_CT_OCTREE_TREE-Chunks
struct TRIBASE_API tbOctreeTreeChunkHeader
{
	D3DFORMAT	IndexFormat;	// D3DFMT_INDEX16 oder D3DFMT_INDEX32
	DWORD		dwIndexSize;	// Größe eines Index
	DWORD		dwNumIndices;	// Anzahl der Indizes
};

// ******************************************************************
// Struktur für einen Octree-Knoten
struct TRIBASE_API tbOctreeNode
{
	BOOL			bIsLeaf;			// Endknoten?
	DWORD			dwStartIndex;		// Startindex
	DWORD			dwNumIndices;		// Anzahl der Indizes
	DWORD*			pdwEffectStart;		// Startpunkte der Effekte
	DWORD*			pdwEffectEnd;		// Endpunkte der Effekte
	DWORD*			pdwMinIndex;		// Minimaler Index der Effekte
	DWORD*			pdwMaxIndex;		// Maximaler Index der Effekte
	tbVector3		vBoundingBoxMin;	// Minimaler Vektor der Bounding-Box
	tbVector3		vBoundingBoxMax;	// Maximaler Vektor der Bounding-Box
	tbOctreeNode*	apChild[8];			// Die 8 Unterknoten
	DWORD*			pdwIndices;			// Indizes
	tbPlane*		pTrianglePlanes;	// Ebenen der Dreiecke
	float			fDistSq;			// Quadrat der Distanz zur Kamera
};

// ******************************************************************
// Struktur für den Header eines einzelnen Effekts
struct TRIBASE_API tbOctreeEffectHeader
{
	char				acName[256];		// Name des Effekts
	BOOL				bAlphaBlended;		// Mit Alpha-Blending rendern?
	D3DPRIMITIVETYPE	PrimitiveType;		// Typ der Primitiven
	DWORD				dwStartIndex;		// Wo mit dem Rendern anfangen?
	DWORD				dwNumPrimitives;	// Wie viele Primitiven rendern?
	DWORD				dwMinIndex;			// Kleinster verwendeter Index
	DWORD				dwNumVertices;		// Größter Index - Kleinster Index + 1
	DWORD				dwEffectCodeSize;	// Größe des Effektquellcodes
};

// ******************************************************************
// Struktur für einen Effekt in einem Octree
struct TRIBASE_API tbOctreeEffect
{
	tbOctreeEffectHeader	Header;			// Header
	char*					pcCode;			// Quellcode
	tbEffect*				pEffect;		// Effektklasse
	DWORD					dwNumTextures;	// Anzahl der Texturen
	PDIRECT3DBASETEXTURE9	apTexture[8];	// Max. 8 Texturen
};

// ******************************************************************
// Klasse für Octrees
class TRIBASE_API tbOctree
{
private:
	// Variablen
	DWORD			m_dwFVF;				// Vertexformat
	D3DFORMAT		m_IndexFormat;			// Indexformat
	DWORD			m_dwNumVertices;		// Anzahl der Vertizes
	DWORD			m_dwNumIndices;			// Anzahl aller Indizes
	DWORD			m_dwNumEffects;			// Anzahl der Effekte
	DWORD			m_dwNumLights;			// Anzahl der Lichter
	DWORD			m_dwNumNodes;			// Anzahl der Knoten
	DWORD			m_dwNumLeaves;			// Anzahl der Endknoten
	tbVertexBuffer*	m_pVertexBuffer;		// Vertex-Buffer
	tbIndexBuffer*	m_pIndexBuffer;			// Index-Buffer
	tbOctreeEffect*	m_pEffects;				// Die Effekte
	D3DLIGHT9*		m_pLights;				// Die Lichter
	tbOctreeNode*	m_pRootNode;			// Wurzelknoten
	tbOctreeNode**	m_ppVisibleList;		// Sichtbarkeitsliste
	DWORD			m_dwNumLeavesVisible;	// Anzahl der sichtbaren Endknoten
	tbVector3*		m_pvVectors;			// Positionsvektoren der Vertizes
	BOOL			m_bExtraData;			// Hat der Octree Extradaten?

public:
	// Konstruktor und Destruktor
	tbOctree();
	~tbOctree();

	// Methoden
	// Aus virtueller Datei initialisieren
	tbResult Init(tbVFile* pVFile,
				  char* pcTexturePrefix = "",
				  char* pcTexturePostfix = "",
		          D3DPOOL VBPool = D3DPOOL_DEFAULT,
				  DWORD dwVBUsage = D3DUSAGE_WRITEONLY,
				  D3DPOOL IBPool = D3DPOOL_DEFAULT,
				  DWORD dwIBUsage = D3DUSAGE_WRITEONLY,
				  BOOL bGenerateExtraData = TRUE);

	// Aus Datei initialisieren
	tbResult Init(char* pcFilename,
				  char* pcTexturePrefix = "",
				  char* pcTexturePostfix = "",
		          D3DPOOL VBPool = D3DPOOL_DEFAULT,
				  DWORD dwVBUsage = D3DUSAGE_WRITEONLY,
				  D3DPOOL IBPool = D3DPOOL_DEFAULT,
				  DWORD dwIBUsage = D3DUSAGE_WRITEONLY,
				  BOOL bGenerateExtraData = TRUE);

	// Aus Speicher initialisieren
	tbResult Init(void* pMemory,
		          int iSize,
				  char* pcTexturePrefix = "",
				  char* pcTexturePostfix = "",
		          D3DPOOL VBPool = D3DPOOL_DEFAULT,
				  DWORD dwVBUsage = D3DUSAGE_WRITEONLY,
				  D3DPOOL IBPool = D3DPOOL_DEFAULT,
				  DWORD dwIBUsage = D3DUSAGE_WRITEONLY,
				  BOOL bGenerateExtraData = TRUE);

	// Aus Ressource initialisieren
	tbResult Init(HMODULE hModule,
		          char* pcResourceName,
				  char* pcResourceType,
				  char* pcTexturePrefix = "",
				  char* pcTexturePostfix = "",
		          D3DPOOL VBPool = D3DPOOL_DEFAULT,
				  DWORD dwVBUsage = D3DUSAGE_WRITEONLY,
				  D3DPOOL IBPool = D3DPOOL_DEFAULT,
				  DWORD dwIBUsage = D3DUSAGE_WRITEONLY,
				  BOOL bGenerateExtraData = TRUE);

	tbResult		LoadEffectTextures(DWORD dwEffect, char* pcTexturePrefix = "", char* pcTexturePostfix = "");																		// Lädt die Texturen eines Effekts
	tbResult		Render(tbPlane* pViewFrustum, tbVector3 vCamera, int iFrom = -1, int iTo = -1, BOOL bRenderOpaque = TRUE, BOOL bRenderAlphaBlended = TRUE, BOOL bDepthSort = TRUE);	// Rendert das Modell
	tbResult		ReadNode(tbOctreeNode* pNode, tbOctreeTreeChunkHeader* pTreeCH, tbVFile* pVFile);																					// Liest einen Knoten rekursiv
	tbResult		ComputeExtraData(tbOctreeNode* pNode);																																// Berechnet die Extradaten rekursiv
	tbResult		DeleteNode(tbOctreeNode* pNode);																																	// Löscht einen Octree-Knoten rekursiv
	void			ComputeVisibility(tbOctreeNode* pNode, tbPlane* pViewFrustum, tbVector3 vCamera, BOOL bDepthSort);																	// Berechnet rekursiv die sichtbaren Endknoten
	tbOctreeNode*	GetLeafByPoint(const tbVector3& vPoint, tbOctreeNode* pNode = NULL);																								// Findet den Endknoten, der den Punkt beinhaltet

	// Inline-Methoden
	inline DWORD			GetFVF()					{return m_dwFVF;}
	inline D3DFORMAT		GetIndexFormat()			{return m_IndexFormat;}
	inline DWORD			GetNumVertices()			{return m_dwNumVertices;}
	inline DWORD			GetNumIndices()				{return m_dwNumIndices;}
	inline DWORD			GetNumEffects()				{return m_dwNumEffects;}
	inline DWORD			GetNumLights()				{return m_dwNumLights;}
	inline tbVertexBuffer*	GetVertexBuffer()			{return m_pVertexBuffer;}
	inline tbIndexBuffer*	GetIndexBuffer()			{return m_pIndexBuffer;}
	inline tbOctreeEffect*	GetEffects()				{return m_pEffects;}
	inline BOOL				ExtraData()					{return m_bExtraData;}

	// friend-Funktionen
	friend TRIBASE_API BOOL tbLineHitsOctree(const tbVector3& vLineA, const tbVector3& vLineB, const tbOctree* pOctree, const float fTolerance, tbVector3* pvOutPos, tbVector3* pvOutNormal, int* piOutTriangle, tbOctreeNode** ppOutNode);
	friend TRIBASE_API BOOL tbLineHitsOctreeRec(const tbVector3& vLineA, const tbVector3& vLineB, const tbOctree* pOctree, const tbOctreeNode* pNode, int* piClosestTriangle, float* pfClosestCollision, tbOctreeNode** ppOutNode, BOOL bSimpleMode, BOOL* pbStop);
	friend TRIBASE_API BOOL tbModelHitsOctree(const tbModel* pModel, const tbMatrix& mMatrix, const tbMatrix& mInvMatrix, const tbOctree* pOctree, tbVector3* pvOutPos, tbVector3* pvOutNormalM, tbVector3* pvOutNormalO, int* piOutTriM, int* piOutTriO, tbOctreeNode** ppOutNodeO);
	friend TRIBASE_API BOOL tbModelHitsOctreeRec(const tbModel* pModel, const tbMatrix& mMatrix, const tbMatrix& mInvMatrix, const tbModelOctreeNode* pModelNode, const tbOctree* pOctree, const tbOctreeNode* pNode, tbVector3* pvOutPos, tbVector3* pvOutNormalM, tbVector3* pvOutNormalO, int* piOutTriM, int* piOutTriO, tbOctreeNode** ppOutNodeO);
};

// ******************************************************************