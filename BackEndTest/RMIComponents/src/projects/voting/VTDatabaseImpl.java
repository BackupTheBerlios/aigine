/*
 * Created on 15.08.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting;

import java.rmi.RemoteException;

import projects.interfaces.HSQLServer;
import API.control.Database;
import API.model.RemoteObject;

/**
 * @author tobi
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class VTDatabaseImpl extends Database implements HSQLServer  {

	/**
	 * 
	 */
	public VTDatabaseImpl() throws RemoteException{
		System.out.println("=>VTDatabaseImpl.constructor\n");
		System.out.println("HSQLServer starten mit der in der properties spezialiseierten Datenbank");
		//Server hsqlserver = new Server();
		System.out.println("\n<= VTDatabaseImpl.constructor\n");
	}

	/* (non-Javadoc)
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject service) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

}
