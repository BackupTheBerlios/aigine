/*
 * Created on 15.08.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting;

import interfaces.PersistenceHelper;

import java.rmi.RemoteException;
import java.util.List;
import java.util.Vector;

import projects.interfaces.VTDatabaseServer;
import projects.interfaces.VTLogicServer;
import projects.voting.model.DBVote;
import projects.voting.model.VoteTable;
import API.control.Database;
import API.model.RemoteObject;

/**
 * @author tobi
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class VTDatabaseImpl extends Database implements VTDatabaseServer {
	
	
	private PersistenceHelper storehelper;

	private VoteTable votes;

	private VTLogicServer logicserver;

	/**
	 * 
	 */
	public VTDatabaseImpl() throws RemoteException {
		System.out.println("=>VTDatabaseImpl.constructor\n");
		System.out.println(
			"VTDatabaseServer starten mit der in der properties spezialiseierten Datenbank");
		//Server hsqlserver = new Server();
		System.out.println("\n<= VTDatabaseImpl.constructor\n");
	}

	/* (non-Javadoc)
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject service) {
		// TODO Auto-generated method stub
		return super.register(service);
	}

	

	/**
		* Speichert einzelne DBVotes in der Datenbank
		* @param title		
		
	public void storeVote(DBVote vote) {
		try {
			Session session = sessionFactory.openSession();
			Transaction tx = session.beginTransaction();
			session.saveOrUpdate(vote);
			tx.commit();
			session.close();
		} catch (HibernateException e) {
			e.printStackTrace();
		}
	}
*/
	
	public void save() {
		storehelper.save();
	}
	public List listVotes() {
		return storehelper.listVotes();
		
	}

	/* (non-Javadoc)
	 * @see projects.interfaces.VTLogicServer#getDatabaseTable()
	 */
	public void getDatabaseTable() {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see projects.interfaces.VTDatabaseServer#storeVotes(java.util.Vector)
	 */
	public void storeVotes(Vector data) throws RemoteException {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see projects.interfaces.VTDatabaseServer#storeVote(projects.voting.model.DBVote)
	 */
	public void storeVote(DBVote vote) throws RemoteException {
		// TODO Auto-generated method stub
		
	}

}
