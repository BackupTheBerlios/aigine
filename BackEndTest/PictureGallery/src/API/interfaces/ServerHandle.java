/*
 * Created on Aug 7, 2004
 *
 */
package API.interfaces;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.Hashtable;

import API.model.RemoteObject;
import API.portal.model.Block;

/**
 * @author tobi
 * Dieses Interface dient für alle 'normalen' Server, die selber nicht managen
 * sollen.
 */
public interface ServerHandle extends Remote {
	
    /**
     * registriert einen Admin Client anhand der Userkennung mit Passwortabfrage.
     * TODO ist das sinnvoll fuer einen normalen Server??
     * @param projects.projects.Client
     * @return aktuelles VoteTable
     * @throws RemoteException
     */
    String registerAdminClient(
        RemoteObject remoteObject,
        String usr,
        String pass)
        throws RemoteException;

	/**
	* Setzt den zuständigen Manager des Servers.
	* @param ManagerHandle
	*/
//	public void setManager(ManagerHandle theManager) throws RemoteException ;

	/**
	* verarbeitet einen 'WebRequest' der momentan vom Webserver an beliebige andere Server geschickt werden kann
	* @author Dennis
	* @since 11.09.2004
	* @param Operation, Parameterpaare und evtl mit übertragenen Body
	* @return String
	* @throws RemoteException
	*/
	public Block executeWebRequest(String op, String whichBlock, Hashtable requestProps) throws RemoteException ;
	
	/**
	* liefert Fehlerdetails, wenn bei executeWebRequest ein Fehler auftritt
	* @author Dennis
	* @since 11.09.2004
	* @param keine
	* @return String
	* @throws RemoteException
	*/
	public String getWebRequestError() throws RemoteException ;
}
