/*
**	Space Runner
**	=========
**
**	SpaceRunner.h
**	-----------
**	Definition allgemeiner Spielfunktionen
*/

#define VERSION "0.0.2"

#include <TriBase.h>
#include <string.h>
//#include "Ship.h"
#include "Intro.h"
#include "Menu.h"
//#include "Projectile.h"
#include "Game.h"
#include "Resource.h"
#include "Camera.h"


// Spielzustände
enum EGameState {
	GS_NONE,		// Kein Spielzustand
	GS_INTRO,		// Intro
	GS_MENU,		// Hauptmenü
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
	CIntro*					m_pIntro;		// Intro
	CMenu*					m_pMenu;		// Hauptmenü
	CGame*					m_pGame;		// Spiel
	EGameState				m_GameState;	// Aktueller Spielzustand
	float					m_fTime;		// Stoppuhr

	// Schriftarten
	tbFont*					m_pCourier_New_6_12;		// Erste Schriftart
	tbFont*					m_pCourier_New_6_18;		// Zweite Schriftart

	// Musik
	tbMusic*				m_pTitle;	// title-Musik
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
extern float*		g_pfButtons;
extern BOOL*		g_pbButtons;
extern BOOL*		g_pbOldButtons;
extern float*		g_pfOldButtons;
extern int			g_Ships[32];

// Globale Funktionen
inline BOOL WasButtonPressed(int iButton)	{return !g_pbOldButtons[iButton] && g_pbButtons[iButton];}
inline BOOL WasButtonReleased(int iButton)	{return g_pbOldButtons[iButton] && !g_pbButtons[iButton];}
