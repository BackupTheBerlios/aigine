/********************************************************************
	|												|
	|	ACHTUNG:									|
	|	========									|
	|	DIESE DATEI MUSS MIT DER COMPILER-OPTION	|
	|	"STRUKTURMITGLIED AUF 1 BYTE AUSRICHTEN"	|
	|	KOMPILIERT WERDEN!							|
	|												|
********************************************************************/

#include "../AiGinE.h"

// ******************************************************************
// Konstruktor der Model-Klasse
Model::Model()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(Model));
}

// ******************************************************************
// Destruktor der Model-Klasse
Model::~Model()
{
	char acVariable[256];

	if(!m_bExtraDataOnly)
	{
		// Puffer löschen
		SAFE_DELETE(m_pVertexBuffer);
		SAFE_DELETE(m_pIndexBuffer);

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
					if(!TextureManager::IsInitialized()) TextureManager::ReleaseTexture(m_pEffects[dwEffect].apTexture[dwTex]);
				}
			}

			// Den Code und die Effektklasse löschen
			SAFE_MEMFREE(m_pEffects[dwEffect].pcCode);
			SAFE_DELETE(m_pEffects[dwEffect].pEffect);
		}

		// Effekt-Array löschen
		SAFE_MEMFREE(m_pEffects);

		// Lichter löschen
		SAFE_MEMFREE(m_pLights);
	}

	if(m_bExtraData)
	{
		// Extradaten löschen
		SAFE_MEMFREE(m_pvVectors);
		SAFE_MEMFREE(m_pdwIndices);
		SAFE_MEMFREE(m_pTrianglePlanes);
		DeleteNode(m_pRootNode);
	}
}

