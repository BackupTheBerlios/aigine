
#include <TriBase.h>

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

tbClient::tbClient()
	{
	client = 0;
	hlist = 0;
	reset();

    CoInitializeEx( NULL, COINIT_MULTITHREADED);
    InitializeCriticalSection( &critsec);
	}

tbClient::~tbClient()
	{
	reset();
    DeleteCriticalSection( &critsec);
    CoUninitialize();
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

int tbClient::init( PFNDPNMESSAGEHANDLER msghandler)
	{
	HRESULT hr;
	DPN_CAPS caps;

    hr = CoCreateInstance( CLSID_DirectPlay8Client, NULL, CLSCTX_INPROC_SERVER,IID_IDirectPlay8Client,(LPVOID*)&client);
	if( hr < 0)
		return hr;
    hr = client->Initialize( 0, msghandler, 0);
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