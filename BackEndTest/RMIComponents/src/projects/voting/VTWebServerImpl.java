/*
 * Created on 08.08.2004
 * File: package projects.voting;.VTWebServer.java
 */
package projects.voting;

import java.rmi.RemoteException;

import projects.interfaces.VTClient;
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
public class VTWebServerImpl extends WebServer implements VTClient{
	private final int default_port= 8080;
	private final String default_root= "public_html";
    /* (non-Javadoc)
     * @see projects.interfaces.VTClient#update(projects.voting.model.VoteTable)
     */
    public void update(VoteTable votes) throws RemoteException {
        // TODO Ausgabe der Votes
        
    }
	/**
	 * Starts a web server with the deafult port and the default document root.
	 * @see API.interfaces.Client#init(API.model.RemoteObject, API.interfaces.ServerHandle)
	 */
	public void init(RemoteObject compProps, ServerHandle server)
		throws RemoteException {
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
}
