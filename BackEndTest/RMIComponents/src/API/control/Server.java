package API.control;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;
import java.rmi.server.UnicastRemoteObject;

import API.model.RemoteObject;
import API.model.RemoteObjectTable;

/**
 * Implementierung häufig benötiger Serverfunktionalitäten, z.B. Anmeldung von Clients.
 * @author danny, tobi
 * @since 25.07.2004
 * @version 0.01
 */

public class Server extends UnicastRemoteObject {
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
	 * Registrierung eines RemoteObjects für anonyme Kommunikation.
	 * @param service
	 * @throws RemoteException
	 */
	public synchronized String register(final RemoteObject remoteObject)
		throws RemoteException {
		System.out.println("=> Server.register(RemoteObject " + remoteObject + ")");
		String status = null;
		if (remoteObjects.contains(remoteObject)) {
			status = " exists";
		} else {
			remoteObjects.add(remoteObject);
			status = " now exists";
		}
		System.out.println("<=Server.register(RemoteObject " + remoteObject + ")");
		return status;
	}

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
			status = "tschesch kollega :D  => " + this.register(remoteObject);
		} else {
			status = "USERNAME oder PASSWORD falsch ! dat war wohl nix :D";
		}

		System.out.println("<= Server.register(mit Password)");
		return status;
	}
}
