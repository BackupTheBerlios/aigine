#pragma pack(1)

#include "../AiGinE.h"

// Aufzählung für die verschiedenen Chunk-Typen
enum ModelChunkType
{
	CT_MODEL_VERTICES,	// Vertexdaten
	CT_MODEL_INDICES,	// Indexdaten
	CT_MODEL_EFFECTS,	// Effektdaten,
	CT_MODEL_LIGHTING,	// Beleuchtungsdaten
	CT_USER = 255		// Benutzerdefiniert (CT_USER + x)
};

// ******************************************************************
// Struktur für einen Chunk-Header
struct ModelChunkHeader
{
	ModelChunkType	ChunkType;	// Typ des Chunks
	DWORD				dwDataSize;	// Größe der Daten des Chunks
};

// ******************************************************************
// Struktur für den Header des CT_MODEL_VERTICES-Chunks
struct ModelVerticesChunkHeader
{
	DWORD		dwFVF;					// Vertexformat
	DWORD		dwVertexSize;			// Größe eines einzelnen Vertex
	DWORD		dwNumVertices;			// Anzahl der Vertizes
	float		fBoundingSphereRadius;	// Radius der umgebenden Kugel
	Vector3D	vBoundingBoxMin;		// Minimumpunkt des umgebenden Quaders
	Vector3D	vBoundingBoxMax;		// Maximumpunkt des umgebenden Quaders
};

// ******************************************************************
// Struktur für den Header des CT_MODEL_INDICES-Chunks
struct ModelIndicesChunkHeader
{
	D3DFORMAT	IndexFormat;	// D3DFMT_INDEX16 oder D3DFMT_INDEX32
	DWORD		dwIndexSize;	// Größe eines Index
	DWORD		dwNumIndices;	// Anzahl der Indizes
};

// ******************************************************************
// Struktur für den Header eines einzelnen Effekts
struct ModelEffectHeader
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
struct ModelEffect
{
	ModelEffectHeader		Header;			// Effektheader
	char*					pcCode;			// Quellcode
	Effect*				pEffect;		// Effektklasse
	DWORD					dwNumTextures;	// Anzahl der Texturen
	PDIRECT3DBASETEXTURE9	apTexture[8];	// Max. 8 Texturen
};

// ******************************************************************
// Struktur für einen Octree-Knoten
struct ModelOctreeNode
{
	BOOL				bIsLeaf;			// Ist es ein Endknoten?
	ModelOctreeNode*	apChild[8];			// Untergeordnete Knoten
	DWORD				dwNumTriangles;		// Anzahl der Dreiecke dieses Knotens
	DWORD*				pdwTriangles;		// Die Dreiecke
	Vector3D			vBoundingBoxMin;	// Minimum des Umgebungsquaders
	Vector3D			vBoundingBoxMax;	// Maximum des Umgebungsquaders
};

// ******************************************************************
// Klasse für 3D-Modelle
class Model
{
	friend class ShadowVolume;

private:
	// Variablen
	DWORD					m_dwFVF;					// Vertexformat
	D3DFORMAT				m_IndexFormat;				// Indexformat
	DWORD					m_dwNumVertices;			// Anzahl der Vertizes
	DWORD					m_dwNumIndices;				// Anzahl der Indizes
	DWORD					m_dwNumEffects;				// Anzahl der Effekte
	DWORD					m_dwNumLights;				// Anzahl der Lichter
	VertexBuffer*			m_pVertexBuffer;			// Vertex-Buffer
	IndexBuffer*			m_pIndexBuffer;				// Index-Buffer
	ModelEffect*			m_pEffects;					// Die Effekte
	D3DLIGHT9*				m_pLights;					// Die Lichter
	float					m_fBoundingSphereRadius;	// Radius der umgebenden Kugel
	Vector3D				m_vBoundingBoxMin;			// Minimumpunkt des umgebenden Quaders
	Vector3D				m_vBoundingBoxMax;			// Maximumpunkt des umgebenden Quaders

	BOOL					m_bExtraData;				// Extradaten generiert?
	BOOL					m_bExtraDataOnly;			// Nur Extradaten?
	ModelOctreeNode*		m_pRootNode;				// Wurzelknoten des Octrees
	Vector3D*				m_pvVectors;				// Positionsvektoren der Vertizes
	DWORD*					m_pdwIndices;				// Alle Indizes
	Plane*				m_pTrianglePlanes;			// Ebenen der Dreiecke (4 pro Dreieck)

public:
	// Konstruktor und Destruktor
	Model();
	~Model();

	// Methoden
	// Aus virtueller Datei initialisieren
	Result Init(VFile* pVFile,
				  char* pcTexturePrefix = "",
				  char* pcTexturePostfix = "",
		          D3DPOOL VBPool = D3DPOOL_DEFAULT,
				  DWORD dwVBUsage = D3DUSAGE_WRITEONLY,
				  D3DPOOL IBPool = D3DPOOL_DEFAULT,
				  DWORD dwIBUsage = D3DUSAGE_WRITEONLY,
				  BOOL bGenerateExtraData = TRUE,
				  BOOL bExtraDataOnly = FALSE);

