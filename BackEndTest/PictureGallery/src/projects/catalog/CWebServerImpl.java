/*
 * Created on 08.08.2004
 * File: package projects.voting;.VTWebServer.java
 */
package projects.catalog;

import java.rmi.RemoteException;
import java.util.Hashtable;

//import projects.catalog.model.Picture;
import projects.interfaces.CClient;
//import projects.interfaces.CLoginServer;
import projects.interfaces.CManagerServer;


import API.control.WebServer;
import API.control.web.Block;
import API.control.web.BlockFrame;
import API.control.web.Frames;
import API.control.web.RequestBlock;
import API.control.web.RequestFrame;
import API.control.web.RequestFrames;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;

/**
 * 
 * @author danny
 * @since 08.08.2004 17:06:01
 * @version 0.01
 */
public class CWebServerImpl extends WebServer implements CClient {
    private final int default_port= 8080;
    private final String default_root= "public_html" ;
    private final String default_template_root = "template_html" ;
    //private CLoginServer loginserver = null;
    private CManagerServer manager = null;
//    private Picture picture;
    
//    /* (non-Javadoc)
//     * @see projects.interfaces.CClient#update(projects.voting.model.VoteTable)
//     */
//    public void update(VoteTable votes) throws RemoteException {
//        // TODO Ausgabe der Votes
//        this.votes= votes;
//    }
    /**
     * Starts a web server with the default port and the default document root.
     * @see API.interfaces.Client#init(API.model.RemoteObject, API.interfaces.ServerHandle)
     */
    public void init(RemoteObject compProps, ServerHandle server)
        throws RemoteException {
        	this.manager = (CManagerServer)server;       
        // TODO zusätzlicher Parameter für Verzeichnisse und Port des Webservers
        // TODO (byDR) Verzeichnisse auf Existenz überprüfen.  
        System.out.println(
            "=> WebServer.init("
                + "RemoteObjectProperties "
                + compProps
                + ", Object"
                + manager);

        init(default_port, default_root);
      
        System.out.println(
            "<= WebServer.init("
                + "RemoteObjectProperties "
                + compProps
                + " , Object"
                + manager);
    }
    
    
    // Nachfolgende Methoden bis einschließlich getactionBody bauen und verwalten das 'FrameSet'
    // (byDR)
    
