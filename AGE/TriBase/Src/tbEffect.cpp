/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbEffect.cpp
	============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit D3DX-Effekten

	Autor:
	David Scherfgen

********************************************************************/

#include <TriBase.h>

// ******************************************************************
// Globale Variablen
LPD3DXEFFECTPOOL tb_g_pEffectPool = NULL;	// Globaler Effektpool

// ******************************************************************
// Konstruktor der tbEffect-Klasse
tbEffect::tbEffect()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(tbEffect));
}

// ******************************************************************
// Destruktor der tbEffect-Klasse
tbEffect::~tbEffect()
{
	// Effekt löschen
	TB_SAFE_RELEASE(m_pEffect);
}

// ******************************************************************
// Erstellen aus einem String
tbResult tbEffect::Init(char* pcCode,
						int iSize)
{
	HRESULT hResult;

	// Parameter prüfen und sicherstellen, dass Direct3D initialisiert wurde
	if(pcCode == NULL)					TB_ERROR_NULL_POINTER("pcCode", TB_ERROR);
	if(iSize == 0 || iSize < -1)		TB_ERROR_INVALID_VALUE("iSize", TB_ERROR);
	if(!tbDirect3D::IsInitialized())	TB_ERROR("Direct3D wurde noch nicht initialisiert!", TB_ERROR);


	// Länge anpassen
	if(iSize == -1) iSize = strlen(pcCode);

	// Jetzt den Effekt erstellen
	if(FAILED(hResult = D3DXCreateEffect(tbDirect3D::GetDevice(),
										 pcCode, iSize, NULL, NULL, 0,
										 tb_g_pEffectPool, &m_pEffect, NULL)))
	{
		// Fehler!
		TB_ERROR_DIRECTX("D3DXCreateEffect", hResult, TB_ERROR);
	}

	// Effektbeschreibung abfragen
	m_pEffect->GetDesc(&m_Desc);

	// Die erste gültige Technik setzen
	SetTechnique(-1);

	return TB_OK;
}

// ******************************************************************
// Erstellen aus einer virtuellen Datei
tbResult tbEffect::Init(tbVFile* pVFile,
						int iSize)
{
	// Parameter prüfen und sicherstellen, dass Direct3D initialisiert wurde
	if(pVFile == NULL)				TB_ERROR_NULL_POINTER("pVFile", TB_ERROR);
	if(iSize == 0 || iSize < -1)	TB_ERROR_INVALID_VALUE("iSize", TB_ERROR);


	// Länge anpassen
	if(iSize == -1) iSize = pVFile->GetSize();

	// Die andere Methode aufrufen
	if(Init((char*)(pVFile->GetBuffer()) + pVFile->GetCursor(), iSize))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Effekts!", TB_ERROR);
	}

	return TB_OK;
}

// ******************************************************************
// Erstellen aus einer echten Datei
tbResult tbEffect::Init(char* pcFilename)
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
	if(Init(pVFile, pVFile->GetSize()))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Effekts!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Aus Speicherbereich initialisieren
tbResult tbEffect::Init(void* pMemory,
					    int iMemorySize)
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
	if(Init(pVFile, pVFile->GetSize()))
	{
		// Fehler!
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Erstellen des Effekts!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Effekt aus Ressource initialisieren
tbResult tbEffect::Init(HMODULE hModule,
					    char* pcResourceName,
					    char* pcResourceType)
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
		TB_SAFE_DELETE(pVFile);
		TB_ERROR("Fehler beim Erstellen der virtuellen Datei!", TB_ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, pVFile->GetSize()))
	{
		// Fehler!
		TB_ERROR("Fehler beim Erstellen des Effekts!", TB_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	TB_SAFE_DELETE(pVFile);

	return TB_OK;
}

// ******************************************************************
// Setzt eine Technik oder sucht die erste gültige Technik heraus (bei Angabe von -1)
tbResult tbEffect::SetTechnique(int iTechnique) // = -1
{
	D3DXHANDLE hTechnique;

	// Parameter prüfen
	if(iTechnique < -1 || iTechnique >= (int)(m_Desc.Techniques)) TB_ERROR_INVALID_VALUE("iTechnique", TB_ERROR);


	// Wenn -1 angegeben wurde, dann müssen wir die erste gültige
	// Technik suchen.
	if(iTechnique == -1)
	{
		// Alle Techniken durchgehen und überprüfen
		for(int iTest = 0; iTest < (int)(m_Desc.Techniques); iTest++)
		{
			// Die Technik abfragen und ValidateTechnique aufrufen, um sie zu prüfen
			hTechnique = m_pEffect->GetTechnique(iTest);
			if(SUCCEEDED(m_pEffect->ValidateTechnique(hTechnique)))
			{
				// Die Technik ist o.k.! Aktivieren und wir sind fertig.
				m_pEffect->SetTechnique(hTechnique);

				return TB_OK;
			}
		}

		// Keine gültige Technik gefunden! Macht aber nichts, die Techniken
		// funktionieren meistens trotzdem.
		TB_WARNING("Es wurde keine gültige Technik gefunden!");
	}
	else
	{
		// Die angegebene Technik abfragen und aktivieren
		m_pEffect->SetTechnique(m_pEffect->GetTechnique(iTechnique));
	}

	return TB_OK;
}

// ******************************************************************
// Starten eines Effekts
int tbEffect::Begin(BOOL bSaveState,		// = TRUE
					BOOL bCaptureOnFinish)	// = TRUE
{
	HRESULT			hResult;
	unsigned int	uiNumPasses;


	// Die Begin-Methode aufrufen
	if(FAILED(hResult = m_pEffect->Begin(&uiNumPasses, bSaveState ? 0 : D3DXFX_DONOTSAVESTATE)))
	{
		// Fehler!
		TB_ERROR("Fehler beim Starten des Effekts!", -1);
	}

	// Angaben kopieren
	m_bStateSaved = bSaveState;
	m_bCaptureOnFinish = bCaptureOnFinish;

	// Anzahl der Durchgänge zurückliefern
	return uiNumPasses;
}

// ******************************************************************
// Beenden eines Effekts
tbResult tbEffect::End()
{
	// Die End-Methode aufrufen
	m_pEffect->End();

	// Wenn zuvor der Status nicht gespeichert wurde, müssen die
	// Statustabellen der tbDirect3D-Klasse neu abgefragt werden,
	// falls der Benutzer das will.
	if(!m_bStateSaved && m_bCaptureOnFinish && tbDirect3D::IsInitialized()) tbDirect3D::Capture();

	return TB_OK;
}

// ******************************************************************
// Aktivieren eines Durchgangs
tbResult tbEffect::BeginPass(int iPass)
{
	// Weiterleiten...
	return FAILED(m_pEffect->BeginPass(iPass)) ? TB_ERROR : TB_OK;
}

// Deaktivieren eines Durchgangs
tbResult tbEffect::EndPass()
{
	// Weiterleiten...
	return FAILED(m_pEffect->EndPass()) ? TB_ERROR : TB_OK;
}

// ******************************************************************