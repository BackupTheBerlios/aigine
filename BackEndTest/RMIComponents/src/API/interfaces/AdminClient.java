/*
 * Created on Aug 7, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.interfaces;

import java.rmi.RemoteException;

import API.model.RemoteObjectTable;

/**
 * Interface für den Zugriff auf eine Koponente vom Typ AdminClient.
 * @author danny, tobi
 * @since 25.07.2004 16:54:02
 * @version 0.01
 */
public interface AdminClient extends Client {

	/**
	 * Gibt die aktuellen Tabelle der RemoteObjects an den Client weiter.
	 * @param remote objects
	 * @throws RemoteException
	 */
	void update(RemoteObjectTable  remoteObjects) throws RemoteException;}
