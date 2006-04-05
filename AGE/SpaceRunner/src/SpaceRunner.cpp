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
int			g_Ships[32];
int			g_CheckPoints[64];
int			g_Tunnels[64];
HRESULT (* clientCallFunc)(PVOID, DWORD, PVOID);
HRESULT (* serverCallFunc)(PVOID, DWORD, PVOID);




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* pcCommandLine, int iShowCommand) {
	tbResult r;

	// Spiel initialisieren
	g_pSpaceRunner = new CSpaceRunner;
	r = g_pSpaceRunner->Init();
	if(r == TB_CANCELED)
	{
		// Der Konfigurationsdialog wurde abgebrochen!
		// Das Programm "leise" verlassen.
		//g_pSpaceRunner->Exit();
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
//	::callFunc = ::clientmessagehandler;
	if(r == TB_CANCELED) return TB_CANCELED;
	else if(r) TB_ERROR("Engine konnte nicht initialisiert werden!", r);

	// Laden...
	if(Load()) TB_ERROR("Fehler beim Laden des Spiels!", TB_ERROR);

	// Klassen für alle Spielzustände erstellen
	m_pIntro = new CIntro;
	m_pMenu = new CMenu;
	m_pGame = new CGame;

	// Schiffs- und Waffentypen laden (noch ohne Modelle)
	m_pGame->LoadShipTypes(FALSE);
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
	TB_SAFE_DELETE(m_pMenu);
	TB_SAFE_DELETE(m_pGame);

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
	m_pCourier_New_6_12 = new tbFont(); if(m_pCourier_New_6_12->Init("Data\\Courier_New_6_12.tga", "Data\\Courier_New_6_12.tbf")) TB_ERROR("Fehler beim Laden einer Schriftart!", TB_ERROR);
	m_pCourier_New_6_18 = new tbFont(); if(m_pCourier_New_6_18->Init("Data\\Courier_New_6_18.tga", "Data\\Courier_New_6_18.tbf")) TB_ERROR("Fehler beim Laden einer Schriftart!", TB_ERROR);

	// Musik laden
	m_pTitle = new tbMusic;
	if(m_pTitle->Init("Data\\2-05CandyLove.mp3")) TB_ERROR("Fehler beim Laden der Titel-Musik!", TB_ERROR);
	m_pAction = new tbMusic;
	if(m_pAction->Init("Data\\05BurnBurn.mp3")) TB_ERROR("Fehler beim Laden der Action-Musik!", TB_ERROR);

	m_clientsReady = 0;

	return TB_OK;
}

// __________________________________________________________________
// Entlädt das Spiel
tbResult CSpaceRunner::Unload() {
	// Schriftarten löschen
	TB_SAFE_DELETE(m_pCourier_New_6_12);
	TB_SAFE_DELETE(m_pCourier_New_6_18);

	// Musik löschen
	TB_SAFE_DELETE(m_pTitle);
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
HRESULT clientmessagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage) {return g_pSpaceRunner->clientmessagehandler(pvUserContext,dwMessageType,pMessage);}
HRESULT servermessagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage) {return g_pSpaceRunner->servermessagehandler(pvUserContext,dwMessageType,pMessage);}


// __________________________________________________________________
// Lässt das Spiel laufen
tbResult CSpaceRunner::Run()
{
	// Nachrichtenschleife betreten
	if(tbDoMessageLoop(::Move, ::Render, ::clientmessagehandler, ::servermessagehandler))
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
	case GS_MENU:		m_pMenu->Exit();			break;
	case GS_GAME:		m_pGame->Exit();			break;
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
	case GS_MENU:		r = m_pMenu->Init();		break;
	case GS_GAME:		r = m_pGame->Init();		break;
	}

	// Eventuelle Fehler abfangen
	if(r) TB_ERROR("Fehler beim Laden des Spielzustands!", TB_ERROR);

	return TB_OK;
}

