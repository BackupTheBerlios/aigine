/*
 * Created on 08.09.2004
 *
 * Created for RMIComponents project :Q
 */
package API.control;

import java.rmi.RemoteException;

import API.interfaces.ServerHandle;
import API.model.RemoteObject;
import API.model.RemoteObjectTable;
import API.model.RemoteObjectTree;

/**
 * 
 * 
 * class Manager.java created by
 * 
 * @author your mama or at his system created on 08.09.2004 2004 at 00:19:13
 */
public class Manager extends Server {
	//TODO RemoteObjectTree ausformulieren um eine hirachische serveice- tabelle erhalten
	//private RemoteObjectTree objectTable;
	
	/**
	 * Constructor for Manager object Tags are:
	 * 
	 * @throws RemoteException
	 *  
	 */
	public Manager() throws RemoteException {
		super();
		// TODO Ein Server der ausschliesslich andere Server (Services)
		// verwaltet
	}

	/**
	 * description:
	 * 
	 * @param compProps
	 * @param server
	 * @see API.control.Server#init(API.model.RemoteObject,
	 *      API.interfaces.ServerHandle)
	 */
	public void init(RemoteObject compProps, ServerHandle server) {
		super.init(compProps, server);
	}

	/**
	 * description: Ueberschreiben der server init um nun eine mehr- dimensionale
	 * RemoteObjectTabelle zu initialisieren 
	 * 
	 * @see API.control.Server#initObjectTable()
	 */
	public void initObjectTable() {
			
		System.out.println("=> Server.initObjectTable()");
		super.initObjectTable();
		//TODO RemoteObjectTable duch die Datenbank ersetzen oder unterstuetzen		
		System.out.println("<= Server.initObjectTable()");

	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param usr
	 * @param pass
	 * @return @throws
	 *         RemoteException
	 * @see API.control.Server#registerAdminClient(API.model.RemoteObject,
	 *      java.lang.String, java.lang.String)
	 */
	public synchronized String registerAdminClient(RemoteObject remoteObject,
			String usr, String pass) throws RemoteException {
		// TODO Anpassen der ueberschriebenen Methoden auf die object tabelle
		return super.registerAdminClient(remoteObject, usr, pass);
	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @return @throws
	 *         RemoteException
	 * @see API.control.Server#registerService(API.model.RemoteObject)
	 */
	protected synchronized String registerService(RemoteObject remoteObject)
			throws RemoteException {
		// TODO Anpassen der ueberschriebenen Methoden auf die object tabelle
		return super.registerService(remoteObject);
	}
	
	protected synchronized RemoteObject getService(String ServiceName) throws RemoteException {
		return super.getService(ServiceName) ;
	}
	
	//TODO getService() bauen mit String-bezeichnung des gewünschten Services und später dann Loadbalancing
	
	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param usr
	 * @param pass
	 * @return
	 * @throws RemoteException 
	 * @see API.control.Server#register(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public synchronized String register(RemoteObject remoteObject, String usr,
			String pass) throws RemoteException {
		// TODO Anpassen der ueberschriebenen Methoden auf die object tabelle 
		return super.register(remoteObject, usr, pass);
	}
	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @return 
	 * @see API.control.Server#registerComponent(API.model.RemoteObject)
	 */
	protected synchronized String registerComponent(RemoteObject remoteObject) {
		// TODO Anpassen der ueberschriebenen Methoden auf die object tabelle
		return super.registerComponent(remoteObject);
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}