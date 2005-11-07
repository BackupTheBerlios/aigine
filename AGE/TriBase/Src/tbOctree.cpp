/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbOctree.cpp
	============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Rendern von Octrees

	Autor:
	David Scherfgen
	_________________________________________________
	|												|
	|	ACHTUNG:									|
	|	========									|
	|	DIESE DATEI MUSS MIT DER COMPILER-OPTION	|
	|	"STRUKTURMITGLIED AUF 1 BYTE AUSRICHTEN"	|
	|	KOMPILIERT WERDEN!							|
	|												|
********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Konstruktor der tbOctree-Klasse
tbOctree::tbOctree()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbOctree));
}

// ******************************************************************
// Destruktor der tbOctree-Klasse
tbOctree::~tbOctree()
{
	char acVariable[256];

	// Puffer löschen
	TB_SAFE_DELETE(m_pVertexBuffer);
	TB_SAFE_DELETE(m_pIndexBuffer);

	// Effekte löschen
	for(DWORD dwEffect = 0; dwEffect < m_dwNumEffects; dwEffect++)
	{
		// Texturen freigeben
		for(DWORD dwTex = 0; dwTex < m_pEffects[dwEffect].dwNumTextures; dwTex++)
		{
			if(m_pEffects[dwEffect].apTexture[dwTex] != NULL)
			{
				// Texturvariable im Effekt zurücksetzen, damit die Texturschnittstelle
				// freigegeben wird (mit SetTexture wurde der Referenzzähler erhöht)
				sprintf(acVariable, "Texture%d", dwTex + 1);
				m_pEffects[dwEffect].pEffect->GetEffect()->SetTexture(acVariable, NULL);

				// Den Texturmanager auffordern, die Textur freizugeben
				if(!tbTextureManager::IsInitialized()) tbTextureManager::ReleaseTexture(m_pEffects[dwEffect].apTexture[dwTex]);
			}
		}

		// Den Code und die Effektklasse löschen
		TB_SAFE_MEMFREE(m_pEffects[dwEffect].pcCode);
		TB_SAFE_DELETE(m_pEffects[dwEffect].pEffect);
	}

	// Effekt-Array löschen
	TB_SAFE_MEMFREE(m_pEffects);

	// Lichter löschen
	TB_SAFE_MEMFREE(m_pLights);

	// Sichtbarkeitsliste löschen
	TB_SAFE_MEMFREE(m_ppVisibleList);

	// Vektoren löschen
	if(m_bExtraData) TB_SAFE_MEMFREE(m_pvVectors);

	// Wurzelknoten löschen
	DeleteNode(m_pRootNode);
}

