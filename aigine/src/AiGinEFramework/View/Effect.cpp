// Effect.cpp: implementation of the Effect class.
//
//////////////////////////////////////////////////////////////////////

#include "../AiGinE.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Globale Variablen
LPD3DXEFFECTPOOL g_pEffectPool = NULL;	// Globaler Effektpool

// ******************************************************************
// Konstruktor der Effect-Klasse
Effect::Effect()
{
	// Alles zurücksetzen
	ZeroMemory(this, sizeof(Effect));
}

// ******************************************************************
// Destruktor der Effect-Klasse
Effect::~Effect()
{
	// Effekt löschen
	AGE_SAFE_RELEASE(m_pEffect);
}

// ******************************************************************
// Erstellen aus einem String
Result Effect::Init(char* pcCode,
						int iSize)
{
	HRESULT hResult;

	// Parameter prüfen und sicherstellen, dass Direct3D initialisiert wurde
	if(pcCode == NULL)					AGE_ERROR_NULL_POINTER("pcCode", AGE_ERROR);
	if(iSize == 0 || iSize < -1)		AGE_ERROR_INVALID_VALUE("iSize", AGE_ERROR);
	if(!Direct3D::IsInitialized())	AGE_ERROR("Direct3D wurde noch nicht initialisiert!", AGE_ERROR);


	// Länge anpassen
	if(iSize == -1) iSize = strlen(pcCode);

	// Jetzt den Effekt erstellen
	if(FAILED(hResult = D3DXCreateEffect(Direct3D::GetDevice(),
										 pcCode, iSize, NULL, NULL, 0,
										 g_pEffectPool, &m_pEffect, NULL)))
	{
		// Fehler!
		AGE_ERROR_DIRECTX("D3DXCreateEffect", hResult, AGE_ERROR);
	}

	// Effekeschreibung abfragen
	m_pEffect->GetDesc(&m_Desc);

	// Die erste gültige Technik setzen
	SetTechnique(-1);

	return AGE_OK;
}

// ******************************************************************
// Erstellen aus einer virtuellen Datei
Result Effect::Init(VFile* pVFile,
						int iSize)
{
	// Parameter prüfen und sicherstellen, dass Direct3D initialisiert wurde
	if(pVFile == NULL)				AGE_ERROR_NULL_POINTER("pVFile", AGE_ERROR);
	if(iSize == 0 || iSize < -1)	AGE_ERROR_INVALID_VALUE("iSize", AGE_ERROR);


	// Länge anpassen
	if(iSize == -1) iSize = pVFile->GetSize();

	// Die andere Methode aufrufen
	if(Init((char*)(pVFile->Geuffer()) + pVFile->GetCursor(), iSize))
	{
		// Fehler!
		AGE_ERROR("Fehler beim Erstellen des Effekts!", AGE_ERROR);
	}

	return AGE_OK;
}

// ******************************************************************
// Erstellen aus einer echten Datei
Result Effect::Init(char* pcFilename)
{
	VFile* pVFile;

	// Parameter prüfen
	if(pcFilename == NULL) AGE_ERROR_NULL_POINTER("pcFilename", AGE_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new VFile;
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
Result Effect::Init(void* pMemory,
					    int iMemorySize)
{
	VFile* pVFile;

	// Parameter prüfen
	if(pMemory == NULL)		AGE_ERROR_NULL_POINTER("pMemory", AGE_ERROR);
	if(iMemorySize <= 0)	AGE_ERROR_INVALID_VALUE("iMemorySize", AGE_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new VFile;
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
Result Effect::Init(HMODULE hModule,
					    char* pcResourceName,
					    char* pcResourceType)
{
	VFile* pVFile;

	// Parameter prüfen
	if(hModule == NULL)			AGE_ERROR_NULL_POINTER("hModule", AGE_ERROR);
	if(pcResourceName == NULL)	AGE_ERROR_NULL_POINTER("pcResourceName", AGE_ERROR);
	if(pcResourceType == NULL)	AGE_ERROR_NULL_POINTER("pcResourceType", AGE_ERROR);


	// Virtuelle Datei erstellen
	pVFile = new VFile;
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
Result Effect::SetTechnique(int iTechnique) // = -1
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
int Effect::Begin(BOOL bSaveState,		// = TRUE
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
Result Effect::End()
{
	// Die End-Methode aufrufen
	m_pEffect->End();

	// Wenn zuvor der Status nicht gespeichert wurde, müssen die
	// Statustabellen der Direct3D-Klasse neu abgefragt werden,
	// falls der Benutzer das will.
	if(!m_bStateSaved && m_bCaptureOnFinish && Direct3D::IsInitialized()) Direct3D::Capture();

	return AGE_OK;
}

// ******************************************************************
// Aktivieren eines Durchgangs
Result Effect::Pass(int iPass)
{
	// Weiterleiten...
	return FAILED(m_pEffect->Pass(iPass)) ? AGE_ERROR : AGE_OK;
}
