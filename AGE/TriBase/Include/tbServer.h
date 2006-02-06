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
	Bereitstellen eines Servers

	Autor:
	Sebastian Blaum

********************************************************************/
HRESULT WINAPI server_messagehandler( PVOID pvUserContext, DWORD dwMessageType, PVOID pMessage);

class TRIBASE_API tbServer {

	public:
		static CRITICAL_SECTION critsec;
		static int status;
		static char hostname[64];
		static char sessionname[64];
		static DWORD portnummer;
		static int maxspieler;
		static IDirectPlay8Server *server;

		static HWND mein_serverdialog;

		static msg_spielerliste slist;

		static inline void lock() { EnterCriticalSection( &critsec);}
		static inline void unlock() { LeaveCriticalSection( &critsec);}

		static int start( PFNDPNMESSAGEHANDLER msghandler, char *sname, int pno, int maxsp);
		static void stop();

		static int reservierung();
		static void storno( PDPNMSG_INDICATED_CONNECT_ABORTED m);
		static int buchung( PDPNMSG_CREATE_PLAYER msg);
		static void remove_player( int pnr);

		static void send_spielerliste();
		static void send_spielerindex( DPNID id, int ix);
		static void send_chatmessage( msg_chat *cm);

};

TRIBASE_API tbResult tbServerInit();
TRIBASE_API tbResult tbServerExit();
TRIBASE_API void next_serverstate( HWND hDlg);
TRIBASE_API void server_chatliste_aktualisieren( HWND hDlg, msg_chat *cm);
TRIBASE_API void display_spieler( HWND hDlg );
TRIBASE_API void display_serverstate( HWND hDlg);
TRIBASE_API void kill_players( HWND hDlg);
