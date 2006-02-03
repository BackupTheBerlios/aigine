

HRESULT WINAPI client_messagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage);


struct host
	{
	host *next;
	GUID instance;
	IDirectPlay8Address *server;
	IDirectPlay8Address *device;
	char sessionname[64];
	char hostname[64];
	DWORD portnummer;
	int latenz;
	};

class TRIBASE_API tbClient
	{
	public:
		static CRITICAL_SECTION critsec;
		static IDirectPlay8Client *client;
		static host *hlist;
		static host *myhost;

		static char spielername[64];

		static msg_spielerliste slist;
		static DWORD index;

		static HWND mein_clientdialog;

		static void reset();
		//tbClient();
		//~tbClient();

		static void lock() { EnterCriticalSection( &critsec);}
		static void unlock() { LeaveCriticalSection( &critsec);}

		static int init( /*PFNDPNMESSAGEHANDLER pfn*/);
		static int host_suchen( char *host, DWORD port);
		static int host_hinzufuegen( PDPNMSG_ENUM_HOSTS_RESPONSE msg);

		static HRESULT anmelden( host *h);

		static void chat( char *text);

	};

TRIBASE_API tbResult tbClientInit();

TRIBASE_API void anmeldung( HWND hDlg);

TRIBASE_API void abmeldung( HWND hDlg);

TRIBASE_API void client_chatliste_aktualisieren( HWND hDlg, msg_chat *cm);

TRIBASE_API void serverliste_aktualisieren( HWND hDlg);

TRIBASE_API void spielerliste_aktualisieren( HWND hDlg);

TRIBASE_API void spielerindex_aktualisieren( HWND hDlg);