// __________________________________________________________________
// Bewegt das Spiel
tbResult CSpaceRunner::Move(float fTime)
{
//	PDIRECT3DSURFACE9	pBackBuffer;
	char				acFilename[256] = "";
	tbResult			r = TB_OK;


	// Eingabegeräte abfragen, alten Status kopieren
	memcpy(g_pbOldButtons, g_pbButtons, tbDirectInput::GetNumButtons() * sizeof(BOOL));
	memcpy(g_pfOldButtons, g_pfButtons, tbDirectInput::GetNumButtons() * sizeof(float));
	tbDirectInput::GetState(g_pfButtons, g_pbButtons);

	// Aktuellen Spielzustand bewegen
	switch(m_GameState)
	{
	case GS_INTRO:		r = m_pIntro->Move(fTime);		break;
	case GS_MENU:		r = m_pMenu->Move(fTime);		break;
	case GS_GAME:		r = m_pGame->Move(fTime);		break;
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
	case GS_MENU:		r = m_pMenu->Render(fTime);		break;
	case GS_GAME:		r = m_pGame->Render(fTime);		break;
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
		case GS_MENU:		m_pMenu->Unload();		break;
		case GS_GAME:		m_pGame->Unload();		break;
		}

		// Das ganze Spiel entladen und dann wieder neu laden
		Unload();
		Load();

		// Aktuellen Spielstatus neu laden
		switch(m_GameState)
		{
		case GS_INTRO:		m_pIntro->Load();		break;
		case GS_MENU:		m_pMenu->Load();		break;
		case GS_GAME:		m_pGame->Load();		break;
		}
	}

	return TB_OK;
}

void CSpaceRunner::send_gameStart() {
	DPN_BUFFER_DESC bdsc;
//    DPNHANDLE async;
	tbServer::lock();

	if( !tbServer::server) return;
	bdsc.dwBufferSize = sizeof( message_spst);
	bdsc.pBufferData = (BYTE*)&message_spst;

	tbServer::server->SendTo( DPNID_ALL_PLAYERS_GROUP, &bdsc, 1, 0, NULL, 0, DPNSEND_SYNC|DPNSEND_GUARANTEED|DPNSEND_NOLOOPBACK);

	tbServer::unlock();
}

void CSpaceRunner::send_gameEnd(int winner) {
	DPN_BUFFER_DESC bdsc;
    DPNHANDLE async;

	msg_spielende m;

	m.msgid = MSG_SPIELENDE;
	m.winner = winner;
	bdsc.dwBufferSize = sizeof(msg_spielende);
	bdsc.pBufferData = (BYTE*) &m;

	tbServer::server->SendTo(DPNID_ALL_PLAYERS_GROUP, &bdsc, 1, 0, NULL, &async, DPNSEND_GUARANTEED|DPNSEND_NOLOOPBACK);
}

void CSpaceRunner::send_playership(int ship) {
	msg_playerShip ps;
    DPN_BUFFER_DESC bdsc;
//    DPNHANDLE async;

	ps.msgid = MSG_PLAYERSHIP;
	ps.playerIndex = (int)tbClient::index;
	ps.shipType = ship;

    bdsc.dwBufferSize = sizeof( msg_playerShip);
    bdsc.pBufferData  = (BYTE*) &ps;

	tbClient::client->Send( &bdsc, 1, 0, 0, 0, DPNSEND_SYNC|DPNSEND_GUARANTEED);

}

void CSpaceRunner::send_move() {
	DPN_BUFFER_DESC bdsc;
    DPNHANDLE async;

	message_move.msgid = MSG_MOVE;

	for(int i = 0; i < 32; i++) { 
		message_move.ships[i] = m_pGame->m_aShip[i];
	}
	for(int j = 0; j < 64; j++) {
        message_move.checkPoints[j] = m_pGame->m_aCheckPoint[j];
	}

	tbServer::lock();
	bdsc.dwBufferSize = sizeof(message_move);
	bdsc.pBufferData = (BYTE*) &message_move;

	tbServer::server->SendTo(DPNID_ALL_PLAYERS_GROUP, &bdsc, 1, 0, NULL, &async, DPNSEND_NOLOOPBACK);

	tbServer::unlock();
}
void CSpaceRunner::send_control() {
	DPN_BUFFER_DESC bdsc;
    DPNHANDLE async;

	message_control.msgid = MSG_CONTROL;
	message_control.playerIndex = (int)tbClient::index;

	bdsc.dwBufferSize = sizeof(message_control);
	bdsc.pBufferData = (BYTE*) &message_control;

	tbClient::client->Send(&bdsc, 1, 0, 0, &async, NULL/* DPNSEND_NOLOOPBACK*/ );
}