// ******************************************************************
// Laden aus einer virtuellen Datei
Result Model::Init(VFile* pVFile,
					   char* pcTexturePrefix,	// = ""
					   char* pcTexturePostfix,	// = ""
					   D3DPOOL VBPool,			// = D3DPOOL_DEFAULT
					   DWORD dwVBUsage,			// = D3DUSAGE_WRITEONLY
					   D3DPOOL IBPool,			// = D3DPOOL_DEFAULT
					   DWORD dwIBUsage,			// = D3DUSAGE_WRITEONLY
					   BOOL bGenerateExtraData,	// = TRUE
					   BOOL bExtraDataOnly)		// = FALSE
{
	ModelChunkHeader			ChunkHeader;
	ModelVerticesChunkHeader	VerticesCH;
	ModelIndicesChunkHeader	IndicesCH;
	DWORD						dwEffect;


	// Parameter prüfen und sicherstellen, dass alles initialisiert wurde
	if(pVFile == NULL)					ERROR_NULL_POINTER("pVFile", ERROR);
	if(!Direct3D::IsInitialized())	ERROR("Direct3D wurde noch nicht initialisiert!", ERROR);


	// Die Datei Chunk für Chunk lesen
	while(!pVFile->IsEOF())
	{
		// Chunk-Header lesen
		if(pVFile->Read(sizeof(ModelChunkHeader), &ChunkHeader))
		{
			// Fehler!
			ERROR("Fehler beim Lesen des Chunk-Headers!", ERROR);
		}

		if(bExtraDataOnly &&
		   (ChunkHeader.ChunkType == CT_MODEL_EFFECTS ||
		    ChunkHeader.ChunkType == CT_MODEL_LIGHTING))
		{
			// Den Chunk umgehen
			ChunkHeader.ChunkType = (ModelChunkType)(0xFFFFFFFF);
		}

		// Je nach Chunk-Typ...
		switch(ChunkHeader.ChunkType)
		{
		case CT_MODEL_VERTICES: // Vertexdaten
			// Vertexdaten-Chunk-Header einlesen
			if(pVFile->Read(sizeof(ModelVerticesChunkHeader), &VerticesCH))
			{
				// Fehler!
				ERROR("Fehler beim Lesen des Vertexdaten-Chunk-Headers!", ERROR);
			}

			// Angaben kopieren
			m_dwFVF					= VerticesCH.dwFVF;
			m_dwNumVertices			= VerticesCH.dwNumVertices;
			m_fBoundingSphereRadius	= VerticesCH.fBoundingSphereRadius;
			m_vBoundingBoxMin		= VerticesCH.vBoundingBoxMin;
			m_vBoundingBoxMax		= VerticesCH.vBoundingBoxMax;

			// Vertex-Buffer erstellen
			m_pVertexBuffer = new VertexBuffer;
			if(m_pVertexBuffer == NULL) ERROR_OUT_OF_MEMORY(ERROR);

			// Initialisierung mit den in der Datei gespeicherten Parametern
			if(m_pVertexBuffer->Init(VerticesCH.dwNumVertices * VerticesCH.dwVertexSize,
				                     VerticesCH.dwVertexSize, VerticesCH.dwFVF,
									 dwVBUsage | D3DUSAGE_WRITEONLY, VBPool))
			{
				// Fehler beim Erstellen des Vertex-Buffers!
				ERROR("Fehler beim Erstellen des Vertex-Buffers!", ERROR);
			}

			// Die Vertexdaten lesen
			if(pVFile->Read(VerticesCH.dwNumVertices * VerticesCH.dwVertexSize,
				            m_pVertexBuffer->Geuffer()))
			{
				// Lesefehler!
				ERROR("Fehler beim Lesen der Vertexdaten!", ERROR);
			}

			// Vertex-Buffer aktualisieren
			m_pVertexBuffer->SetFirstVertex(0);
			m_pVertexBuffer->SetLastVertex(VerticesCH.dwNumVertices - 1);
			if(m_pVertexBuffer->Update()) ERROR("Fehler beim Aktualisieren des Vertex-Buffers!", ERROR);
			break;


		case CT_MODEL_INDICES: // Indexdaten
			// Indexdaten-Chunk-Header einlesen
			if(pVFile->Read(sizeof(ModelIndicesChunkHeader), &IndicesCH))
			{
				// Fehler!
				ERROR("Fehler beim Lesen des Indexdaten-Chunk-Headers!", ERROR);
			}

			// Angaben kopieren
			m_IndexFormat = IndicesCH.IndexFormat;
			m_dwNumIndices = IndicesCH.dwNumIndices;

			// Index-Buffer erstellen
			m_pIndexBuffer = new IndexBuffer;
			if(m_pIndexBuffer == NULL) ERROR_OUT_OF_MEMORY(ERROR);

			// Initialisierung mit den in der Datei gespeicherten Parametern
			if(m_pIndexBuffer->Init(IndicesCH.dwNumIndices * IndicesCH.dwIndexSize,
				                    IndicesCH.dwIndexSize, IndicesCH.IndexFormat,
									dwIBUsage | D3DUSAGE_WRITEONLY, IBPool))
			{
				// Fehler beim Erstellen des Index-Buffers!
				ERROR("Fehler beim Ersstellen des Index-Buffers!", ERROR);
			}

			// Die Indexdaten lesen
			if(pVFile->Read(IndicesCH.dwNumIndices * IndicesCH.dwIndexSize,
				            m_pIndexBuffer->Geuffer()))
			{
				// Lesefehler!
				ERROR("Fehler beim Lesen der Indexdaten!", ERROR);
			}

			// Index-Buffer aktualisieren
			m_pIndexBuffer->SetFirstIndex(0);
			m_pIndexBuffer->SetLastIndex(IndicesCH.dwNumIndices - 1);
			if(m_pIndexBuffer->Update()) ERROR("Fehler beim Aktualisieren des Index-Buffers!", ERROR);
			break;


		case CT_MODEL_EFFECTS: // Effekte
			// Anzahl der Effekte lesen
			if(pVFile->Read(sizeof(DWORD), &m_dwNumEffects)) ERROR("Fehler beim Lesen der Effektanzahl!", ERROR);

			// Genug Speicher für die Effekte reservieren
			m_pEffects = (ModelEffect*)(MemAlloc(m_dwNumEffects * sizeof(ModelEffect)));
			if(m_pEffects == NULL) ERROR_OUT_OF_MEMORY(ERROR);

			// Jeden Effekt durchgehen
			for(dwEffect = 0; dwEffect < m_dwNumEffects; dwEffect++)
			{
				// Den Effekt-Header lesen
				if(pVFile->Read(sizeof(ModelEffectHeader), &m_pEffects[dwEffect].Header))
				{
					// Fehler!
					ERROR("Fehler beim Lesen des Effekt-Headers!", ERROR);
				}

				// Speicher für den Effektcode reservieren
				m_pEffects[dwEffect].pcCode = (char*)(MemAlloc(m_pEffects[dwEffect].Header.dwEffectCodeSize));
				if(m_pEffects[dwEffect].pcCode == NULL) ERROR_OUT_OF_MEMORY(ERROR);

				// Effektcode lesen
				if(pVFile->Read(m_pEffects[dwEffect].Header.dwEffectCodeSize,
								m_pEffects[dwEffect].pcCode))
				{
					// Fehler!
					ERROR("Fehler beim Lesen des Effektcodes!", ERROR);
				}

				// Effect-Klasseninstanz erstellen
				m_pEffects[dwEffect].pEffect = new Effect;
				if(m_pEffects[dwEffect].pEffect == NULL)
				{
					// Fehler!
					ERROR_OUT_OF_MEMORY(ERROR);
				}

				// Den Effekt initialisieren
				if(m_pEffects[dwEffect].pEffect->Init(m_pEffects[dwEffect].pcCode,
													  m_pEffects[dwEffect].Header.dwEffectCodeSize))
				{
					// Fehler beim Erstellen des Effekts!
					ERROR("Fehler beim Erstellen des Effekts!", ERROR);
				}

				// Die Texturen für den Effekt laden
				if(LoadEffectTextures(dwEffect, pcTexturePrefix, pcTexturePostfix))
				{
					// Fehler!
					ERROR("Fehler beim Laden der Effekttexturen!", ERROR);
				}
			}
			break;


		case CT_MODEL_LIGHTING: // Beleuchtung
			// Anzahl der Lichter lesen
			if(pVFile->Read(sizeof(DWORD), &m_dwNumLights))
			{
				// Fehler!
				ERROR("Fehler beim Lesen der Anzahl der Lichter!", ERROR);
			}

			// Speicher für die Lichter reservieren
			m_pLights = (D3DLIGHT9*)(MemAlloc(m_dwNumLights * sizeof(D3DLIGHT9)));
			if(m_pLights == NULL) ERROR_OUT_OF_MEMORY(ERROR);

			// Die Lichtdaten einlesen
			if(pVFile->Read(m_dwNumLights * sizeof(D3DLIGHT9), m_pLights))
			{
				// Fehler!
				ERROR("Fehler beim Lesen der Lichter!", ERROR);
			}
			break;


		default:
			// Dieser Chunk wird nicht gelesen - wir überspringen ihn!
			if(pVFile->Seek(VFSO_CURSOR, ChunkHeader.dwDataSize))
			{
				// Fehler!
				ERROR("Fehler beim Überspringen eines Chunks!", ERROR);
			}
			break;
		}
	}

	m_bExtraData = bGenerateExtraData;
	m_bExtraDataOnly = bExtraDataOnly;

	if(m_bExtraData)
	{
		// Speicher für die Positionsvektoren reservieren
		m_pvVectors = (Vector3*)(MemAlloc(m_dwNumVertices * sizeof(Vector3)));
		if(m_pvVectors == NULL) ERROR_OUT_OF_MEMORY(ERROR);

		// Nun für die Indizes
		m_pdwIndices = (DWORD*)(MemAlloc(m_dwNumIndices * sizeof(DWORD)));
		if(m_pdwIndices == NULL) ERROR_OUT_OF_MEMORY(ERROR);

		// Speicher für die Ebenen der Dreiecke reservieren
		m_pTrianglePlanes = (Plane*)(MemAlloc(m_dwNumIndices / 3 * 4 * sizeof(Plane)));
		if(m_pTrianglePlanes == NULL) ERROR_OUT_OF_MEMORY(ERROR);

		// Die Extradaten berechnen
		UpdateExtraData();
	}

	// Die anderen Daten - falls nicht erwünscht - wieder löschen
	if(m_bExtraDataOnly)
	{
		// Puffer löschen
		SAFE_DELETE(m_pVertexBuffer);
		SAFE_DELETE(m_pIndexBuffer);
	}

	return OK;
}

