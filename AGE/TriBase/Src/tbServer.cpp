/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbServer.cpp
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Bereitstellen eines Servers

	Autor:
	Sebastian Blaum

********************************************************************/
#include <TriBase.h>

CRITICAL_SECTION tbServer::critsec;
int tbServer::status;
char tbServer::hostname[64];
char tbServer::sessionname[64];
DWORD tbServer::portnummer;
int tbServer::maxspieler;
IDirectPlay8Server *tbServer::server;
HWND tbServer::mein_serverdialog;
msg_spielerliste tbServer::slist;



int tbServer::start( PFNDPNMESSAGEHANDLER msghandler, char *sname, int pno, int maxsp) {
    HRESULT hr;
    PDIRECTPLAY8ADDRESS adr = 0;
    DPN_APPLICATION_DESC adsc;
	WCHAR buf[64];
	DWORD buflen = 64;
	DWORD anza = 1;
	DWORD type;

	strcpy( sessionname, sname);
	portnummer = pno;
//	maxspieler = maxsp;

	slist.maximum = maxspieler = maxsp;

	hr = CoCreateInstance( CLSID_DirectPlay8Server, NULL, CLSCTX_INPROC_SERVER,
                                       IID_IDirectPlay8Server, (LPVOID*) &server);
	if( hr < 0)
		goto RETURN;
	hr = server->Initialize( 0, msghandler, 0);
	if( hr < 0)
		goto RETURN;;
	hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
						   CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address, 
						   (LPVOID*)&adr );
	if( hr < 0)
		goto RETURN;
	hr = adr->SetSP( &CLSID_DP8SP_TCPIP);
	if( hr < 0)
		goto RETURN;
	adr->AddComponent( DPNA_KEY_PORT, &portnummer, sizeof(portnummer), DPNA_DATATYPE_DWORD);
	DXUtil_ConvertGenericStringToWide( buf, sessionname);
	ZeroMemory( &adsc, sizeof(DPN_APPLICATION_DESC));
	adsc.dwSize = sizeof( DPN_APPLICATION_DESC);
	adsc.dwFlags = DPNSESSION_CLIENT_SERVER;
	adsc.guidApplication = tb_guid;
	adsc.pwszSessionName = buf;
	adsc.dwMaxPlayers = maxspieler + 1;
	hr = server->Host( &adsc, &adr, 1, NULL, NULL, (void *)-1, 0);
	if( hr < 0)
		goto RETURN;
	server->GetLocalHostAddresses(&adr, &anza, 0);
	adr->GetComponentByName( DPNA_KEY_HOSTNAME, buf, &buflen, &type);
	DXUtil_ConvertWideStringToAnsi( hostname, buf, 64);

	RETURN:
	if( adr)
		adr->Release();
	return hr;
}

void tbServer::stop() {
    if( server) {
        server->Close( 0);
        server->Release();
		server = 0;
	}
	status = SERVER_ANGEHALTEN;
}

int tbServer::buchung( PDPNMSG_CREATE_PLAYER msg) {
    HRESULT hr;
    DWORD size = 0;
    DPN_PLAYER_INFO* playinfo = 0;
	int pnr;

    hr = server->GetClientInfo( msg->dpnidPlayer, playinfo, &size, 0);
    if((hr < 0) && hr != DPNERR_BUFFERTOOSMALL )
        return hr;
    playinfo = (DPN_PLAYER_INFO*)calloc( 1, size);
    playinfo->dwSize = sizeof( DPN_PLAYER_INFO);
    hr = server->GetClientInfo( msg->dpnidPlayer, playinfo, &size, 0 );
    if( hr < 0) {
		free( playinfo);
        return hr;
	}

	lock();
	pnr = (int)msg->pvPlayerContext;
	slist.sp[pnr].status = BESETZT;
	slist.sp[ pnr].dpnid = msg->dpnidPlayer;
	DXUtil_ConvertWideStringToGeneric( slist.sp[ pnr].name, playinfo->pwszName, MAX_NAMLEN);

	send_spielerliste();
	send_spielerindex( msg->dpnidPlayer, pnr);

	unlock();
    free( playinfo);
	return hr;
}

void tbServer::storno( PDPNMSG_INDICATED_CONNECT_ABORTED m) {
	lock();
	slist.sp[(int)m->pvPlayerContext].status = FREI;
	slist.angemeldet--;
	unlock();
}

