package API.control;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;
import java.rmi.server.UnicastRemoteObject;

import API.interfaces.AdminClient;
import API.model.RemoteObject;
import API.model.RemoteObjectTable;

/**
 * Implementierung häufig benötiger Serverfunktionalitäten, z.B. Anmeldung von Clients.
 * @author danny, tobi
 * @since 25.07.2004
 * @version 0.01
 */

public abstract class Server extends UnicastRemoteObject {
	/**
	 * Die registrierten Kommunikationspartner
	 * entweder Clients oder Services oder wiederum Server ... WAT EVER !
	 */
	protected RemoteObjectTable remoteObjects = null;

	/**
	 * @throws java.rmi.RemoteException
	 */
	public Server() throws RemoteException {
		super();
	}

	/**
	 * @param port
	 * @throws java.rmi.RemoteException
	 */
	public Server(int port) throws RemoteException {
		super(port);
	}

	/**
	 * @param port
	 * @param csf
	 * @param ssf
	 * @throws java.rmi.RemoteException
	 */
	public Server(
		int port,
		RMIClientSocketFactory csf,
		RMIServerSocketFactory ssf)
		throws RemoteException {
		super(port, csf, ssf);
	}

	/**
	 * Initialisiert die Tabelle mit den Kommunikationspartnern.
	 */
	public void initObjectTable() {
		remoteObjects = new RemoteObjectTable();
	}

	/**
	 * Registrierung eines RemoteObjects, wird von allen anderen register() Methoden
	 * verwendet, um eine mögliche Überblendung zu vermeiden. 
	 * @param service
	 * @throws RemoteException
	 */
	protected synchronized String registerComponent(final RemoteObject remoteObject){
		System.out.println("=> Server.registerComponent(RemoteObject " + remoteObject + ")");
		String status = null;
		if (remoteObjects.contains(remoteObject)) {
			status = " exists";
		} else {
			remoteObjects.add(remoteObject);
			status = " now exists";
		}
		System.out.println("<=Server.registerComponent(RemoteObject " + remoteObject + ")");
		return status;
	}
	
	/**
	 * Registrierung eines RemoteObjects für anonyme Kommunikation.
	 * @param service
	 * @throws RemoteException

	public synchronized String register(final RemoteObject remoteObject)
		throws RemoteException {
		System.out.println("=> Server.register(RemoteObject " + remoteObject + ")");
		System.out.println("TROTTEL, die Methode mußt du doch überschreiben.");
		System.out.println("<= Server.register(RemoteObject " + remoteObject + ")");
		return "überschreiben oder registerComponent nutzen.";
	}
	*/

	/**
	 * Registrierung eines RemoteObjects mit Username und Password.
	 * @param remoteObject
	 * @param username
	 * @param password
	 * @throws RemoteException
	 */
	public synchronized String register(
		final RemoteObject remoteObject,
		final String usr,
		final String pass)
		throws RemoteException {
		System.out.println(
			"=> Server.register(remotObject, usrname ="
				+ usr
				+ " , password = "
				+ pass
				+ ")");
		String status = null;

		// TODO Testdaten durch Zugriff auf Datenspeicher ersetzem (Usertabelle in DB)!
		if (usr.equals("nice") && pass.equals("yourMama")) {
			status = "tschesch kollega :D  => " + this.registerComponent(remoteObject);
		} else {
			status = "USERNAME oder PASSWORD falsch ! dat war wohl nix :D";
		}

		System.out.println("<= Server.register(mit Password) > " + status);
		return status;
	}
	
	/**
	 * Registrierung eines Remote Admin Objects mit Username und Password.
	 * Wird vom Loader aufgerufen, wenn <code>typ=admin</code>.
	 * @param remoteObject
	 * @param username
	 * @param password
	 * @throws RemoteException
	 */
	public synchronized String registerAdminClient(
		final RemoteObject remoteObject,
		final String usr,
		final String pass)
		throws RemoteException {
		System.out.println(
			"=> Server.registerAdminClient(remotObject, usrname ="
				+ usr
				+ " , password = "
				+ pass
				+ ")");
			
			String status = register(remoteObject, usr, pass);
			System.out.println("\t registriert HOFFENTLICH :D > status:"
				 + status);
			// Daten an Client schicken
			AdminClient app = (AdminClient)remoteObject.getApp();
			System.out.println("\t try update client call > " + app);
			app.update(remoteObjects);
			System.out.println("\t updated client call > " + app);
		return status;
	}	
}
