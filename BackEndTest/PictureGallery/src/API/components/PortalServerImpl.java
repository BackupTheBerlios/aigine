/*
 * Created on 31.10.2004
 *
 * 
 */
package API.components;

import java.rmi.RemoteException;
import java.util.Hashtable;

//import API.model.RemoteObject;
import API.control.Server ;
// import API.interfaces.ManagerHandle;
import API.interfaces.PortalHandle ;
import API.control.web.*;
//import API.control.web.BlockFrame;
//import API.control.web.Frames;
//import API.control.web.RequestBlock;
//import API.control.web.RequestFrame;
//import API.control.web.RequestFrames;
import API.interfaces.ServerHandle;

/**
 * @author Dennis
 * Entlastet den WebServer von allen Portalfunktionalitäten
 * 
 */
public class PortalServerImpl extends Server implements PortalHandle {
//	private ManagerHandle manager = null ;
	/**
	 * Constructor for Manager object Tags are:
	 * @throws RemoteException
	 *  
	 */
	public PortalServerImpl() throws RemoteException {
		super();
	}

	// Nachfolgende Methoden bauen und verwalten das 'FrameSet'
	// (byDR)
    
	/**
	 * Description:
	 * holt einen einzelnen Block von einem per executeWebRequest() erreichbaren Server oder liefert einen Block mit Fehlermeldung zurück
	 * @param server, operation, whichblock, requestProps
	 * @return API.control.web.Block
	 * @throws RemoteException 
	 * @see 
	 */
    
	private Block getBlock(String server, String operation, String whichblock, Hashtable requestProps) throws RemoteException 
	{	// hier findet nur das reine Holen eines Blocks statt
		ServerHandle theServer = null ;
		Block theBlock = null ;
		System.out.println("==> API.components.PortalServerImpl.getBlock: Anfrage an Server '" + server + "' für BlockContent") ;
		System.out.println("  > fordere Block '" + whichblock + "' beim Server '" + server + "' mit op=" + operation + " an ...") ;
		if ((server != null) && (operation != null)) // (requestProps.get("srv") != null) && (requestProps.get("op") != null)
		{
			// passenden Server vom Manager erfragen
			System.out.println("  > frage nach Server ...") ;
			theServer = getServer(server) ;
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
		} else {
			// Fehler behandeln
			theBlock = new Block("--- falsch konfigurierter Block! (Server oder Operation = null)") ;
			// ^damit wird der Fehler mit geparst und an der entsprechenden Stelle in der Seite angezeigt
			System.out.println("--- falsch konfigurierte Block! Server oder Operation = null!\n") ;
		}
		System.out.println("<== API.components.PortalServerImpl.getBlock()") ;
		return theBlock ;
	}


//			RemoteObject ro = manager.getService(server) ;
//			if (ro != null) {
//				System.out.println("+++ Ich habe ein RemoteObject erhalten:\n\t" + ro) ;
//				if (ro.getServicetyp().indexOf("server") == 0) {
				// TODO (byDR) hier statt indexOf compareTo testen!	
//					System.out.println("+++ Ich habe einen Server erhalten!!") ;
//					theServer = ro.getServerApp() ;

//					} else {
//						System.out.println("--- Das Holen des Servers hat leider nicht geklappt!") ;
//						theBlock = new Block("--- Serveransprache ist gescheitert ...") ;
//					}
//				}				
//			} else {
//				// Fehler behandeln
//				theBlock = new Block("--- Server wurde (beim Manager) nicht gefunden ...") ;
				// ^damit wird der Fehler mit geparst und an der entsprechenden Stelle in der Seite angezeigt
//				System.out.println("--- angeforderter Server ist bei meinem Manager nicht erreichbar!!!\n") ;
//			}

	/** RequestFrames
	 * Description:
	 * liefert ein FrameSet zurück, abhängig vom Sessionzustand bzw. der verwendeten Vorlage
	 * @param server, operation, whichblock, requestProps
	 * @return API.control.web.Block
	 * @throws RemoteException 
	 * @see 
	 */ 
    
