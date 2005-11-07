/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tb3DSReader.cpp
	===============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Lesen von 3DS-Dateien

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
// Standardkonstruktor
tb3DSFile::tb3DSFile()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tb3DSFile));

	// Alle Listen beginnen (damit später tbMemReAlloc verwendet werden kann)
	m_pMaterials	= (tb3DSMaterial*)(tbMemAlloc(1));
	m_pModels		= (tb3DSModel*)(tbMemAlloc(1));
	m_pLights		= (tb3DSLight*)(tbMemAlloc(1));
}

// ******************************************************************
// Destruktor
tb3DSFile::~tb3DSFile()
{
	// Alle Daten freigeben
	TB_SAFE_MEMFREE(m_pMaterials);
	TB_SAFE_MEMFREE(m_pLights);
	for(int iModel = 0; iModel < m_iNumModels; iModel++)
	{
		TB_SAFE_MEMFREE(m_pModels[iModel].pVertices);
		TB_SAFE_MEMFREE(m_pModels[iModel].pFaces);
	}

	TB_SAFE_MEMFREE(m_pModels);
}

// ******************************************************************
// Liest einen ASCIIZ-String aus einer virtuellen Datei
tbResult ReadASCIIZString(tbVFile* pVFile,
						  int iMaxChars,
						  char* pcOut)
{
	// So lange Zeichen einlesen, bis die maximale Anzahl erreicht ist
	// oder eine null gelesen wird
	for(int iChar = 0; iChar < iMaxChars; iChar++)
	{
		if(pVFile->Read(1, &pcOut[iChar])) return TB_ERROR;
		if(pcOut[iChar] == 0) return TB_OK;
	}

	// Ende des Strings manuell hinzufügen
	pcOut[iMaxChars - 1] = 0;

	return TB_OK;
}

// ******************************************************************
// Schreitet bis zu einem bestimmten Chunk fort
tbResult tbProceedTo3DSChunk(tbVFile* pVFile,
							 WORD wChunkID)
{
	tb3DSChunkHeader ChunkHeader;

	// So lange lesen, bis der Chunk gefunden ist oder ein Fehler auftritt
	while(TRUE)
	{
		if(pVFile->Read(sizeof(tb3DSChunkHeader), &ChunkHeader)) return TB_ERROR;
		if(ChunkHeader.wChunkID == wChunkID) return TB_OK;
		else if(pVFile->Seek(TB_VFSO_CURSOR, ChunkHeader.dwChunkSize - sizeof(tb3DSChunkHeader))) return TB_ERROR;
	}
}

