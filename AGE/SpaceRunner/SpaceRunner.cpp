/*
**	Space runner
**	=========
**
**	SpaceRunner.cpp
**	--------
**	Definition allgemeiner Spielfunktionen
*/

#include "SpaceRunner.h"


// Globale Variablen
CSpaceRunner*	g_pSpaceRunner = NULL;
float*		g_pfButtons = NULL;
BOOL*		g_pbButtons = NULL;
BOOL*		g_pbOldButtons = NULL;
float*		g_pfOldButtons = NULL;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* pcCommandLine, int iShowCommand) {
	tbResult r;

	// Spiel initialisieren
	g_pSpaceRunner = new CSpaceRunner;
	r = g_pSpaceRunner->Init();
	if(r == TB_CANCELED)
	{
		// Der Konfigurationsdialog wurde abgebrochen!
		// Das Programm "leise" verlassen.
		TB_SAFE_DELETE(g_pSpaceRunner);
		return 0;
	}
	else if(r)
	{
		g_pSpaceRunner->Exit();
		TB_SAFE_DELETE(g_pSpaceRunner);
		MessageBox(NULL, "Fehler beim Initialisieren des Spiels!",
			       "Fehler", MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}

	// Spiel laufen lassen
	if(g_pSpaceRunner->Run())
	{
		g_pSpaceRunner->Exit();
		TB_SAFE_DELETE(g_pSpaceRunner);
		MessageBox(NULL, "Fehler im Spiel!",
			       "Fehler", MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}

	// Spiel verlassen
	g_pSpaceRunner->Exit();
	TB_SAFE_DELETE(g_pSpaceRunner);

	return 0;
}

// Initialisiert das Spiel komplett
tbResult CSpaceRunner::Init()
{
	tbResult r;

	// Alles zurücksetzen
	ZeroMemory(this, sizeof(CSpaceRunner));

	// Die TriBase-Engine initialisieren und den Konfigurationsdialog aufrufen
	if(tbInit()) return TB_ERROR;
	r = tbDoConfigDialog(&m_Config);
	if(r == TB_CANCELED) return TB_CANCELED;
	else if(r) TB_ERROR("Engine konnte nicht initialisiert werden!", r);

	// Laden...
	if(Load()) TB_ERROR("Fehler beim Laden des Spiels!", TB_ERROR);

	// Klassen für alle Spielzustände erstellen
	m_pIntro = new CIntro;
//	m_pMainMenu = new CMainMenu;
//	m_pGame = new CGame;

	// Schiffs- und Waffentypen laden (noch ohne Modelle)
//	m_pGame->LoadShipTypes(FALSE);
//	m_pGame->LoadWeaponTypes(FALSE);

	// Wir beginnen beim Intro!
	SetGameState(GS_INTRO);

	return TB_OK;
}

// __________________________________________________________________
// Fährt das Spiel herunter
tbResult CSpaceRunner::Exit()
{
	// Kein Spielstatus (aktuellen Spielstatus entladen)
	SetGameState(GS_NONE);

	// Entladen...
	Unload();

	// Die Klassen für die Spielzustände löschen
	TB_SAFE_DELETE(m_pIntro);
//	TB_SAFE_DELETE(m_pMainMenu);
//	TB_SAFE_DELETE(m_pGame);

	// Die Engine herunterfahren
	tbExit();

	return TB_OK;
}

// __________________________________________________________________
// Lädt das Spiel
tbResult CSpaceRunner::Load()
{
	// Direct3D initialisieren
	if(tbDirect3D::Init(&m_Config, "Space Runner", NULL, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1)))) {
		// Fehler!
		TB_ERROR("Fehler beim Initialisieren von Direct3D!", TB_ERROR);
	}

	// Statusblock für Direct3D erstellen
	tbDirect3D::GetDevice()->CreateStateBlock(D3DSBT_ALL, &m_pStateBlock);

	// Texturmanager erstellen
	if(tbTextureManager::Init())
	{
		// Fehler!
		TB_ERROR("Texturmanager konnte nicht initialisiert werden!", TB_ERROR);
	}

	// DirectInput initialisieren
	if(tbDirectInput::Init(NULL, DISCL_FOREGROUND | (m_Config.Direct3D.bWindowed ? DISCL_NONEXCLUSIVE : DISCL_EXCLUSIVE))) {
		// Fehler!
		TB_ERROR("DirectInput konnte nicht initialisiert werden!", TB_ERROR);
	}

	// Speicher für die analogen Knöpfe reservieren
	g_pfButtons = new float[tbDirectInput::GetNumButtons()];
	g_pbButtons = new BOOL[tbDirectInput::GetNumButtons()];
	g_pbOldButtons = new BOOL[tbDirectInput::GetNumButtons()];
	g_pfOldButtons = new float[tbDirectInput::GetNumButtons()];
	ZeroMemory(g_pfButtons, tbDirectInput::GetNumButtons() * sizeof(float));
	ZeroMemory(g_pbButtons, tbDirectInput::GetNumButtons() * sizeof(BOOL));
	ZeroMemory(g_pbOldButtons, tbDirectInput::GetNumButtons() * sizeof(BOOL));
	ZeroMemory(g_pfOldButtons, tbDirectInput::GetNumButtons() * sizeof(float));

	// Und nun noch DirectSound...
	if(tbDirectSound::Init(&m_Config, NULL, DSSCL_PRIORITY, TRUE)) {
		// Fehler!
		TB_ERROR("DirectSound konnte nicht initialisiert werden!", TB_ERROR);
	}

	// Dopplerfaktor auf 0.75 setzen
	tbDirectSound::GetListener()->SetDopplerFactor(0.75f, DS3D_IMMEDIATE);

	// ------------------------------------------------------------------

	// Schriftarten laden
	m_pMonotypeCorsiva18 = new tbFont(); if(m_pMonotypeCorsiva18->Init("Data\\Monotype_Corsiva_18.tga", "Data\\Monotype_Corsiva_18.tbf")) TB_ERROR("Fehler beim Laden einer Schriftart!", TB_ERROR);
//	m_pFont2 = new tbFont(); if(m_pFont2->Init("Data\\Font2.tga", "Data\\Font2.tbf")) TB_ERROR("Fehler beim Laden einer Schriftart!", TB_ERROR);

	// Musik laden
//	m_pBriefing = new tbMusic;
//	if(m_pBriefing->Init("Data\\Briefing.mp3")) TB_ERROR("Fehler beim Laden der Briefing-Musik!", TB_ERROR);
//	m_pAction = new tbMusic;
//	if(m_pAction->Init("Data\\Action.mp3")) TB_ERROR("Fehler beim Laden der Action-Musik!", TB_ERROR);

	return TB_OK;
}

