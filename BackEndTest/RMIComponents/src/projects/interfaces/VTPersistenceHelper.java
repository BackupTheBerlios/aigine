/*
 * Created on Aug 28, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.interfaces;

import java.rmi.RemoteException;
import java.util.List;

import projects.voting.model.VoteTable;
import API.interfaces.PersistenceHelper;

/**
 * @author tobi
 *
 * Project : RMIComponents, At : Aug 28, 2004
 */
public interface VTPersistenceHelper extends PersistenceHelper {
	
	List listVotes() throws RemoteException;
	 
	 VoteTable getVoteTable() throws RemoteException;
	 
	 void setVoteTable(VoteTable votes);

}