int tbServer::reservierung() {
	int pnr;

	lock();
	for( pnr = 0; pnr < MAX_PLAYERS; pnr++) {
		if( slist.sp[pnr].status == FREI) {
			slist.sp[pnr].status = RESERVIERT;
			slist.angemeldet++;
			break;
		}
	}
	unlock();
	return pnr;
}

void tbServer::remove_player( int pnr) {
	lock();
	slist.sp[pnr].status = FREI;
	slist.angemeldet--;
	unlock();
	send_spielerliste();
}

void tbServer::send_spielerliste() {
	msg_spielerliste sl;
    DPN_BUFFER_DESC bdsc;
    DPNHANDLE async;

	lock();
	sl = slist;
	unlock();

    bdsc.dwBufferSize = sizeof( msg_spielerliste);
    bdsc.pBufferData  = (BYTE*) &sl;

    server->SendTo( DPNID_ALL_PLAYERS_GROUP, &bdsc, 1, 0, NULL, &async, DPNSEND_GUARANTEED|DPNSEND_NOLOOPBACK);
}

void tbServer::send_spielerindex( DPNID id, int ix) {
	msg_spielerindex six;
    DPN_BUFFER_DESC bdsc;
    DPNHANDLE async;

	six.msgid = MSG_SPIELERINDEX;
	six.index = ix;
    bdsc.dwBufferSize = sizeof( msg_spielerindex);
    bdsc.pBufferData  = (BYTE*)&six;

    server->SendTo( id, &bdsc, 1, 0, NULL, &async, DPNSEND_GUARANTEED);
}

void tbServer::send_chatmessage( msg_chat *cm) {
    DPN_BUFFER_DESC bdsc;
    DPNHANDLE async;

	if( !server) return;
    bdsc.dwBufferSize = sizeof( msg_chat);
    bdsc.pBufferData = (BYTE*)cm;

    server->SendTo( DPNID_ALL_PLAYERS_GROUP, &bdsc, 1, 0, NULL, &async, DPNSEND_GUARANTEED|DPNSEND_NOLOOPBACK);
}

tbResult tbServerInit() {

	int i;

	CoInitializeEx( NULL, COINIT_MULTITHREADED);
	InitializeCriticalSection( &tbServer::critsec);

	tbServer::server = 0;
	tbServer::hostname[0] = 0;
	tbServer::maxspieler = 2;
	strcpy( tbServer::sessionname, "TriBase-Server2");
	tbServer::portnummer = 14711;
	tbServer::status = SERVER_ANGEHALTEN;

	tbServer::slist.msgid = MSG_SPIELERLISTE;
	tbServer::slist.angemeldet = 0;
	tbServer::slist.maximum = tbServer::maxspieler;
	for( i = 0; i < MAX_PLAYERS; i++)
	{
		tbServer::slist.sp[i].status = FREI;
	}

	return TB_OK;
}

tbResult tbServerExit() {
    DeleteCriticalSection( &tbServer::critsec);
    CoUninitialize();
	return TB_OK;
}

