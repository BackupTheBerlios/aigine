/*
 * Created on Aug 28, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting;

import java.rmi.RemoteException;

import projects.interfaces.VTConfigServer;
import API.control.Server;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;

/**
 * @author tobi
 *
 * Project : RMIComponents, At : Aug 28, 2004
 */
public class VTConfigServerImpl extends Server implements VTConfigServer {


	VTConfigServer configserver ;
	/**
	 * @throws RemoteException
	 */
	public VTConfigServerImpl() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub
	}
	
	public void init(RemoteObject compProps, ServerHandle server) {
			configserver = (VTConfigServer)server;
		}

	/* (non-Javadoc)
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject service) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}
	
	public synchronized String registerService(final RemoteObject remoteObject, ServerHandle service){
			return null;
		}

}
