/*
**	GALACTICA
**	=========
**
**	Intro.h
**	-------
**	Der Vorspann
*/

// __________________________________________________________________
// Klasse für das Intro
class CIntro
{
public:
	// Variablen
	PDIRECT3DTEXTURE9 m_pTitle;	// Titelbild

	// Konstruktor
	inline CIntro() {ZeroMemory(this, sizeof(CIntro));}

	// Methoden
	tbResult Init();				// Initialisierung
	tbResult Exit();				// Herunterfahren
	tbResult Load();				// Laden
	tbResult Unload();				// Entladen
	tbResult Move(float fTime);		// Bewegen
	tbResult Render(float fTime);	// Rendern
};

// __________________________________________________________________