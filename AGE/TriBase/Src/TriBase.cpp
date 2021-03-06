/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`�__\ /\ \ \ \  __\ /\ __\_ /\  __\   /�__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	TriBase.cpp
	===========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Deklaration allgemeiner Funktionen

	Autor:
	David Scherfgen

	Berarbeitet:
	Snorky
	[24.9.03]

********************************************************************/

#include <TriBase.h>


// ******************************************************************
// Globale Variablen
HMODULE		tb_g_DLLHandle = NULL;		// Handle der TriBase-DLL-Datei
BOOL		tb_g_bInitCalled = FALSE;	// Wurde tbInit aufgerufen?
BOOL		tb_g_bExitCalled = FALSE;	// Wurde tbExit aufgerufen?
BOOL		tb_g_bAppActive = TRUE;		// Ist das Anwendungsfenster aktiv?
LONGLONG	tb_g_llFrequency = 0;		// Frequenz des Zeitgebers
double		tb_g_dFrequency = 0.0;		// Frequenz des Zeitgebers (float)
float		tb_g_fTime = 0.0f;			// Zeitz�hler
double		tb_g_dNumSecsPassed = 0.0f;	// Vergangene Sekunden seit letztem Frame
double		tb_g_dFramebrake = 1000.0;	// Framebremse, die maximale Framezahl

HWND		mein_clientdialog = 0;
HWND		mein_serverdialog = 0;
HRESULT (* clientCallFunc)(PVOID, DWORD, PVOID);
HRESULT (* serverCallFunc)(PVOID, DWORD, PVOID);

//PFNDPNMESSAGEHANDLER callFunc;
// ******************************************************************
// DLL-Hauptfunktion
BOOL APIENTRY DllMain(HMODULE hModule, 
                      DWORD dwReasonForCall, 
                      LPVOID pvReserved)
{
	tb_g_DLLHandle = hModule;

    switch(dwReasonForCall)
	{
		case DLL_PROCESS_DETACH:
			if(tb_g_bInitCalled && !tb_g_bExitCalled)
			{
				TB_WARNING("tbExit wurde nicht aufgerufen! Wird automatisch erledigt...");
				tbExit();
			}
			break;
    }

    return TRUE;
}

// ******************************************************************
// Initialisierung der Engine
TRIBASE_API tbResult tbInit()
{
	// Logbuchdatei initialisieren
	tbInitLog();

	// Logo schreiben
	tbWriteToLogDirect("<html>\n");
	tbWriteToLogDirect("<head>\n");
	tbWriteToLogDirect("<title>TriBase-Logbuchdatei</title>\n");
	tbWriteToLogDirect("</head>\n");
	tbWriteToLogDirect("<body>\n");
	tbWriteToLogDirect("<font face=\"Courier New\" size=\"1\">\n");
	tbWriteToLogDirect("&nbsp;_________&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; __&nbsp;&nbsp;&nbsp; _____<br>");
	tbWriteToLogDirect("/\\___&nbsp; ___\\&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; /\\_\\&nbsp; /\\&nbsp; __\\<br>");
	tbWriteToLogDirect("\\/__/\\ \\__/ _&nbsp; __\\/_/_ \\ \\ \\_\\\\&nbsp;&nbsp; ____&nbsp;&nbsp;&nbsp; _____&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; __<br>");
	tbWriteToLogDirect("&nbsp;&nbsp;&nbsp; \\ \\ \\&nbsp; /\\`�__\\ /\\ \\ \\ \\&nbsp; __\\ /\\ __\\_ /\\&nbsp; __\\&nbsp;&nbsp; /�__`\\<br>");
	tbWriteToLogDirect("&nbsp;&nbsp;&nbsp;&nbsp; \\ \\ \\ \\ \\ \\/&nbsp; \\ \\ \\ \\ \\ \\_\\\\\\ \\\\_\\ \\\\ \\____\\ /\\&nbsp; __/<br>");
	tbWriteToLogDirect("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \\ \\_\\ \\ \\_\\&nbsp;&nbsp; \\ \\_\\ \\ \\____\\\\ \\___\\ \\ \\____\\\\ \\____\\<br>");
	tbWriteToLogDirect("&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; \\/_/&nbsp; \\/_/&nbsp;&nbsp;&nbsp; \\/_/&nbsp; \\/____/ \\/__/&nbsp;&nbsp; \\/____/ \\/____/<br><br>");
	tbWriteToLogDirect("</font>\n");
	tbWriteToLogDirect("<font face=\"Tahoma\" size=\"2\">\n");
	tbWriteToLogDirect("<table>\n");

	TB_INFO("TriBase-Engine wird initialisiert...");

	// Speichermanager initialisieren
	if(tbMemInit()) TB_ERROR("Initialisierung des Speichermanagers fehlgeschlagen!", TB_ERROR);

	// COM initialisieren
	if(FAILED(CoInitialize(NULL))) TB_ERROR("COM-Initialisierung fehlgeschlagen!", TB_ERROR);

	tbClient::Init(client_messagehandler);
	tbServer::Init();

	// Uhr und Zufallsgenerator initialisieren
	QueryPerformanceFrequency((LARGE_INTEGER*)(&tb_g_llFrequency));
	tb_g_dFrequency = (float)(tb_g_llFrequency);
	srand(timeGetTime() % 10000);

	// Einen globalen D3DX-Effektpool erstellen
	D3DXCreateEffectPool(&tb_g_pEffectPool);

	tb_g_bInitCalled = TRUE;

	TB_INFO("Initialisierung komplett!");

	return TB_OK;
}

// ******************************************************************
// Herunterfahren der Engine
TRIBASE_API tbResult tbExit()
{
	TB_INFO("TriBase-Engine wird heruntergefahren...");

	// Eventuell Direct3D herunterfahren
	if(tbDirect3D::IsInitialized())
	{
		TB_WARNING("Direct3D wurde nicht heruntergefahren! Wird automatisch erledigt...");
		tbDirect3D::Exit();
	}

	// Eventuell DirectInput herunterfahren
	if(tbDirectInput::IsInitialized())
	{
		TB_WARNING("DirectInput wurde nicht heruntergefahren! Wird automatisch erledigt...");
		tbDirectInput::Exit();
	}

	// Eventuell DirectSound herunterfahren
	if(tbDirectSound::IsInitialized())
	{
		TB_WARNING("DirectSound wurde nicht heruntergefahren! Wird automatisch erledigt...");
		tbDirectSound::Exit();
	}

	//Client runterfahren
	if(tbClient::IsInitialized()) {
		TB_WARNING("Client wurde nicht heruntergefahren! Wird automatisch erledigt...");
		tbClient::Exit();
	}

	//Server runterfahren
	if(tbServer::IsInitialized()) {
		TB_WARNING("Server wurde nicht heruntergefahren! Wird automatisch erledigt...");
		tbServer::Exit();
	}

	// Eventuell alle Texturen l�schen
	if(tbTextureManager::IsInitialized())
	{
		TB_WARNING("Texturen werden automatisch gel�scht! Bitte die Exit-Methode von tbTextureManager aufrufen!");
		tbTextureManager::Exit();
	}

	// Speicherverwaltung herunterfahren
	tbMemExit();

	// Den Effektpool l�schen
	TB_SAFE_RELEASE(tb_g_pEffectPool);

	TB_INFO("Herunterfahren abgeschlossen!");

	tbWriteToLog("</table>\n");
	tbWriteToLog("</font>\n");
	tbWriteToLog("</body>\n");
	tbWriteToLog("</html>\n");

	// Logbuchdatei schlie�en
	tbExitLog();

	tb_g_bExitCalled = TRUE;

	// COM herunterfahren
	CoUninitialize();

	return TB_OK;
}

// ******************************************************************
// Framebremse setzen
TRIBASE_API	tbResult tbSetFramebrake(double dFramebrake)
{
	if(dFramebrake == 0.0) dFramebrake = 1000.0;
	tb_g_dFramebrake = 1.0 / dFramebrake;
	
	return TB_OK;
}

// ******************************************************************
// Nachrichtenschleife
TRIBASE_API tbResult tbDoMessageLoop(tbResult (* pMoveProc)(float),
									 tbResult (* pRenderProc)(float),
									 HRESULT  (* pClientHandler)( PVOID , DWORD , PVOID),
									 HRESULT  (* pServerHandler)( PVOID , DWORD , PVOID),
									 double dMaxFPS)
{
	MSG			Message;		// Nachricht
	LONGLONG	llStartTime;	// Startzeitpunkt
	LONGLONG	llEndTime;		// Endzeitpunkt
	double		dTime;			// Zeitspanne in Sekunden
	BOOL		bQuit = FALSE;

	clientCallFunc = pClientHandler;
	serverCallFunc = pServerHandler;
	// Frameratenbremse setzen
	tbSetFramebrake(dMaxFPS);

	TB_INFO("Nachrichtenschleife wird betreten...");
	ZeroMemory(&Message, sizeof(MSG));

	// Startzeitpunkt vorab messen f�r die erste Schleife
	QueryPerformanceCounter((LARGE_INTEGER*)(&llStartTime));

	// Nachrichtenschleife
	dTime = 0.0001;
	while(!bQuit)
	{
		// Nachrichten verarbeiten
		while(PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);

			if(Message.message == WM_QUIT)
			{
				// Schleife verlassen!
				bQuit = TRUE;
			}	
		}

		if(tb_g_bAppActive)
		{
			// Die Sekunden seit dem letzten Frame speichern
			tb_g_dNumSecsPassed = dTime;

			// Bewegungen durchf�hren, falls gew�nscht
			if(pMoveProc != NULL) if(pMoveProc((float)(dTime)) == TB_STOP) break;

			
			// Frameratenz�hler vor dem rendern midestens einmal durchlaufen und ggf. abbremsen
			do{
				// Endzeitpunkt messen und Anzahl der vergangenen Sekunden berechnen
				QueryPerformanceCounter((LARGE_INTEGER*)(&llEndTime));
				if(llEndTime == llStartTime) dTime = 0.0001;
				else dTime = (float)(llEndTime - llStartTime) / tb_g_dFrequency;
			}while(dTime < tb_g_dFramebrake);
			
			// Zeitz�hler erh�hen
			tb_g_fTime += (float)(dTime);
			
			// Neuen Startzeitpunkt nach der Framebremse berechnen
			QueryPerformanceCounter((LARGE_INTEGER*)(&llStartTime));


			// Spielzustand zeichnen, falls gew�nscht
			if(pRenderProc != NULL) if(pRenderProc((float)(dTime)) == TB_STOP) break;

			if(tbDirectSound::IsInitialized())
			{
				if(tbDirectSound::GetListener() != NULL)
				{
					// Aufgeschobene 3D-Sound-�nderungen wirksam machen
					tbDirectSound::GetListener()->CommitDeferredSettings();
				}
			}
		}
	}

	TB_INFO("Nachrichtenschleife beendet!");

	return TB_OK;

}

// ******************************************************************
// Macht eine Pause
TRIBASE_API tbResult tbDelay(DWORD dwDuration)
{
	DWORD dwEndTime;

	dwEndTime = timeGetTime() + dwDuration;
	while(timeGetTime() < dwEndTime) {}

	return TB_OK;
}

// ******************************************************************
// Setzt den Aktivierungsstatus der Anwendung
TRIBASE_API tbResult tbSetAppActive(BOOL bActive)
{
	// Status kopieren
	tb_g_bAppActive = bActive;

	return TB_OK;
}

// ******************************************************************

HRESULT WINAPI server_messagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage) {
	int ret = S_OK;

	tbServer::lock();

    switch( dwMessageType) {
	case DPN_MSGID_INDICATE_CONNECT:
		if( tbServer::status == SERVER_SPIEL_LAEUFT)
			ret = !S_OK;
		else
			((PDPNMSG_INDICATE_CONNECT)pMessage)->pvPlayerContext = (void *)tbServer::reservierung();
		break;
	case DPN_MSGID_INDICATED_CONNECT_ABORTED:
		tbServer::storno((PDPNMSG_INDICATED_CONNECT_ABORTED)pMessage);
		break;
    case DPN_MSGID_CREATE_PLAYER:
		if((int)((PDPNMSG_CREATE_PLAYER)pMessage)->pvPlayerContext != -1) { // nicht der Server selbst
 			tbServer::buchung( (PDPNMSG_CREATE_PLAYER)pMessage);
			PostMessage( mein_serverdialog, WM_SPIELER_AKTUALISIEREN, 0, 0);
		}
        break;
   case DPN_MSGID_DESTROY_PLAYER:
		tbServer::remove_player( (int)((PDPNMSG_DESTROY_PLAYER)pMessage)->pvPlayerContext);
		PostMessage( mein_serverdialog, WM_SPIELER_AKTUALISIEREN, 0, 0);
        break;
   case DPN_MSGID_RECEIVE:
        PBYTE rd = ((PDPNMSG_RECEIVE)pMessage)->pReceiveData;
        switch( NETWORK_MSGID( rd)) {
		case MSG_CHAT:
			tbServer::send_chatmessage( (msg_chat *)rd);
			server_chatliste_aktualisieren( mein_serverdialog, (msg_chat *)rd);
			break;
		default:
			if(serverCallFunc != NULL) {
				serverCallFunc(pvUserContext, dwMessageType, pMessage);
			}
			else TB_WARNING("serverCallFunc ist NULL");

		}
	}
	tbServer::unlock();
    return ret;
}

HRESULT WINAPI client_messagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage) {
	tbClient::lock();
    switch( dwMessageType) {
    case DPN_MSGID_ENUM_HOSTS_RESPONSE:
		if( tbClient::host_hinzufuegen((PDPNMSG_ENUM_HOSTS_RESPONSE)pMessage)) {
			if( mein_clientdialog)
				PostMessage( mein_clientdialog, WM_SERVERLISTE_AKTUALISIEREN, 0, 0 );
		}
		break;
    case DPN_MSGID_TERMINATE_SESSION:
		if( mein_clientdialog)
			PostMessage( mein_clientdialog, WM_SERVER_TERMINATE, 0, 0);
        break;
    case DPN_MSGID_RECEIVE:
        PBYTE rd = ((PDPNMSG_RECEIVE)pMessage)->pReceiveData;
        switch( NETWORK_MSGID( rd)) {
		case MSG_SPIELERINDEX:
			tbClient::index = ((msg_spielerindex *)rd)->index;
            if( mein_clientdialog)
                PostMessage( mein_clientdialog, WM_SPIELERINDEX_AKTUALISIEREN, 0, 0 );
			break;
		case MSG_SPIELERLISTE:
			tbClient::slist = *(msg_spielerliste *)rd;
            if( mein_clientdialog)
                PostMessage( mein_clientdialog, WM_SPIELERLISTE_AKTUALISIEREN, 0, 0 );
			break;
		case MSG_CHAT:
			if( mein_clientdialog)
				client_chatliste_aktualisieren( mein_clientdialog, (msg_chat *)rd);
			break;
		default:
			if(clientCallFunc != NULL) {
				clientCallFunc(pvUserContext, dwMessageType, pMessage);
			}
			else TB_WARNING("clientCallFunc ist NULL");
		}
        break;
	}
	tbClient::unlock();
    return S_OK;
}