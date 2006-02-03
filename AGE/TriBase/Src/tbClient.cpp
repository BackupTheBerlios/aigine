
#include <TriBase.h>

CRITICAL_SECTION tbClient::critsec;
IDirectPlay8Client *tbClient::client;
host *tbClient::hlist;
host *tbClient::myhost;
char tbClient::spielername[64];
msg_spielerliste tbClient::slist;
DWORD tbClient::index;
HWND tbClient::mein_clientdialog;

void tbClient::reset()
	{
	host *h;
	int i;

	if( client)
		client->Close(0);
	client = 0;
	myhost = 0;
	index = -1;
	slist.angemeldet = 0;
	slist.maximum = 0;
	for( i = 0; i < MAX_PLAYERS; i++)
		slist.sp[i].status = FREI;
	while( h = hlist)
		{
		hlist = h->next;
		h->server->Release();
		h->device->Release();
		free( h);
		}
	}

int tbClient::host_suchen( char *host, DWORD port)
	{
	HRESULT hr;
    DPN_APPLICATION_DESC app;
    DPNHANDLE async; // Global um ende im callback-handler zu erkennen
	IDirectPlay8Address *hostadr;
	IDirectPlay8Address *myadr;
    WCHAR whost[64];

	hostadr = 0;
	myadr = 0;

    hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address,(LPVOID*)&myadr);
	if( hr < 0)
		goto RETURN; 
    hr = myadr->SetSP( &CLSID_DP8SP_TCPIP);
	if( hr < 0)
		goto RETURN; 
    hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address,(LPVOID*)&hostadr);
	if( hr < 0)
		goto RETURN; 
    hr = hostadr->SetSP( &CLSID_DP8SP_TCPIP);
	if( hr < 0)
		goto RETURN;
	if( host)
		{
        DXUtil_ConvertGenericStringToWide( whost, host);
        hostadr->AddComponent( DPNA_KEY_HOSTNAME, whost, (strlen(host)+1)*sizeof(WCHAR), DPNA_DATATYPE_STRING);
		}
	if( port != 0)
       hostadr->AddComponent( DPNA_KEY_PORT, &port, sizeof(DWORD), DPNA_DATATYPE_DWORD);
	ZeroMemory( &app, sizeof( DPN_APPLICATION_DESC));
    app.dwSize = sizeof( DPN_APPLICATION_DESC );
    app.guidApplication = tb_guid;
    hr = client->EnumHosts( &app, hostadr, myadr, NULL, 0, INFINITE, 0, INFINITE, NULL, &async, 0);
    if( hr == DPNERR_PENDING)
        hr = S_OK;

	RETURN:
	if( myadr)
		myadr->Release();
	if( hostadr)
		hostadr->Release();
	return hr;
	}

int tbClient::host_hinzufuegen( PDPNMSG_ENUM_HOSTS_RESPONSE msg)
	{
	WCHAR buf[64];
	DWORD buflen;
	DWORD type;
	host *h;

	for( h = hlist; h; h = h->next)
		{
		if( msg->pApplicationDescription->guidInstance == h->instance)
			return 0;
		}

	h = (host *)malloc( sizeof( host));
	h->server = msg->pAddressSender;
	h->server->AddRef();
	h->device = msg->pAddressDevice;
	h->device->AddRef();
	h->instance = msg->pApplicationDescription->guidInstance;
	buflen = 64;
	h->server->GetComponentByName( DPNA_KEY_HOSTNAME, buf, &buflen, &type);
	DXUtil_ConvertWideStringToAnsi( h->hostname, buf, 64);
	buflen = sizeof( DWORD);
	h->server->GetComponentByName( DPNA_KEY_PORT, &(h->portnummer), &buflen, &type);
	DXUtil_ConvertWideStringToAnsi( h->sessionname, msg->pApplicationDescription->pwszSessionName, 64);
	h->latenz = msg->dwRoundTripLatencyMS;
	h->next = hlist;
	hlist = h;
	return 1;
	}

int tbClient::init( /*PFNDPNMESSAGEHANDLER msghandler*/)
	{
	HRESULT hr;
	DPN_CAPS caps;

    hr = CoCreateInstance( CLSID_DirectPlay8Client, NULL, CLSCTX_INPROC_SERVER,IID_IDirectPlay8Client,(LPVOID*)&client);
	if( hr < 0)
		return hr;
//    hr = client->Initialize( 0, msghandler, 0);
	hr = client->Initialize( 0, client_messagehandler, 0);
	if( hr < 0)
		return hr;
	caps.dwSize = sizeof( DPN_CAPS);
	caps.dwConnectTimeout = 5000;
	caps.dwConnectRetries = 1;
	caps.dwTimeoutUntilKeepAlive = 1000;
	caps.dwFlags = 0;
	hr = client->SetCaps( &caps, 0);
	return hr;
	}
