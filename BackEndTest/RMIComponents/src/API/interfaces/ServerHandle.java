/*
 * Created on Aug 7, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.interfaces;

import java.rmi.Remote;
import java.rmi.RemoteException;

import API.model.RemoteObject;

/**
 * @author tobi
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public interface ServerHandle extends Remote{
	/**
	 * registriert einen Client ohne Anmeldung
	 * @param projects.projects.Client
	 * @return aktuelles VoteTable
	 * @throws RemoteException
	 */
	String register(RemoteObject service) throws RemoteException;

	/**
	 * registriert einen Client anhand der Userkennung mit Passwortabfrage.
	 * @param projects.projects.Client
	 * @return aktuelles VoteTable
	 * @throws RemoteException
	 */
	String register(RemoteObject remoteObject, String usr, String pass) throws RemoteException;

	/**
	 * registriert einen Admin Client anhand der Userkennung mit Passwortabfrage.
	 * @param projects.projects.Client
	 * @return aktuelles VoteTable
	 * @throws RemoteException
	 */
	String registerAdminClient(RemoteObject remoteObject, String usr, String pass) throws RemoteException;
}
