/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbEffect.h
	==========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Umgang mit D3DX-Effekten

	Autor:
	David Scherfgen

********************************************************************/

// Globale Variablen
extern LPD3DXEFFECTPOOL tb_g_pEffectPool; // Der globale Effektpool

// ******************************************************************
// Klasse für Effekte
class ageD_D3DEffect
{
private:
	// Variablen
	LPD3DXEFFECT	m_pEffect;			// Die Effektschnittstelle
	D3DXEFFECT_DESC	m_Desc;				// Effektbeschreibung
	BOOL			m_bStateSaved;		// Wurde der Status gespeichert?
	BOOL			m_bCaptureOnFinish;	// Capture nach dem Effekt aufrufen?

public:
	// Konstruktor und Destruktor
	ageD_D3DEffect();
	~ageD_D3DEffect();

	// Methoden
	age_Result	Init(char* pcCode, int iSize);										// Aus String erstellen
	age_Result	Init(tbVFile* pVFile, int iSize);									// Aus virtueller Datei initialisieren
	age_Result	Init(char* pcFilename);												// Aus Datei initialisieren
	age_Result	Init(void* pMemory, int iMemorySize);								// Aus Speicher initialisieren
	age_Result	Init(HMODULE hModule, char* pcResourceName, char* pcResourceType);	// Aus Ressource initialisieren
	age_Result	SetTechnique(int iTechnique = -1);									// Wählt eine Technik
	int			Begin(BOOL bSaveState = TRUE, BOOL bCaptureOnFinish = TRUE);		// Effekt starten
	age_Result	End();																// Effekt beenden
	age_Result	Pass(int iPass);													// Durchgang aktivieren

	// Inline-Methoden
	inline LPD3DXEFFECT	GetEffect()			{return m_pEffect;}
	inline int			GetNumTechniques()	{return m_Desc.Techniques;}
};

// ******************************************************************