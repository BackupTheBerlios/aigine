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
	}

	/**
	 * @param port
	 * @throws RemoteException
	 */
	public Database(int port) throws RemoteException {
		super(port);
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
	}

	/* Register Methode des Servers mit Authentifizierung
	 * @see API.control.Server#register(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public synchronized String register(
		RemoteObject remoteObject,
		String usr,
		String pass)
		throws RemoteException {
		return super.register(remoteObject, usr, pass);
	}

	/* Register Methode des Servers als anonnymer client
		 * @see API.control.Server#register(API.model.RemoteObject, java.lang.String, java.lang.String)
		 */
	public synchronized String register(RemoteObject remoteObject) {
		return super.registerComponent(remoteObject);
	}
	
}
