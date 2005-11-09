/*
**	Space Runner
**	=========
**
**	SpaceRunner.h
**	-----------
**	Definition allgemeiner Spielfunktionen
*/

#include <TriBase.h>
//#include "Ship.h"
//#include "Intro.h"
//#include "MainMenu.h"
//#include "Projectile.h"
//#include "Game.h"
#include "Resource.h"


// Spielzustände
enum EGameState {
	GS_NONE,		// Kein Spielzustand
	GS_INTRO,		// Intro
	GS_MAIN_MENU,	// Hauptmenü
	GS_GAME			// Spiel
};

// CGalactica-Klasse
class CSpaceRunner {
public:
	// Variablen
	// DirectX
	tbConfig				m_Config;		// Konfiguration
	PDIRECT3DSTATEBLOCK9	m_pStateBlock;	// Statusblock für Direct3D

	// Die Spielzustände
//	CIntro*					m_pIntro;		// Intro
//	CMainMenu*				m_pMainMenu;	// Hauptmenü
//	CGame*					m_pGame;		// Spiel
	EGameState				m_GameState;	// Aktueller Spielzustand
	float					m_fTime;		// Stoppuhr

	// Schriftarten
//	tbFont*					m_pFont1;		// Erste Schriftart
//	tbFont*					m_pFont2;		// Zweite Schriftart

	// Musik
//	tbMusic*				m_pBriefing;	// Briefing-Musik
//	tbMusic*				m_pAction;		// Action-Musik

	// Methoden
	tbResult Init();								// Initialisiert das Spiel komplett
	tbResult Exit();								// Fährt das Spiel herunter
	tbResult Load();								// Lädt die Spieldaten
	tbResult Unload();								// Entlädt die Spieldaten
	tbResult Run();									// Lässt das Spiel laufen
	tbResult SetGameState(EGameState NewGameState);	// Setzt einen neuen Spielzustand
	tbResult Move(float fTime);						// Bewegt das Spiel
	tbResult Render(float fTime);					// Rendert das Spiel
};

// Globale Variablen
extern CSpaceRunner*	g_pSpaceRunner;
//extern float*		g_pfButtons;
//extern BOOL*		g_pbButtons;
//extern BOOL*		g_pbOldButtons;
//extern float*		g_pfOldButtons;
//extern int			g_aiTeam[4][32];
