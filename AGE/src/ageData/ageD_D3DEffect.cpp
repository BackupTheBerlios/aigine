/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	ageD_D3DEffect.cpp
	============
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit D3DX-Effekten

	Autor:
	David Scherfgen

********************************************************************/

#include <AGE.h>

// ******************************************************************
// Globale Variablen
LPD3DXEFFECTPOOL age_g_pEffectPool = NULL;	// Globaler Effektpool

// ******************************************************************
// Konstruktor der ageD_D3DEffect-Klasse
ageD_D3DEffect::ageD_D3DEffect()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(ageD_D3DEffect));
}

// ******************************************************************
// Destruktor der ageD_D3DEffect-Klasse
ageD_D3DEffect::~ageD_D3DEffect()
{
	// Effekt löschen
	AGE_SAFE_RELEASE(m_pEffect);
}

// ******************************************************************
// Erstellen aus einem String
age_Result ageD_D3DEffect::Init(char* pcCode,
						int iSize)
{
	HRESULT hResult;

	// Parameter prüfen und sicherstellen, dass Direct3D initialisiert wurde
	if(pcCode == NULL)					AGE_ERROR_NULL_POINTER("pcCode", AGE_ERROR);
	if(iSize == 0 || iSize < -1)		AGE_ERROR_INVALID_VALUE("iSize", AGE_ERROR);
	if(!ageG_Direct3D::IsInitialized())	AGE_ERROR("Direct3D wurde noch nicht initialisiert!", AGE_ERROR);


	// Länge anpassen
	if(iSize == -1) iSize = strlen(pcCode);

	// Jetzt den Effekt erstellen
	if(FAILED(hResult = D3DXCreateEffect(ageG_Direct3D::GetDevice(),
										 pcCode, iSize, NULL, NULL, 0,
										 age_g_pEffectPool, &m_pEffect, NULL)))
	{
		// Fehler!
		AGE_ERROR_DIRECTX("D3DXCreateEffect", hResult, AGE_ERROR);
	}

	// Effektbeschreibung abfragen
	m_pEffect->GetDesc(&m_Desc);

	// Die erste gültige Technik setzen
	SetTechnique(-1);

	return AGE_OK;
}

// ******************************************************************
// Erstellen aus einer virtuellen Datei
age_Result ageD_D3DEffect::Init(ageD_VFile* pVFile,
						int iSize)
{
	// Parameter prüfen und sicherstellen, dass Direct3D initialisiert wurde
	if(pVFile == NULL)				AGE_ERROR_NULL_POINTER("pVFile", AGE_ERROR);
	if(iSize == 0 || iSize < -1)	AGE_ERROR_INVALID_VALUE("iSize", AGE_ERROR);


	// Länge anpassen
	if(iSize == -1) iSize = pVFile->GetSize();

	// Die andere Methode aufrufen
	if(Init((char*)(pVFile->GetBuffer()) + pVFile->GetCursor(), iSize))
	{
		// Fehler!
		AGE_ERROR("Fehler beim Erstellen des Effekts!", AGE_ERROR);
	}

	return AGE_OK;
}