// ******************************************************************
// Liest einen Material-Chunk einer 3DS-Datei
tbResult Read3DSMaterialChunk(tbVFile* pVFile,
							  tb3DSMaterial* pOut)
{
	tb3DSChunkHeader	ChunkHeader;
	BYTE				r;
	BYTE				g;
	BYTE				b;
	WORD				wPercentage;
	WORD				wShading;


	while(TRUE)
	{
		// Chunk-Header lesen
		if(pVFile->IsEOF()) return TB_OK;
		if(pVFile->Read(sizeof(tb3DSChunkHeader), &ChunkHeader)) return TB_ERROR;

		// Chunk verarbeiten
		switch(ChunkHeader.wChunkID)
		{
		case 0xA000: // Materialname
			// Name einlesen und speichern
			ReadASCIIZString(pVFile, 256, pOut->acName);
			break;

		case 0xA010: // Indirekte Farbe
		case 0xA020: // Streufarbe
		case 0xA030: // Glanzfarbe
			// Farbe lesen
			tbProceedTo3DSChunk(pVFile, 0x0011);
			pVFile->Read(sizeof(BYTE), &r);
			pVFile->Read(sizeof(BYTE), &g);
			pVFile->Read(sizeof(BYTE), &b);

			// Farbe ins Fließkomma-RGB-Format umwandeln und speichern
			switch(ChunkHeader.wChunkID)
			{
			case 0xA010: pOut->AmbientColor = tbColor((BYTE)(r), g, b, 0); break;
			case 0xA020: pOut->DiffuseColor = tbColor((BYTE)(r), g, b, 255); break;
			case 0xA030: pOut->SpecularColor = tbColor((BYTE)(r), g, b, 0); break;
			}
			break;

		case 0xA040: // Glanzstärke
			tbProceedTo3DSChunk(pVFile, 0x0030);
			pVFile->Read(sizeof(WORD), &wPercentage);
			pOut->fShininess = (float)(wPercentage) / 100.0f;
			break;

		case 0xA041: // Glanzmultiplikator
			tbProceedTo3DSChunk(pVFile, 0x0030);
			pVFile->Read(sizeof(WORD), &wPercentage);
			pOut->fSpecularPower = (float)(wPercentage) / 100.0f;
			break;

		case 0xA050: // Transparenz
			tbProceedTo3DSChunk(pVFile, 0x0030);
			pVFile->Read(sizeof(WORD), &wPercentage);
			pOut->fOpacity = (float)(100 - wPercentage) / 100.0f;
			if(wPercentage != 0) pOut->bBlended = TRUE;
			break;

		case 0xA080: // Eigenfarbe
			tbProceedTo3DSChunk(pVFile, 0x0011);
			pVFile->Read(sizeof(BYTE), &r);
			pVFile->Read(sizeof(BYTE), &g);
			pVFile->Read(sizeof(BYTE), &b);
			pOut->SelfIllumination = tbColor((BYTE)(r), g, b, 0);
			pOut->bSelfIllumColor = TRUE;
			break;

		case 0xA081: // Zweiseitiges Material
			pOut->bTwoSided = TRUE;
			break;

		case 0xA084: // Selbsterhellung
			tbProceedTo3DSChunk(pVFile, 0x0030);
			pVFile->Read(sizeof(WORD), &wPercentage);
			pOut->fSelfIllumination = (float)(wPercentage) / 100.0f;
			break;

		case 0xA085: // Drahtmaterial
			pOut->bWire = TRUE;
			break;

		case 0xA100: // Shading
			pVFile->Read(2, &wShading);
			if(wShading == 1) pOut->bNoShading = TRUE;
			break;

		case 0xA200: // Textur
		case 0xA210: // Opazitäts-Map
			tbProceedTo3DSChunk(pVFile, 0xA300);
			switch(ChunkHeader.wChunkID)
			{
			case 0xA200:
				ReadASCIIZString(pVFile, 256, pOut->acTextureMap);

				// Bei einer TGA-Datei ist es wahrscheinlich, dass sie einen Alphakanal enthält.
				// Daher wird in diesem Fall die Variable bBlended auf TRUE gesetzt.
				if(!stricmp(tbGetFilenameExtension(pOut->acTextureMap), "tga")) pOut->bBlended = TRUE;
				break;

			case 0xA210:
				ReadASCIIZString(pVFile, 256, pOut->acOpacityMap);
				pOut->bBlended = TRUE;
				break;
			}
			break;

		default:
			// Es scheint ein unwichtiger Chunk zu sein. Möglicherweise ist es aber auch
			// bereits einer, der von dieser Funktion nicht behandelt wird. In dem Fall
			// wird die Datei zurückgefahren und die Kontrolle wird an die übergeordnete
			// Funktion zurückgegeben.
			if(  (ChunkHeader.wChunkID >= 0x1000 && ChunkHeader.wChunkID < 0xA000)
			   || ChunkHeader.wChunkID >= 0xAFFF)
			{
				if(pVFile->Seek(TB_VFSO_CURSOR, -(int)(sizeof(tb3DSChunkHeader)))) return TB_ERROR;
				return TB_OK;
			}

			// Chunk überspringen
			if(pVFile->Seek(TB_VFSO_CURSOR, ChunkHeader.dwChunkSize - sizeof(tb3DSChunkHeader))) return TB_ERROR;
			break;
		}
	}

	return TB_OK;
}

