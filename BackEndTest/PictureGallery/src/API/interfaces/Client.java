/*
 * Created on Aug 7, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.interfaces;

import java.rmi.RemoteException;

import API.model.RemoteObject;

/**
 * Interface für den Zugriff auf eine Koponente vom Typ Client.
 * @author danny, tobi
 * @since 25.07.2004 16:54:02
 * @version 0.01
 */
public interface Client extends Application {

  /**
   * Inittialisierung mit Übergabe des Server Objektes.
   * @param Eigenschaften der Komponente
   * @param Serverobjekt
   */
	public void init(RemoteObject compProps, ServerHandle server) throws RemoteException;
}
