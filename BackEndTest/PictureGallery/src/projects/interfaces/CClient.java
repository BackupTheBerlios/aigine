package projects.interfaces;

import API.interfaces.ManagerHandle;
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
	ManagerHandle manager = null;

	
	void setManager(ManagerHandle manager) throws RemoteException;
	
	ManagerHandle getManager() throws RemoteException;
}
