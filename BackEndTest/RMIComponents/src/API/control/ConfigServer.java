/*
 * Created on Aug 28, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.control;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;

import org.jconfig.server.ConfigurationServer;

/**
 * @author tobi
 *
 * Project : RMIComponents, At : Aug 28, 2004
 */
public abstract class ConfigServer extends Server {

	
	/**
	 * @throws RemoteException
	 */
	public ConfigServer() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * @param port
	 * @throws RemoteException
	 */
	public ConfigServer(int port) throws RemoteException {
		super(port);
		// TODO Auto-generated constructor stub
	}

	/**
	 * @param port
	 * @param csf
	 * @param ssf
	 * @throws RemoteException
	 */
	public ConfigServer(
		int port,
		RMIClientSocketFactory csf,
		RMIServerSocketFactory ssf)
		throws RemoteException {
		super(port, csf, ssf);
		// TODO Auto-generated constructor stub
	}
	
	protected void init(int port, String root) {
		
		ConfigurationServer configServer = new ConfigurationServer(port, root);
		
	}

}