HRESULT CSpaceRunner::clientmessagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage) {
	tbClient::lock();
    switch( dwMessageType) {
    case DPN_MSGID_RECEIVE:
        PBYTE rd = ((PDPNMSG_RECEIVE)pMessage)->pReceiveData;
        switch( NETWORK_MSGID( rd)) {
		case MSG_SPIELSTART:
			TB_INFO("MSG_SPIELSTART geschickt");
			//CheckPoints erstellen
			if(tbServer::status != SERVER_GESTARTET) {
				if(m_pGame->Load()) TB_ERROR("Fehler beim Laden des Spielzustands!", TB_ERROR);
				int iCP;
				tbVector3 p;
				for (int i = 0; i < ((msg_spielstart*)rd)->numCheckPoints; i++) {
					iCP = m_pGame->CreateCheckPoint(((msg_spielstart*)rd)->checkPoints[i]);
					p = ((msg_spielstart*)rd)->pos[i];
					m_pGame->m_aCheckPoint[iCP].SetPosition(p);
				}
				for(int j = 0; j < 32; j++) {
					g_Ships[j] = ((msg_spielstart*)rd)->ships[j];
				}
				g_bStartGame = TRUE;
			}
			break;
		case MSG_SPIELENDE:
			TB_INFO("MSG_SPIELENDE geschickt");
			g_pSpaceRunner->SetGameState(GS_MENU);
			break;
		case MSG_MOVE:
			TB_INFO("MSG_MOVE geschickt");
			if(tbServer::status != SERVER_GESTARTET) {
				for( int i = 0; i < 64; i++) {
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_bExists = ((msg_move*)rd)->checkPoints[i].m_bExists;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_fMass = ((msg_move*)rd)->checkPoints[i].m_fMass;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_fMovementFriction = ((msg_move*)rd)->checkPoints[i].m_fMovementFriction;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_fRadius = ((msg_move*)rd)->checkPoints[i].m_fRadius;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_fRotationFriction = ((msg_move*)rd)->checkPoints[i].m_fRotationFriction;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_iIndex = ((msg_move*)rd)->checkPoints[i].m_iIndex;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_isActive = ((msg_move*)rd)->checkPoints[i].m_isActive;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_mInvMatrix = ((msg_move*)rd)->checkPoints[i].m_mInvMatrix;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_mMatrix = ((msg_move*)rd)->checkPoints[i].m_mMatrix;

//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_vPosition = ((msg_move*)rd)->checkPoints[i].m_vPosition;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_vRotation = ((msg_move*)rd)->checkPoints[i].m_vRotation;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_vScaling = ((msg_move*)rd)->checkPoints[i].m_vScaling;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_vVelocity = ((msg_move*)rd)->checkPoints[i].m_vVelocity;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_vXAxis = ((msg_move*)rd)->checkPoints[i].m_vXAxis;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_vYAxis = ((msg_move*)rd)->checkPoints[i].m_vYAxis;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].m_vZAxis = ((msg_move*)rd)->checkPoints[i].m_vZAxis;
//					g_pSpaceRunner->m_pGame->m_aCheckPoint[i].Update();
				}
				for( int j = 0; j < 32; j++) {
					//g_pSpaceRunner->m_pGame->m_aShip[j] = ((msg_move*)rd)->ships[j];
					g_pSpaceRunner->m_pGame->m_aShip[j].m_bExists = ((msg_move*)rd)->ships[j].m_bExists;
//					g_pSpaceRunner->m_pGame->m_aShip[j].m_fMass = ((msg_move*)rd)->ships[j].m_fMass;
//					g_pSpaceRunner->m_pGame->m_aShip[j].m_fMovementFriction = ((msg_move*)rd)->ships[j].m_fMovementFriction;
//					g_pSpaceRunner->m_pGame->m_aShip[j].m_fRadius = ((msg_move*)rd)->ships[j].m_fRadius;
//					g_pSpaceRunner->m_pGame->m_aShip[j].m_fRotationFriction = ((msg_move*)rd)->ships[j].m_fRotationFriction;
//					g_pSpaceRunner->m_pGame->m_aShip[j].m_iIndex = ((msg_move*)rd)->ships[j].m_iIndex;
//					g_pSpaceRunner->m_pGame->m_aShip[j].m_mInvMatrix = ((msg_move*)rd)->ships[j].m_mInvMatrix;
//					g_pSpaceRunner->m_pGame->m_aShip[j].m_mMatrix = ((msg_move*)rd)->ships[j].m_mMatrix;



	//				g_pSpaceRunner->m_pGame->m_aShip[j].m_vPosition = ((msg_move*)rd)->ships[j].m_vPosition;
					g_pSpaceRunner->m_pGame->m_aShip[j].m_vRotation = ((msg_move*)rd)->ships[j].m_vRotation;
	//				g_pSpaceRunner->m_pGame->m_aShip[j].m_vScaling = ((msg_move*)rd)->ships[j].m_vScaling;
					g_pSpaceRunner->m_pGame->m_aShip[j].m_vVelocity = ((msg_move*)rd)->ships[j].m_vVelocity;
	//				g_pSpaceRunner->m_pGame->m_aShip[j].m_vXAxis = ((msg_move*)rd)->ships[j].m_vXAxis;
	//				g_pSpaceRunner->m_pGame->m_aShip[j].m_vYAxis = ((msg_move*)rd)->ships[j].m_vYAxis;
	//				g_pSpaceRunner->m_pGame->m_aShip[j].m_vZAxis = ((msg_move*)rd)->ships[j].m_vZAxis;
					//g_pSpaceRunner->m_pGame->m_aShip[j].AddVelocityRel(tbVector3(0.0f, 0.0f, g_pSpaceRunner->m_pGame->m_aShip[j].m_fThrottle * g_pSpaceRunner->m_pGame->m_aShip[j].m_pType->fMaxAccel * g_pSpaceRunner->m_pGame->m_aShip[j].m_fEngineEfficiency * m_fTime));
					//g_pSpaceRunner->m_pGame->m_aShip[j].AddRotationRel(g_pSpaceRunner->m_pGame->m_aShip[j].m_vSteering * TB_DEG_TO_RAD(g_pSpaceRunner->m_pGame->m_aShip[j].m_pType->fMaxAngularAccel) * g_pSpaceRunner->m_pGame->m_aShip[j].m_fEngineEfficiency * m_fTime);
					g_pSpaceRunner->m_pGame->m_aShip[j].Move(this->m_fTime);
				}
			}
			break;

		}
        break;
	}
	tbClient::unlock();
    return S_OK;
}

