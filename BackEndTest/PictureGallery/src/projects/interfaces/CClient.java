package projects.interfaces;

import java.rmi.RemoteException;

import API.interfaces.Client;

/**
 * Interface für einen CClient.
 * @author danny
 * @since 05.05.2004 20:12:54
 */
public interface CClient extends Client {
	/**
	 * 
	 */
	//void update(VoteTable  votes) throws RemoteException;
	CManagerServer manager = null;

	
	void setManager(CManagerServer manager) throws RemoteException;
	
	CManagerServer getManager() throws RemoteException;
}