	// Aus Datei initialisieren
	Result Init(char* pcFilename,
				  char* pcTexturePrefix = "",
				  char* pcTexturePostfix = "",
		          D3DPOOL VBPool = D3DPOOL_DEFAULT,
				  DWORD dwVBUsage = D3DUSAGE_WRITEONLY,
				  D3DPOOL IBPool = D3DPOOL_DEFAULT,
				  DWORD dwIBUsage = D3DUSAGE_WRITEONLY,
				  BOOL bGenerateExtraData = TRUE,
				  BOOL bExtraDataOnly = FALSE);

	// Aus Speicher initialisieren
	Result Init(void* pMemory,
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
	Result Init(HMODULE hModule,
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

	Result LoadEffectTextures(DWORD dwEffect, char* pcTexturePrefix = "", char* pcTexturePostfix = "");		// Lädt die Texturen eines Effekts
	Result Render(int iFrom = -1, int iTo = -1, BOOL bRenderOpaque = TRUE, BOOL bRenderAlphaBlended = TRUE);	// Rendert das Modell
	Result UpdateExtraData();																				// Aktualisiert die Kollisionsdaten
	Result SplitNode(ModelOctreeNode* pNode, int iMaxDepth);												// Teilt einen Octree-Knoten rekursiv auf
	Result DeleteNode(ModelOctreeNode* pNode);																// Löscht einen Octree-Knoten rekursiv

	// Inline-Methoden
	inline DWORD			GetFVF()					{return m_dwFVF;}
	inline D3DFORMAT		GetIndexFormat()			{return m_IndexFormat;}
	inline DWORD			GetNumVertices()			{return m_dwNumVertices;}
	inline DWORD			GetNumIndices()				{return m_dwNumIndices;}
	inline DWORD			GetNumEffects()				{return m_dwNumEffects;}
	inline DWORD			GetNumLights()				{return m_dwNumLights;}
	inline VertexBuffer*	GetVertexBuffer()			{return m_pVertexBuffer;}
	inline IndexBuffer*	GetIndexBuffer()			{return m_pIndexBuffer;}
	inline ModelEffect*	GetEffects()				{return m_pEffects;}
	inline BOOL				ExtraData()					{return m_bExtraData;}
	inline BOOL				ExtraDataOnly()				{return m_bExtraDataOnly;}
	inline float			GeoundingSphereRadius()	{return m_fBoundingSphereRadius;}
	inline Vector3D		GeoundingBoxMin()			{return m_vBoundingBoxMin;}
	inline Vector3D		GeoundingBoxMax()			{return m_vBoundingBoxMax;}

	// friend-Funktionen
	friend BOOL LineHitsModel(const Vector3D& vLineA, const Vector3D& vLineB, const Model* pModel, const Matrix& mMatrix, const Matrix& mInvMatrix, const float fTolerance, Vector3D* pvOutPos, Vector3D* pvOutNormal, int* piOutTriangle);
	friend BOOL LineHitsModelRec(const Vector3D& vLineA, const Vector3D& vLineB, const Model* pModel, const ModelOctreeNode* pNode, int* piClosestTriangle, float* pfClosestCollision, BOOL bSimpleMode, BOOL* pbStop);
	friend BOOL ModelHitsModel(const Model* pModelA, const Matrix& mMatrixA, const Matrix& mInvMatrixA, const Model* pModelB, const Matrix& mMatrixB, const Matrix& mInvMatrixB, Vector3D* pvOutPos, Vector3D* pvOutNormalA, Vector3D* pvOutNormalB, int* piOutTriA, int* piOutTriB);
	friend BOOL ModelHitsModelRec(const Model* pModelA, const Matrix& mMatrixA, const Matrix& mInvMatrixA, const ModelOctreeNode* pNodeA, const Model* pModelB, const Matrix& mMatrixB, const Matrix& mInvMatrixB, const ModelOctreeNode* pNodeB, Vector3D* pvOutPos, Vector3D* pvOutNormalA, Vector3D* pvOutNormalB, int* piOutTriA, int* piOutTriB, BOOL bSkipBBTest);
	friend BOOL ModelHitsOctree(const Model* pModel, const Matrix& mMatrix, const Matrix& mInvMatrix, const Octree* pOctree, Vector3D* pvOutPos, Vector3D* pvOutNormalM, Vector3D* pvOutNormalO, int* piOutTriM, int* piOutTriO, OctreeNode** ppOutNodeO);
	friend BOOL ModelHitsOctreeRec(const Model* pModel, const Matrix& mMatrix, const Matrix& mInvMatrix, const ModelOctreeNode* pModelNode, const Octree* pOctree, const OctreeNode* pNode, Vector3D* pvOutPos, Vector3D* pvOutNormalM, Vector3D* pvOutNormalO, int* piOutTriM, int* piOutTriO, OctreeNode** ppOutNodeO);
};

// ******************************************************************