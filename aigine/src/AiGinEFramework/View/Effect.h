// Effect.h: interface for the Effect class.
//
//////////////////////////////////////////////////////////////////////

#ifndef EFFECT_H
#define EFFECT_H

#include "../AiGinE.h"

// Globale Variablen
extern LPD3DXEFFECTPOOL g_pEffectPool; // Der globale Effektpool

// ******************************************************************
// Klasse für Effekte
class Effect
{
private:
	// Variablen
	LPD3DXEFFECT	m_pEffect;			// Die Effektschnittstelle
	D3DXEFFECT_DESC	m_Desc;				// Effekeschreibung
	BOOL			m_bStateSaved;		// Wurde der Status gespeichert?
	BOOL			m_bCaptureOnFinish;	// Capture nach dem Effekt aufrufen?

public:
	// Konstruktor und Destruktor
	Effect();
	~Effect();

	// Methoden
	Result	Init(char* pcCode, int iSize);										// Aus String erstellen
	Result	Init(VFile* pVFile, int iSize);									// Aus virtueller Datei initialisieren
	Result	Init(char* pcFilename);												// Aus Datei initialisieren
	Result	Init(void* pMemory, int iMemorySize);								// Aus Speicher initialisieren
	Result	Init(HMODULE hModule, char* pcResourceName, char* pcResourceType);	// Aus Ressource initialisieren
	Result	SetTechnique(int iTechnique = -1);									// Wählt eine Technik
	int			Begin(BOOL bSaveState = TRUE, BOOL bCaptureOnFinish = TRUE);		// Effekt starten
	Result	End();																// Effekt beenden
	Result	Pass(int iPass);													// Durchgang aktivieren

	// Inline-Methoden
	inline LPD3DXEFFECT	GetEffect()			{return m_pEffect;}
	inline int			GetNumTechniques()	{return m_Desc.Techniques;}
};

#endif //EFFECT_H
