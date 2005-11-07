/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbModel.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit 3D-Modellen

	Autor:
	David Scherfgen

********************************************************************/

#pragma pack(1)

// Aufzählung für die verschiedenen Chunk-Typen
enum TRIBASE_API tbModelChunkType
{
	TB_CT_MODEL_VERTICES,	// Vertexdaten
	TB_CT_MODEL_INDICES,	// Indexdaten
	TB_CT_MODEL_EFFECTS,	// Effektdaten,
	TB_CT_MODEL_LIGHTING,	// Beleuchtungsdaten
	TB_CT_USER = 255		// Benutzerdefiniert (TB_CT_USER + x)
};

// ******************************************************************
// Struktur für einen Chunk-Header
struct TRIBASE_API tbModelChunkHeader
{
	tbModelChunkType	ChunkType;	// Typ des Chunks
	DWORD				dwDataSize;	// Größe der Daten des Chunks
};

// ******************************************************************
// Struktur für den Header des TB_CT_MODEL_VERTICES-Chunks
struct TRIBASE_API tbModelVerticesChunkHeader
{
	DWORD		dwFVF;					// Vertexformat
	DWORD		dwVertexSize;			// Größe eines einzelnen Vertex
	DWORD		dwNumVertices;			// Anzahl der Vertizes
	float		fBoundingSphereRadius;	// Radius der umgebenden Kugel
	tbVector3	vBoundingBoxMin;		// Minimumpunkt des umgebenden Quaders
	tbVector3	vBoundingBoxMax;		// Maximumpunkt des umgebenden Quaders
};

// ******************************************************************
// Struktur für den Header des TB_CT_MODEL_INDICES-Chunks
struct TRIBASE_API tbModelIndicesChunkHeader
{
	D3DFORMAT	IndexFormat;	// D3DFMT_INDEX16 oder D3DFMT_INDEX32
	DWORD		dwIndexSize;	// Größe eines Index
	DWORD		dwNumIndices;	// Anzahl der Indizes
};

// ******************************************************************
// Struktur für den Header eines einzelnen Effekts
struct TRIBASE_API tbModelEffectHeader
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
// Struktur für einen Effekt in einem Modell
struct TRIBASE_API tbModelEffect
{
	tbModelEffectHeader		Header;			// Effektheader
	char*					pcCode;			// Quellcode
	tbEffect*				pEffect;		// Effektklasse
	DWORD					dwNumTextures;	// Anzahl der Texturen
	PDIRECT3DBASETEXTURE9	apTexture[8];	// Max. 8 Texturen
};

// ******************************************************************
// Struktur für einen Octree-Knoten
struct TRIBASE_API tbModelOctreeNode
{
	BOOL				bIsLeaf;			// Ist es ein Endknoten?
	tbModelOctreeNode*	apChild[8];			// Untergeordnete Knoten
	DWORD				dwNumTriangles;		// Anzahl der Dreiecke dieses Knotens
	DWORD*				pdwTriangles;		// Die Dreiecke
	tbVector3			vBoundingBoxMin;	// Minimum des Umgebungsquaders
	tbVector3			vBoundingBoxMax;	// Maximum des Umgebungsquaders
};

// ******************************************************************
// Klasse für 3D-Modelle
class TRIBASE_API tbModel
{
	friend class tbShadowVolume;

private:
	// Variablen
	DWORD					m_dwFVF;					// Vertexformat
	D3DFORMAT				m_IndexFormat;				// Indexformat
	DWORD					m_dwNumVertices;			// Anzahl der Vertizes
	DWORD					m_dwNumIndices;				// Anzahl der Indizes
	DWORD					m_dwNumEffects;				// Anzahl der Effekte
	DWORD					m_dwNumLights;				// Anzahl der Lichter
	tbVertexBuffer*			m_pVertexBuffer;			// Vertex-Buffer
	tbIndexBuffer*			m_pIndexBuffer;				// Index-Buffer
	tbModelEffect*			m_pEffects;					// Die Effekte
	D3DLIGHT9*				m_pLights;					// Die Lichter
	float					m_fBoundingSphereRadius;	// Radius der umgebenden Kugel
	tbVector3				m_vBoundingBoxMin;			// Minimumpunkt des umgebenden Quaders
	tbVector3				m_vBoundingBoxMax;			// Maximumpunkt des umgebenden Quaders

	BOOL					m_bExtraData;				// Extradaten generiert?
	BOOL					m_bExtraDataOnly;			// Nur Extradaten?
	tbModelOctreeNode*		m_pRootNode;				// Wurzelknoten des Octrees
	tbVector3*				m_pvVectors;				// Positionsvektoren der Vertizes
	DWORD*					m_pdwIndices;				// Alle Indizes
	tbPlane*				m_pTrianglePlanes;			// Ebenen der Dreiecke (4 pro Dreieck)

public:
	// Konstruktor und Destruktor
	tbModel();
	~tbModel();

	// Methoden
	// Aus virtueller Datei initialisieren
	tbResult Init(tbVFile* pVFile,
				  char* pcTexturePrefix = "",
				  char* pcTexturePostfix = "",
		          D3DPOOL VBPool = D3DPOOL_DEFAULT,
				  DWORD dwVBUsage = D3DUSAGE_WRITEONLY,
				  D3DPOOL IBPool = D3DPOOL_DEFAULT,
				  DWORD dwIBUsage = D3DUSAGE_WRITEONLY,
				  BOOL bGenerateExtraData = TRUE,
				  BOOL bExtraDataOnly = FALSE);

