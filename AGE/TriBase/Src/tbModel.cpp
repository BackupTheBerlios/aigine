/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbModel.cpp
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit 3D-Modellen

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
// Konstruktor der tbModel-Klasse
tbModel::tbModel()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbModel));
}

// ******************************************************************
// Destruktor der tbModel-Klasse
tbModel::~tbModel()
{
	char acVariable[256];

	if(!m_bExtraDataOnly)
	{
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
	}

	if(m_bExtraData)
	{
		// Extradaten löschen
		TB_SAFE_MEMFREE(m_pvVectors);
		TB_SAFE_MEMFREE(m_pdwIndices);
		TB_SAFE_MEMFREE(m_pTrianglePlanes);
		DeleteNode(m_pRootNode);
	}
}

// ******************************************************************
// Laden aus einer virtuellen Datei
tbResult tbModel::Init(tbVFile* pVFile,
					   char* pcTexturePrefix,	// = ""
					   char* pcTexturePostfix,	// = ""
					   D3DPOOL VBPool,			// = D3DPOOL_DEFAULT
					   DWORD dwVBUsage,			// = D3DUSAGE_WRITEONLY
					   D3DPOOL IBPool,			// = D3DPOOL_DEFAULT
					   DWORD dwIBUsage,			// = D3DUSAGE_WRITEONLY
					   BOOL bGenerateExtraData,	// = TRUE
					   BOOL bExtraDataOnly)		// = FALSE
{
	tbModelChunkHeader			ChunkHeader;
	tbModelVerticesChunkHeader	VerticesCH;
	tbModelIndicesChunkHeader	IndicesCH;
	DWORD						dwEffect;


	// Parameter prüfen und sicherstellen, dass alles initialisiert wurde
	if(pVFile == NULL)					TB_ERROR_NULL_POINTER("pVFile", TB_ERROR);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Die Datei Chunk für Chunk lesen
	while(!pVFile->IsEOF())
	{
		// Chunk-Header lesen
		if(pVFile->Read(sizeof(tbModelChunkHeader), &ChunkHeader))
		{
			// Fehler!
			TB_ERROR("Fehler beim Lesen des Chunk-Headers!", TB_ERROR);
		}

		if(bExtraDataOnly &&
		   (ChunkHeader.ChunkType == TB_CT_MODEL_EFFECTS ||
		    ChunkHeader.ChunkType == TB_CT_MODEL_LIGHTING))
		{
			// Den Chunk umgehen
			ChunkHeader.ChunkType = (tbModelChunkType)(0xFFFFFFFF);
		}

		// Je nach Chunk-Typ...
		switch(ChunkHeader.ChunkType)
		{
		case TB_CT_MODEL_VERTICES: // Vertexdaten
			// Vertexdaten-Chunk-Header einlesen
			if(pVFile->Read(sizeof(tbModelVerticesChunkHeader), &VerticesCH))
			{
				// Fehler!
				TB_ERROR("Fehler beim Lesen des Vertexdaten-Chunk-Headers!", TB_ERROR);
			}

			// Angaben kopieren
			m_dwFVF					= VerticesCH.dwFVF;
			m_dwNumVertices			= VerticesCH.dwNumVertices;
			m_fBoundingSphereRadius	= VerticesCH.fBoundingSphereRadius;
			m_vBoundingBoxMin		= VerticesCH.vBoundingBoxMin;
			m_vBoundingBoxMax		= VerticesCH.vBoundingBoxMax;

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


		case TB_CT_MODEL_INDICES: // Indexdaten
			// Indexdaten-Chunk-Header einlesen
			if(pVFile->Read(sizeof(tbModelIndicesChunkHeader), &IndicesCH))
			{
				// Fehler!
				TB_ERROR("Fehler beim Lesen des Indexdaten-Chunk-Headers!", TB_ERROR);
			}

			// Angaben kopieren
			m_IndexFormat = IndicesCH.IndexFormat;
			m_dwNumIndices = IndicesCH.dwNumIndices;

			// Index-Buffer erstellen
			m_pIndexBuffer = new tbIndexBuffer;
			if(m_pIndexBuffer == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			// Initialisierung mit den in der Datei gespeicherten Parametern
			if(m_pIndexBuffer->Init(IndicesCH.dwNumIndices * IndicesCH.dwIndexSize,
				                    IndicesCH.dwIndexSize, IndicesCH.IndexFormat,
									dwIBUsage | D3DUSAGE_WRITEONLY, IBPool))
			{
				// Fehler beim Erstellen des Index-Buffers!
				TB_ERROR("Fehler beim Ersstellen des Index-Buffers!", TB_ERROR);
			}

			// Die Indexdaten lesen
			if(pVFile->Read(IndicesCH.dwNumIndices * IndicesCH.dwIndexSize,
				            m_pIndexBuffer->GetBuffer()))
			{
				// Lesefehler!
				TB_ERROR("Fehler beim Lesen der Indexdaten!", TB_ERROR);
			}

			// Index-Buffer aktualisieren
			m_pIndexBuffer->SetFirstIndex(0);
			m_pIndexBuffer->SetLastIndex(IndicesCH.dwNumIndices - 1);
			if(m_pIndexBuffer->Update()) TB_ERROR("Fehler beim Aktualisieren des Index-Buffers!", TB_ERROR);
			break;


		case TB_CT_MODEL_EFFECTS: // Effekte
			// Anzahl der Effekte lesen
			if(pVFile->Read(sizeof(DWORD), &m_dwNumEffects)) TB_ERROR("Fehler beim Lesen der Effektanzahl!", TB_ERROR);

			// Genug Speicher für die Effekte reservieren
			m_pEffects = (tbModelEffect*)(tbMemAlloc(m_dwNumEffects * sizeof(tbModelEffect)));
			if(m_pEffects == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			// Jeden Effekt durchgehen
			for(dwEffect = 0; dwEffect < m_dwNumEffects; dwEffect++)
			{
				// Den Effekt-Header lesen
				if(pVFile->Read(sizeof(tbModelEffectHeader), &m_pEffects[dwEffect].Header))
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


		case TB_CT_MODEL_LIGHTING: // Beleuchtung
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
	m_bExtraDataOnly = bExtraDataOnly;

	if(m_bExtraData)
	{
		// Speicher für die Positionsvektoren reservieren
		m_pvVectors = (tbVector3*)(tbMemAlloc(m_dwNumVertices * sizeof(tbVector3)));
		if(m_pvVectors == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

		// Nun für die Indizes
		m_pdwIndices = (DWORD*)(tbMemAlloc(m_dwNumIndices * sizeof(DWORD)));
		if(m_pdwIndices == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

		// Speicher für die Ebenen der Dreiecke reservieren
		m_pTrianglePlanes = (tbPlane*)(tbMemAlloc(m_dwNumIndices / 3 * 4 * sizeof(tbPlane)));
		if(m_pTrianglePlanes == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

		// Die Extradaten berechnen
		UpdateExtraData();
	}

	// Die anderen Daten - falls nicht erwünscht - wieder löschen
	if(m_bExtraDataOnly)
	{
		// Puffer löschen
		TB_SAFE_DELETE(m_pVertexBuffer);
		TB_SAFE_DELETE(m_pIndexBuffer);
	}

	return TB_OK;
}

// ******************************************************************
// Erstellen aus einer echten Datei
tbResult tbModel::Init(char* pcFilename,
					   char* pcTexturePrefix,	// = ""
					   char* pcTexturePostfix,	// = ""
					   D3DPOOL VBPool,			// = D3DPOOL_DEFAULT
					   DWORD dwVBUsage,			// = D3DUSAGE_WRITEONLY
					   D3DPOOL IBPool,			// = D3DPOOL_DEFAULT
					   DWORD dwIBUsage,			// = D3DUSAGE_WRITEONLY
					   BOOL bGenerateExtraData,	// = TRUE
					   BOOL bExtraDataOnly)		// = FALSE
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
		    bGenerateExtraData, bExtraDataOnly))
	{
		// Fehler!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Erstellen des Modells!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Aus Speicherbereich initialisieren
tbResult tbModel::Init(void* pMemory,
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
		    bGenerateExtraData, bExtraDataOnly))
	{
		// Fehler!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Erstellen des Modells!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Aus Ressource initialisieren
tbResult tbModel::Init(HMODULE hModule,
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
		    bGenerateExtraData, bExtraDataOnly))
	{
		// Fehler!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Erstellen des Modells!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Laden der Texturen eines Effekts
tbResult tbModel::LoadEffectTextures(DWORD dwEffect,
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
// Diese Methode rendert das Modell.
tbResult tbModel::Render(int iFrom,					// = -1
						 int iTo,					// = -1
						 BOOL bRenderOpaque,		// = TRUE
						 BOOL bRenderAlphaBlended)	// = TRUE
{
	HRESULT	hResult;
	DWORD	dwOldFVF;
	int		iNumPasses;


	// Parameter prüfen
	if(iFrom < -1 || iFrom >= (int)(m_dwNumEffects))	TB_ERROR_INVALID_VALUE("iFrom", TB_ERROR);
	if(iTo < -1 || iTo >= (int)(m_dwNumEffects))		TB_ERROR_INVALID_VALUE("iTo", TB_ERROR);


	// Werte anpassen
	if(iFrom == -1) iFrom = 0;
	if(iTo == -1) iTo = m_dwNumEffects - 1;

	// Altes Vertexformat speichern
	dwOldFVF = tbDirect3D::GetFVF();

	// Vertexformat sowie Vertex- und Index-Buffer setzen
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

				// Rendern
				hResult = tbDirect3D::GetDevice()->DrawIndexedPrimitive(m_pEffects[iEffect].Header.PrimitiveType,
																		0,
																		m_pEffects[iEffect].Header.dwMinIndex,
																		m_pEffects[iEffect].Header.dwNumVertices,
																		m_pEffects[iEffect].Header.dwStartIndex,
																		m_pEffects[iEffect].Header.dwNumPrimitives);
				if(FAILED(hResult))
				{
					// Fehler beim Rendern!
					m_pEffects[iEffect].pEffect->End();
					TB_ERROR_DIRECTX("tbDirect3D::GetDevice()->DrawIndexedPrimitive", hResult, TB_ERROR);
				}
				m_pEffects[iEffect].pEffect->EndPass();
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
// Berechnet die Extradaten des Modells
tbResult tbModel::UpdateExtraData()
{
	DWORD		dwVertexSize;
	DWORD		dwIndexSize;
	BYTE*		pVector;
	BYTE*		pIndex;
	tbVector3	vTriA;
	tbVector3	vTriB;
	tbVector3	vTriC;
	tbVector3	vTemp;


	// Hat das Modell Platz für Extradaten und hat es noch einen Vertex- und Index-Buffer?
	if(!m_bExtraData)										TB_ERROR("Das Modell wurde ohne Unterstützung für Extradaten erstellt!", TB_ERROR);
	if(m_pVertexBuffer == NULL || m_pIndexBuffer == NULL)	TB_ERROR("Das Modell hat keinen Vertex-/Index-Buffer mehr!", TB_ERROR);

	// Vertex- und Indexgröße ermitteln
	dwVertexSize = m_pVertexBuffer->GetVertexSize();
	dwIndexSize = m_pIndexBuffer->GetIndexSize();

	// Die Positionsvektoren kopieren
	pVector = (BYTE*)(m_pVertexBuffer->GetBuffer());
	for(DWORD i = 0; i < m_dwNumVertices; i++)
	{
		// Die Positionsangabe ist immer der erste Teil eines Vertex.
		m_pvVectors[i] = *((tbVector3*)(pVector));
		pVector += dwVertexSize;
	}

	// Die Indizes kopieren
	pIndex = (BYTE*)(m_pIndexBuffer->GetBuffer());
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
		m_pTrianglePlanes[i * 4] = tbPlaneNormalize(tbPlaneFromPoints(vTriA, vTriB, vTriC));

		// Die drei Seitenebenen berechnen
		vTemp = tbVector3Normalize(tbVector3Cross(vTriA - vTriB, m_pTrianglePlanes[i * 4].n));
		m_pTrianglePlanes[i * 4 + 1] = tbPlaneFromPointNormal(vTriA, vTemp);

		vTemp = tbVector3Normalize(tbVector3Cross(vTriB - vTriC, m_pTrianglePlanes[i * 4].n));
		m_pTrianglePlanes[i * 4 + 2] = tbPlaneFromPointNormal(vTriB, vTemp);

		vTemp = tbVector3Normalize(tbVector3Cross(vTriC - vTriA, m_pTrianglePlanes[i * 4].n));
		m_pTrianglePlanes[i * 4 + 3] = tbPlaneFromPointNormal(vTriC, vTemp);
	}

	// Umgebungskugel und Umgebungsquader neu berechnen
	m_fBoundingSphereRadius = 0.0f;
	m_vBoundingBoxMin = tbVector3(100000.0f);
	m_vBoundingBoxMax = tbVector3(-100000.0f);
	for(i = 0; i < m_dwNumVertices; i++)
	{
		m_fBoundingSphereRadius = TB_MAX(m_fBoundingSphereRadius, tbVector3Length(m_pvVectors[i]));
		m_vBoundingBoxMin = tbVector3Min(m_vBoundingBoxMin, m_pvVectors[i]);
		m_vBoundingBoxMax = tbVector3Max(m_vBoundingBoxMax, m_pvVectors[i]);
	}

	// Wurzelknoten für Octree erstellen
	m_pRootNode = (tbModelOctreeNode*)(tbMemAlloc(sizeof(tbModelOctreeNode)));
	m_pRootNode->bIsLeaf = TRUE;
	m_pRootNode->dwNumTriangles = m_dwNumIndices / 3;
	m_pRootNode->vBoundingBoxMin = m_vBoundingBoxMin;
	m_pRootNode->vBoundingBoxMax = m_vBoundingBoxMax;

	// Diesem Knoten werden erst einmal alle Dreiecke zugewiesen.
	m_pRootNode->pdwTriangles = (DWORD*)(tbMemAlloc(m_pRootNode->dwNumTriangles * sizeof(DWORD)));
	for(i = 0; i < m_pRootNode->dwNumTriangles; i++)
	{
		// Dreieck dem Knoten zuteilen
		m_pRootNode->pdwTriangles[i] = i;
	}

	// Diesen Knoten rekursiv aufteilen, mit einer maximalen Tiefe von 4
	SplitNode(m_pRootNode, 4);

	return TB_OK;
}

// ******************************************************************
// Teilt einen Octree-Knoten rekursiv auf
tbResult tbModel::SplitNode(tbModelOctreeNode* pNode,
							int iMaxDepth)
{
	DWORD*		pdwTemp;
	DWORD		dwCounter;
	tbVector3	vCenter;
	tbVector3	vTriA;
	tbVector3	vTriB;
	tbVector3	vTriC;


	// Wenn die Tiefe null erreicht hat, wird abgebrochen.
	if(iMaxDepth <= 0) return TB_OK;

	// Mittelpunkt dieses Knotens berechnen
	vCenter = 0.5f * (pNode->vBoundingBoxMin + pNode->vBoundingBoxMax);

	// 8 untergeordnete Knoten erstellen
	for(DWORD i = 0; i < 8; i++)
	{
		// Speicher für diesen Knoten reservieren
		pNode->apChild[i] = (tbModelOctreeNode*)(tbMemAlloc(sizeof(tbModelOctreeNode)));

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
			if(tbPointHitsBox(vTriA, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, tbMatrixIdentity()) ||
			   tbPointHitsBox(vTriB, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, tbMatrixIdentity()) ||
			   tbPointHitsBox(vTriC, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, tbMatrixIdentity()) ||
			   tbLineHitsBox(vTriA, vTriB, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, tbMatrixIdentity()) ||
			   tbLineHitsBox(vTriB, vTriC, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, tbMatrixIdentity()) ||
			   tbLineHitsBox(vTriC, vTriA, pNode->apChild[i]->vBoundingBoxMin, pNode->apChild[i]->vBoundingBoxMax, tbMatrixIdentity()))
			{
				// Dieses Dreieck zur temporären Liste hinzufügen
				pdwTemp[dwCounter] = pNode->pdwTriangles[j];
				dwCounter++;
			}
		}

		// Die Temporäre Liste zum untergeordneten Knoten kopieren
		pNode->apChild[i]->pdwTriangles = (DWORD*)(tbMemAlloc(dwCounter * sizeof(DWORD)));
		memcpy(pNode->apChild[i]->pdwTriangles, pdwTemp, dwCounter * sizeof(DWORD));
		pNode->apChild[i]->dwNumTriangles = dwCounter;

		// Temporäre Liste wieder freigeben
		TB_SAFE_DELETE(pdwTemp);

		// Dieser Knoten ist nun ein Endknoten!
		pNode->apChild[i]->bIsLeaf = TRUE;

		// Diesen Unterknoten rekursiv unterteilen, wenn es sich von der Anzahl der
		// Dreiecke her lohnt.
		if(dwCounter >= 32) SplitNode(pNode->apChild[i], iMaxDepth - 1);
	}

	// Eigene Dreiecksliste löschen
	TB_SAFE_MEMFREE(pNode->pdwTriangles);

	// Dieser Knoten ist nun kein Endknoten mehr.
	pNode->bIsLeaf = FALSE;

	return TB_OK;
}

// ******************************************************************
// Löscht einen Octree-Knoten rekursiv
tbResult tbModel::DeleteNode(tbModelOctreeNode* pNode)
{
	// Ist es ein Endknoten? Wenn ja, werden seine Dreiecke gelöscht.
	if(pNode->bIsLeaf) TB_SAFE_MEMFREE(pNode->pdwTriangles)
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