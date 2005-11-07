/*
**	GALACTICA
**	=========
**
**	MainMenu.h
**	----------
**	Das Hauptmenü
*/

// __________________________________________________________________
// Klasse für das Hauptmenü
class CMainMenu
{
public:
	// Variablen
	LPDIRECT3DTEXTURE9	m_pBackground;	// Hintergrundbild
	tbGUI*				m_pGUI;			// Benutzeroberfläche
	tbGUISkin			m_GUISkin;		// Skin für die Benutzeroberfläche

	// Konstruktor
	inline CMainMenu() {ZeroMemory(this, sizeof(CMainMenu));}

	// Methoden
	tbResult Init();				// Initialisierung
	tbResult Exit();				// Herunterfahren
	tbResult Load();				// Laden
	tbResult Unload();				// Entladen
	tbResult Move(float fTime);		// Bewegen
	tbResult Render(float fTime);	// Rendern
};

tbResult MainMenuMsgProc(int iID, tbGUIMessage* pMsg, tbGUI* pGUI);

// __________________________________________________________________