	// Aus Datei initialisieren
	tbResult Init(char* pcFilename,
				  char* pcTexturePrefix = "",
				  char* pcTexturePostfix = "",
		          D3DPOOL VBPool = D3DPOOL_DEFAULT,
				  DWORD dwVBUsage = D3DUSAGE_WRITEONLY,
				  D3DPOOL IBPool = D3DPOOL_DEFAULT,
				  DWORD dwIBUsage = D3DUSAGE_WRITEONLY,
				  BOOL bGenerateExtraData = TRUE,
				  BOOL bExtraDataOnly = FALSE);

	// Aus Speicher initialisieren
	tbResult Init(void* pMemory,
		          int iSize,
				  char* pcTexturePrefix = "",
				  char* pcTexturePostfix = "",
		          D3DPOOL VBPool = D3DPOOL_DEFAULT,
				  DWORD dwVBUsage = D3DUSAGE_WRITEONLY,
				  D3DPOOL IBPool = D3DPOOL_DEFAULT,
				  DWORD dwIBUsage = D3DUSAGE_WRITEONLY,
				  BOOL bGenerateExtraData = TRUE,
				  BOOL bExtraDataOnly = FALSE);

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
				  BOOL bGenerateExtraData = TRUE,
				  BOOL bExtraDataOnly = FALSE);

	tbResult LoadEffectTextures(DWORD dwEffect, char* pcTexturePrefix = "", char* pcTexturePostfix = "");		// Lädt die Texturen eines Effekts
	tbResult Render(int iFrom = -1, int iTo = -1, BOOL bRenderOpaque = TRUE, BOOL bRenderAlphaBlended = TRUE);	// Rendert das Modell
	tbResult UpdateExtraData();																				// Aktualisiert die Kollisionsdaten
	tbResult SplitNode(tbModelOctreeNode* pNode, int iMaxDepth);												// Teilt einen Octree-Knoten rekursiv auf
	tbResult DeleteNode(tbModelOctreeNode* pNode);																// Löscht einen Octree-Knoten rekursiv

	// Inline-Methoden
	inline DWORD			GetFVF()					{return m_dwFVF;}
	inline D3DFORMAT		GetIndexFormat()			{return m_IndexFormat;}
	inline DWORD			GetNumVertices()			{return m_dwNumVertices;}
	inline DWORD			GetNumIndices()				{return m_dwNumIndices;}
	inline DWORD			GetNumEffects()				{return m_dwNumEffects;}
	inline DWORD			GetNumLights()				{return m_dwNumLights;}
	inline tbVertexBuffer*	GetVertexBuffer()			{return m_pVertexBuffer;}
	inline tbIndexBuffer*	GetIndexBuffer()			{return m_pIndexBuffer;}
	inline tbModelEffect*	GetEffects()				{return m_pEffects;}
	inline BOOL				ExtraData()					{return m_bExtraData;}
	inline BOOL				ExtraDataOnly()				{return m_bExtraDataOnly;}
	inline float			GetBoundingSphereRadius()	{return m_fBoundingSphereRadius;}
	inline tbVector3		GetBoundingBoxMin()			{return m_vBoundingBoxMin;}
	inline tbVector3		GetBoundingBoxMax()			{return m_vBoundingBoxMax;}

	// friend-Funktionen
	friend TRIBASE_API BOOL tbLineHitsModel(const tbVector3& vLineA, const tbVector3& vLineB, const tbModel* pModel, const tbMatrix& mMatrix, const tbMatrix& mInvMatrix, const float fTolerance, tbVector3* pvOutPos, tbVector3* pvOutNormal, int* piOutTriangle);
	friend TRIBASE_API BOOL tbLineHitsModelRec(const tbVector3& vLineA, const tbVector3& vLineB, const tbModel* pModel, const tbModelOctreeNode* pNode, int* piClosestTriangle, float* pfClosestCollision, BOOL bSimpleMode, BOOL* pbStop);
	friend TRIBASE_API BOOL tbModelHitsModel(const tbModel* pModelA, const tbMatrix& mMatrixA, const tbMatrix& mInvMatrixA, const tbModel* pModelB, const tbMatrix& mMatrixB, const tbMatrix& mInvMatrixB, tbVector3* pvOutPos, tbVector3* pvOutNormalA, tbVector3* pvOutNormalB, int* piOutTriA, int* piOutTriB);
	friend TRIBASE_API BOOL tbModelHitsModelRec(const tbModel* pModelA, const tbMatrix& mMatrixA, const tbMatrix& mInvMatrixA, const tbModelOctreeNode* pNodeA, const tbModel* pModelB, const tbMatrix& mMatrixB, const tbMatrix& mInvMatrixB, const tbModelOctreeNode* pNodeB, tbVector3* pvOutPos, tbVector3* pvOutNormalA, tbVector3* pvOutNormalB, int* piOutTriA, int* piOutTriB, BOOL bSkipBBTest);
	friend TRIBASE_API BOOL tbModelHitsOctree(const tbModel* pModel, const tbMatrix& mMatrix, const tbMatrix& mInvMatrix, const tbOctree* pOctree, tbVector3* pvOutPos, tbVector3* pvOutNormalM, tbVector3* pvOutNormalO, int* piOutTriM, int* piOutTriO, tbOctreeNode** ppOutNodeO);
	friend TRIBASE_API BOOL tbModelHitsOctreeRec(const tbModel* pModel, const tbMatrix& mMatrix, const tbMatrix& mInvMatrix, const tbModelOctreeNode* pModelNode, const tbOctree* pOctree, const tbOctreeNode* pNode, tbVector3* pvOutPos, tbVector3* pvOutNormalM, tbVector3* pvOutNormalO, int* piOutTriM, int* piOutTriO, tbOctreeNode** ppOutNodeO);
};

// ******************************************************************