// ******************************************************************
// Liest einen Knoten rekursiv
tbResult tbOctree::ReadNode(tbOctreeNode* pNode,
							tbOctreeTreeChunkHeader* pTreeCH,
							tbVFile* pVFile)
{
	tbOctreeNodeHeader	NodeHeader;
	BYTE*				pData;


	// Knoten-Header lesen
	if(pVFile->Read(sizeof(tbOctreeNodeHeader), &NodeHeader))
	{
		// Fehler!
		TB_ERROR("Fehler beim Lesen eines Knoten-Headers!", TB_ERROR);
	}

	m_dwNumNodes++;

	// Daten kopieren
	pNode->bIsLeaf			= NodeHeader.bIsLeaf;
	pNode->dwStartIndex		= NodeHeader.dwStartIndex;
	pNode->dwNumIndices		= NodeHeader.dwNumIndices;
	pNode->vBoundingBoxMin	= NodeHeader.vBoundingBoxMin;
	pNode->vBoundingBoxMax	= NodeHeader.vBoundingBoxMax;

	// Ist es ein Endknoten?
	if(pNode->bIsLeaf)
	{
		m_dwNumLeaves++;

		// Indexdaten einlesen
		pData = (BYTE*)(m_pIndexBuffer->GetBuffer());
		pData += NodeHeader.dwStartIndex * pTreeCH->dwIndexSize;
		if(pVFile->Read(pNode->dwNumIndices * pTreeCH->dwIndexSize, pData))
		{
			// Fehler!
			TB_ERROR("Fehler beim Lesen der Indexdaten!", TB_ERROR);
		}

		// Speicher für Start, Ende und minimalen/maximalen Index jedes Effekts reservieren
		pNode->pdwEffectStart = (DWORD*)(tbMemAlloc(m_dwNumEffects * sizeof(DWORD)));
		pNode->pdwEffectEnd = (DWORD*)(tbMemAlloc(m_dwNumEffects * sizeof(DWORD)));
		pNode->pdwMinIndex = (DWORD*)(tbMemAlloc(m_dwNumEffects * sizeof(DWORD)));
		pNode->pdwMaxIndex = (DWORD*)(tbMemAlloc(m_dwNumEffects * sizeof(DWORD)));
		if(pNode->pdwEffectStart == NULL ||
		   pNode->pdwEffectEnd == NULL ||
		   pNode->pdwMinIndex == NULL ||
		   pNode->pdwMaxIndex == NULL)
		{
			// Fehler!
			TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
		}

		// Start, Ende und minimalen/maximalen Index jedes Effekts einlesen
		for(DWORD i = 0; i < m_dwNumEffects; i++)
		{
			pVFile->Read(sizeof(DWORD), &pNode->pdwEffectStart[i]);
			pVFile->Read(sizeof(DWORD), &pNode->pdwEffectEnd[i]);
			pVFile->Read(sizeof(DWORD), &pNode->pdwMinIndex[i]);
			pVFile->Read(sizeof(DWORD), &pNode->pdwMaxIndex[i]);
		}
	}
	else
	{
		// Speicher für die Unterknoten reservieren und sie rekursiv einlesen
		for(int i = 0; i < 8; i++)
		{
			// Speicher reservieren
			pNode->apChild[i] = (tbOctreeNode*)(tbMemAlloc(sizeof(tbOctreeNode)));
			if(pNode->apChild[i] == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			// Unterknoten einlesen
			if(ReadNode(pNode->apChild[i], pTreeCH, pVFile))
			{
				// Fehler!
				TB_ERROR("Fehler beim Lesen eines Unterknotens!", TB_ERROR);
			}
		}
	}

	return TB_OK;
}

// ******************************************************************
// Berechnet die Extradaten des Octrees
tbResult tbOctree::ComputeExtraData(tbOctreeNode* pNode)
{
	DWORD		dwVertexSize;
	BYTE*		pVector;
	DWORD		dwIndexSize;
	BYTE*		pIndex;
	tbVector3	vTriA;
	tbVector3	vTriB;
	tbVector3	vTriC;
	tbVector3	vTemp;


	if(pNode == m_pRootNode)
	{
		// Speicher für die Positionsvektoren reservieren
		m_pvVectors = (tbVector3*)(tbMemAlloc(m_dwNumVertices * sizeof(tbVector3)));
		if(m_pvVectors == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

		// Vertexgröße ermitteln
		dwVertexSize = m_pVertexBuffer->GetVertexSize();

		// Die Positionsvektoren kopieren
		pVector = (BYTE*)(m_pVertexBuffer->GetBuffer());
		for(DWORD i = 0; i < m_dwNumVertices; i++)
		{
			// Die Positionsangabe ist immer der erste Teil eines Vertex.
			m_pvVectors[i] = *((tbVector3*)(pVector));
			pVector += dwVertexSize;
		}
	}

	if(pNode->bIsLeaf)
	{
		// Indexgröße ermitteln
		dwIndexSize = m_pIndexBuffer->GetIndexSize();

		// Platz für die Indizes und die Dreiecksebenen reservieren
		pNode->pdwIndices = (DWORD*)(tbMemAlloc(pNode->dwNumIndices * sizeof(DWORD)));
		if(pNode->pdwIndices == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
		pNode->pTrianglePlanes = (tbPlane*)(tbMemAlloc(pNode->dwNumIndices / 3 * 4 * sizeof(tbPlane)));
		if(pNode->pTrianglePlanes == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

		// Die Indizes kopieren
		pIndex = (BYTE*)(m_pIndexBuffer->GetBuffer());
		pIndex += pNode->dwStartIndex * dwIndexSize;
		for(DWORD i = 0; i < pNode->dwNumIndices; i++)
		{
			// Index herausfinden
			if(m_pIndexBuffer->GetIndexFormat() == D3DFMT_INDEX16) pNode->pdwIndices[i] = *((WORD*)(pIndex));
			else if(m_pIndexBuffer->GetIndexFormat() == D3DFMT_INDEX32) pNode->pdwIndices[i] = *((DWORD*)(pIndex));
			else pNode->pdwIndices[i] = 0;
			pIndex += dwIndexSize;
		}

		// Jeweils vier Ebenen pro Dreieck berechnen
		for(i = 0; i < pNode->dwNumIndices / 3; i++)
		{
			// Die drei Vektoren des Dreiecks kopieren
			vTriA = m_pvVectors[pNode->pdwIndices[i * 3]];
			vTriB = m_pvVectors[pNode->pdwIndices[i * 3 + 1]];
			vTriC = m_pvVectors[pNode->pdwIndices[i * 3 + 2]];

			// Ebene des Dreiecks berechnen
			pNode->pTrianglePlanes[i * 4] = tbPlaneNormalize(tbPlaneFromPoints(vTriA, vTriB, vTriC));

			// Die drei Seitenebenen berechnen
			vTemp = tbVector3Normalize(tbVector3Cross(vTriA - vTriB, pNode->pTrianglePlanes[i * 4].n));
			pNode->pTrianglePlanes[i * 4 + 1] = tbPlaneFromPointNormal(vTriA, vTemp);

			vTemp = tbVector3Normalize(tbVector3Cross(vTriB - vTriC, pNode->pTrianglePlanes[i * 4].n));
			pNode->pTrianglePlanes[i * 4 + 2] = tbPlaneFromPointNormal(vTriB, vTemp);

			vTemp = tbVector3Normalize(tbVector3Cross(vTriC - vTriA, pNode->pTrianglePlanes[i * 4].n));
			pNode->pTrianglePlanes[i * 4 + 3] = tbPlaneFromPointNormal(vTriC, vTemp);
		}
	}
	else
	{
		// Die Extradaten der Unterknoten berechnen
		for(int i = 0; i < 8; i++)
		{
			if(ComputeExtraData(pNode->apChild[i]))
			{
				// Fehler!
				TB_ERROR("Fehler beim Berechnen der Extradaten!", TB_ERROR);
			}
		}
	}

	return TB_OK;
}

// ******************************************************************
// Laden aus einer virtuellen Datei
tbResult tbOctree::Init(tbVFile* pVFile,
						char* pcTexturePrefix,		// = ""
						char* pcTexturePostfix,		// = ""
						D3DPOOL VBPool,				// = D3DPOOL_DEFAULT
						DWORD dwVBUsage,			// = D3DUSAGE_WRITEONLY
						D3DPOOL IBPool,				// = D3DPOOL_DEFAULT
						DWORD dwIBUsage,			// = D3DUSAGE_WRITEONLY
						BOOL bGenerateExtraData)	// = TRUE
{
	tbOctreeChunkHeader			ChunkHeader;
	tbOctreeVerticesChunkHeader	VerticesCH;
	tbOctreeTreeChunkHeader		TreeCH;
	DWORD						dwEffect;


	// Parameter prüfen und sicherstellen, dass alles initialisiert wurde
	if(pVFile == NULL)					TB_ERROR_NULL_POINTER("pVFile", TB_ERROR);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Die Datei Chunk für Chunk lesen
	while(!pVFile->IsEOF())
	{
		// Chunk-Header lesen
		if(pVFile->Read(sizeof(tbOctreeChunkHeader), &ChunkHeader))
		{
			// Fehler!
			TB_ERROR("Fehler beim Lesen des Chunk-Headers!", TB_ERROR);
		}

		// Je nach Chunk-Typ...
		switch(ChunkHeader.ChunkType)
		{
		case TB_CT_OCTREE_VERTICES: // Vertexdaten
			// Vertexdaten-Chunk-Header einlesen
			if(pVFile->Read(sizeof(tbOctreeVerticesChunkHeader), &VerticesCH))
			{
				// Fehler!
				TB_ERROR("Fehler beim Lesen des Vertexdaten-Chunk-Headers!", TB_ERROR);
			}

			// Angaben kopieren
			m_dwFVF			= VerticesCH.dwFVF;
			m_dwNumVertices	= VerticesCH.dwNumVertices;

			// Vertex-Buffer erstellen
			m_pVertexBuffer = new tbVertexBuffer;
			if(m_pVertexBuffer == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			// Initialisierung mit den in der Datei gespeicherten Parametern
			if(m_pVertexBuffer->Init(VerticesCH.dwNumVertices * VerticesCH.dwVertexSize,
				                     VerticesCH.dwVertexSize, VerticesCH.dwFVF,
									 dwVBUsage | D3DUSAGE_WRITEONLY, VBPool))
			{
				// Fehler beim Erstellen des Vertex-Buffers!
				TB_ERROR("Fehler beim Erstellen des Vertex-Buffers!", TB_ERROR);
			}

			// Die Vertexdaten lesen
			if(pVFile->Read(VerticesCH.dwNumVertices * VerticesCH.dwVertexSize,
				            m_pVertexBuffer->GetBuffer()))
			{
				// Lesefehler!
				TB_ERROR("Fehler beim Lesen der Vertexdaten!", TB_ERROR);
			}

			// Vertex-Buffer aktualisieren
			m_pVertexBuffer->SetFirstVertex(0);
			m_pVertexBuffer->SetLastVertex(VerticesCH.dwNumVertices - 1);
			if(m_pVertexBuffer->Update()) TB_ERROR("Fehler beim Aktualisieren des Vertex-Buffers!", TB_ERROR);
			break;


		case TB_CT_OCTREE_EFFECTS: // Effekte
			// Anzahl der Effekte lesen
			if(pVFile->Read(sizeof(DWORD), &m_dwNumEffects)) TB_ERROR("Fehler beim Lesen der Effektanzahl!", TB_ERROR);

			// Genug Speicher für die Effekte reservieren
			m_pEffects = (tbOctreeEffect*)(tbMemAlloc(m_dwNumEffects * sizeof(tbOctreeEffect)));
			if(m_pEffects == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			// Jeden Effekt durchgehen
			for(dwEffect = 0; dwEffect < m_dwNumEffects; dwEffect++)
			{
				// Den Effekt-Header lesen
				if(pVFile->Read(sizeof(tbOctreeEffectHeader), &m_pEffects[dwEffect].Header))
				{
					// Fehler!
					TB_ERROR("Fehler beim Lesen des Effekt-Headers!", TB_ERROR);
				}

				// Speicher für den Effektcode reservieren
				m_pEffects[dwEffect].pcCode = (char*)(tbMemAlloc(m_pEffects[dwEffect].Header.dwEffectCodeSize));
				if(m_pEffects[dwEffect].pcCode == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

				// Effektcode lesen
				if(pVFile->Read(m_pEffects[dwEffect].Header.dwEffectCodeSize,
								m_pEffects[dwEffect].pcCode))
				{
					// Fehler!
					TB_ERROR("Fehler beim Lesen des Effektcodes!", TB_ERROR);
				}

				// tbEffect-Klasseninstanz erstellen
				m_pEffects[dwEffect].pEffect = new tbEffect;
				if(m_pEffects[dwEffect].pEffect == NULL)
				{
					// Fehler!
					TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
				}

				// Den Effekt initialisieren
				if(m_pEffects[dwEffect].pEffect->Init(m_pEffects[dwEffect].pcCode,
													  m_pEffects[dwEffect].Header.dwEffectCodeSize))
				{
					// Fehler beim Erstellen des Effekts!
					TB_ERROR("Fehler beim Erstellen des Effekts!", TB_ERROR);
				}

				// Die Texturen für den Effekt laden
				if(LoadEffectTextures(dwEffect, pcTexturePrefix, pcTexturePostfix))
				{
					// Fehler!
					TB_ERROR("Fehler beim Laden der Effekttexturen!", TB_ERROR);
				}
			}
			break;


		case TB_CT_OCTREE_LIGHTING: // Beleuchtung
			// Anzahl der Lichter lesen
			if(pVFile->Read(sizeof(DWORD), &m_dwNumLights))
			{
				// Fehler!
				TB_ERROR("Fehler beim Lesen der Anzahl der Lichter!", TB_ERROR);
			}

			// Speicher für die Lichter reservieren
			m_pLights = (D3DLIGHT9*)(tbMemAlloc(m_dwNumLights * sizeof(D3DLIGHT9)));
			if(m_pLights == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			// Die Lichtdaten einlesen
			if(pVFile->Read(m_dwNumLights * sizeof(D3DLIGHT9), m_pLights))
			{
				// Fehler!
				TB_ERROR("Fehler beim Lesen der Lichter!", TB_ERROR);
			}
			break;


		case TB_CT_OCTREE_TREE: // Octree-Daten
			// Header lesen
			if(pVFile->Read(sizeof(tbOctreeTreeChunkHeader), &TreeCH))
			{
				// Fehler!
				TB_ERROR("Fehler beim Lesen des Octree-Chunk-Headers!", TB_ERROR);
			}

			// Index-Buffer-Daten kopieren und Index-Buffer erzeugen
			m_IndexFormat = TreeCH.IndexFormat;
			m_dwNumIndices = TreeCH.dwNumIndices;

			// Index-Buffer erstellen
			m_pIndexBuffer = new tbIndexBuffer;
			if(m_pIndexBuffer == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			// Initialisierung mit den in der Datei gespeicherten Parametern
			if(m_pIndexBuffer->Init(TreeCH.dwNumIndices * TreeCH.dwIndexSize,
				                    TreeCH.dwIndexSize, TreeCH.IndexFormat,
									dwIBUsage | D3DUSAGE_WRITEONLY, IBPool))
			{
				// Fehler beim Erstellen des Index-Buffers!
				TB_ERROR("Fehler beim Ersstellen des Index-Buffers!", TB_ERROR);
			}

			// Wurzelknoten erzeugen
			m_pRootNode = (tbOctreeNode*)(tbMemAlloc(sizeof(tbOctreeNode)));
			if(m_pRootNode == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
			
			// Baum rekursiv lesen
			if(ReadNode(m_pRootNode, &TreeCH, pVFile))
			{
				// Fehler!
				TB_ERROR("Fehler beim Lesen des Octrees!", TB_ERROR);
			}

			// Index-Buffer aktualisieren
			m_pIndexBuffer->SetFirstIndex(0);
			m_pIndexBuffer->SetLastIndex(TreeCH.dwNumIndices - 1);
			if(m_pIndexBuffer->Update()) TB_ERROR("Fehler beim Aktualisieren des Index-Buffers!", TB_ERROR);

			// Speicher für die Sichtbarkeitsliste reservieren
			m_ppVisibleList = (tbOctreeNode**)(tbMemAlloc(m_dwNumLeaves * sizeof(tbOctreeNode*)));
			if(m_ppVisibleList == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
			break;


		default:
			// Dieser Chunk wird nicht gelesen - wir überspringen ihn!
			if(pVFile->Seek(TB_VFSO_CURSOR, ChunkHeader.dwDataSize))
			{
				// Fehler!
				TB_ERROR("Fehler beim Überspringen eines Chunks!", TB_ERROR);
			}
			break;
		}
	}

	m_bExtraData = bGenerateExtraData;

	if(m_bExtraData)
	{
		// Die Extradaten berechnen
		ComputeExtraData(m_pRootNode);
	}

	return TB_OK;
}

// ******************************************************************
// Erstellen aus einer echten Datei
tbResult tbOctree::Init(char* pcFilename,
						char* pcTexturePrefix,		// = ""
						char* pcTexturePostfix,		// = ""
						D3DPOOL VBPool,				// = D3DPOOL_DEFAULT
						DWORD dwVBUsage,			// = D3DUSAGE_WRITEONLY
						D3DPOOL IBPool,				// = D3DPOOL_DEFAULT
						DWORD dwIBUsage,			// = D3DUSAGE_WRITEONLY
						BOOL bGenerateExtraData)	// = TRUE
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", TB_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Aus Datei laden
	if(pVFile->Init(pcFilename))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, pcTexturePrefix, pcTexturePostfix, VBPool, dwVBUsage, IBPool, dwIBUsage,
		    bGenerateExtraData))
	{
		// Fehler!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Erstellen des Octrees!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Aus Speicherbereich initialisieren
tbResult tbOctree::Init(void* pMemory,
						int iMemorySize,
						char* pcTexturePrefix,		// = ""
						char* pcTexturePostfix,		// = ""
						D3DPOOL VBPool,				// = D3DPOOL_DEFAULT
						DWORD dwVBUsage,			// = D3DUSAGE_WRITEONLY
						D3DPOOL IBPool,				// = D3DPOOL_DEFAULT
						DWORD dwIBUsage,			// = D3DUSAGE_WRITEONLY
						BOOL bGenerateExtraData)	// = TRUE
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(pMemory == NULL)		TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);
	if(iMemorySize <= 0)	TB_ERROR_INVALID_VALUE("iMemorySize", TB_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Aus Speicher laden
	if(pVFile->Init(pMemory, iMemorySize))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, pcTexturePrefix, pcTexturePostfix, VBPool, dwVBUsage, IBPool, dwIBUsage,
		    bGenerateExtraData))
	{
		// Fehler!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Erstellen des Octrees!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Aus Ressource initialisieren
tbResult tbOctree::Init(HMODULE hModule,
						char* pcResourceName,
						char* pcResourceType,
						char* pcTexturePrefix,		// = ""
						char* pcTexturePostfix,		// = ""
						D3DPOOL VBPool,				// = D3DPOOL_DEFAULT
						DWORD dwVBUsage,			// = D3DUSAGE_WRITEONLY
						D3DPOOL IBPool,				// = D3DPOOL_DEFAULT
						DWORD dwIBUsage,			// = D3DUSAGE_WRITEONLY
						BOOL bGenerateExtraData)	// = TRUE
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(hModule == NULL)			TB_ERROR_NULL_POINTER("hModule", TB_ERROR);
	if(pcResourceName == NULL)	TB_ERROR_NULL_POINTER("pcResourceName", TB_ERROR);
	if(pcResourceType == NULL)	TB_ERROR_NULL_POINTER("pcResourceType", TB_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new tbVFile;
	if(pVFile == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Aus Ressource laden
	if(pVFile->Init(hModule, pcResourceName, pcResourceType))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, pcTexturePrefix, pcTexturePostfix, VBPool, dwVBUsage, IBPool, dwIBUsage,
		    bGenerateExtraData))
	{
		// Fehler!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Erstellen des Octrees!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Laden der Texturen eines Effekts
tbResult tbOctree::LoadEffectTextures(DWORD dwEffect,
									  char* pcTexturePrefix,	// = ""
									  char* pcTexturePostfix)	// = ""
{
	char	acVariable[256];
	char*	pcTextureFilename;
	char	acNewTextureFilename[256];
	DWORD	dwNumTextures;
	DWORD	dwTextureType;
	DWORD	dwColorKey;


	if(pcTexturePrefix == NULL) pcTexturePrefix = "";
	if(pcTexturePostfix == NULL) pcTexturePostfix = "";
    
	// Anzahl der Texturen lesen und in die Effektstruktur eintragen
	dwNumTextures = 0;
	m_pEffects[dwEffect].pEffect->GetEffect()->GetInt("NumTextures", (int*)(&dwNumTextures));
	m_pEffects[dwEffect].dwNumTextures = dwNumTextures;

	// Alle Texturen durchgehen
	for(DWORD dwTex = 0; dwTex < dwNumTextures; dwTex++)
	{
		// Dateiname abfragen
		sprintf(acVariable, "Texture%dFilename", dwTex + 1);
		if(SUCCEEDED(m_pEffects[dwEffect].pEffect->GetEffect()->GetString(acVariable, (LPCSTR*)(&pcTextureFilename))))
		{
			if(!tbTextureManager::IsInitialized()) TB_ERROR("Der Texturmanager wurde noch nicht initialisiert!", TB_ERROR);

			// Präfix und Postfix einfügen
			sprintf(acNewTextureFilename, "%s%s%s", pcTexturePrefix, pcTextureFilename, pcTexturePostfix);

			// Standardtexturtyp: 2D
			dwTextureType = 1;

			// Typ der Textur abfragen
			sprintf(acVariable, "Texture%dType", dwTex + 1);
			m_pEffects[dwEffect].pEffect->GetEffect()->GetInt(acVariable, (int*)(&dwTextureType));

			// Color-Key abfragen
			dwColorKey = 0;
			sprintf(acVariable, "Texture%dColorKey", dwTex + 1);
			m_pEffects[dwEffect].pEffect->GetEffect()->GetInt(acVariable, (int*)(&dwColorKey));

			// Nun die Textur laden
			if(dwTextureType == 1) m_pEffects[dwEffect].apTexture[dwTex] = tbTextureManager::GetTexture(acNewTextureFilename, TRUE, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DFMT_UNKNOWN, 0, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, dwColorKey);
			else if(dwTextureType == 2) m_pEffects[dwEffect].apTexture[dwTex] = tbTextureManager::GetCubeTexture(acNewTextureFilename, TRUE, D3DX_DEFAULT, D3DX_DEFAULT, D3DFMT_UNKNOWN, 0, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, dwColorKey);
			else if(dwTextureType == 3) m_pEffects[dwEffect].apTexture[dwTex] = tbTextureManager::GetVolumeTexture(acNewTextureFilename, TRUE, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DFMT_UNKNOWN, 0, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, dwColorKey);
			else
			{
				// Unbekannter Texturtyp!
				TB_ERROR("Unbekannter Texturtyp!", TB_ERROR);
			}

			// Prüfen
			if(m_pEffects[dwEffect].apTexture[dwTex] == NULL)
			{
				// Fehler!
				TB_ERROR("Fehler beim Laden einer Textur!", TB_ERROR);
			}

			// Dem Effekt die Textur zuweisen
			sprintf(acVariable, "Texture%d", dwTex + 1);
			m_pEffects[dwEffect].pEffect->GetEffect()->SetTexture(acVariable, m_pEffects[dwEffect].apTexture[dwTex]);
		}
	}

	return TB_OK;
}

// ******************************************************************
// Rekursive Sichtbarkeitsberechnung
void tbOctree::ComputeVisibility(tbOctreeNode* pNode,
								 tbPlane* pViewFrustum,
								 tbVector3 vCamera,
								 BOOL bDepthSort)
{
	// Wenn der Knoten unsichtbar ist, brechen wir sofort ab.
	if(!tbAABoxVisible(pNode->vBoundingBoxMin,
		               pNode->vBoundingBoxMax,
					   pViewFrustum)) return;

	if(pNode->bIsLeaf)
	{
		// Den Knoten als sichtbar eintragen
		m_ppVisibleList[m_dwNumLeavesVisible] = pNode;
		m_dwNumLeavesVisible++;

		// Wenn nach Tiefe sortiert werden soll, dann berechnen wir noch das
		// Quadrat der Entfernung der Mitte des Knotens zur Kamera.
		if(bDepthSort)
		{
			pNode->fDistSq = tbVector3LengthSq((0.5f * (pNode->vBoundingBoxMin + pNode->vBoundingBoxMax)) -
				                               vCamera);
		}
	}
	else
	{
		for(int i = 0; i < 8; i++)
		{
			// Sichtbarkeit dieses Unterknotens testen
			ComputeVisibility(pNode->apChild[i], pViewFrustum, vCamera, bDepthSort);
		}
	}
}

// ******************************************************************
// Rückruffunktion für qsort zum Sortieren der Knoten nach Tiefe
int LeafCmpFunc(const void* pE1,
				const void* pE2)
{
	tbOctreeNode* pNode1 = (tbOctreeNode*)(pE1);
	tbOctreeNode* pNode2 = (tbOctreeNode*)(pE2);

	return (int)(pNode1->fDistSq - pNode2->fDistSq);
}

// ******************************************************************
// Diese Methode rendert den Octree.
tbResult tbOctree::Render(tbPlane* pViewFrustum,
						  tbVector3 vCamera,
						  int iFrom,				// = -1
						  int iTo,					// = -1
						  BOOL bRenderOpaque,		// = TRUE
						  BOOL bRenderAlphaBlended,	// = TRUE
						  BOOL bDepthSort)			// = TRUE
{
	HRESULT			hResult;
	DWORD			dwOldFVF;
	int				iNumPasses;
	tbOctreeNode*	pLeaf;


	// Parameter prüfen
	if(iFrom < -1 || iFrom >= (int)(m_dwNumEffects))	TB_ERROR_INVALID_VALUE("iFrom", TB_ERROR);
	if(iTo < -1 || iTo >= (int)(m_dwNumEffects))		TB_ERROR_INVALID_VALUE("iTo", TB_ERROR);


	// Werte anpassen
	if(iFrom == -1) iFrom = 0;
	if(iTo == -1) iTo = m_dwNumEffects - 1;

	// Sichtbarkeitsliste erstellen
	m_dwNumLeavesVisible = 0;
	ComputeVisibility(m_pRootNode, pViewFrustum, vCamera, bDepthSort);

	// Wenn erwünscht, nach Tiefe sortieren
	if(bDepthSort) qsort(m_ppVisibleList, m_dwNumLeavesVisible, sizeof(tbOctreeNode*), LeafCmpFunc);

	// Altes Vertexformat speichern
	dwOldFVF = tbDirect3D::GetFVF();

	// Vertexformat so wie Vertex- und Index-Buffer setzen
	tbDirect3D::SetFVF(m_dwFVF);
	tbDirect3D::GetDevice()->SetStreamSource(0, m_pVertexBuffer->GetVB(), 0, m_pVertexBuffer->GetVertexSize());
	tbDirect3D::GetDevice()->SetIndices(m_pIndexBuffer->GetIB());

	// Jeden Effekt durchgehen
	for(int iEffect = iFrom; iEffect <= iTo; iEffect++)
	{
		// Effekt aktivieren und alle Durchgänge rendern.
		// tbDirect3D::Capture wird später manuell aufgerufen.
		if(!m_pEffects[iEffect].Header.bAlphaBlended && bRenderOpaque ||
		   m_pEffects[iEffect].Header.bAlphaBlended && bRenderAlphaBlended)
		{
			iNumPasses = m_pEffects[iEffect].pEffect->Begin(TRUE, FALSE);
			for(int iPass = 0; iPass < iNumPasses; iPass++)
			{
				// Durchgang aktivieren
				m_pEffects[iEffect].pEffect->BeginPass(iPass);

				// Die Sichtbarkeitsliste rendern
				for(DWORD dwLeaf = 0; dwLeaf < m_dwNumLeavesVisible; dwLeaf++)
				{
					pLeaf = m_ppVisibleList[dwLeaf];
					if(pLeaf->pdwEffectStart[iEffect] != 0xFFFFFFFF &&
					   pLeaf->pdwMinIndex[iEffect] != 0xFFFFFFFF &&
					   pLeaf->pdwMaxIndex[iEffect] != 0)
					{
						// Rendern
						if(FAILED(hResult = tbDirect3D::GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
							                                                              0,
																						  pLeaf->pdwMinIndex[iEffect],
																						  pLeaf->pdwMaxIndex[iEffect] - pLeaf->pdwMinIndex[iEffect] + 1,
																						  pLeaf->pdwEffectStart[iEffect],
																						  (pLeaf->pdwEffectEnd[iEffect] - pLeaf->pdwEffectStart[iEffect]) / 3 + 1)))
						{
							// Fehler!
							m_pEffects[iEffect].pEffect->End();
							TB_ERROR_DIRECTX("tbDirect3D::GetDevice()->DrawIndexedPrimitive", hResult, TB_ERROR);
						}
					}
				m_pEffects[iEffect].pEffect->EndPass();
				}
			}

			// Effekt deaktivieren
			m_pEffects[iEffect].pEffect->End();
		}
	}

	// Capture aufrufen, um die Tabellen in tbDirect3D zu aktualisieren
	tbDirect3D::Capture();

	// Das alte Vertexformat wieder setzen
	tbDirect3D::SetFVF(dwOldFVF);

	return TB_OK;
}

// ******************************************************************
// Löscht einen Octree-Knoten rekursiv
tbResult tbOctree::DeleteNode(tbOctreeNode* pNode)
{
	// Ist es ein Endknoten? Wenn ja, werden seine Daten gelöscht.
	if(pNode->bIsLeaf)
	{
		TB_SAFE_MEMFREE(pNode->pdwEffectStart);
		TB_SAFE_MEMFREE(pNode->pdwEffectEnd);
		TB_SAFE_MEMFREE(pNode->pdwMinIndex);
		TB_SAFE_MEMFREE(pNode->pdwMaxIndex);

		if(m_bExtraData)
		{
			TB_SAFE_MEMFREE(pNode->pTrianglePlanes);
			TB_SAFE_MEMFREE(pNode->pdwIndices);
		}
	}
	else
	{
		// Die 8 untergeordneten Knoten rekursiv löschen
		for(DWORD i = 0; i < 8; i++) DeleteNode(pNode->apChild[i]);
	}

	// Den Knoten selbst löschen
	TB_SAFE_MEMFREE(pNode);

	return TB_OK;
}

// ******************************************************************
tbOctreeNode* tbOctree::GetLeafByPoint(const tbVector3& vPoint,
									   tbOctreeNode* pNode) // = NULL
{
	tbOctreeNode* pResult;

	if(pNode == NULL) pNode = m_pRootNode;

	// Befindet sich der Punkt innerhalb der Bounding-Box dieses Knotens?
	if(vPoint.x >= pNode->vBoundingBoxMin.x &&
	   vPoint.y >= pNode->vBoundingBoxMin.y &&
	   vPoint.z >= pNode->vBoundingBoxMin.z &&
	   vPoint.x <= pNode->vBoundingBoxMax.x &&
	   vPoint.y <= pNode->vBoundingBoxMax.y &&
	   vPoint.z <= pNode->vBoundingBoxMax.z)
	{
		// Ja! Wenn es ein Endknoten ist, dann sind wir fertig.
		if(pNode->bIsLeaf) return pNode;

		// Der Punkt muss in einem der Unterknoten liegen.
		for(int i = 0; i < 8; i++)
		{
			pResult = GetLeafByPoint(vPoint, pNode->apChild[i]);
			if(pResult != NULL) return pResult;
		}
	}

	// Es gibt keinen Knoten um diesen Punkt!
	return NULL;
}

// ******************************************************************