    private Block getBlock(String server, String operation, String whichblock, Hashtable requestProps) throws RemoteException 
    {	// hier findet nur das reine Holen eines Blocks statt
    	ServerHandle theServer = null ;
		Block theBlock = null ;
		System.out.println("==> projects.catalog.CWebServerImpl.getBlock: Anfrage an Server '" + server + "' für BlockContent") ;
		System.out.println("  > fordere Block '" + whichblock + "' beim Server mit op=" + operation + " an ...") ;
		if ((server != null) && (operation != null)) // (requestProps.get("srv") != null) && (requestProps.get("op") != null)
		{
			// passenden Server vom Manager erfragen
			RemoteObject ro = manager.getService((String) requestProps.get("srv")) ;
			if (ro != null) {
//				System.out.println("+++ Ich habe ein RemoteObject erhalten:\n\t" + ro) ;
				if (ro.getServicetyp().indexOf("server") == 0) {
				// TODO (byDR) hier statt indexOf compareTo testen!	
					System.out.println("+++ Ich habe einen Server erhalten!!") ;
					theServer = ro.getServerApp() ;
					if (theServer != null) {
						theBlock = theServer.executeWebRequest(operation, whichblock, requestProps) ;
						if (theBlock != null) {
							System.out.println("+++ habe einen Block als Antwort erhalten") ; 
						} else {
							System.out.println("--- habe null als Antwort auf meine Blockanfrage erhalten") ;
							System.out.println("  - Fehler: " + theServer.getWebRequestError()) ;
							theBlock = new Block(theServer.getWebRequestError()) ; // dieser Block gibt dann nach dem Parsen einen Fehler aus
						}
					} else {
						System.out.println("--- Das Holen des Servers hat leider nicht geklappt!") ;
						theBlock = new Block("--- Serveransprache ist gescheitert ...") ;
					}
				}				
			} else {
				// Fehler behandeln
				theBlock = new Block("--- Server wurde (beim Manager) nicht gefunden ...") ;
				// ^damit wird der Fehler mit geparst und an der entsprechenden Stelle in der Seite angezeigt
				System.out.println("--- angeforderter Server ist bei meinem Manager nicht erreichbar!!!\n") ;
			}
		} else {
			// Fehler behandeln
			theBlock = new Block("--- falsch konfigurierter Block! (Server oder Operation = null)") ;
			// ^damit wird der Fehler mit geparst und an der entsprechenden Stelle in der Seite angezeigt
			System.out.println("--- falsch konfigurierte Block! Server oder Operation = null!\n") ;
		}
		System.out.println("<== projects.catalog.CWebServerImpl.getBlock()") ;
    	return theBlock ;
    }
    
    
    private RequestFrames getFrames() {
    	// TODO (byDR) hier wird dann später die Benutzerkonfiguration aus der Datenbank verwendet, evtl. über CLoginServer?!
    	BlockFrame headFrame = new BlockFrame() ;
    	RequestFrames rfs = new RequestFrames() ;
    	RequestFrame rf = new RequestFrame(0) ;
 		rf.addBlock("seitenkopf", "CProjectServer", "seitenkopf", 0) ;
    	rfs.addFrame(rf, 0) ;
    	rf = new RequestFrame(1) ;
    	rf.addBlock("serverinfo", "CProjectServer", "serverinfo", 0) ;
    	rf.addBlock("userinfo", "CProjectServer", "userinfo", 1) ;
		rf.addBlock("toplist", "CProjectServer", "toplist", 2) ;
		rfs.addFrame(rf, 1) ; // linker Frame
		rf = new RequestFrame(2) ;
		rf.addBlock("main", "CProjectServer", "main", 0) ;
		rfs.addFrame(rf, 2) ; // Hauptbereich
		// vorerst mal überall nur die Serverinfo
		rf = new RequestFrame(3) ;
		rf.addBlock("kategorien", "CProjectServer", "kategorien", 0) ;
		rfs.addFrame(rf, 3) ; // rechter Frame
		return rfs ;
    }
    
    
    // diese hier geht die Frames-Struktur durch und ruft die entsprechenden Blöcke beim ProjectServer ab
    // und baut damit vorerst hart kodiert die Antwort-Seite auf 
    private Frames executeRequests(RequestFrames userFrames, Hashtable requestProps) throws RemoteException {
    	// , String operation, String BlockName, Hashtable requestProps
    	// TODO neuester Request muss den entsprechenden Block aktualisieren!
    	// mehrere Blocks zu aktualisieren mit einem Reload? vl. mit b1, b2, falls b nicht als Parameter mit übergeben wurde?
		String site = null ;
		RequestFrame aktuellerFrame = null ;
		BlockFrame contentFrame = null ;
		Frames contentFrames = new Frames() ;
		
		int zaehler = 0 ;
		int blockzaehler = 0 ;
		System.out.println("=> projects.catalog.CWebServerImpl.executeRequests: Abfrage der Frames von den Servern ..") ;
		
		if (userFrames != null) {
			zaehler = 0 ;
			aktuellerFrame = userFrames.getFrame(zaehler) ;
			while (aktuellerFrame != null) {
				System.out.println("  > FrameNummer/laut Frame: " + zaehler + "/" + aktuellerFrame.getFrameNumber()) ;
				contentFrame = new BlockFrame() ;
				blockzaehler = 0 ;
				RequestBlock rb = aktuellerFrame.getRequestBlock(blockzaehler) ;
				while (rb != null) {
					System.out.println("  > BlockNummer: " + blockzaehler) ;

					contentFrame.addBlock(getBlock(rb.getServer(), rb.getOperation(), rb.getName(), requestProps)) ;

					blockzaehler++ ;
					rb = aktuellerFrame.getRequestBlock(blockzaehler) ;
				}
				
				contentFrames.addFrame(contentFrame, zaehler) ;
				zaehler++ ;
				aktuellerFrame = userFrames.getFrame(zaehler) ;
			}
		} else {
			System.out.println("--- Es wurde kein FrameSet übergeben!") ;		
		}
		
/*		if (site == null) {
			// Fehler behandeln
			System.out.println("Da scheint etwas schief gegangen zu sein .. [site == null]") ;
			site = new String ("schwerer Fehler aufgetreten ...\n\t" + theServer.getWebRequestError() + "\n") ;
		} else
		{
			System.out.println("Sinnvollen Content erhalten!") ;
		} */
		System.out.println("\n<= projects.catalog.CWebServerImpl.executeRequests") ;
    	
    	return contentFrames ;
    }
    
