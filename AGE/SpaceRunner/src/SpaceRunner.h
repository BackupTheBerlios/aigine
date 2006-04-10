/*
**	Space Runner
**	=========
**
**	SpaceRunner.h
**	-----------
**	Definition allgemeiner Spielfunktionen
*/

#define VERSION "0.0.5-("__DATE__")-("__TIME__")"
#pragma message("Build: " VERSION)
//#define FF
//#define WITH_MUSIC

#include <TriBase.h>
#include <string.h>
#include "Ship.h"
#include "CheckPoint.h"
#include "Camera.h"
#include "Intro.h"
#include "Menu.h"
//#include "Projectile.h"
#include "DriftElement.h"
#include "Game.h"
#include "Resource.h"


// Spielzustände
enum EGameState {
	GS_NONE,		// Kein Spielzustand
	GS_INTRO,		// Intro
	GS_MENU,		// Hauptmenü
	GS_GAME			// Spiel
};

#define MSG_SPIELSTART			1004
#define MSG_SPIELENDE			1005
#define MSG_PLAYERSHIP			1006
#define MSG_SHIPS				1007
#define MSG_CONTROL				1008
#define MSG_MOVE				1009

//Network Messages
struct msg_spielstart {
	DWORD			msgid;
	int				numCheckPoints;
	int				checkPoints[64];
	tbVector3		pos[64];
	int				ships[MAX_PLAYERS];
};

struct msg_spielende {
	DWORD			msgid;
	int				winner;
};

struct msg_playerShip {
	DWORD			msgid;
	int				playerIndex;
	int				shipType;
};

struct msg_control {
	DWORD			msgid;
	int				playerIndex;
	float			throttle;
	tbVector3		steering;
};

struct msg_move {
	DWORD			msgid;
	tbVector3		m_vScaling[MAX_PLAYERS];
	tbVector3		m_vXAxis[MAX_PLAYERS];
	tbVector3		m_vYAxis[MAX_PLAYERS];
	tbVector3		m_vZAxis[MAX_PLAYERS];
	tbVector3		m_vPosition[MAX_PLAYERS];
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
	tbMusic*				m_pAction;		// Action-Musik

	int						m_clientsReady;
	//BOOL					m_serverReady;
	msg_spielstart			message_spst;
	msg_move				message_move;
	msg_control				message_control;

	

	// Methoden
	tbResult Init();								// Initialisiert das Spiel komplett
	tbResult Exit();								// Fährt das Spiel herunter
	tbResult Load();								// Lädt die Spieldaten
	tbResult Unload();								// Entlädt die Spieldaten
	tbResult Run();									// Lässt das Spiel laufen
	tbResult SetGameState(EGameState NewGameState);	// Setzt einen neuen Spielzustand
	tbResult Move(float fTime);						// Bewegt das Spiel
	tbResult Render(float fTime);					// Rendert das Spiel

	HRESULT	clientmessagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage);
	HRESULT	servermessagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage);

	void send_gameStart();
	void send_gameEnd(int winner);
	void send_playership(int ship);
//	void send_ships(CShip ships[MAX_PLAYERS]);
	void send_move();
	void send_control();
};

// Globale Variablen
extern CSpaceRunner*	g_pSpaceRunner;
extern float*		g_pfButtons;
extern BOOL*		g_pbButtons;
extern BOOL*		g_pbOldButtons;
extern float*		g_pfOldButtons;
extern int			g_Ships[MAX_PLAYERS];
extern int			g_CheckPoints[64];
extern int			g_Tunnels[64];
extern BOOL			g_bStartGame;

// Globale Funktionen
inline BOOL WasButtonPressed(int iButton)	{return !g_pbOldButtons[iButton] && g_pbButtons[iButton];}
inline BOOL WasButtonReleased(int iButton)	{return g_pbOldButtons[iButton] && !g_pbButtons[iButton];}
