/*
 * Created on 05.05.2004
 *
 */
package projects.interfaces;

import java.rmi.RemoteException;

import projects.voting.model.VoteTable;

import API.interfaces.ServerHandle;

/**
 * Interface für einen VTServer.
 * @author danny
 * @since 05.05.2004 20:31:45
 */
public interface VTServer extends ServerHandle{

	
	/**
	 * Nimmt Vote des Clients entgegen.
	 * @param voteid
	 * @throws RemoteException
	 */
	void vote(String voteid) throws RemoteException;

    /**
     * @param object
     */
	VoteTable webvote(String voteid) throws RemoteException;
}
