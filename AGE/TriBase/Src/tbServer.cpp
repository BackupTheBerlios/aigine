/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbVFile.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Bereitstellen eines Servers

	Autor:
	Sebastian Blaum

********************************************************************/
#include <TriBase.h>

//tbServer* g_pServer = NULL;
//HRESULT WINAPI server_messagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage);
//HRESULT WINAPI server_messagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage);

tbServer::tbServer() {
	Init();
}

tbServer::~tbServer() {
	Exit();
}

tbResult tbServer::Init() {

	int i;

	CoInitializeEx( NULL, COINIT_MULTITHREADED);
    InitializeCriticalSection( &critsec);

	server = 0;
	hostname[0] = 0;
	maxspieler = 4;
	strcpy( sessionname, "Duell");
	portnummer = 4711;
	status = SERVER_ANGEHALTEN;

	slist.msgid = MSG_SPIELERLISTE;
	slist.angemeldet = 0;
	slist.maximum = maxspieler;
	for( i = 0; i < MAX_PLAYERS; i++)
	{
		slist.sp[i].status = FREI;
	}

	return TB_OK;
}

tbResult tbServer::Exit()
{
    DeleteCriticalSection( &critsec);
    CoUninitialize();
	return TB_OK;
}

int tbServer::start( PFNDPNMESSAGEHANDLER msghandler, char *sname, int pno, int maxsp)
	{
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

void tbServer::stop()
	{
    if( server)
		{
        server->Close( 0);
        server->Release();
		server = 0;
		}
	status = SERVER_ANGEHALTEN;
	}

int tbServer::buchung( PDPNMSG_CREATE_PLAYER msg)
	{
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
    if( hr < 0)
		{
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
void tbServer::storno( PDPNMSG_INDICATED_CONNECT_ABORTED m)
	{
	lock();
	slist.sp[(int)m->pvPlayerContext].status = FREI;
	slist.angemeldet--;
	unlock();
	}

int tbServer::reservierung()
	{
	int pnr;

	lock();
	for( pnr = 0; pnr < MAX_PLAYERS; pnr++)
		{
		if( slist.sp[pnr].status == FREI)
			{
			slist.sp[pnr].status = RESERVIERT;
			slist.angemeldet++;
			break;
			}
		}
	unlock();
	return pnr;
	}

void tbServer::send_spielerliste()
	{
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

void tbServer::send_spielerindex( DPNID id, int ix)
	{
	msg_spielerindex six;
    DPN_BUFFER_DESC bdsc;
    DPNHANDLE async;

	six.msgid = MSG_SPIELERINDEX;
	six.index = ix;
    bdsc.dwBufferSize = sizeof( msg_spielerindex);
    bdsc.pBufferData  = (BYTE*)&six;

    server->SendTo( id, &bdsc, 1, 0, NULL, &async, DPNSEND_GUARANTEED);
	}

