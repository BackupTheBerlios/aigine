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


class TRIBASE_API tbServer {

	public:
		CRITICAL_SECTION critsec;
		int status;
		char hostname[64];
		char sessionname[64];
		DWORD portnummer;
		int maxspieler;
		IDirectPlay8Server *server;

		msg_spielerliste slist;

		tbServer();
		~tbServer();

		tbResult Init();
		tbResult Exit();

		inline void lock() { EnterCriticalSection( &critsec);}
		inline void unlock() { LeaveCriticalSection( &critsec);}

		int start( PFNDPNMESSAGEHANDLER msghandler, char *sname, int pno, int maxsp);
		void stop();

		int reservierung();
		void storno( PDPNMSG_INDICATED_CONNECT_ABORTED m);
		int buchung( PDPNMSG_CREATE_PLAYER msg);

		void send_spielerliste();
		void send_spielerindex( DPNID id, int ix);

};