HRESULT CSpaceRunner::servermessagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage) {
	//TB_WARNING("servermassegehandler aufgerufen...");
	tbServer::lock();
    switch( dwMessageType) {
	case DPN_MSGID_RECEIVE:

        PBYTE rd = ((PDPNMSG_RECEIVE)pMessage)->pReceiveData;
        switch( NETWORK_MSGID( rd)) {
		case MSG_PLAYERSHIP:
			TB_INFO("MSG_PLAYERSHIP geschickt");
			g_Ships[((msg_playerShip*)rd)->playerIndex] = (int)((msg_playerShip*)rd)->shipType;
			m_clientsReady++;
			break;
        case MSG_CONTROL:
			TB_INFO("MSG_CONTROL geschickt");
			if(tbServer::status == SERVER_GESTARTET && tbClient::index != ((msg_playerShip*)rd)->playerIndex) {
				TB_WARNING("MSG_CONTROL von Spieler erhalten");
				m_pGame->m_aShip[((msg_playerShip*)rd)->playerIndex].m_vSteering = (tbVector3)((msg_control*)rd)->steering;
				m_pGame->m_aShip[((msg_playerShip*)rd)->playerIndex].m_fThrottle = - (float)((msg_control*)rd)->throttle;
			}

			break;
		}
        break;
	}
	tbServer::unlock();
    return S_OK;
}