// ******************************************************************
// Modellchunk aus einer 3DS-Datei lesen
tbResult Read3DSModelChunk(tbVFile* pVFile,
						   tb3DSFile* p3DSFile,
						   tb3DSModel* pOut)
{
	tb3DSChunkHeader	ChunkHeader;
	WORD				wNumFaces;
	WORD				wFace;
	WORD				wNumTexCoords;
	char				acMaterial[256];
	char				acText[256];
	int					iMaterialIndex;
	float				fRadiusSq;
	float				fFileRadiusSq;


	fFileRadiusSq = p3DSFile->m_fBoundingSphereRadius *
				    p3DSFile->m_fBoundingSphereRadius;

	pOut->mTransformation = tbMatrixIdentity();

	while(TRUE)
	{
		// Chunk-Header lesen
		if(pVFile->IsEOF()) return TB_OK;
		if(pVFile->Read(sizeof(tb3DSChunkHeader), &ChunkHeader)) return TB_ERROR;

		// Chunk verarbeiten
		switch(ChunkHeader.wChunkID)
		{
		case 0x4110: // Vertizes
			{
			// Anzahl der Vertizes lesen
			pVFile->Read(sizeof(WORD), &pOut->wNumVertices);

			// Speicher für die Vertizes reservieren
			pOut->pVertices = (tb3DSVertex*)(tbMemAlloc(pOut->wNumVertices * sizeof(tb3DSVertex)));
			if(pOut->pVertices == NULL) return TB_ERROR;

			// Die Vertizes selbst lesen. Die Koordinaten sind in 3DS-Dateien anders als
			// es üblich ist, deshalb werden y- und z-Achse vertauscht. Ähnliches gilt bei
			// der Reihenfolge der Vertizes innerhalb eines Dreiecks und bei den
			// Texturkoordinaten.
			for(int iVertex = 0; iVertex < (int)(pOut->wNumVertices); iVertex++)
			{
				pVFile->Read(sizeof(float), &pOut->pVertices[iVertex].p.x);
				pVFile->Read(sizeof(float), &pOut->pVertices[iVertex].p.z);
				pVFile->Read(sizeof(float), &pOut->pVertices[iVertex].p.y);

				// Bricht dieser Vertex den Rekord (Umgebungskugel, Umgebungsquader)?
				fRadiusSq = tbVector3LengthSq(pOut->pVertices[iVertex].p);
				if(fRadiusSq > fFileRadiusSq)
				{
					// Ja - neuer Kugelrekord!
					fFileRadiusSq = fRadiusSq;
					p3DSFile->m_fBoundingSphereRadius = sqrtf(fFileRadiusSq);
				}

				// Umgebungsquader erweitern?
				for(int c = 0; c < 3; c++)
				{
					if(pOut->pVertices[iVertex].p.c[c] < p3DSFile->m_vBoundingBoxMin.c[c]) p3DSFile->m_vBoundingBoxMin.c[c] = pOut->pVertices[iVertex].p.c[c];
					if(pOut->pVertices[iVertex].p.c[c] > p3DSFile->m_vBoundingBoxMax.c[c]) p3DSFile->m_vBoundingBoxMax.c[c] = pOut->pVertices[iVertex].p.c[c];
				}
			}
			}
			break;

		case 0x4120: // Dreiecke
			{
			// Anzahl der Dreiecke lesen
			pVFile->Read(sizeof(WORD), &pOut->wNumFaces);

			// Speicher für die Dreiecke reservieren
			pOut->pFaces = (tb3DSFace*)(tbMemAlloc(pOut->wNumFaces * sizeof(tb3DSFace)));
			if(pOut->pFaces == NULL) return TB_ERROR;

			// Die Dreiecke lesen. Sie sind in Form von Indizes gespeichert, die sich auf
			// die Vertizes dieses Objekts beziehen. Die Reihenfolge ist nicht A, B, C,
			// sondern A, C, B. Jedes Dreieck besitzt noch ein zusätzliches Feld (16 Bits),
			// das aber nicht verwertet wird.
			for(int iFace = 0; iFace < (int)(pOut->wNumFaces); iFace++)
			{
				pVFile->Read(sizeof(WORD), &pOut->pFaces[iFace].a);
				pVFile->Read(sizeof(WORD), &pOut->pFaces[iFace].c);
				pVFile->Read(sizeof(WORD), &pOut->pFaces[iFace].b);
				
				// Unwichtiges Feld überspringen
				pVFile->Seek(TB_VFSO_CURSOR, sizeof(WORD));
			}
			}
			break;

		case 0x4130: // Materialien der Dreiecke
			{
			// Den Namen des Materials einlesen
			ReadASCIIZString(pVFile, 256, acMaterial);

			// Nach dem Material suchen
			iMaterialIndex = -1;
			for(int iMaterial = 0; iMaterial < p3DSFile->m_iNumMaterials; iMaterial++)
			{
				if(!strcmp(acMaterial, p3DSFile->m_pMaterials[iMaterial].acName))
				{
					// Das ist das Material!
					iMaterialIndex = iMaterial;
					break;
				}
			}

			if(iMaterialIndex == -1)
			{
				// Das beschriebene Material wurde nicht gefunden! Wir weisen den Dreiecken
				// einfach das erste Material zu, geben aber eine Warnung aus.
				sprintf(acText, "Ein Material (\"%s\") wurde nicht gefunden!\n", acMaterial);
				TB_WARNING(acText);
				iMaterialIndex = 0;
			}

			// Anzahl der nun kommenden Dreiecke lesen
			pVFile->Read(sizeof(WORD), &wNumFaces);

			// Die Liste der Dreiecke lesen, denen das Material mit dem eben eingelesenen
			// Namen zugeordnet werden soll.
			for(int iFace = 0; iFace < (int)(wNumFaces); iFace++)
			{
				pVFile->Read(sizeof(WORD), &wFace);
				pOut->pFaces[wFace].iMaterial = iMaterialIndex;
			}

			if(wNumFaces > 0) p3DSFile->m_pMaterials[iMaterialIndex].bHasFaces = TRUE;
			}
			break;

		case 0x4140: // Texturkoordinaten
			{
			// Anzahl der Texturkoordinaten lesen
			pVFile->Read(sizeof(WORD), &wNumTexCoords);
			for(int iVertex = 0; iVertex < (int)(wNumTexCoords); iVertex++)
			{
				// Texturkoordinaten für diesen Vertex lesen
				pVFile->Read(sizeof(float), &pOut->pVertices[iVertex].t.x);
				pVFile->Read(sizeof(float), &pOut->pVertices[iVertex].t.y);
				pOut->pVertices[iVertex].t.y = 1.0f - pOut->pVertices[iVertex].t.y;
			}
			}
			break;

		case 0x4150: // Glättungsgruppen der Dreiecke
			{
			// Jedes Dreieck durchgehen und die vier Bytes große Glättungsgruppe lesen
			for(int iFace = 0; iFace < (int)(pOut->wNumFaces); iFace++)
			{
				// Glättungsgruppe lesen
				pVFile->Read(sizeof(DWORD), &pOut->pFaces[iFace].dwSmoothingGroup);
			}
			}
			break;

		case 0x4160: // Transformationsmatrix
			{
			// Matrixelemente lesen
			pVFile->Read(sizeof(float), &pOut->mTransformation.m11);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m13);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m12);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m31);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m33);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m32);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m21);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m23);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m22);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m41);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m43);
			pVFile->Read(sizeof(float), &pOut->mTransformation.m42);
			}
			break;

		default:
			// Es scheint ein unwichtiger Chunk zu sein. Möglicherweise ist es aber auch
			// bereits einer, der von dieser Funktion nicht behandelt wird. In dem Fall
			// wird die Datei zurückgefahren und die Kontrolle wird an die übergeordnete
			// Funktion zurückgegeben.
			if((ChunkHeader.wChunkID >= 0x1000 && ChunkHeader.wChunkID <= 0x4000) ||
			   ChunkHeader.wChunkID >= 0x4FFF)
			{
				if(pVFile->Seek(TB_VFSO_CURSOR, -(int)(sizeof(tb3DSChunkHeader)))) return TB_ERROR;
				return TB_OK;
			}

			// Chunk überspringen
			if(pVFile->Seek(TB_VFSO_CURSOR, ChunkHeader.dwChunkSize - sizeof(tb3DSChunkHeader))) return TB_ERROR;
			break;
		}
	}

	return TB_OK;
}