// __________________________________________________________________
// Entlädt das Spiel
tbResult CSpaceRunner::Unload() {
	// Schriftarten löschen
	TB_SAFE_DELETE(m_pMonotypeCorsiva18);
//	TB_SAFE_DELETE(m_pFont2);

	// Musik löschen
//	TB_SAFE_DELETE(m_pBriefing);
//	TB_SAFE_DELETE(m_pAction);

	// DirectX-Klassen löschen
	TB_SAFE_RELEASE(m_pStateBlock);
	TB_SAFE_DELETE_ARRAY(g_pfButtons);
	TB_SAFE_DELETE_ARRAY(g_pbButtons);
	tbDirect3D::Exit();
	tbDirectInput::Exit();
	tbDirectSound::Exit();
	tbTextureManager::Exit();

	return TB_OK;
}

// __________________________________________________________________
// Move- und Render-Funktion (Kapselung)
tbResult Move(float fTime) {return g_pSpaceRunner->Move(fTime);}
tbResult Render(float fTime) {return g_pSpaceRunner->Render(fTime);}

// __________________________________________________________________
// Lässt das Spiel laufen
tbResult CSpaceRunner::Run()
{
	// Nachrichtenschleife betreten
	if(tbDoMessageLoop(::Move, ::Render))
	{
		// Fehler!
		TB_ERROR("Fehler in der Nachrichtenschleife!", TB_ERROR);
	}

	return TB_OK;
}

