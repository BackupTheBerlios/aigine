/*
 * Created on Aug 28, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting.control;

import java.rmi.RemoteException;
import java.util.List;

import org.jconfig.Configuration;

import projects.voting.model.VoteTable;

/**
 * @author tobi
 *
 * Project : RMIComponents, At : Aug 28, 2004
 */
public class HelperDatabasePersistence implements VTPersistenceHelper {

//	TODO eine Conection zu dem Datenbank Server aufbauen. dieser oeffnet eine neue SessionFactory
		  //als naechstes dessen methoden fuer die gewuenschten daten aufrufen ( getKompleteVotes, getVote, saveVote, saveAllVotes...)

		  //		VTDatabaseServer hsqlDB = (VTDatabaseServer) dbserver; 
		  //		try {
		  //			List votelist = hsqlDB.listVotes();
		  //		} catch (RemoteException e) {
		  //			System.out.println("HelperDatabasePersistence.constructor. listVotes of VTDatabaseServer faild!!");
		  //			e.printStackTrace();
		  //		}
	
	Configuration conf;
	
	
	/**
	 * 
	 */
	public HelperDatabasePersistence() {
		
		
		
	}

	/* (non-Javadoc)
	 * @see projects.interfaces.VTPersistenceHelper#listVotes()
	 */
	public List listVotes() throws RemoteException {
		// TODO Auto-generated method stub
		
		
		
		return null;
	}

	/* (non-Javadoc)
	 * @see projects.interfaces.VTPersistenceHelper#getVoteTable()
	 */
	public VoteTable getVoteTable() throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see API.interfaces.PersistenceHelper#save()
	 */
	public void save() {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see API.interfaces.PersistenceHelper#refreshConfiguration()
	 */
	public void refreshConfiguration() {
		// TODO Auto-generated method stub
		
	}

}