// ******************************************************************
// Erstellen aus einer echten Datei
age_Result ageD_D3DEffect::Init(char* pcFilename)
{
	ageD_VFile* pVFile;

	// Parameter prüfen
	if(pcFilename == NULL) AGE_ERROR_NULL_POINTER("pcFilename", AGE_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new ageD_VFile;
	if(pVFile == NULL) AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);

	// Aus Datei laden
	if(pVFile->Init(pcFilename))
	{
		// Fehler!
		AGE_ERROR("Fehler beim Erstellen der virtuellen Datei!", AGE_ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, pVFile->GetSize()))
	{
		// Fehler!
		AGE_ERROR("Fehler beim Erstellen des Effekts!", AGE_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	AGE_SAFE_DELETE(pVFile);

	return AGE_OK;
}

// ******************************************************************
// Aus Speicherbereich initialisieren
age_Result ageD_D3DEffect::Init(void* pMemory,
					    int iMemorySize)
{
	ageD_VFile* pVFile;

	// Parameter prüfen
	if(pMemory == NULL)		AGE_ERROR_NULL_POINTER("pMemory", AGE_ERROR);
	if(iMemorySize <= 0)	AGE_ERROR_INVALID_VALUE("iMemorySize", AGE_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new ageD_VFile;
	if(pVFile == NULL) AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);

	// Aus Speicher laden
	if(pVFile->Init(pMemory, iMemorySize))
	{
		// Fehler!
		AGE_ERROR("Fehler beim Erstellen der virtuellen Datei!", AGE_ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, pVFile->GetSize()))
	{
		// Fehler!
		AGE_SAFE_DELETE(pVFile);
		AGE_ERROR("Fehler beim Erstellen des Effekts!", AGE_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	AGE_SAFE_DELETE(pVFile);

	return AGE_OK;
}

// ******************************************************************
// Effekt aus Ressource initialisieren
age_Result ageD_D3DEffect::Init(HMODULE hModule,
					    char* pcResourceName,
					    char* pcResourceType)
{
	ageD_VFile* pVFile;

	// Parameter prüfen
	if(hModule == NULL)			AGE_ERROR_NULL_POINTER("hModule", AGE_ERROR);
	if(pcResourceName == NULL)	AGE_ERROR_NULL_POINTER("pcResourceName", AGE_ERROR);
	if(pcResourceType == NULL)	AGE_ERROR_NULL_POINTER("pcResourceType", AGE_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new ageD_VFile;
	if(pVFile == NULL) AGE_ERROR_OUT_OF_MEMORY(AGE_ERROR);

	// Aus Ressource laden
	if(pVFile->Init(hModule, pcResourceName, pcResourceType))
	{
		// Fehler!
		AGE_SAFE_DELETE(pVFile);
		AGE_ERROR("Fehler beim Erstellen der virtuellen Datei!", AGE_ERROR);
	}

	// Die andere Methode aufrufen
	if(Init(pVFile, pVFile->GetSize()))
	{
		// Fehler!
		AGE_ERROR("Fehler beim Erstellen des Effekts!", AGE_ERROR);
	}

	// Die virtuelle Datei wieder freigeben
	AGE_SAFE_DELETE(pVFile);

	return AGE_OK;
}

// ******************************************************************
// Setzt eine Technik oder sucht die erste gültige Technik heraus (bei Angabe von -1)
age_Result ageD_D3DEffect::SetTechnique(int iTechnique) // = -1
{
	D3DXHANDLE hTechnique;

	// Parameter prüfen
	if(iTechnique < -1 || iTechnique >= (int)(m_Desc.Techniques)) AGE_ERROR_INVALID_VALUE("iTechnique", AGE_ERROR);


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

				return AGE_OK;
			}
		}

		// Keine gültige Technik gefunden! Macht aber nichts, die Techniken
		// funktionieren meistens trotzdem.
		AGE_WARNING("Es wurde keine gültige Technik gefunden!");
	}
	else
	{
		// Die angegebene Technik abfragen und aktivieren
		m_pEffect->SetTechnique(m_pEffect->GetTechnique(iTechnique));
	}

	return AGE_OK;
}

// ******************************************************************
// Starten eines Effekts
int ageD_D3DEffect::Begin(BOOL bSaveState,		// = TRUE
					BOOL bCaptureOnFinish)	// = TRUE
{
	HRESULT			hResult;
	unsigned int	uiNumPasses;


	// Die Begin-Methode aufrufen
	if(FAILED(hResult = m_pEffect->Begin(&uiNumPasses,
		                                 bSaveState ? 0 : D3DXFX_DONOTSAVESTATE)))
	{
		// Fehler!
		AGE_ERROR("Fehler beim Starten des Effekts!", -1);
	}

	// Angaben kopieren
	m_bStateSaved = bSaveState;
	m_bCaptureOnFinish = bCaptureOnFinish;

	// Anzahl der Durchgänge zurückliefern
	return uiNumPasses;
}

// ******************************************************************
// Beenden eines Effekts
age_Result ageD_D3DEffect::End()
{
	// Die End-Methode aufrufen
	m_pEffect->End();

	// Wenn zuvor der Status nicht gespeichert wurde, müssen die
	// Statustabellen der ageG_Direct3D-Klasse neu abgefragt werden,
	// falls der Benutzer das will.
	if(!m_bStateSaved && m_bCaptureOnFinish && ageG_Direct3D::IsInitialized()) ageG_Direct3D::Capture();

	return AGE_OK;
}

// ******************************************************************
// Aktivieren eines Durchgangs
age_Result ageD_D3DEffect::Pass(int iPass)
{
	// Weiterleiten...
	return FAILED(m_pEffect->Pass(iPass)) ? AGE_ERROR : AGE_OK;
}

// ******************************************************************