/*
 * Created on 31.10.2004
 *
 * 
 */
package API.portal;

import java.rmi.RemoteException;
import java.util.Hashtable;

import API.control.Server ;
import API.interfaces.PortalHandle ;
import API.interfaces.DbServerHandle ;
import API.interfaces.ServerHandle;
import API.portal.model.*;
import API.portal.render.*;

/**
 * @author Dennis
 * Entlastet den WebServer von allen Portalfunktionalitäten
 * 
 * Die Sessions sollten, solange sie nicht an User gebunden sind, in einem RAM-Cache gespeichert werden  
 * 
 */
public class PortalServerImpl extends Server implements PortalHandle {
	private Render theRender = null ;
	private RequestFrameSet rfs = null ; 
	// Dieses RequestFrameSet dient dem provisorischen Cachen fuer einen User 
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


	/** 
	 * Description:
	 * liefert ein FrameSet zurück, abhängig vom Sessionzustand bzw. der verwendeten Vorlage
	 * @param server, operation, whichblock, requestProps
	 * @return API.control.web.Block
	 * @throws RemoteException 
	 *  
	 * */ 
	private RequestFrameSet getFrames() throws RemoteException {
		// TODO (byDR) hier wird dann später die Benutzerkonfiguration aus der Datenbank verwendet!?
//		RequestFrameSet rfs1 = null ; 
/*		DbServerHandle theHandle = (DbServerHandle) this.getServer("PortalDBServer") ;
		if (theHandle != null) {
			// Hier wird das aktuelle FrameSet vom Portal-DBServer geholt. Gibt es dort kein aktuelles, wird auf eine Vorlage zurueckgegriffen.
			// Welche Vorlage, wird hier gleich mit konfiguriert.
			rfs = theHandle.getFrameSet("test1", "test2") ;
		} */
		
	//	if (theHandle == null || rfs == null) {
		if (this.rfs == null) {
			rfs = new RequestFrameSet() ;
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
			rf.addBlock("main2", "CProjectServer", "showimages", 1) ;
			rfs.addFrame(rf, 2) ; // Hauptbereich
			// vorerst mal überall nur die Serverinfo
			rf = new RequestFrame(3) ;
			rf.addBlock("kategorien", "CProjectServer", "kategorien", 0) ;
			rfs.addFrame(rf, 3) ; // rechter Frame
		}
		return rfs ;
	}

	
	/** 
	 * Description: Hier findet das persistente Abspeichern der Request-Zusammenstellung des aktuellen Aufrufes statt
	 * 
	 * */
	private void setFrames(String session, RequestFrameSet rfs) throws RemoteException {
		// TODO Aufrufen des db-Helpers des Portals und von dort dann Ansprache des DB-Servers
	/*	DbServerHandle theHandle = (DbServerHandle) this.getServer("PortalDBServer") ;
		if (theHandle != null) {
			// Hier wird das FrameSet an den Portal-DBServer übergeben und dort dann hoffentlich in die Datenbank geschrieben
			theHandle.saveFrameSet(rfs) ;
			
		} */
		
	}

	
	/**
	 * Diese Methode durchlaeuft das Frameset des Users und wendet die aktuellen Requestparameter darauf an.
	 * Dabei werden den einzelnen Blöcken jeweils die Server und die Operation übergeben
	 * @param block, server, operation
	 */
	private RequestFrameSet updateFrames(String block, String server, String operation, String session) throws RemoteException  {
		System.out.println("==> API.components.PortalServerImpl.updateFrames") ;
		
		//if ()
		RequestFrameSet rfs = getFrames() ;
		//^ hier wird momentan nur die Standardkonf der Frames geholt
		
		if ((block != null) && (server != null) && (operation != null)) {
			for (int fs=0;fs < rfs.getFrameCount(); fs++) {
				RequestFrame rf = rfs.getFrame(fs) ;
				for (int f = 0;f < rf.getBlockCount(); f++) {
					RequestBlock rb = rf.getRequestBlock(f) ;
//					System.out.println("  > Block: " + rb.getName() + " |Server: " + rb.getServer() + " |Operation: " + rb.getOperation()) ;
					if (rb.getName().compareTo(block) == 0) {
						rb.setServer(server) ;
						rb.setOperation(operation) ;
						// TODO
//						System.out.println("+++ aktualisiere diesen Block mit dem letzten Link") ;
					} else {
						System.out.println("  > dieser Block war nicht zu aktualisieren") ;
					}
				}
			}
		} else {
			System.out.println("--- kein Block wurde aktualisiert aufgrund ungültiger Parameter-Übergabe.") ;
		}
		// die aktualisierte Requestzusammenstellung wird hier gespeichert
		this.setFrames("test", rfs) ;
		System.out.println("<== API.components.PortalServerImpl.updateFrames\n") ;
		return rfs ;
	}

