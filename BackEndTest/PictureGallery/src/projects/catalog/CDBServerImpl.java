/*
 * Created on 08.09.2004
 *
 * Created for PictureGallary project :Q
 */
package projects.catalog;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;

import API.control.Database;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;

/**
 * 
 * 
 * class CDBServerImpl.java created by @author your mama or at his system
 * created on 08.09.2004 2004 at 14:14:46 
 */
public class CDBServerImpl extends Database implements ServerHandle {

	//TODO initialiserit die voting.control.HelperDatabasePersistence
	
	
	
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
		// TODO gegister methode mit usr and passwd (for adminClients mybe)
		String regresult = super.register(remoteObject, usr, pass); 
		return regresult ;
	}
	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @return 
	 * @see API.control.Database#register(API.model.RemoteObject)
	 */
	public synchronized String register(RemoteObject remoteObject) {
		// TODO Auto-generated method stub
		return super.register(remoteObject);
	}
	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @return 
	 * @see API.control.Server#registerComponent(API.model.RemoteObject)
	 */
	protected synchronized String registerComponent(RemoteObject remoteObject) {
		// TODO Auto-generated method stub
		return super.registerComponent(remoteObject);
	}
	/**
	 * Constructor for CDBServerImpl object
	 * @throws RemoteException
	 * 
	 */
	public CDBServerImpl() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub
	}
	/**
	 * Constructor for CDBServerImpl object
	 * @param port
	 * @throws RemoteException
	 * 
	 */
	public CDBServerImpl(int port) throws RemoteException {
		super(port);
		// TODO Auto-generated constructor stub
	}
	/**
	 * Constructor for CDBServerImpl object
	 * @param port
	 * @param csf
	 * @param ssf
	 * @throws RemoteException
	 * 
	 */
	public CDBServerImpl(int port, RMIClientSocketFactory csf,
			RMIServerSocketFactory ssf) throws RemoteException {
		super(port, csf, ssf);
		// TODO Auto-generated constructor stub
	}
	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param service
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#registerService(API.model.RemoteObject, API.interfaces.ServerHandle)
	 */
	public String registerService(RemoteObject remoteObject) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

 
	
	
}
