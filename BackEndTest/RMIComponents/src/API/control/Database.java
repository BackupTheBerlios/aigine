/*
 * Created on Aug 11, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.control;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;

import API.model.RemoteObject;

/**
 * @author tobi
 *
 * Project : RMIComponents, At : Aug 11, 2004
 */
public abstract class Database extends Server {

	/**
	 * @throws RemoteException
	 */
	public Database() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * @param port
	 * @throws RemoteException
	 */
	public Database(int port) throws RemoteException {
		super(port);
		// TODO Auto-generated constructor stub
	}

	/**
	 * @param port
	 * @param csf
	 * @param ssf
	 * @throws RemoteException
	 */
	public Database(
		int port,
		RMIClientSocketFactory csf,
		RMIServerSocketFactory ssf)
		throws RemoteException {
		super(port, csf, ssf);
		// TODO Auto-generated constructor stub
	}
	
	
	
	/* (non-Javadoc)
	 * @see API.control.Server#register(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public synchronized String register(
		RemoteObject remoteObject,
		String usr,
		String pass)
		throws RemoteException {
		// TODO Auto-generated method stub
		return super.register(remoteObject, usr, pass);
	}

}
