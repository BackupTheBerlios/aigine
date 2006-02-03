


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
		CRITICAL_SECTION critsec;
		IDirectPlay8Client *client;
		host *hlist;
		host *myhost;

		char spielername[64];

		msg_spielerliste slist;
		DWORD index;


		void reset();
		tbClient();
		~tbClient();

		void lock() { EnterCriticalSection( &critsec);}
		void unlock() { LeaveCriticalSection( &critsec);}

		int init( PFNDPNMESSAGEHANDLER pfn);
		int host_suchen( char *host, DWORD port);
		int host_hinzufuegen( PDPNMSG_ENUM_HOSTS_RESPONSE msg);

		HRESULT anmelden( host *h);

		void chat( char *text);

	};