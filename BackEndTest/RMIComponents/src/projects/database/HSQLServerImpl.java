/*
 * Created on Aug 11, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.database;

import java.rmi.RemoteException;

import org.hsqldb.Server;

import projects.interfaces.HSQLServer;

import API.model.RemoteObject;
import API.model.RemoteObjectTable;

/**
 * @author tobi
 *
 * Project : RMIComponents, At : Aug 11, 2004
 */
public class HSQLServerImpl implements HSQLServer {

	/**
	 * dev constructor
	 */
	public HSQLServerImpl() throws RemoteException {
		System.out.println("=>HSQLServerImpl.constructor\n");
		System.out.println("HSQLServer starten mit der in der properties spezialiseierten Datenbank");
		Server hsqlserver = new Server();
		System.out.println("\n<= HSQLServerImpl.constructor\n");
	}
	
	

	/**
	 * Test Main klasse
	 * @param args
	 * HSQLServerImpl.java
	 */
	public static void main(String[] args) {
	}



	/* (non-Javadoc)
	 * @see projects.interfaces.HSQLServer#runDatabase()
	 */
	public void runDatabase() {
		// TODO implementieren der runDatabase in HSQLServerImpl
		
	}



	/* (non-Javadoc)
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject service, String user, String passwd) throws RemoteException {
		
		// TODO Register methode mit der sich der Datenbank -server bei einem Manager Registriert
		return null;
	}



	/* (non-Javadoc)
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject service) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}



	/* (non-Javadoc)
	 * @see API.interfaces.ServerHandle#registerAdminClient(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public String registerAdminClient(RemoteObject remoteObject, String usr, String pass) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}



	/* (non-Javadoc)
	 * @see API.interfaces.ServerHandle#checkClientConnections()
	 */
	public RemoteObjectTable checkClientConnections() throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}
}
