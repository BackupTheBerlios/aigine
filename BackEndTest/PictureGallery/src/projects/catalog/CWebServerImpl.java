/*
 * Created on 08.08.2004
 * File: package projects.voting;.VTWebServer.java
 */
package projects.catalog;

import java.rmi.RemoteException;
import java.util.Hashtable;

import projects.catalog.model.Picture;
import projects.interfaces.CClient;
import projects.interfaces.CLoginServer;
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
    private final String default_root= "public_html";
    //private CLoginServer loginserver = null;
    private CManagerServer manager = null;
    private Picture picture;
    
//    /* (non-Javadoc)
//     * @see projects.interfaces.CClient#update(projects.voting.model.VoteTable)
//     */
//    public void update(VoteTable votes) throws RemoteException {
//        // TODO Ausgabe der Votes
//        this.votes= votes;
//    }
    /**
     * Starts a web server with the deafult port and the default document root.
     * @see API.interfaces.Client#init(API.model.RemoteObject, API.interfaces.ServerHandle)
     */
    public void init(RemoteObject compProps, ServerHandle server)
        throws RemoteException {
        	this.manager = (CManagerServer)server;       
        // TODO zusätzlicher Paramter für Verzeichnis und Port des Webservers 
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
     * Führt die jeweilige Aktion, abhängig von dem übergebenen Property "op" aus.
     * @see API.control.WebServer#getActionBody(java.lang.String[])
     */
    protected byte[] getActionBody(String[] request, Hashtable requestProps) throws RemoteException {
        String site= new String();

        if (requestProps.get("op").equals("login")) {
            site = actionLogin(requestProps);
            System.out.println("LOGIN from user : " + requestProps.get("name"));
        } else if (requestProps.get("op").equals("galary")) {
        	site = actionGalary(requestProps);
        }
        return site.getBytes();
    }
//    /**
//     * Führt ein Voting aus.
//     * @param requestProps
//     * @return
//     */
    private String actionGalary(Hashtable requestProps) throws RemoteException {
		StringBuffer site= new StringBuffer();
		site.append(
			"\n<h1>request for , "
				+ requestProps.get("pic_request")
				+ " !</h1>\n");
		//loginserver.webvote((String) requestProps.get("voteradio"));
		//site.append(votes.toHTML());
        return site.toString();
    }
    
    /**
     * EInloggen des Users
     * @param requestProps
     * @return
     */
    private String actionLogin(Hashtable requestProps) {
    	// TODO "login" vom Server aufrufen und prüfen ob der User registriet ist.
        StringBuffer site= new StringBuffer();
        long usrID = Integer.parseInt((String) requestProps.get("name"));
        try {
        	manager.createSession(usrID);
		} catch (RemoteException e) {
			
			System.out.println("Error: exception type: RemoteException" +
					"with the message: \n "+e.getMessage());
			
		}
        
        
		site.append(
			 "\n<h1>Tschesch Kollega , "
				 + requestProps.get("name")
				 + " !</h1>\n");
		 site.append("\n<form action='galary.html' method='GET'>");
		 site.append("\n<input type='hidden' name='op' value='galary'>");
		 site.append(picture.toHtml());
		 site.append("\n<input type='submit' value='wählen'>");
		 site.append("\n</form>");
        return site.toString();
    }
	
	
	/**
	 * @return Returns the manager.
	 */
	public CManagerServer getManager() {
		return manager;
	}
	/**
	 * @param manager The manager to set.
	 */
	public void setManager(CManagerServer manager) {
		this.manager = manager;
	}
}
