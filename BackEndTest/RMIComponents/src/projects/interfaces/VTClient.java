package projects.interfaces;

import java.rmi.RemoteException;

import projects.voting.model.VoteTable;
import API.interfaces.Client;

/**
 * Interface für einen VTClient.
 * @author danny
 * @since 05.05.2004 20:12:54
 */
public interface VTClient extends Client {
	/**
	 * Gibt die aktuellen Votings an den Client weiter.
	 * @param votes
	 * @throws RemoteException
	 */
	void update(VoteTable  votes) throws RemoteException;
	
}