HRESULT WINAPI client_messagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage)
{
	tbClient::lock();
    switch( dwMessageType)
		{
    case DPN_MSGID_ENUM_HOSTS_RESPONSE:
		if( tbClient::host_hinzufuegen((PDPNMSG_ENUM_HOSTS_RESPONSE)pMessage))
			{
				if( tbClient::mein_clientdialog)
				PostMessage( tbClient::mein_clientdialog, WM_SERVERLISTE_AKTUALISIEREN, 0, 0 );
			}
		break;
    case DPN_MSGID_TERMINATE_SESSION:
		if( tbClient::mein_clientdialog)
			PostMessage( tbClient::mein_clientdialog, WM_SERVER_TERMINATE, 0, 0);
        break;

    case DPN_MSGID_RECEIVE:
        PBYTE rd = ((PDPNMSG_RECEIVE)pMessage)->pReceiveData;
        switch( NETWORK_MSGID( rd))
			{
		case MSG_SPIELERINDEX:
			tbClient::index = ((msg_spielerindex *)rd)->index;
            if( tbClient::mein_clientdialog)
                PostMessage( tbClient::mein_clientdialog, WM_SPIELERINDEX_AKTUALISIEREN, 0, 0 );
			break;
		case MSG_SPIELERLISTE:
			tbClient::slist = *(msg_spielerliste *)rd;
            if( tbClient::mein_clientdialog)
                PostMessage( tbClient::mein_clientdialog, WM_SPIELERLISTE_AKTUALISIEREN, 0, 0 );
			break;
		case MSG_CHAT:
			if( tbClient::mein_clientdialog)
				client_chatliste_aktualisieren( tbClient::mein_clientdialog, (msg_chat *)rd);
			break;

			}
        break;
		}
	tbClient::unlock();
    return S_OK;
}
HRESULT tbClient::anmelden( host *h)
{
	DPN_PLAYER_INFO pinfo;
    DPN_APPLICATION_DESC   appdsc;
	WCHAR wname[100];
	HRESULT hr;

	DXUtil_ConvertGenericStringToWide( wname, spielername);
	ZeroMemory( &pinfo, sizeof(DPN_PLAYER_INFO) );
	pinfo.dwSize = sizeof(DPN_PLAYER_INFO);
	pinfo.dwInfoFlags = DPNINFO_NAME;
	pinfo.pwszName = wname;
	hr = client->SetClientInfo( &pinfo, NULL, NULL, DPNOP_SYNC);

	if( hr != S_OK)
		return hr;

	ZeroMemory( &appdsc, sizeof( DPN_APPLICATION_DESC));
	appdsc.dwSize = sizeof( DPN_APPLICATION_DESC );
	appdsc.guidApplication = tb_guid;
	appdsc.guidInstance = h->instance;
	hr = client->Connect( &appdsc, h->server, h->device, NULL, NULL,NULL, 0, NULL, 0,DPNCONNECT_SYNC);
	if( hr != S_OK)
		return hr;
	myhost = h;
	return hr;
}
void tbClient::chat( char *text)
	{
	msg_chat cm;
    DPN_BUFFER_DESC bdsc;
    DPNHANDLE async;

	cm.msgid = MSG_CHAT;
	cm.spielerindex = index;
	strcpy( cm.text, text);

    bdsc.dwBufferSize = sizeof( msg_chat);
    bdsc.pBufferData  = (BYTE*) &cm;

	client->Send( &bdsc, 1, 0, 0, &async, DPNSEND_GUARANTEED);
	}

tbResult tbClientInit() {
	tbClient::client = 0;
	tbClient::hlist = 0;
	tbClient::reset();

    CoInitializeEx( NULL, COINIT_MULTITHREADED);
	InitializeCriticalSection( &tbClient::critsec);

	tbClient::init();

	return TB_OK;
}

