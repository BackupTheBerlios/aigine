/*
 * Created on 08.08.2004
 *
 */
package API.components;

import java.rmi.RemoteException;
import java.util.Hashtable;

import projects.interfaces.CClient;

import API.interfaces.PortalHandle ;
import API.control.WebServer;
//import API.control.web.Block;
//import API.control.web.BlockFrame;
//import API.control.web.Frames;
//import API.control.web.RequestBlock;
//import API.control.web.RequestFrame;
//import API.control.web.RequestFrames;
//import API.interfaces.ServerHandle;
import API.interfaces.ManagerHandle;
import API.model.RemoteObject;

/**
 * 
 * @author danny, Dennis
 * @since 08.08.2004 17:06:01
 * @version 0.1.1
 */

// TASK Portal erreichen ohne Dummydatei, nur durch Parameterübergabe im Browser (domain.de?srv=CProjectserver)

public class WebServerImpl extends WebServer implements CClient {
    private final int default_port= 8080;
    private final String default_root= "public_html" ;
    private final String default_template_root = "template_html" ;
    private ManagerHandle manager = null;

    
    /**
     * Starts a web server with the default port and the default document root.
     * @see API.interfaces.Client#init(API.model.RemoteObject, API.interfaces.ServerHandle)
     */
    public void init(RemoteObject compProps, ManagerHandle server) throws RemoteException {
		// TODO zusätzlicher Parameter für Verzeichnisse und Port des Webservers
		// TODO (byDR) Verzeichnisse auf Existenz überprüfen.
		
		System.out.println("==> CWebServerImpl.init(" + "RemoteObjectProperties "
                + compProps  + ", Object" + manager);
                
  		try {
			this.manager = server; // (CManagerServer)
			init(default_port, default_root);	
  		} catch (Exception e) {
			e.printStackTrace();
			System.out.println(
				"Fehler in BootStrapedApplicationStarter.runClient() > UUHUHUHUHU >> "
					+ e.fillInStackTrace()
					+ "\n"
					+ e.getMessage());
		}

        System.out.println("<== WebServer.init(" + "RemoteObjectProperties "
                + compProps + " , Object" + manager);
    }
    
      
    /**
     * Führt die jeweilige Aktion, abhängig von dem übergebenen Property "op" aus.
     * ab 20041107:
     * 		Methode ist reduziert auf Aufruf des PortalServer und Ausgabe dessen
     * 		byte[]-Rückgabe
     *
     * @see API.control.WebServer#getActionBody(java.lang.String[])
     */
    // TODO (byDR): Diese Methode muss erweitert werden für den Fall einer übergebenen Datei (Upload)
    //				oder dies wird vom WebServer gleich in zwei getrennten Methoden abgehandelt
    // TODO (byDR): Aufruf auch ohne vorangestellte Datei muss möglich sein, dazu muss
    // 				die eigentliche Webserver-Klasse geändert werden.
    protected byte[] getActionBody(String[] request, Hashtable requestProps) throws RemoteException {
        /* Diese Methode könnte auch erweitert werden, damit sie nur bei bestimmten 
         * Aufrufformen (Parametern) an den Portalserver geht und ansonsten andere
         * Aufrufe anders weiterleitet, ist aber für IM nicht erforderlich. */
        byte[] site = new String("leerer Durchlauf").getBytes() ;
        PortalHandle theServer = null ;
		// wichtigste Faehigkeit des WebServers
		System.out.println("==> API.components.WebServerImpl.getActionBody: Stelle Anfrage an den Portalserver ...") ;

		RemoteObject ro = manager.getService("PortalServer") ;
		if (ro != null) {
			if (ro.getServicetyp().indexOf("server") == 0) {
			// TODO (byDR) hier statt indexOf compareTo testen!	
				System.out.println("+++ Ich habe einen Server erhalten!!") ;
				theServer = (PortalHandle) ro.getServerApp() ;
				if (theServer != null) {
					site = theServer.getPortalAsHTML(requestProps) ;
				} else {
					// Fehler behandeln
					site = new String("--- PortalServer wurde (beim Manager) nicht gefunden ...").getBytes() ;
					// ^damit wird der Fehler mit geparst und als Ausgabe des WebServers angezeigt.
					System.out.println("--- angeforderter PortalServer ist bei meinem Manager nicht erreichbar!!!\n") ;
				}
			} else {
				// Fehler behandeln
				site = new String("--- Handle ist nicht vom Typ 'server'!").getBytes() ;
				// ^damit wird der Fehler mit geparst und als Ausgabe des WebServers angezeigt.
				System.out.println("--- Übergebenes RemoteObject ist nicht vom benötigten Typ 'server'!\n") ;
			}
		} else {
			// Fehler behandeln
			site = new String("--- Handle ist null!").getBytes() ;
			// ^damit wird der Fehler mit geparst und als Ausgabe des WebServers angezeigt.
			System.out.println("--- Übergebenes RemoteObject ist null!\n") ;
		}

		System.out.println(">>> site (am Ende von getActionBody) = " + site) ;
		System.out.println("<== API.components.WebServerImpl") ;
		return site ;
        // return site.getBytes();
    }
    
	/**
	 * @return Returns the manager.
	 */
	public ManagerHandle getManager() {
		//TODO ist das notwendig?
		return manager;
	}

	/**
	 * @param manager The manager to set.
	 */
	public void setManager(ManagerHandle manager) {
		this.manager = manager;
	}
}
