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
        // TODO Verzeichnisse auf Existenz überprüfen.  
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
    /**
     * durch Dennis: Diese Methode ist im CWebServerImpl allgemein für Manager gelöst ..
     * Führt die jeweilige Aktion, abhängig von dem übergebenen Property "op" aus.
     * @see API.control.WebServer#getActionBody(java.lang.String[])
     */
    protected byte[] getActionBody(String[] request, Hashtable requestProps) throws RemoteException {
        String site = new String("leerer Durchlauf") ;
        ServerHandle theServer = null ;
		// wichtigste Faehigkeit des WebServers
		System.out.println("=> projects.catalog.CWebServerImpl: versuche Aktion an entfernten Server zu übergeben ...") ;
		
		if ((requestProps.get("srv") != null) && (requestProps.get("op") != null))
		{
			// passenden Server vom Manager erfragen
			RemoteObject ro = manager.getService((String) requestProps.get("srv")) ;
			if (ro != null) {
				System.out.println("Ich habe ein RemoteObject erhalten:\n\t" + ro) ;
				if (ro.getServicetyp().indexOf("server") == 0) {
					System.out.println("Ich habe sogar einen Server erhalten!!\n") ;
					theServer = ro.getServerApp() ;
//					theServer = ro.gettheServer() ;
					System.out.println("Ich sende nun den WebRequest an: \n" + theServer) ;
					if (theServer != null) {
						site = theServer.executeWebRequest((String) requestProps.get("op"), requestProps) ;
						System.out.println("Ich schaue nun ob der WebRequest etwas sinnvolles enthält ...\n") ;
						if (site == null) {
							// Fehler behandeln
							System.out.println("Da scheint etwas schief gegangen zu sein .. [site == null]") ;
							site = new String ("schwerer Fehler aufgetreten ...\n\t" + theServer.getWebRequestError() + "\n") ;
						}
					} else {
						System.out.println("Das Holen des Servers hat leider nicht geklappt!") ;
					}
					// TODO selbst wenn das soweit läuft, was passiert mit einer hochzuladenen Datei?
					// Wie gelangt diese zum veranwortlichen Server?
				}				
			} else {
				// Fehler behandeln
				site = new String ("Server nicht gefunden ...") ;
				System.out.println("\n<<< >>>\nangeforderter Server ist bei meinem Manager nicht erreichbar!!!\n<<< >>>\n") ;
			}
		}
		System.out.println("\tsite (am Ende von getActionBody) = " + site) ;
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