// ******************************************************************
// Liest einen Licht-Chunk einer 3DS-Datei
tbResult Read3DSLightChunk(tbVFile* pVFile,
						   tb3DSLight* pOut)
{
	tb3DSChunkHeader ChunkHeader;

	// Position und Farbe des Lichts einlesen. Sie stehen direkt am Anfang.
	pVFile->Read(sizeof(float), &pOut->vPosition.x);
	pVFile->Read(sizeof(float), &pOut->vPosition.z);
	pVFile->Read(sizeof(float), &pOut->vPosition.y);
	tbProceedTo3DSChunk(pVFile, 0x0010);
	pVFile->Read(sizeof(float), &pOut->Color.r);
	pVFile->Read(sizeof(float), &pOut->Color.g);
	pVFile->Read(sizeof(float), &pOut->Color.b);

	while(TRUE)
	{
		// Chunk-Header lesen
		if(pVFile->IsEOF()) return TB_OK;
		if(pVFile->Read(sizeof(tb3DSChunkHeader), &ChunkHeader)) return TB_ERROR;

		// Chunk verarbeiten
		switch(ChunkHeader.wChunkID)
		{
		case 0x4610: // Spotlicht
			// Typ ändern und dann Zielposition und Kegelwinkel lesen
			pOut->Type = TB_3DSLT_SPOT;
			pVFile->Read(sizeof(float), &pOut->vTarget.x);
			pVFile->Read(sizeof(float), &pOut->vTarget.z);
			pVFile->Read(sizeof(float), &pOut->vTarget.y);
			pVFile->Read(sizeof(float), &pOut->fInnerCone);
			pVFile->Read(sizeof(float), &pOut->fOuterCone);
			break;

		case 0x4625: // Lichtabschwächung
			pOut->bAttenuate = TRUE;
			break;

		case 0x4630: // Der Schatten für dieses Licht soll berechnet werden.
			pOut->bShadowed = TRUE;
			break;

		case 0x465A: // Reichweite des Lichts
			pVFile->Read(sizeof(float), &pOut->fRange);
			break;

		case 0x465B: // Multiplikator des Lichts
			pVFile->Read(sizeof(float), &pOut->fMultiplier);
			break;

		default:
			// Es scheint ein unwichtiger Chunk zu sein. Möglicherweise ist es aber auch
			// bereits einer, der von dieser Funktion nicht behandelt wird. In dem Fall
			// wird die Datei zurückgefahren und die Kontrolle wird an die übergeordnete
			// Funktion zurückgegeben.
			if((ChunkHeader.wChunkID >= 0x1000 && ChunkHeader.wChunkID <= 0x4000) ||
			   ChunkHeader.wChunkID >= 0x4FFF)
			{
				if(pVFile->Seek(TB_VFSO_CURSOR, -(int)(sizeof(tb3DSChunkHeader)))) return TB_ERROR;
				return TB_OK;
			}

			// Chunk überspringen
			if(pVFile->Seek(TB_VFSO_CURSOR, ChunkHeader.dwChunkSize - sizeof(tb3DSChunkHeader))) return TB_ERROR;
			break;
		}
	}

	return TB_OK;
}