// __________________________________________________________________
// Setzt einen neuen Spielzustand
tbResult CSpaceRunner::SetGameState(EGameState NewGameState)
{
	tbResult r = TB_OK;

	// Alten Spielzustand entladen
	switch(m_GameState)
	{
	case GS_INTRO:		m_pIntro->Exit();			break;
//	case GS_MAIN_MENU:	m_pMainMenu->Exit();		break;
//	case GS_GAME:		m_pGame->Exit();			break;
	}

	// ------------------------------------------------------------------

	// Zeit zurücksetzen
	m_fTime = 0.0f;

	// Direct3D-Status zurücksetzen
	m_pStateBlock->Apply();
	tbDirect3D::Capture();

	// ------------------------------------------------------------------

	// Neuen Spielzustand laden
	m_GameState = NewGameState;
	switch(m_GameState)
	{
	case GS_INTRO:		r = m_pIntro->Init();		break;
//	case GS_MAIN_MENU:	r = m_pMainMenu->Init();	break;
//	case GS_GAME:		r = m_pGame->Init();		break;
	}

	// Eventuelle Fehler abfangen
	if(r) TB_ERROR("Fehler beim Laden des Spielzustands!", TB_ERROR);

	return TB_OK;
}

// __________________________________________________________________
// Bewegt das Spiel
tbResult CSpaceRunner::Move(float fTime)
{
	PDIRECT3DSURFACE9	pBackBuffer;
	char				acFilename[256] = "";
	tbResult			r = TB_OK;


	// Eingabegeräte abfragen, alten Status kopieren
	memcpy(g_pbOldButtons, g_pbButtons, tbDirectInput::GetNumButtons() * sizeof(BOOL));
	memcpy(g_pfOldButtons, g_pfButtons, tbDirectInput::GetNumButtons() * sizeof(float));
	tbDirectInput::GetState(g_pfButtons, g_pbButtons);

	// Screenshots werden mit SysRq gemacht.
/*	if(WasButtonPressed(TB_KEY_SYSRQ))
	{
		// Freien Screenshot suchen
		for(int iShot = 1; iShot < 100000; iShot++)
		{
			// Existiert dieser Screenshot schon?
			sprintf(acFilename, "Screenshot%d.bmp", iShot);
			if(!tbFileExists(acFilename)) break;
		}

		// Bildpuffer abfragen und in Datei speichern
		tbDirect3D::GetDevice()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
		D3DXSaveSurfaceToFile(acFilename, D3DXIFF_BMP, pBackBuffer, NULL, NULL);
		pBackBuffer->Release();
	}
*/
	// Aktuellen Spielzustand bewegen
	switch(m_GameState)
	{
	case GS_INTRO:		r = m_pIntro->Move(fTime);		break;
//	case GS_MAIN_MENU:	r = m_pMainMenu->Move(fTime);	break;
//	case GS_GAME:		r = m_pGame->Move(fTime);		break;
	}

	// Eventuelle Fehler abfangen
	if(r) TB_ERROR("Fehler beim Bewegen des Spielzustands!", TB_ERROR);

	// Zeit addieren
	m_fTime += fTime;

	return TB_OK;
}

// __________________________________________________________________
// Rendert das Spiel
tbResult CSpaceRunner::Render(float fTime)
{
	tbResult r = TB_OK;

	// Aktuellen Spielzustand rendern
	switch(m_GameState)
	{
	case GS_INTRO:		r = m_pIntro->Render(fTime);	break;
//	case GS_MAIN_MENU:	r = m_pMainMenu->Render(fTime);	break;
//	case GS_GAME:		r = m_pGame->Render(fTime);		break;
	}

	// Eventuelle Fehler abfangen
	if(r) TB_ERROR("Fehler beim Rendern des Spielzustands!", TB_ERROR);

	// Bildpuffer anzeigen
	if(tbDirect3D::Present())
	{
		// Anzeigen ist fehlgeschlagen!
		// Wahrscheinlich läuft das Programm im Vollbildmodus und es
		// wurde zwischenzeitlich minimiert.
		// Wir initialisieren das Spiel komplett neu.

		// Aktuellen Spielzustand entladen
		switch(m_GameState)
		{
		case GS_INTRO:		m_pIntro->Unload();		break;
//		case GS_MAIN_MENU:	m_pMainMenu->Unload();	break;
//		case GS_GAME:		m_pGame->Unload();		break;
		}

		// Das ganze Spiel entladen und dann wieder neu laden
		Unload();
		Load();

		// Aktuellen Spielstatus neu laden
		switch(m_GameState)
		{
		case GS_INTRO:		m_pIntro->Load();		break;
//		case GS_MAIN_MENU:	m_pMainMenu->Load();	break;
//		case GS_GAME:		m_pGame->Load();		break;
		}
	}

	return TB_OK;
}
