/*
 * Created on 08.08.2004
 * File: package projects.voting;.VTWebServer.java
 */
package projects.voting;

import java.rmi.RemoteException;
import java.util.Hashtable;

import projects.interfaces.VTClient;
import projects.interfaces.VTServer;
import projects.voting.model.VoteTable;
import API.control.WebServer;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;

/**
 * 
 * @author danny
 * @since 08.08.2004 17:06:01
 * @version 0.01
 */
public class VTWebServerImpl extends WebServer implements VTClient {
    private final int default_port= 8080;
    private final String default_root= "public_html";
    private VoteTable votes= null;
    private VTServer server = null;
    
    /* (non-Javadoc)
     * @see projects.interfaces.VTClient#update(projects.voting.model.VoteTable)
     */
    public void update(VoteTable votes) throws RemoteException {
        // TODO Ausgabe der Votes
        this.votes= votes;
    }
    /**
     * Starts a web server with the deafult port and the default document root.
     * @see API.interfaces.Client#init(API.model.RemoteObject, API.interfaces.ServerHandle)
     */
    public void init(RemoteObject compProps, ServerHandle server)
        throws RemoteException {
        	this.server = (VTServer)server;       
        // TODO zusätzlicher Paramter für Verzeichnis und Port des Webservers 
        System.out.println(
            "=> WebServer.init("
                + "RemoteObjectProperties "
                + compProps
                + ", Object"
                + server);

        init(default_port, default_root);

        System.out.println(
            "<= WebServer.init("
                + "RemoteObjectProperties "
                + compProps
                + " , Object"
                + server);
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
        } else if (requestProps.get("op").equals("vote")) {
        	site = actionVote(requestProps);
        }
        return site.getBytes();
    }
    /**
     * Führt ein Voting aus.
     * @param requestProps
     * @return
     */
    private String actionVote(Hashtable requestProps) throws RemoteException {
		StringBuffer site= new StringBuffer();
		site.append(
			"\n<h1>vote for , "
				+ requestProps.get("voteradio")
				+ " !</h1>\n");
		server.webvote((String) requestProps.get("voteradio"));
		site.append(votes.toHTML());
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
		site.append(
			 "\n<h1>Tschesch Kollega , "
				 + requestProps.get("name")
				 + " !</h1>\n");
		 site.append("\n<form action='vote.html' method='GET'>");
		 site.append("\n<input type='hidden' name='op' value='vote'>");
		 site.append(votes.toHTML());
		 site.append("\n<input type='submit' value='wählen'>");
		 site.append("\n</form>");
        return site.toString();
    }
}