// ******************************************************************
// 3DS-Datei aus einer virtuellen Datei laden
tbResult tb3DSFile::Init(tbVFile* pVFile)
{
	tb3DSChunkHeader	ChunkHeader;
	tb3DSMaterial		Material;
	tb3DSModel			Model;
	tb3DSLight			Light;
	char				acObjectName[256];


	// Parameter prüfen
	if(pVFile == NULL) TB_ERROR_NULL_POINTER("pVFile", TB_ERROR);


	while(TRUE)
	{
		// Chunk-Header einlesen
		if(pVFile->Read(sizeof(tb3DSChunkHeader), &ChunkHeader)) break;
		if(ChunkHeader.dwChunkSize < sizeof(tb3DSChunkHeader)) TB_ERROR("Ungültiger Chunk in der 3DS-Datei wurde entdeckt!", TB_ERROR);

		// Chunk verarbeiten
		switch(ChunkHeader.wChunkID)
		{
		case 0x4D4D: break; // Haupt-Chunk
		case 0x3D3D: break; // 3D-Editor-Chunk

		case 0xAFFF: // Material-Chunk
			// Material lesen
			ZeroMemory(&Material, sizeof(tb3DSMaterial));
			if(Read3DSMaterialChunk(pVFile, &Material)) TB_ERROR("Fehler beim Lesen des Material-Chunks!", TB_ERROR);

			// Dieses neue Material in die Materialliste eintragen
			m_pMaterials = (tb3DSMaterial*)(tbMemReAlloc(m_pMaterials,
			                                             -(int)(sizeof(tb3DSMaterial))));
			if(m_pMaterials == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			m_pMaterials[m_iNumMaterials] = Material;
			m_iNumMaterials++;
			break;

		case 0x4000: // Objekt-Chunk
			// Objektname einlesen und zwischenspeichern
			ReadASCIIZString(pVFile, 256, acObjectName);
			break;

		case 0x4100: // Modell-Chunk
			// Modell lesen
			ZeroMemory(&Model, sizeof(tb3DSModel));
			if(Read3DSModelChunk(pVFile, this, &Model)) TB_ERROR("Fehler beim Lesen des Modell-Chunks!", TB_ERROR);

			// Dieses neue Modell in die Modellliste eintragen, vorher den Namen eintragen
			strcpy(Model.acName, acObjectName);
			m_pModels = (tb3DSModel*)(tbMemReAlloc(m_pModels,
				                                   -(int)(sizeof(tb3DSModel))));
			if(m_pModels == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			m_pModels[m_iNumModels] = Model;
			m_iNumModels++;
			break;

		case 0x4600: // Licht-Chunk
			// Licht lesen. Der Standardlichttyp ist das Punktlicht und der Multiplikator ist 1.
			ZeroMemory(&Light, sizeof(tb3DSLight));
			Light.Type = TB_3DSLT_POINT;
			Light.fMultiplier = 1.0f;
			if(Read3DSLightChunk(pVFile, &Light)) TB_ERROR("Fehler beim Lesen des Licht-Chunks!", TB_ERROR);

			// Das neue Licht in die Liste packen und den Namen eintragen
			strcpy(Light.acName, acObjectName);
			m_pLights = (tb3DSLight*)(tbMemReAlloc(m_pLights, -(int)(sizeof(tb3DSLight))));
			if(m_pLights == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

			m_pLights[m_iNumLights] = Light;
			m_iNumLights++;
			break;

		default:
			// Unwichtiger Chunk - er wird übersprungen!
			if(pVFile->Seek(TB_VFSO_CURSOR, ChunkHeader.dwChunkSize - sizeof(tb3DSChunkHeader))) TB_ERROR("3DS-Datei ist zu kurz oder beschädigt!", TB_ERROR);
			break;
		}
	}

	return TB_OK;
}

// ******************************************************************
// 3DS-Datei aus einer echten Datei laden
tbResult tb3DSFile::Init(char* pcFilename)
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(pcFilename == NULL) TB_ERROR_NULL_POINTER("pcFilename", TB_ERROR);


	// Virtuelle Datei erzeugen
	pVFile = new tbVFile;
	if(pVFile->Init(pcFilename)) TB_ERROR("Es konnte keine virtuelle Datei erzeugt werden!", TB_ERROR);

	// Die Funktion zum Laden von 3DS-Dateien aus virtuellen Dateien aufrufen
	if(Init(pVFile))
	{
		delete pVFile;
		return TB_ERROR;
	}

	delete pVFile;
	return TB_OK;
}

// ******************************************************************
// 3DS-Datei aus dem Speicher laden
tbResult tb3DSFile::Init(void* pMemory,
						 int iSize)
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(pMemory == NULL)	TB_ERROR_NULL_POINTER("pMemory", TB_ERROR);
	if(iSize <= 0)		TB_ERROR_NULL_POINTER("iSize", TB_ERROR);


	// Virtuelle Datei erzeugen
	pVFile = new tbVFile;
	if(pVFile->Init(pMemory, iSize)) TB_ERROR("Es konnte keine virtuelle Datei erzeugt werden!", TB_ERROR);

	// Die Funktion zum Laden von 3DS-Dateien aus virtuellen Dateien aufrufen
	if(Init(pVFile))
	{
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Laden der 3DS-Datei!", TB_ERROR);
	}

	// Die virtuelle Datei freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// 3DS-Datei aus einer Ressource erzeugen
tbResult tb3DSFile::Init(HMODULE hModule,
						 char* pcResourceName,
						 char* pcResourceType)
{
	tbVFile* pVFile;

	// Parameter prüfen
	if(hModule == NULL)			TB_ERROR_NULL_POINTER("hModule", TB_ERROR);
	if(pcResourceName == NULL)	TB_ERROR_NULL_POINTER("pcResourceName", TB_ERROR);
	if(pcResourceType == NULL)	TB_ERROR_NULL_POINTER("pcResourceType", TB_ERROR);


	// Virtuelle Datei erzeugen
	pVFile = new tbVFile;
	if(pVFile->Init(hModule, pcResourceName, pcResourceType)) TB_ERROR("Es konnte keine virtuelle Datei erzeugt werden!", TB_ERROR);

	// Die Funktion zum Laden von 3DS-Dateien aus virtuellen Dateien aufrufen
	if(Init(pVFile))
	{
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Laden der 3DS-Datei!", TB_ERROR);
	}

	// Die virtuelle Datei freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Berechnen der Gesamtanzahl der Vertizes
int tb3DSFile::GetTotalNumVertices()
{
	int iTotal;

	// Alle Modelle durchlaufen
	iTotal = 0;
	for(int iModel = 0; iModel < m_iNumModels; iModel++) iTotal += m_pModels[iModel].wNumVertices;

	return iTotal;
}

// ******************************************************************
// Berechnen der Gesamtanzahl der Dreiecke
int tb3DSFile::GetTotalNumFaces()
{
	int iTotal;

	// Alle Modelle durchlaufen
	iTotal = 0;
	for(int iModel = 0; iModel < m_iNumModels; iModel++) iTotal += m_pModels[iModel].wNumFaces;

	return iTotal;
}

// ******************************************************************
// Diese Methode generiert "rohe" Vertex- und Materialdaten
tbResult tb3DSFile::GenerateRawData(int iMaterial,
									int iBaseIndex,
									tb3DSVertexEx** ppVerticesOut,
									WORD** ppwIndicesOut,
									DWORD* pdwNumVerticesOut,
									DWORD* pdwNumIndicesOut)
{
	tbVector3		vNormalSum;
	tbVector3		vNormal;
	DWORD			dwNumMatFaces;
	DWORD			dwNumTriangles;
	DWORD			dwNumVerticesOut;
	DWORD			dwVertexCounter;
	BOOL			bFound;
	tb3DSVertexEx*	pVertices;


	// Parameter prüfen
	if(ppVerticesOut == NULL)	TB_ERROR_NULL_POINTER("ppVerticesOut", TB_ERROR);
	if(ppwIndicesOut == NULL)	TB_ERROR_NULL_POINTER("ppwIndicesOut", TB_ERROR);


	// Diese Methode generiert aus den 3DS-Daten einen einzelnen Datenstrom, der
	// alle Modelle enthält. Die Vertizes sind nicht indiziert und besitzen einen
	// Normalvektor.

	// Zählen, wie viele Faces/Vertizes zu dem Material gehören
	dwNumMatFaces = 0;
	for(DWORD dwModel = 0; dwModel < (DWORD)(m_iNumModels); dwModel++)
	{
		for(DWORD dwFace = 0; dwFace < m_pModels[dwModel].wNumFaces; dwFace++)
		{
			if(m_pModels[dwModel].pFaces[dwFace].iMaterial == iMaterial) dwNumMatFaces++;
		}
	}

	// Speicher für die Vertizes reservieren
	pVertices = (tb3DSVertexEx*)(tbMemAlloc(3 * dwNumMatFaces * sizeof(tb3DSVertexEx)));
	if(pVertices == NULL) TB_ERROR_OUT_OF_MEMORY(TB_ERROR);

	// Ausgabezähler zurücksetzen
	dwVertexCounter = 0;

	// Jedes Modell durchgehen
	for(dwModel = 0; dwModel < (DWORD)(m_iNumModels); dwModel++)
	{
		// Jedes Dreieck dieses Modells durchgehen
		for(DWORD dwFace1 = 0; dwFace1 < m_pModels[dwModel].wNumFaces; dwFace1++)
		{
			if(m_pModels[dwModel].pFaces[dwFace1].iMaterial == iMaterial)
			{
				// Und nun jeden Index dieses Dreiecks durchgehen
				for(DWORD dwIndex1 = 0; dwIndex1 < 3; dwIndex1++)
				{
					// Ab hier wird gezählt, wie viele Dreiecke in diesem Modell den Vertex
					// beinhalten. Deren Normalvektoren werden addiert und der Durchschnitt
					// wird gebildet. Das ist dann der Vertexnormalvektor.
					vNormalSum = tbVector3(0.0f);
					dwNumTriangles = 0;

					for(DWORD dwFace2 = 0; dwFace2 < m_pModels[dwModel].wNumFaces; dwFace2++)
					{
						if(m_pModels[dwModel].pFaces[dwFace2].iMaterial == iMaterial &&
						   m_pModels[dwModel].pFaces[dwFace1].dwSmoothingGroup == m_pModels[dwModel].pFaces[dwFace2].dwSmoothingGroup)
						{
							bFound = FALSE;
							for(DWORD dwIndex2 = 0; dwIndex2 < 3; dwIndex2++)
							{
								// Wenn dieser Vertex mit dem anderen übereinstimmt und beide
								// Dreiecke die gleiche Glättungsgruppe haben, bilden sie eine
								// weiche Kante. In dem Fall wird der Normalvektor dieses
								// Dreiecks zur Normalvektorsumme addiert.
								if(m_pModels[dwModel].pFaces[dwFace1].i[dwIndex1] ==
								   m_pModels[dwModel].pFaces[dwFace2].i[dwIndex2])
								{
									// Dieses Dreieck beinhaltet den Vertex auch!
									bFound = TRUE;
									break;
								}
							}

							if(bFound)
							{
								// Es wurde ein Dreieck gefunden, das den Vertex auch beinhaltet.
								// Normalvektor des zweiten Dreiecks bilden.
								vNormal = tbComputeTriangleNormal(m_pModels[dwModel].pVertices[m_pModels[dwModel].pFaces[dwFace2].a].p,
																  m_pModels[dwModel].pVertices[m_pModels[dwModel].pFaces[dwFace2].b].p,
																  m_pModels[dwModel].pVertices[m_pModels[dwModel].pFaces[dwFace2].c].p);
								// Hinzuaddieren
								vNormalSum += vNormal;
								dwNumTriangles++;
							}
						}
					}

					// Den Durchschnitt aus allen Normalvektoren bilden und ihn normalisieren.
					// Das ist der Vertexnormalvektor.
					vNormalSum /= (float)(dwNumTriangles);

					// Diesen Vertex mit dem Normalvektor in den Ausgabespeicherbereich schreiben,
					// welcher der Methode als Parameter übergeben wurde
					pVertices[dwVertexCounter].p = m_pModels[dwModel].pVertices[m_pModels[dwModel].pFaces[dwFace1].i[dwIndex1]].p;
					pVertices[dwVertexCounter].n = tbVector3NormalizeEx(vNormalSum);
					pVertices[dwVertexCounter].t.x = m_pModels[dwModel].pVertices[m_pModels[dwModel].pFaces[dwFace1].i[dwIndex1]].t.x;
					pVertices[dwVertexCounter].t.y = m_pModels[dwModel].pVertices[m_pModels[dwModel].pFaces[dwFace1].i[dwIndex1]].t.y;
					dwVertexCounter++;
				}
			}
		}
	}

	// Maximalen Speicher für die Ausgabevertizes reservieren
	*ppVerticesOut = (tb3DSVertexEx*)(tbMemAlloc(3 * dwNumMatFaces * sizeof(tb3DSVertexEx)));
	if(*ppVerticesOut == NULL)
	{
		// Fehler!
		TB_SAFE_MEMFREE(pVertices);
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Jetzt haben wir die Vertexdaten vorliegen.
	// Einige Vertizes kommen mit großer Wahrscheinlichkeit mehrfach vor,
	// daher werden jetzt Indizes generiert.
	*ppwIndicesOut = (WORD*)(tbMemAlloc(3 * dwNumMatFaces * sizeof(WORD)));
	if(*ppwIndicesOut == NULL)
	{
		// Fehler!
		TB_SAFE_MEMFREE(pVertices);
		TB_SAFE_MEMFREE(*ppVerticesOut);
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Jeden Vertex durchgehen
	dwNumVerticesOut = 0;
	for(DWORD dwVertex = 0; dwVertex < dwNumMatFaces * 3; dwVertex++)
	{
		// Prüfen, ob dieser Vertex bereits im Ausgabespeicher liegt
		for(DWORD dwOutVtx = 0; dwOutVtx < dwNumVerticesOut; dwOutVtx++)
		{
			// Vergleichen...
			if(!memcmp(&pVertices[dwVertex], &((*ppVerticesOut)[dwOutVtx]), sizeof(tb3DSVertexEx)))
			{
				// Ja, der Vertex existiert schon! Er muss daher nicht noch einmal
				// übernommen werden. Wir schreiben nur den Index in die Indexausgabe.
				(*ppwIndicesOut)[dwVertex] = (WORD)(dwOutVtx) + iBaseIndex;
				break;
			}
		}

		if(dwOutVtx == dwNumVerticesOut)
		{
			// Der Vertex wurde nicht gefunden!
			// Jetzt tragen wir ihn ein und erhöhen den Zähler.
			(*ppVerticesOut)[dwNumVerticesOut] = pVertices[dwVertex];
			(*ppwIndicesOut)[dwVertex] = (WORD)(dwNumVerticesOut) + iBaseIndex;
			dwNumVerticesOut++;
		}
	}

	// Den Ausgabespeicherbereich anpassen
	*ppVerticesOut = (tb3DSVertexEx*)(tbMemReAlloc(*ppVerticesOut, dwNumVerticesOut * sizeof(tb3DSVertexEx)));
	if(*ppVerticesOut == NULL)
	{
		TB_SAFE_MEMFREE(*ppVerticesOut);
		TB_SAFE_MEMFREE(*ppwIndicesOut);
		TB_SAFE_MEMFREE(pVertices);
		TB_ERROR_OUT_OF_MEMORY(TB_ERROR);
	}

	// Die Ausgabevariablen füllen, wenn gewünscht
	if(pdwNumVerticesOut != NULL) *pdwNumVerticesOut = dwNumVerticesOut;
	if(pdwNumIndicesOut != NULL) *pdwNumIndicesOut = dwNumMatFaces * 3;

	// Speicherbereiche freigeben
	TB_SAFE_MEMFREE(pVertices);

	return TB_OK;
}

// ******************************************************************
// 3DS-Material in ein Direct3D-Material umwandeln
TRIBASE_API tbResult tbConvert3DSMaterialToD3DMaterial(tb3DSMaterial* p3DSMaterial,
													   D3DMATERIAL9* pOut)
{
	// Parameter prüfen
	if(p3DSMaterial == NULL)	TB_ERROR_NULL_POINTER("p3DSMaterial", TB_ERROR);
	if(pOut == NULL)			TB_ERROR_NULL_POINTER("pOut", TB_ERROR);


	// Direct3D-Material generieren
	pOut->Ambient	= p3DSMaterial->AmbientColor;
	pOut->Diffuse	= p3DSMaterial->DiffuseColor;
	pOut->Specular	= p3DSMaterial->SpecularColor * p3DSMaterial->fSpecularPower;
	pOut->Power		= p3DSMaterial->fShininess * 100.0f;
	
	// Die Opazität des Materials spielt nur in der Streufarbe eine Rolle.
	pOut->Diffuse.a = p3DSMaterial->fOpacity;

	// Eigenfarbe eintragen
	if(p3DSMaterial->bSelfIllumColor) pOut->Emissive = p3DSMaterial->SelfIllumination;
	else pOut->Emissive = p3DSMaterial->DiffuseColor * p3DSMaterial->fSelfIllumination;

	// Alpha gibt es nur bei der Streufarbe.
	pOut->Ambient.a = pOut->Emissive.a = pOut->Specular.a = 0.0f;

	return TB_OK;
}

// ******************************************************************
// 3DS-Licht in ein Direct3D-Licht umwandeln
TRIBASE_API tbResult tbConvert3DSLightToD3DLight(tb3DSLight* p3DSLight,
												 D3DLIGHT9* pOut)
{
	// Parameter prüfen
	if(p3DSLight == NULL)	TB_ERROR_NULL_POINTER("p3DSLight", TB_ERROR);
	if(pOut == NULL)		TB_ERROR_NULL_POINTER("pOut", TB_ERROR);


	// Direct3D-Licht generieren
	switch(p3DSLight->Type)
	{
	case TB_3DSLT_POINT: pOut->Type = D3DLIGHT_POINT; break;
	case TB_3DSLT_SPOT: pOut->Type = D3DLIGHT_SPOT; break;
	}

	pOut->Position		= p3DSLight->vPosition;
	pOut->Direction		= tbVector3NormalizeEx(p3DSLight->vTarget - p3DSLight->vPosition);
	pOut->Ambient		= p3DSLight->Color * p3DSLight->fMultiplier;
	pOut->Diffuse		= p3DSLight->Color * p3DSLight->fMultiplier;
	pOut->Specular		= p3DSLight->Color * p3DSLight->fMultiplier;
	pOut->Range			= p3DSLight->fRange;
	pOut->Attenuation0	= p3DSLight->bAttenuate ? 0.0f : 1.0f;
	pOut->Attenuation1	= p3DSLight->bAttenuate ? 1.0f / (0.01f * p3DSLight->fRange) : 0.0f;
	pOut->Attenuation2	= 0.0f;
	pOut->Theta			= TB_DEG_TO_RAD(p3DSLight->fInnerCone);
	pOut->Phi			= TB_DEG_TO_RAD(p3DSLight->fOuterCone);
	pOut->Falloff		= 1.0f;

	return TB_OK;
}

// ******************************************************************