void next_serverstate( HWND hDlg) {
	char sessionname[64];
	int portnummer;
	int maxsp;
	int hr;
	

	switch( tbServer::status) {
	case SERVER_ANGEHALTEN:
		SetCursor( LoadCursor(NULL, IDC_WAIT));
		GetDlgItemText( hDlg, IDC_SESSION_NAME, sessionname, 64);
		portnummer = GetDlgItemInt( hDlg, IDC_PORT, 0, FALSE);
		maxsp = GetDlgItemInt( hDlg, IDC_MAXSPIELER, 0, 0);
		if( maxsp < 2)
			maxsp = 2;
		if( maxsp > MAX_PLAYERS)
			maxsp = MAX_PLAYERS;
		SetDlgItemInt( hDlg, IDC_MAXSPIELER, maxsp, FALSE );
		hr = tbServer::start( server_messagehandler, sessionname, portnummer, maxsp);
		if( hr == S_OK) {
				SetDlgItemText( hDlg, IDC_IP_ADRESSE, tbServer::hostname);
				tbServer::status = SERVER_GESTARTET;
		}
		else {
			MessageBox( hDlg, DXGetErrorDescription9( hr), "Duell-Meldung", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			tbServer::stop();
		}
		SetCursor( LoadCursor(NULL, IDC_ARROW));
		break;
	}
}

void server_chatliste_aktualisieren( HWND hDlg, msg_chat *cm) {
	HWND lst;
	LVITEM lvi;
	int i;

	lst = GetDlgItem(hDlg, IDC_CHATLISTE_SERVER);

	ZeroMemory( &lvi, sizeof(lvi));
	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
	if( cm->spielerindex >= 0)
		lvi.pszText = tbServer::slist.sp[cm->spielerindex].name;
	else
		lvi.pszText = "Server";
	ListView_InsertItem( lst, &lvi);
	ListView_SetItemText( lst, 0, 1, cm->text);		
	i = ListView_GetItemCount( lst);
	if( i > 20)
		ListView_DeleteItem( lst, i-1);
}

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
			PostMessage( tbServer::mein_serverdialog, WM_SPIELER_AKTUALISIEREN, 0, 0);
		}
        break;
   case DPN_MSGID_DESTROY_PLAYER:
		tbServer::remove_player( (int)((PDPNMSG_DESTROY_PLAYER)pMessage)->pvPlayerContext);
		PostMessage( tbServer::mein_serverdialog, WM_SPIELER_AKTUALISIEREN, 0, 0);
        break;
   case DPN_MSGID_RECEIVE:
        PBYTE rd = ((PDPNMSG_RECEIVE)pMessage)->pReceiveData;
        switch( NETWORK_MSGID( rd)) {
		case MSG_CHAT:
			tbServer::send_chatmessage( (msg_chat *)rd);
			server_chatliste_aktualisieren( tbServer::mein_serverdialog, (msg_chat *)rd);
			break;
		}
	}
	tbServer::unlock();
    return ret;
}

void display_spieler( HWND hDlg ) {
	HWND lst;
	DWORD i;
	char buf[128];
	msg_spielerliste slist;
	LVITEM lvi;

	tbServer::lock();
	slist = tbServer::slist;
	tbServer::unlock();
	lst = GetDlgItem(hDlg, IDC_SPIELERLISTE_SERVER);
    SendMessage( lst, LVM_DELETEALLITEMS, 0, 0 );
	ZeroMemory( &lvi, sizeof(lvi));
	lvi.mask = LVIF_TEXT;
	lvi.pszText = buf;
	for( i = 0; i < slist.maximum; i++) {
		lvi.iItem = i;
		sprintf( buf, "%d", i+1);
		ListView_InsertItem( lst, &lvi);
		if( slist.sp[i].status == BESETZT)
			ListView_SetItemText( lst, i, 1, slist.sp[i].name);		
	}
	EnableWindow( GetDlgItem(hDlg, IDC_CHAT_SERVER), slist.angemeldet); 
}

void display_serverstate( HWND hDlg) {
	int st;

	switch( tbServer::status) {
	case SERVER_ANGEHALTEN:
        SetDlgItemText( hDlg, IDC_STATUS, "Server angehalten");
		SetDlgItemText( hDlg, IDC_START, "Server starten");
		SetDlgItemText( hDlg, IDC_SESSION_NAME, tbServer::sessionname);
		SetDlgItemInt( hDlg, IDC_PORT, tbServer::portnummer, FALSE );
		SetDlgItemInt( hDlg, IDC_MAXSPIELER, tbServer::maxspieler, FALSE );
		break;
	case SERVER_GESTARTET:
		SetDlgItemText( hDlg, IDC_STATUS, "Server läuft");
        SetDlgItemText( hDlg, IDC_START, "Spiel laden");
		break;
	}
	st = (tbServer::status == SERVER_ANGEHALTEN);
	EnableWindow( GetDlgItem(hDlg, IDC_SESSION_NAME), st);
	EnableWindow( GetDlgItem(hDlg, IDC_PORT), st);
	EnableWindow( GetDlgItem(hDlg, IDC_MAXSPIELER), st);
}

void kill_players( HWND hDlg) {
	HWND lst;
	DWORD i;

	tbServer::lock();
	lst = GetDlgItem(hDlg, IDC_SPIELERLISTE_SERVER);
	for( i = 0; i < tbServer::slist.maximum; i++) {
		if( (tbServer::slist.sp[i].status == BESETZT) && ListView_GetItemState( lst, i, LVIS_SELECTED))
			tbServer::server->DestroyClient( tbServer::slist.sp[i].dpnid, 0, 0, 0);
	}
	tbServer::unlock();
}