	/** 
	 * Description: diese hier geht die Frames-Struktur durch und ruft die entsprechenden Blöcke bei den gewünschten Servern ab
	 * und baut damit vorerst hart kodiert die Antwort-Seite auf 
	 * @author drichter
	 * */
	private BlockFrameSet executeRequests(Hashtable requestProps) throws RemoteException {
		// mehrere Blocks zu aktualisieren mit einem Reload? vl. mit b1, b2, falls b nicht als Parameter mit übergeben wurde?
		String site = null ;
		RequestFrame aktuellerFrame = null ;
		BlockFrame contentFrame = null ;
		BlockFrameSet contentFrames = new BlockFrameSet() ;
		
		RequestFrameSet userFrames = updateFrames((String) requestProps.get("block"),(String)  requestProps.get("srv"),(String) requestProps.get("op"), (String) requestProps.get("session")) ;
		
		int zaehler = 0 ;
		int blockzaehler = 0 ;
		System.out.println("=> API.components.PortalServerImpl.executeRequests: Abfrage der Frames von den Servern ..") ;
		
		if (userFrames != null) {
			zaehler = 0 ;
			aktuellerFrame = userFrames.getFrame(zaehler) ;
			while (aktuellerFrame != null) {
				contentFrame = new BlockFrame() ;
				blockzaehler = 0 ;
				RequestBlock rb = aktuellerFrame.getRequestBlock(blockzaehler) ;
				while (rb != null) {
					// this.getBlock() holt die Inhalte von den gewuenschten Servern
					contentFrame.addBlock(this.getBlock(rb.getServer(), rb.getOperation(), rb.getName(), requestProps)) ;
					// TODO hier müssen die Requestdaten an die Inhaltsblöcke übergeben werden, oder? Zwecks Blocksteuerung
					
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
	 * Description: Diese Methode parst unabhängig von dem gewünschten Zielmedium mit Hilfe des eingestellten Renders
	 * @author Dennis
	 * */
	private byte[] parseFrames(BlockFrameSet theFrames) {
		// Question Wie kann gleich alle Rückgabe in einem byte[] Array sammeln?
		StringBuffer site = new StringBuffer() ;
		// LATER Diese Variable (userTemplate) kann dann einfach eine andere Templatebezeichnung enthalten oder aber die Frames bzw. sogar 
		// 		Blöcke speichern für sich, welches Template angewendet werden soll
		String userTemplate = new String("std") ;

		// Der Titel kommt dann auch eher aus dem Template, sollte aber vielleicht auch als Block generiert werden
		// um unterschiedliche Titel je nach Request verwenden zu können!!
		// TODO (byDR) Titel als Block, damit CProjectServer den an den aktuellen Request anpassen kann.
		// TODO (byDR) Die Verknüpfung zwischen Frames und den jeweiligen Template-Teilen muss variabel werden, ein Weg wäre das Speichern dieser
		//		Informationen in den RequestFrames

		System.out.println("\n==> API.components.PortalServerImpl.parseFrames") ;
		
		site.append(theRender.getHead(userTemplate)) ;
		
		site.append(theRender.getFrameStart(userTemplate,1)) ;
		site.append(theRender.getRenderResult(theFrames.getFrame(0).getBlocks(), userTemplate)) ;
		site.append(theRender.getFrameEnd(userTemplate,1)) ;
		site.append(theRender.getFrameStart(userTemplate,2)) ;
		site.append(theRender.getRenderResult(theFrames.getFrame(1).getBlocks(), userTemplate)) ;
		site.append(theRender.getFrameEnd(userTemplate,2)) ;
		site.append(theRender.getFrameStart(userTemplate,3)) ;
		site.append(theRender.getRenderResult(theFrames.getFrame(2).getBlocks(), userTemplate)) ;
		site.append(theRender.getFrameEnd(userTemplate,2)) ;
		site.append(theRender.getFrameStart(userTemplate,4)) ;
		site.append(theRender.getRenderResult(theFrames.getFrame(3).getBlocks(), userTemplate)) ;
		site.append(theRender.getFrameEnd(userTemplate,2)) ;
		
		site.append(theRender.getFoot(userTemplate)) ;
		
		System.out.println("\n<== API.components.PortalServerImpl.ParseFrames") ;
		return site.toString().getBytes() ;
	}
	
	/**
	 * Description: Methode erzeugt aus einem Aufruf eine HTML-Seite als Byte-Strom, es wird der Render RenderHTML eingesetzt
	 * @author drichter
	 *  
	 * @see API.interfaces.PortalHandle#getPortalAsHTML()
	 */
	public byte[] getPortalAsHTML(Hashtable requestProps) throws RemoteException {
		// String site = null ;
		System.out.println("==> API.portal.PortalServerImpl.getPortalAsHTML()") ;
		
		this.theRender = new RenderHTML() ;
		// ^^ zum Rendern wird die Render-Klasse RenderHTML eingesetzt! 
		// Hier muss das FrameSet zusammengepuzzelt werden, daher gehen die eigentlichen Aufrufe in Untermethoden.
		BlockFrameSet theFrameSet = executeRequests(requestProps) ;
		// aktualisiert das UserFrameset mit dem aktuellen Request und aktualisiert
		// dann alle Blocks durch Anfragen an die zuständigen Server
		byte[] bytes = parseFrames(theFrameSet) ;
		System.out.println("<== API.portal.PortalServerImpl.getPortalAsHTML()") ;
		return bytes ;
	}

//	private 

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