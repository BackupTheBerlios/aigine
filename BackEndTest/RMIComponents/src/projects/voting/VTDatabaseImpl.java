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
import projects.voting.control.HelperXmlPersitence;
import projects.voting.model.DBVote;
import projects.voting.model.Vote;
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
		storehelper = new HelperXmlPersitence("shellvotes");
		votes = storehelper.getVoteTable();
		System.out.println("\n<= VTDatabaseImpl.constructor\n");
	}

	/* (non-Javadoc)
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject service) {
		System.out.println("=>VTDatabaseImpl.register() > " + service);
		// TODO Auto-generated method stub
		System.out.println("<=VTDatabaseImpl.register() > " + service);
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
     * @see projects.interfaces.VTDatabaseServer#getVoteTable()
     */
    public VoteTable getVoteTable() throws RemoteException {
		System.out.println("<=>VTDatabaseImpl.getVoteTable()");
        return votes;
        
    }
	/**
	 * Registrierung eines Logik Service
	 * verwendet, um eine mögliche Überblendung zu vermeiden. 
	 * @param service
	 * @throws RemoteException
	 */
	public synchronized String registerService(final RemoteObject remoteObject){
		return super.registerService(remoteObject);
	}

    /* (non-Javadoc)
     * @see projects.interfaces.VTDatabaseServer#refreshConfiguration()
     */
    public void refreshConfiguration() {
		storehelper.refreshConfiguration();
        
    }
    /**
     * @return
     */
    public PersistenceHelper getStorehelper() {
        return storehelper;
    }

    /**
     * @param helper
     */
    public void setStorehelper(PersistenceHelper helper) {
        storehelper= helper;
    }
    
	/**
	 * Abgabe eines Votes.
	 * @param voteid - des gewählten Votes.
	 */
	public synchronized void vote(String voteid) throws RemoteException {
		System.out.println("=> VTDatabaseImpl.vote()");
		if (votes.containsKey(voteid)) {
			Vote vote= (Vote)votes.get(voteid);
			vote.setCount(vote.getCount() + 1);
			save();
		}
		System.out.println("<= VTDatabaseImpl.vote() > new votes: " + votes);
	}
}