     /**
     * @author Dennis
     */
	// TODO (byDR) parseFrame() mit templates laufen lassen    
    private String parseFrames(Frames theFrames) {
    	StringBuffer site = new StringBuffer() ;
		// hier kann dann ein Template eingesetzt werden mit einfachem Zahlenreplacement für die nummerierten Frames
		// doch bis dahin:
		site.append("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
		// Der Titel kommt dann auch eher aus dem Template, sollte aber vielleicht auch als Block generiert werden
		// um unterschiedliche Titel je nach Request verwenden zu können!!
		// TODO (byDR) Titel als Block, damit CProjectServer den an den aktuellen Request anpassen kann.
		site.append("<html>\n<head>\n<title>" + "Catalog-Portal für IM (2004-08)" + "</title>\n");
		site.append("\t<link rel=\"stylesheet\" href=\"catalog.css\" type=\"text/css\" />\n");
		site.append("</head>\n<body>\n");
		
    	site.append("<table class=\"site\">\n<tr>\n\t<td width=\"100%\" colspan=\"3\">\n") ;
		site.append(theFrames.getFrame(0).getBlocks()) ;
    	site.append("</td></tr>\n<tr><td width=\"150\">\n") ;
		site.append(theFrames.getFrame(1).getBlocks()) ;
		site.append("</td>\n<td width=\"90%\">\n") ;
		site.append(theFrames.getFrame(2).getBlocks()) ;
		site.append("</td>\n<td width=\"150\">\n") ;
		site.append(theFrames.getFrame(3).getBlocks()) ;
		site.append("</td></tr>\n</table>\n") ;
		site.append("<div class=\"copyright\" align=center>by DG, TH, FO, DR, (C) 2004</div>") ;
		site.append("</body></html>") ;

    	
    	return site.toString() ;
    }
    
    
    /**
     * durch Dennis: Diese Methode ist im CWebServerImpl allgemein für Manager gelöst ..
     * Führt die jeweilige Aktion, abhängig von dem übergebenen Property "op" aus.
     *
     * @see API.control.WebServer#getActionBody(java.lang.String[])
     */
    // TODO (byDR): Diese Methode muss erweitert werden für den Fall einer übergebenen Datei (Upload)
    //				oder dies wird vom WebServer gleich in zwei getrennten Methoden abgehandelt
    protected byte[] getActionBody(String[] request, Hashtable requestProps) throws RemoteException {
        String site = new String("leerer Durchlauf") ;
        ServerHandle theServer = null ;
		// wichtigste Faehigkeit des WebServers
		System.out.println("=> projects.catalog.CWebServerImpl.getActionBody: beginne Antwortseite aufzubauen ...") ;
		
		// Hier muss das FrameSet zusammengepuzzelt werden, daher gehen die eigentlichen Aufrufe in Untermethoden.
		Frames theFrameSet = executeRequests(getFrames(), requestProps) ;
		// aktualisiert das UserFrameset mit dem aktuellen Request und aktualisiert
		// dann alle Blocks durch Anfragen an die zuständigen Server
		site = parseFrames(theFrameSet) ;

		System.out.println(">>> site (am Ende von getActionBody) = " + site) ;
		System.out.println("<= projects.catalog.CWebServerImpl") ;
        return site.getBytes();

		/**        if (requestProps.get("op").equals("login")) {
					site = actionLogin(requestProps);
					System.out.println("LOGIN from user : " + requestProps.get("name"));
				} else if (requestProps.get("op").equals("galary")) {
					site = actionGalary(requestProps);
				} */
    }
    
    
//    /**
//     * Führt ein Voting aus.
//     * @param requestProps
//     * @return
//     */
/*    private String actionGalary(Hashtable requestProps) throws RemoteException {
		StringBuffer site= new StringBuffer();
		site.append(
			"\n<h1>request for , "
				+ requestProps.get("pic_request")
				+ " !</h1>\n");
		//loginserver.webvote((String) requestProps.get("voteradio"));
		//site.append(votes.toHTML());
        return site.toString();
    } */
    
    /**
     * Einloggen des Users
     * @param requestProps
     * @return
     */
    private String actionLogin(Hashtable requestProps) {
    	// TODO: Diese Funktion hat hier eigentlich nichts zu suchen
    	// TODO alt: "login" vom Server aufrufen und prüfen ob der User registriert ist.
        StringBuffer site= new StringBuffer();
//        long usrID = Integer.parseInt((String) requestProps.get("name"));
//        try {
//        	manager.createSession(usrID);
//		} catch (RemoteException e) {
//			
//			System.out.println("Error: exception type: RemoteException" +
//					"with the message: \n "+e.getMessage());
//			
//		}
        
        
//		site.append(
//			 "\n<h1>Tschesch Kollega , "
//				 + requestProps.get("name")
//				 + " !</h1>\n");
//		 site.append("\n<form action='galary.html' method='GET'>");
//		 site.append("\n<input type='hidden' name='op' value='galary'>");
//		 site.append(picture.toHtml());
//		 site.append("\n<input type='submit' value='wählen'>");
//		 site.append("\n</form>");
        return site.toString();
    }
	
	
	/**
	 * @return Returns the manager.
	 */
	public CManagerServer getManager() {
		//TODO ist das notwendig?
		return manager;
	}
	/**
	 * @param manager The manager to set.
	 */
	public void setManager(CManagerServer manager) {
		this.manager = manager;
	}
}