// ******************************************************************
// Erstellen aus einer echten Datei
Result Model::Init(char* pcFilename,
					   char* pcTexturePrefix,	// = ""
					   char* pcTexturePostfix,	// = ""
					   D3DPOOL VBPool,			// = D3DPOOL_DEFAULT
					   DWORD dwVBUsage,			// = D3DUSAGE_WRITEONLY
					   D3DPOOL IBPool,			// = D3DPOOL_DEFAULT
					   DWORD dwIBUsage,			// = D3DUSAGE_WRITEONLY
					   BOOL bGenerateExtraData,	// = TRUE
					   BOOL bExtraDataOnly)		// = FALSE
{
	VFile* pVFile;

	// Parameter prüfen
	if(pcFilename == NULL) ERROR_NULL_POINTER("pcFilename", ERROR);


	// Virtuelle Datei erstellen
	pVFile = new VFile;
	if(pVFile == NULL) ERROR_OUT_OF_MEMORY(ERROR);

	// Aus Datei laden
	if(pVFile->Init(pcFilename))
	{
		// Fehler!
		ERROR("Fehler beim Erstellen der virtuellen Datei!", ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, pcTexturePrefix, pcTexturePostfix, VBPool, dwVBUsage, IBPool, dwIBUsage,
		    bGenerateExtraData, bExtraDataOnly))
	{
		// Fehler!
		SAFE_DELETE(pVFile);
		ERROR("Fehler beim Erstellen des Modells!", ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	SAFE_DELETE(pVFile);

	return OK;
}

// ******************************************************************
// Aus Speicherbereich initialisieren
Result Model::Init(void* pMemory,
				       int iMemorySize,
					   char* pcTexturePrefix,	// = ""
					   char* pcTexturePostfix,	// = ""
					   D3DPOOL VBPool,			// = D3DPOOL_DEFAULT
					   DWORD dwVBUsage,			// = D3DUSAGE_WRITEONLY
					   D3DPOOL IBPool,			// = D3DPOOL_DEFAULT
					   DWORD dwIBUsage,			// = D3DUSAGE_WRITEONLY
					   BOOL bGenerateExtraData,	// = TRUE
					   BOOL bExtraDataOnly)		// = FALSE
{
	VFile* pVFile;

	// Parameter prüfen
	if(pMemory == NULL)		ERROR_NULL_POINTER("pMemory", ERROR);
	if(iMemorySize <= 0)	ERROR_INVALID_VALUE("iMemorySize", ERROR);


	// Virtuelle Datei erstellen
	pVFile = new VFile;
	if(pVFile == NULL) ERROR_OUT_OF_MEMORY(ERROR);

	// Aus Speicher laden
	if(pVFile->Init(pMemory, iMemorySize))
	{
		// Fehler!
		ERROR("Fehler beim Erstellen der virtuellen Datei!", ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, pcTexturePrefix, pcTexturePostfix, VBPool, dwVBUsage, IBPool, dwIBUsage,
		    bGenerateExtraData, bExtraDataOnly))
	{
		// Fehler!
		SAFE_DELETE(pVFile);
		ERROR("Fehler beim Erstellen des Modells!", ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	SAFE_DELETE(pVFile);

	return OK;
}

// ******************************************************************
// Aus Ressource initialisieren
Result Model::Init(HMODULE hModule,
					   char* pcResourceName,
					   char* pcResourceType,
					   char* pcTexturePrefix,		// = ""
					   char* pcTexturePostfix,		// = ""
					   D3DPOOL VBPool,				// = D3DPOOL_DEFAULT
					   DWORD dwVBUsage,				// = D3DUSAGE_WRITEONLY
					   D3DPOOL IBPool,				// = D3DPOOL_DEFAULT
					   DWORD dwIBUsage,				// = D3DUSAGE_WRITEONLY
					   BOOL bGenerateExtraData,	// = TRUE
					   BOOL bExtraDataOnly)		// = FALSE
{
	VFile* pVFile;

	// Parameter prüfen
	if(hModule == NULL)			ERROR_NULL_POINTER("hModule", ERROR);
	if(pcResourceName == NULL)	ERROR_NULL_POINTER("pcResourceName", ERROR);
	if(pcResourceType == NULL)	ERROR_NULL_POINTER("pcResourceType", ERROR);


	// Virtuelle Datei erstellen
	pVFile = new VFile;
	if(pVFile == NULL) ERROR_OUT_OF_MEMORY(ERROR);

	// Aus Ressource laden
	if(pVFile->Init(hModule, pcResourceName, pcResourceType))
	{
		// Fehler!
		ERROR("Fehler beim Erstellen der virtuellen Datei!", ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, pcTexturePrefix, pcTexturePostfix, VBPool, dwVBUsage, IBPool, dwIBUsage,
		    bGenerateExtraData, bExtraDataOnly))
	{
		// Fehler!
		SAFE_DELETE(pVFile);
		ERROR("Fehler beim Erstellen des Modells!", ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	SAFE_DELETE(pVFile);

	return OK;
}

// ******************************************************************
// Laden der Texturen eines Effekts
Result Model::LoadEffectTextures(DWORD dwEffect,
									 char* pcTexturePrefix,		// = ""
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
			if(!TextureManager::IsInitialized()) ERROR("Der Texturmanager wurde noch nicht initialisiert!", ERROR);

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
			if(dwTextureType == 1) m_pEffects[dwEffect].apTexture[dwTex] = TextureManager::GetTexture(acNewTextureFilename, TRUE, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DFMT_UNKNOWN, 0, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, dwColorKey);
			else if(dwTextureType == 2) m_pEffects[dwEffect].apTexture[dwTex] = TextureManager::GetCubeTexture(acNewTextureFilename, TRUE, D3DX_DEFAULT, D3DX_DEFAULT, D3DFMT_UNKNOWN, 0, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, dwColorKey);
			else if(dwTextureType == 3) m_pEffects[dwEffect].apTexture[dwTex] = TextureManager::GetVolumeTexture(acNewTextureFilename, TRUE, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DFMT_UNKNOWN, 0, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, dwColorKey);
			else
			{
				// Unbekannter Texturtyp!
				ERROR("Unbekannter Texturtyp!", ERROR);
			}

			// Prüfen
			if(m_pEffects[dwEffect].apTexture[dwTex] == NULL)
			{
				// Fehler!
				ERROR("Fehler beim Laden einer Textur!", ERROR);
			}

			// Dem Effekt die Textur zuweisen
			sprintf(acVariable, "Texture%d", dwTex + 1);
			m_pEffects[dwEffect].pEffect->GetEffect()->SetTexture(acVariable, m_pEffects[dwEffect].apTexture[dwTex]);
		}
	}

	return OK;
}

// ******************************************************************
// Diese Methode rendert das Modell.
Result Model::Render(int iFrom,					// = -1
						 int iTo,					// = -1
						 BOOL bRenderOpaque,		// = TRUE
						 BOOL bRenderAlphaBlended)	// = TRUE
{
	HRESULT	hResult;
	DWORD	dwOldFVF;
	int		iNumPasses;


	// Parameter prüfen
	if(iFrom < -1 || iFrom >= (int)(m_dwNumEffects))	ERROR_INVALID_VALUE("iFrom", ERROR);
	if(iTo < -1 || iTo >= (int)(m_dwNumEffects))		ERROR_INVALID_VALUE("iTo", ERROR);


	// Werte anpassen
	if(iFrom == -1) iFrom = 0;
	if(iTo == -1) iTo = m_dwNumEffects - 1;

	// Altes Vertexformat speichern
	dwOldFVF = Direct3D::GetFVF();

	// Vertexformat sowie Vertex- und Index-Buffer setzen
	Direct3D::SetFVF(m_dwFVF);
	Direct3D::GetDevice()->SetStreamSource(0, m_pVertexBuffer->GetVB(), 0, m_pVertexBuffer->GetVertexSize());
	Direct3D::GetDevice()->SetIndices(m_pIndexBuffer->GetIB());

	// Jeden Effekt durchgehen
	for(int iEffect = iFrom; iEffect <= iTo; iEffect++)
	{
		// Effekt aktivieren und alle Durchgänge rendern.
		// Direct3D::Capture wird später manuell aufgerufen.
		if(!m_pEffects[iEffect].Header.bAlphaBlended && bRenderOpaque ||
		   m_pEffects[iEffect].Header.bAlphaBlended && bRenderAlphaBlended)
		{
			iNumPasses = m_pEffects[iEffect].pEffect->Begin(TRUE, FALSE);
			for(int iPass = 0; iPass < iNumPasses; iPass++)
			{
				// Durchgang aktivieren
				m_pEffects[iEffect].pEffect->Pass(iPass);

				// Rendern
				hResult = Direct3D::GetDevice()->DrawIndexedPrimitive(m_pEffects[iEffect].Header.PrimitiveType,
																		0,
																		m_pEffects[iEffect].Header.dwMinIndex,
																		m_pEffects[iEffect].Header.dwNumVertices,
																		m_pEffects[iEffect].Header.dwStartIndex,
																		m_pEffects[iEffect].Header.dwNumPrimitives);
				if(FAILED(hResult))
				{
					// Fehler beim Rendern!
					m_pEffects[iEffect].pEffect->End();
					ERROR_DIRECTX("Direct3D::GetDevice()->DrawIndexedPrimitive", hResult, ERROR);
				}
			}

			// Effekt deaktivieren
			m_pEffects[iEffect].pEffect->End();
		}
	}

	// Capture aufrufen, um die Tabellen in Direct3D zu aktualisieren
	Direct3D::Capture();

	// Das alte Vertexformat wieder setzen
	Direct3D::SetFVF(dwOldFVF);

	return OK;
}

// ******************************************************************
// Berechnet die Extradaten des Modells
Result Model::UpdateExtraData()
{
	DWORD		dwVertexSize;
	DWORD		dwIndexSize;
	BYTE*		pVector;
	BYTE*		pIndex;
	Vector3	vTriA;
	Vector3	vTriB;
	Vector3	vTriC;
	Vector3	vTemp;


	// Hat das Modell Platz für Extradaten und hat es noch einen Vertex- und Index-Buffer?
	if(!m_bExtraData)										ERROR("Das Modell wurde ohne Unterstützung für Extradaten erstellt!", ERROR);
	if(m_pVertexBuffer == NULL || m_pIndexBuffer == NULL)	ERROR("Das Modell hat keinen Vertex-/Index-Buffer mehr!", ERROR);

	// Vertex- und Indexgröße ermitteln
	dwVertexSize = m_pVertexBuffer->GetVertexSize();
	dwIndexSize = m_pIndexBuffer->GetIndexSize();

	// Die Positionsvektoren kopieren
	pVector = (BYTE*)(m_pVertexBuffer->Geuffer());
	for(DWORD i = 0; i < m_dwNumVertices; i++)
	{
		// Die Positionsangabe ist immer der erste Teil eines Vertex.
		m_pvVectors[i] = *((Vector3*)(pVector));
		pVector += dwVertexSize;
	}

	// Die Indizes kopieren
	pIndex = (BYTE*)(m_pIndexBuffer->Geuffer());
	for(i = 0; i < m_dwNumIndices; i++)
	{
		// Index herausfinden
		if(m_pIndexBuffer->GetIndexFormat() == D3DFMT_INDEX16) m_pdwIndices[i] = *((WORD*)(pIndex));
		else if(m_pIndexBuffer->GetIndexFormat() == D3DFMT_INDEX32) m_pdwIndices[i] = *((DWORD*)(pIndex));
		else m_pdwIndices[i] = 0;
		pIndex += dwIndexSize;
	}

	// Jeweils vier Ebenen pro Dreieck berechnen
	for(i = 0; i < m_dwNumIndices / 3; i++)
	{
		// Die drei Vektoren des Dreiecks kopieren
		vTriA = m_pvVectors[m_pdwIndices[i * 3]];
		vTriB = m_pvVectors[m_pdwIndices[i * 3 + 1]];
		vTriC = m_pvVectors[m_pdwIndices[i * 3 + 2]];

		// Ebene des Dreiecks berechnen
		m_pTrianglePlanes[i * 4] = PlaneNormalize(PlaneFromPoints(vTriA, vTriB, vTriC));

		// Die drei Seitenebenen berechnen
		vTemp = Vector3Normalize(Vector3Cross(vTriA - vTriB, m_pTrianglePlanes[i * 4].n));
		m_pTrianglePlanes[i * 4 + 1] = PlaneFromPointNormal(vTriA, vTemp);

		vTemp = Vector3Normalize(Vector3Cross(vTriB - vTriC, m_pTrianglePlanes[i * 4].n));
		m_pTrianglePlanes[i * 4 + 2] = PlaneFromPointNormal(vTriB, vTemp);

		vTemp = Vector3Normalize(Vector3Cross(vTriC - vTriA, m_pTrianglePlanes[i * 4].n));
		m_pTrianglePlanes[i * 4 + 3] = PlaneFromPointNormal(vTriC, vTemp);
	}

	// Umgebungskugel und Umgebungsquader neu berechnen
	m_fBoundingSphereRadius = 0.0f;
	m_vBoundingBoxMin = Vector3(100000.0f);
	m_vBoundingBoxMax = Vector3(-100000.0f);
	for(i = 0; i < m_dwNumVertices; i++)
	{
		m_fBoundingSphereRadius = MAX(m_fBoundingSphereRadius, Vector3Length(m_pvVectors[i]));
		m_vBoundingBoxMin = Vector3Min(m_vBoundingBoxMin, m_pvVectors[i]);
		m_vBoundingBoxMax = Vector3Max(m_vBoundingBoxMax, m_pvVectors[i]);
	}

	// Wurzelknoten für Octree erstellen
	m_pRootNode = (ModelOctreeNode*)(MemAlloc(sizeof(ModelOctreeNode)));
	m_pRootNode->bIsLeaf = TRUE;
	m_pRootNode->dwNumTriangles = m_dwNumIndices / 3;
	m_pRootNode->vBoundingBoxMin = m_vBoundingBoxMin;
	m_pRootNode->vBoundingBoxMax = m_vBoundingBoxMax;

	// Diesem Knoten werden erst einmal alle Dreiecke zugewiesen.
	m_pRootNode->pdwTriangles = (DWORD*)(MemAlloc(m_pRootNode->dwNumTriangles * sizeof(DWORD)));
	for(i = 0; i < m_pRootNode->dwNumTriangles; i++)
	{
		// Dreieck dem Knoten zuteilen
		m_pRootNode->pdwTriangles[i] = i;
	}

	// Diesen Knoten rekursiv aufteilen, mit einer maximalen Tiefe von 4
	SplitNode(m_pRootNode, 4);

	return OK;
}

// ******************************************************************
// Teilt einen Octree-Knoten rekursiv auf
Result Model::SplitNode(ModelOctreeNode* pNode,
							int iMaxDepth)
{
	DWORD*		pdwTemp;
	DWORD		dwCounter;
	Vector3	vCenter;
	Vector3	vTriA;
	Vector3	vTriB;
	Vector3	vTriC;


	// Wenn die Tiefe null erreicht hat, wird abgebrochen.
	if(iMaxDepth <= 0) return OK;

	// Mittelpunkt dieses Knotens berechnen
	vCenter = 0.5f * (pNode->vBoundingBoxMin + pNode->vBoundingBoxMax);

	// 8 untergeordnete Knoten erstellen
	for(DWORD i = 0; i < 8; i++)
	{
		// Speicher für diesen Knoten reservieren
		pNode->apChild[i] = (ModelOctreeNode*)(MemAlloc(sizeof(ModelOctreeNode)));

		// Den Umgebungsquader für diesen Knoten festlegen
		switch(i)
		{
		case 0: // x-, y-, z-
			pNode->apChild[i]->vBoundingBoxMin = pNode->vBoundingBoxMin;
			pNode->apChild[i]->vBoundingBoxMax = vCenter;
			break;

		case 1: // x+, y-, z-
			pNode->apChild[i]->vBoundingBoxMin = pNode->vBoundingBoxMin;
			pNode->apChild[i]->vBoundingBoxMin.x = vCenter.x;
			pNode->apChild[i]->vBoundingBoxMax = vCenter;
			pNode->apChild[i]->vBoundingBoxMax.x = pNode->vBoundingBoxMax.x;
			break;

		case 2: // x-, y+, z-
			pNode->apChild[i]->vBoundingBoxMin = pNode->vBoundingBoxMin;
			pNode->apChild[i]->vBoundingBoxMin.y = vCenter.y;
			pNode->apChild[i]->vBoundingBoxMax = vCenter;
			pNode->apChild[i]->vBoundingBoxMax.y = pNode->vBoundingBoxMax.y;
			break;

		case 3: // x+, y+, z-
			pNode->apChild[i]->vBoundingBoxMin = pNode->vBoundingBoxMin;
			pNode->apChild[i]->vBoundingBoxMin.x = vCenter.x;
			pNode->apChild[i]->vBoundingBoxMin.y = vCenter.y;
			pNode->apChild[i]->vBoundingBoxMax = vCenter;
			pNode->apChild[i]->vBoundingBoxMax.x = pNode->vBoundingBoxMax.x;
			pNode->apChild[i]->vBoundingBoxMax.y = pNode->vBoundingBoxMax.y;
			break;

		case 4: // x-, y-, z+
			pNode->apChild[i]->vBoundingBoxMin = pNode->vBoundingBoxMin;
			pNode->apChild[i]->vBoundingBoxMin.z = vCenter.z;
			pNode->apChild[i]->vBoundingBoxMax = vCenter;
			pNode->apChild[i]->vBoundingBoxMax.z = pNode->vBoundingBoxMax.z;
			break;

		case 5: // x+, y-, z+
			pNode->apChild[i]->vBoundingBoxMin = pNode->vBoundingBoxMin;
			pNode->apChild[i]->vBoundingBoxMin.x = vCenter.x;
			pNode->apChild[i]->vBoundingBoxMin.z = vCenter.z;
			pNode->apChild[i]->vBoundingBoxMax = vCenter;
			pNode->apChild[i]->vBoundingBoxMax.x = pNode->vBoundingBoxMax.x;
			pNode->apChild[i]->vBoundingBoxMax.z = pNode->vBoundingBoxMax.z;
			break;

		case 6: // x-, y+, z+
			pNode->apChild[i]->vBoundingBoxMin = pNode->vBoundingBoxMin;
			pNode->apChild[i]->vBoundingBoxMin.y = vCenter.y;
			pNode->apChild[i]->vBoundingBoxMin.z = vCenter.z;
			pNode->apChild[i]->vBoundingBoxMax = vCenter;
			pNode->apChild[i]->vBoundingBoxMax.y = pNode->vBoundingBoxMax.y;
			pNode->apChild[i]->vBoundingBoxMax.z = pNode->vBoundingBoxMax.z;
			break;

		case 7: // x+, y+, z+
			pNode->apChild[i]->vBoundingBoxMin = vCenter;
			pNode->apChild[i]->vBoundingBoxMax = pNode->vBoundingBoxMax;
			break;
		}

		// Temporäre Dreiecksliste erstellen
		pdwTemp = new DWORD[pNode->dwNumTriangles];
		dwCounter = 0;

		// Alle Dreiecke durchgehen und prüfen, ob sie im Bereich dieses
		// untergeordneten Knotens liegen
		for(DWORD j = 0; j < pNode->dwNumTriangles; j++)
		{
			// Vektoren dieses Dreiecks herausfinden
			vTriA = m_pvVectors[m_pdwIndices[pNode->pdwTriangles[j] * 3]];
			vTriB = m_pvVectors[m_pdwIndices[pNode->pdwTriangles[j] * 3 + 1]];
			vTriC = m_pvVectors[m_pdwIndices[pNode->pdwTriangles[j] * 3 + 2]];

			// Wenn auch nur einer der Vektoren innerhalb der Bounding-Box
			// liegt, dann wird das Dreieck zu diesem untergeordneten Knoten gehören.
			// Gleiches gilt auch, wenn eine Seite des Dreiecks die Bounding-Box schneidet.
			if(PointHitsBox(vTriA, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, MatrixIdentity()) ||
			   PointHitsBox(vTriB, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, MatrixIdentity()) ||
			   PointHitsBox(vTriC, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, MatrixIdentity()) ||
			   LineHitsBox(vTriA, vTriB, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, MatrixIdentity()) ||
			   LineHitsBox(vTriB, vTriC, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, MatrixIdentity()) ||
			   LineHitsBox(vTriC, vTriA, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, MatrixIdentity()))
			{
				// Dieses Dreieck zur temporären Liste hinzufügen
				pdwTemp[dwCounter] = pNode->pdwTriangles[j];
				dwCounter++;
			}
		}

		// Die Temporäre Liste zum untergeordneten Knoten kopieren
		pNode->apChild[i]->pdwTriangles = (DWORD*)(MemAlloc(dwCounter * sizeof(DWORD)));
		memcpy(pNode->apChild[i]->pdwTriangles, pdwTemp, dwCounter * sizeof(DWORD));
		pNode->apChild[i]->dwNumTriangles = dwCounter;

		// Temporäre Liste wieder freigeben
		SAFE_DELETE(pdwTemp);

		// Dieser Knoten ist nun ein Endknoten!
		pNode->apChild[i]->bIsLeaf = TRUE;

		// Diesen Unterknoten rekursiv unterteilen, wenn es sich von der Anzahl der
		// Dreiecke her lohnt.
		if(dwCounter >= 32) SplitNode(pNode->apChild[i], iMaxDepth - 1);
	}

	// Eigene Dreiecksliste löschen
	SAFE_MEMFREE(pNode->pdwTriangles);

	// Dieser Knoten ist nun kein Endknoten mehr.
	pNode->bIsLeaf = FALSE;

	return OK;
}

// ******************************************************************
// Löscht einen Octree-Knoten rekursiv
Result Model::DeleteNode(ModelOctreeNode* pNode)
{
	// Ist es ein Endknoten? Wenn ja, werden seine Dreiecke gelöscht.
	if(pNode->bIsLeaf) SAFE_MEMFREE(pNode->pdwTriangles)
	else
	{
		// Die 8 untergeordneten Knoten rekursiv löschen
		for(DWORD i = 0; i < 8; i++) DeleteNode(pNode->apChild[i]);
	}

	// Den Knoten selbst löschen
	SAFE_MEMFREE(pNode);

	return OK;
}

// ******************************************************************