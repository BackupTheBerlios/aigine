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
//		Vote dbvote = new Vote();
		//System.out.println("Vote wird mal vorgeladen... mit der id "+ dbvote.getId());		
		
		//cfg = new Configuration();
//		URL url = null;
//		try {
//			url = new URL(sessionConfigURL);
//			System.out.println("reding the first url--------------------------1");
//			cfg = cfg.addURL(url);			
//		} catch (MappingException e1) {
//			System.out.println(
//			"\n\n<<<<<<<<<<<<<<<<<<your mamas foult>>>>>>>>>>>>>>>>>>>\n\n" +//			"the given path : " + url.getPath()+" | given port:"+url.getPort()+" | given file "+url.getFile());
//			System.out.println(
//				"Fehler in "
//					+ this.getClass().getName()
//					+ " => "
//					+ e1.getMessage());			
//		} catch (MalformedURLException e1) {
//			System.out.println(
//				"Fehler in "
//					+ this.getClass().getName()
//					+ " => "
//					+ e1.getMessage());
//		}
		
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
