/*
 * Created on Aug 7, 2004
 *
 */
package API.interfaces;

import java.rmi.Remote;
import java.rmi.RemoteException;
import java.util.Hashtable;

import API.control.web.Block;
import API.model.RemoteObject;
import API.model.RemoteObjectTable;

/**
 * @author tobi
 *
 */
public interface ServerHandle extends Remote {
	


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
    String register(RemoteObject remoteObject, String usr, String pass)
        throws RemoteException;

    /**
     * registriert einen Admin Client anhand der Userkennung mit Passwortabfrage.
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
    * registriert einen Service
    * @param projects.projects.Client
    * @return String
    * @throws RemoteException
    */
    String registerService(RemoteObject remoteObject) throws RemoteException;

    /**
     * Gibt die aktuelle Tabelle der registrierten Komponenten zurück.
     * TODO was geht das die Aussenwelt ausser einen Administrator an?
     * @author danny
     * @since 10.08.2004 02:15:12
     * @version 0.01
     */
    public RemoteObjectTable checkClientConnections() throws RemoteException;
    
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
