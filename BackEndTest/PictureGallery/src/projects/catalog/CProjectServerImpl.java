/*
 * Created on 10.09.2004
 *
 * Created for PictureGallery project :Q
 */
package projects.catalog;

import java.rmi.RemoteException;

import projects.interfaces.CProjectServer;
import projects.interfaces.CManagerServer;

import API.control.Server ;
import API.model.RemoteObject;
import API.model.RemoteObjectTable;

/**
 * 
 * 
 * class CProjectServerImpl.java created by @author your mama or at his system
 * created on 10.09.2004 2004 at 14:52:37 
 */
public class CProjectServerImpl extends Server implements CProjectServer {
	private CManagerServer manager;
	/**
	 * Constructor for CProjectServerImpl object
	 * 
	 * 
	 */
	public CProjectServerImpl() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * description:
	 * 
	 * @param service
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject service) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param usr
	 * @param pass
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public String register(RemoteObject remoteObject, String usr, String pass)
			throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param usr
	 * @param pass
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#registerAdminClient(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public String registerAdminClient(RemoteObject remoteObject, String usr,
			String pass) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#registerService(API.model.RemoteObject)
	 */
	public String registerService(RemoteObject remoteObject)
			throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#checkClientConnections()
	 */
	public RemoteObjectTable checkClientConnections() throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @return 
	 * @see projects.interfaces.CLoginServer#getManager()
	 */
	public CManagerServer getManager() {
		return manager;
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
