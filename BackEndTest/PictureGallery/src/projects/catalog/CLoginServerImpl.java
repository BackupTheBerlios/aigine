/*
 * Created on 07.09.2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package projects.catalog;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;

import projects.catalog.control.CSession;
import projects.interfaces.CDBServer;
import projects.interfaces.CLoginServer;
import projects.interfaces.CManagerServer;
import API.control.Server;
import API.model.RemoteObject;

/**
 * @author your mama
 *
 */
public class CLoginServerImpl extends Server implements CLoginServer {
	
	private CManagerServer manager;
	private CDBServer dbserver;

	/**
	 * @throws RemoteException
	 */
	public CLoginServerImpl() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * @param port
	 * @throws RemoteException
	 */
	public CLoginServerImpl(int port) throws RemoteException {
		super(port);
		// TODO Auto-generated constructor stub
	}

	/**
	 * @param port
	 * @param csf
	 * @param ssf
	 * @throws RemoteException
	 */
	public CLoginServerImpl(int port, RMIClientSocketFactory csf,
			RMIServerSocketFactory ssf) throws RemoteException {
		super(port, csf, ssf);
		// TODO Auto-generated constructor stub
	}

	/**
	 * Kreiert eine neue CSession welche von Certificat abgeleitet ist und 
	 * die verschlüsselung der user- daten uebernimmt
	 * 
	 * @see projects.interfaces.CLoginServer#createSession(java.lang.Long)
	 */
	public CSession createSession(long usrID) {
		// TODO Schauen ob der user für die session vorhanden ist
		
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
	 * @see API.interfaces.ServerHandle#registerService(API.model.RemoteObject, API.interfaces.ServerHandle)
	 */
	public String registerService(RemoteObject remoteObject) throws RemoteException {
		super.registerService(remoteObject);
		// TODO Loginserver könnte ja auch als manager dienen...
		return null;
	}

	/**
	 * description:
	 * 
	 * @return 
	 * @see projects.interfaces.CLoginServer#getManager()
	 */
	public CManagerServer getManager() {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @param manager 
	 * @see projects.interfaces.CLoginServer#setManager(projects.interfaces.CManagerServer)
	 */
	public void setManager(CManagerServer localmanager) {
		this.manager = localmanager ;
	}

}