	private RequestFrames getFrames() {
		// TODO (byDR) hier wird dann später die Benutzerkonfiguration aus der Datenbank verwendet, evtl. über CLoginServer?!
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

	/**
	 * @param block, server, operation
	 */
	private RequestFrames updateFrames(String block, String server, String operation, String session) {
		System.out.println("==> API.components.PortalServerImpl.updateFrames") ;
		
		//if ()
		RequestFrames rfs = getFrames() ;
		if ((block != null) && (server != null) && (operation != null)) {
			//^ hier wird momentan nur die Standardkonf der Frames geholt
			for (int fs=0;fs < rfs.getFrameCount(); fs++) {
				RequestFrame rf = rfs.getFrame(fs) ;
				for (int f = 0;f < rf.getBlockCount(); f++) {
					RequestBlock rb = rf.getRequestBlock(f) ;
					System.out.println("  > Block: " + rb.getName() + " |Server: " + rb.getServer() + " |Operation: " + rb.getOperation()) ;
					if (rb.getName().compareTo(block) == 0) {
						rb.setServer(server) ;
						rb.setOperation(operation) ;
						System.out.println("+++ aktualisiere diesen Block mit dem letzten Link") ;
					} else {
						System.out.println("  > dieser Block war nicht zu aktualisieren") ;
					}
				}
			}
		} else {
			System.out.println("--- kein Block wurde aktualisiert aufgrund ungültiger Parameter-Übergabe.") ;
		}
		System.out.println("<== API.components.PortalServerImpl.updateFrames\n\n") ;
		
		return rfs ;
	}
    
	// diese hier geht die Frames-Struktur durch und ruft die entsprechenden Blöcke beim ProjectServer ab
	// und baut damit vorerst hart kodiert die Antwort-Seite auf 
	private Frames executeRequests(Hashtable requestProps) throws RemoteException {
		// TODO neuester Request muss den entsprechenden Block aktualisieren!
		// mehrere Blocks zu aktualisieren mit einem Reload? vl. mit b1, b2, falls b nicht als Parameter mit übergeben wurde?
		String site = null ;
		RequestFrames userFrames = updateFrames((String) requestProps.get("block"),(String)  requestProps.get("srv"),(String) requestProps.get("op"), (String) requestProps.get("session")) ;
		RequestFrame aktuellerFrame = null ;
		BlockFrame contentFrame = null ;
		Frames contentFrames = new Frames() ;
		
		int zaehler = 0 ;
		int blockzaehler = 0 ;
		System.out.println("=> API.components.PortalServerImpl.executeRequests: Abfrage der Frames von den Servern ..") ;
		
		if (userFrames != null) {
			zaehler = 0 ;
			aktuellerFrame = userFrames.getFrame(zaehler) ;
			while (aktuellerFrame != null) {
//				System.out.println("  > FrameNummer/laut Frame: " + zaehler + "/" + aktuellerFrame.getFrameNumber()) ;
				contentFrame = new BlockFrame() ;
				blockzaehler = 0 ;
				RequestBlock rb = aktuellerFrame.getRequestBlock(blockzaehler) ;
				while (rb != null) {
//					System.out.println("  > BlockNummer: " + blockzaehler) ;

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
		System.out.println("\n<== API.components.PortalServerImpl.executeRequests") ;
		return contentFrames ;
	}
    
	 /**
	 * @author Dennis
	 */
	// TODO (byDR) parseFrame() mit templates laufen lassen    
	private String parseFrames(Frames theFrames) {
		StringBuffer site = new StringBuffer() ;
		// hier kann dann ein Template eingesetzt werden mit einfachem Zahlenreplacement für die nummerierten Frames
		System.out.println("\n==> API.components.PortalServerImpl.parseFrames") ;
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

		System.out.println("\n<== API.components.PortalServerImpl.ParseFrames") ;
		return site.toString() ;
	}
	
	/* (non-Javadoc)
	 * @see API.interfaces.PortalHandle#getPortalAsHTML()
	 */
	public String getPortalAsHTML(Hashtable requestProps) throws RemoteException {
		String site = null ;
		// Hier muss das FrameSet zusammengepuzzelt werden, daher gehen die eigentlichen Aufrufe in Untermethoden.
		Frames theFrameSet = executeRequests(requestProps) ;
		// aktualisiert das UserFrameset mit dem aktuellen Request und aktualisiert
		// dann alle Blocks durch Anfragen an die zuständigen Server
		site = parseFrames(theFrameSet) ;
		return site;
	}

	/**
	 * description:
	 * TODO Sessionverwaltung mit Ansprechen des LoginServers
	 * @param usrID
	 * @return
	 * @throws RemoteException 
	 * @see projects.interfaces.CManagerServer#createSession(long)
	 */
/*	public CSession createSession(long usrID) throws RemoteException {
		RemoteObject ro = this.getService("CLoginServer") ;
		if (ro == null) {
			return null ;
		}
		CLoginServer cloginserver = (CLoginServer) ro.getServerApp() ;
		cloginserver.createSession(usrID) ;
		return null;
	} */
	
}