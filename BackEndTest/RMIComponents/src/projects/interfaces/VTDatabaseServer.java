/*
 * Created on Aug 11, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.interfaces;

import java.rmi.RemoteException;
import java.util.List;

import projects.voting.model.VoteTable;
import API.interfaces.ServerHandle;

/**
 * @author tobi
 *
 * Project : RMIComponents, At : Aug 11, 2004
 */
public interface VTDatabaseServer extends ServerHandle {

	void save() throws RemoteException;

	 void refreshConfiguration() throws RemoteException;

	 List listVotes() throws RemoteException;
	 
	 VoteTable getVoteTable() throws RemoteException;
	 
	 
	void vote(String voteid) throws RemoteException;
}