/********************************************************************
	 _________        __    _____
	/\___  ___\      /\_\  /\  __\
	\/__/\ \__/ _  __\/_/_ \ \ \_\\   ____    _____      __
	    \ \ \  /\`´__\ /\ \ \ \  __\ /\ __\_ /\  __\   /´__`\
	     \ \ \ \ \ \/  \ \ \ \ \ \_\\\ \\_\ \\ \____\ /\  __/
	      \ \_\ \ \_\   \ \_\ \ \____\\ \___\ \ \____\\ \____\
	       \/_/  \/_/    \/_/  \/____/ \/__/   \/____/ \/____/

	tbServer.h
	=========
	Diese Datei ist Teil der TriBase-Engine.

	Zweck:
	Bereitstellen eines Clients

	Autor:
	Sebastian Blaum

********************************************************************/

//HRESULT WINAPI client_messagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage);


struct TRIBASE_API host {
	host *next;
	GUID instance;
	IDirectPlay8Address *server;
	IDirectPlay8Address *device;
	char sessionname[64];
	char hostname[64];
	DWORD portnummer;
	int latenz;
};

class TRIBASE_API tbClient {
private:
		static BOOL	m_bInitialized;
public:
		static CRITICAL_SECTION critsec;
		static IDirectPlay8Client *client;
		static host *hlist;
		static host *myhost;
		static char spielername[64];
		static msg_spielerliste slist;
		static DWORD index;

		static tbResult Init(PFNDPNMESSAGEHANDLER msghandler);
		static tbResult Exit();

		static int reInit(PFNDPNMESSAGEHANDLER msghandler);
		static void reset();

		static void lock() { EnterCriticalSection( &tbClient::critsec);}
		static void unlock() { LeaveCriticalSection( &tbClient::critsec);}

		static int host_suchen( char *host, DWORD port);
		static int host_hinzufuegen( PDPNMSG_ENUM_HOSTS_RESPONSE msg);

		static HRESULT anmelden( host *h);
		static void chat( char *text);
		static void send_msg(void* msg);
		static inline BOOL					IsInitialized()	{return m_bInitialized;}
};

//TRIBASE_API tbResult tbClientInit();
/*
TRIBASE_API void anmeldung( HWND hDlg);
TRIBASE_API void abmeldung( HWND hDlg);
TRIBASE_API void client_chatliste_aktualisieren( HWND hDlg, msg_chat *cm);
TRIBASE_API void serverliste_aktualisieren( HWND hDlg);
TRIBASE_API void spielerliste_aktualisieren( HWND hDlg);
TRIBASE_API void spielerindex_aktualisieren( HWND hDlg);
*/