void anmeldung( HWND hDlg)
	{
	HWND lst;
	LVITEM lvi;
	int ix;
	host *h;
	HRESULT hr;

	lst = GetDlgItem(hDlg, IDC_HOSTLISTE);
	if( ListView_GetSelectedCount( lst) == 1)
		{
		ix = ListView_GetSelectionMark( lst);
		GetDlgItemText( hDlg, IDC_SPIELER, tbClient::spielername, 64);
		if( isalnum(tbClient::spielername[0]))
			{
			lvi.iItem = ix;
			lvi.iSubItem = 0;
			lvi.mask = LVIF_PARAM;
			ListView_GetItem( lst, &lvi);
			h = (host *)lvi.lParam;
			hr = tbClient::anmelden( h);
			if( hr == S_OK)
				{
				SetDlgItemText( hDlg, IDC_ANMELDEN, "Abmelden");
				EnableWindow( GetDlgItem(hDlg, IDC_SPIELER), FALSE);
				EnableWindow( GetDlgItem(hDlg, IDC_CHAT_CLIENT), TRUE);
				SetDlgItemText( hDlg, IDC_SESSION, h->sessionname);
				}
//			else
				//MessageBox( hDlg, DXGetErrorDescription8( hr), "Duell-Meldung", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			}
//		else
			//MessageBox( hDlg, "Gib erst einen Spielernamen ein!", "Duell-Meldung", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
		}
//	else
		//MessageBox( hDlg, "Kein Server ausgewählt!", "Duell-Meldung", MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
	}

void abmeldung( HWND hDlg)
	{
	tbClient::lock();
	tbClient::reset();
	tbClient::init( /*client_messagehandler*/);
	serverliste_aktualisieren( hDlg);
	spielerliste_aktualisieren( hDlg);
	tbClient::unlock();
	SetDlgItemText( hDlg, IDC_ANMELDEN, "Anmelden");
    EnableWindow( GetDlgItem(hDlg, IDC_SPIELER), TRUE);
    EnableWindow( GetDlgItem(hDlg, IDC_CHAT_CLIENT), FALSE);
    SetDlgItemText( hDlg, IDC_SESSION, "");
    SetDlgItemText( hDlg, IDC_SPIELERINDEX, "");
	}

void client_chatliste_aktualisieren( HWND hDlg, msg_chat *cm)
	{
	HWND lst;
	LVITEM lvi;
	int i;

		lst = GetDlgItem(hDlg, IDC_CHATLISTE_CLIENT);

	ZeroMemory( &lvi, sizeof(lvi));
	lvi.mask = LVIF_TEXT;
	lvi.iItem = 0;
	if( cm->spielerindex >= 0)
		lvi.pszText = tbClient::slist.sp[cm->spielerindex].name;
	else
		lvi.pszText = "Server";
	ListView_InsertItem( lst, &lvi);
	ListView_SetItemText( lst, 0, 1, cm->text);		
	i = ListView_GetItemCount( lst);
	if( i > 20)
		ListView_DeleteItem( lst, i-1);

}

void serverliste_aktualisieren( HWND hDlg)
	{
	HWND lst;
	DWORD i;
	char buf[128];
	LVITEM lvi;
	host *h;

	lst = GetDlgItem(hDlg, IDC_HOSTLISTE);
    SendMessage( lst, LVM_DELETEALLITEMS, 0, 0 );

	ZeroMemory( &lvi, sizeof(lvi));
	lvi.mask = LVIF_TEXT|LVIF_PARAM;

	tbClient::lock();
	for( i = 0, h = tbClient::hlist; h ; h = h->next, i++)
		{
		lvi.iItem = i;
		lvi.pszText = h->sessionname;
		lvi.lParam = (LPARAM)h;
		ListView_InsertItem( lst, &lvi);
		ListView_SetItemText( lst, i, 1, h->hostname);
		sprintf( buf, "%d", h->portnummer);
		ListView_SetItemText( lst, i, 2, buf);
		sprintf( buf, "%d", h->latenz);
		ListView_SetItemText( lst, i, 3, buf);
		if( h == tbClient::myhost)
			ListView_SetItemState( lst, i, LVIS_FOCUSED|LVIS_SELECTED, LVIS_FOCUSED|LVIS_SELECTED);
		}

	tbClient::unlock();
	}

void spielerliste_aktualisieren( HWND hDlg)
	{
	HWND lst;
	DWORD i;
	char buf[128];
	msg_spielerliste slist;
	LVITEM lvi;

	tbClient::lock();
	slist = tbClient::slist;
	tbClient::unlock();

	lst = GetDlgItem(hDlg, IDC_SPIELERLISTE_CLIENT);
    SendMessage( lst, LVM_DELETEALLITEMS, 0, 0 );
	ZeroMemory( &lvi, sizeof(lvi));
	lvi.mask = LVIF_TEXT;
	lvi.pszText = buf;

	for( i = 0; i < slist.maximum; i++)
		{
		lvi.iItem = i;
		sprintf( buf, "%d", i+1);
		ListView_InsertItem( lst, &lvi);
		if( slist.sp[i].status == BESETZT)
			ListView_SetItemText( lst, i, 1, slist.sp[i].name);		
		}
	}
void spielerindex_aktualisieren( HWND hDlg)
	{
	int ix;

	tbClient::lock();
	ix = tbClient::index;
	tbClient::unlock();
	SetDlgItemInt( hDlg, IDC_SPIELERINDEX, ix + 1, FALSE);
	}