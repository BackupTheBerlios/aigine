/*
 * Created on Aug 11, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.interfaces;

import java.rmi.RemoteException;
import java.util.Vector;

import projects.voting.model.DBVote;
import API.interfaces.ServerHandle;

/**
 * @author tobi
 *
 * Project : RMIComponents, At : Aug 11, 2004
 */
public interface VTDatabaseServer extends ServerHandle {

	/**
		 * Methode zum speichern der daten. Nimmt einen Vector mit DBVortes entgegen
		 * und speichert sie einzeln in der Datenbank
		 *@param Vector contains DBVotes
		 */
	void storeVotes(Vector data) throws RemoteException;
	
	/**
	 *  Methode zum speichern der daten. 
	 * @param vote Datenbank Votes (mit extra parameter)
	 */
	void storeVote(DBVote vote) throws RemoteException;

}